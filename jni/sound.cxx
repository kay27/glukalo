#include "sound.h"

void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
  (*bq)->Enqueue(bq, globalsoundbuffer, MY_AUDIO_BUFFER_FRAMES*sizeof(short));
}


bool SLAudio::CreateEngine()
{
  const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
  const SLboolean     req[1] = {SL_BOOLEAN_FALSE};

  result = slCreateEngine(&engine, 0, NULL, 0, NULL, NULL);
  if(result != SL_RESULT_SUCCESS) return false;

  result = (*engine)->Realize(engine, SL_BOOLEAN_FALSE);
  if(result != SL_RESULT_SUCCESS) return false;

  result = (*engine)->GetInterface(engine, SL_IID_ENGINE, (void*)&itf);
  if(result != SL_RESULT_SUCCESS) return false;

  result = (*itf)->CreateOutputMix(itf, &mix, 1, ids, req);
  if(result != SL_RESULT_SUCCESS) return false;

  result = (*mix)->Realize(mix, SL_BOOLEAN_FALSE);
  if(result != SL_RESULT_SUCCESS) return false;

  result = (*mix)->GetInterface(mix, SL_IID_ENVIRONMENTALREVERB, &reverb);
  if(result == SL_RESULT_SUCCESS) result = (*reverb)->SetEnvironmentalReverbProperties(reverb, &REVERB_SETTINGS);

  return true;
}

void SLAudio::DestroyEngine()
{
  if (mix != nullptr) (*mix)->Destroy(mix);
  mix = nullptr;
  reverb = nullptr;
  if (engine != nullptr) (*engine)->Destroy(engine);
  engine = nullptr;
  itf = nullptr;
}

bool SLAudio::CreatePlayer()
{
  SLDataLocator_AndroidSimpleBufferQueue locBq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};

  SLDataFormat_PCM pcm =
  {
    SL_DATAFORMAT_PCM,               // format type
    1,                               // numChannels
    MY_AUDIO_SAMPLE_RATE * 1000,     // samplesPerSec ??? sampling rate in milliHertz
    SL_PCMSAMPLEFORMAT_FIXED_16,     // bitsPerSample
    SL_PCMSAMPLEFORMAT_FIXED_16,     // containerSize
    SL_SPEAKER_FRONT_CENTER,         // channelMask
    SL_BYTEORDER_LITTLEENDIAN        // endianness
  };

  SLDataSource src = {&locBq, &pcm};

  SLDataLocator_OutputMix locMix = {SL_DATALOCATOR_OUTPUTMIX, mix};
  SLDataSink sink = {&locMix, NULL};

  const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME  };
  const SLboolean     req[3] = {SL_BOOLEAN_TRUE,    SL_BOOLEAN_TRUE,   SL_BOOLEAN_TRUE};

  result = (*itf)->CreateAudioPlayer(itf, &player, &src, &sink, 1, ids, req);
  if(result != SL_RESULT_SUCCESS) return false;

  result = (*player)->Realize(player, SL_BOOLEAN_FALSE);
  if(result != SL_RESULT_SUCCESS) return false;

  result = (*player)->GetInterface(player, SL_IID_PLAY, &play);
  if(result != SL_RESULT_SUCCESS) return false;

  result = (*player)->GetInterface(player, SL_IID_BUFFERQUEUE, &bq);
  if(result != SL_RESULT_SUCCESS) return false;

  result = (*bq)->RegisterCallback(bq, bqPlayerCallback, NULL);
  if(result != SL_RESULT_SUCCESS) return false;

  (*player)->GetInterface(player, SL_IID_EFFECTSEND, &effect);

  (*player)->GetInterface(player, SL_IID_VOLUME, &volume);

  result = (*play)->SetPlayState(play, SL_PLAYSTATE_PLAYING);
  if(result != SL_RESULT_SUCCESS) return false;

  return true;
}

void SLAudio::DestroyPlayer()
{
  if(player == nullptr) return;
  (*player)->Destroy(player);
  player = nullptr;
  play = nullptr;
  bq = nullptr;
  effect = nullptr;
  volume = nullptr;
}

SLAudio::SLAudio()
{
  globalsoundbuffer = &soundbuffer;
}

SLAudio::~SLAudio()
{
}

MyAudio::MyAudio()
{
  sampleRate = MY_AUDIO_SAMPLE_RATE;
  noiseReminder = 0;
  NextNoiseValue();
  a = new SLAudio();
  for(auto i=0; i<MY_AUDIO_BUFFER_FRAMES; i++) ((short*)globalsoundbuffer)[i]=0;
}

void MyAudio::MakeNoise(unsigned freq)
{
  Noise((short*)globalsoundbuffer, MY_AUDIO_BUFFER_FRAMES, freq);
}

MyAudio::~MyAudio()
{
  if(a) delete a;
  a = NULL;
}

void MyAudio::NextNoiseValue()
{
  noiseValue = (rand() % (MY_AUDIO_NOISE_VOLUME<<1)) - MY_AUDIO_NOISE_VOLUME;
}

void MyAudio::Noise(short *buffer, unsigned length, unsigned freq)
{
  short *dst = buffer;
  short *stop = dst + sizeof(short)*length;

  while(dst < stop)
  {
    *(dst++) = noiseValue;

    noiseReminder += freq;
    if (noiseReminder < sampleRate)
      continue;

    noiseReminder %= sampleRate;
    NextNoiseValue();
  }
}

bool MyAudio::Play()
{
  if(!a) return false;
  if(!a->CreateEngine()) return false;
  if(!a->CreatePlayer()) return false;
  SLAndroidSimpleBufferQueueItf & bq = a->GetBQ();
  (*bq)->Enqueue(bq, globalsoundbuffer, MY_AUDIO_BUFFER_FRAMES*sizeof(short));
  return true;
}

void MyAudio::Stop()
{
  if(!a) return;
  a->DestroyPlayer();
  a->DestroyEngine();
}
