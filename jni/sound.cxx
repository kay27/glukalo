#include "sound.h"

void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq_, void *context)
{
//  result = (*bq)->Enqueue(bq, soundbuffer, sizeof(soundbuffer));
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

//  auto callback = [&](SLAndroidSimpleBufferQueueItf bq_, void *context)
//  {
//    (*bq)->Enqueue(this->bq, this->soundbuffer, sizeof(this->soundbuffer));
//  };
//  result = (*bq)->RegisterCallback(bq, callback, NULL);
  result = (*bq)->RegisterCallback(bq, ([this](SLAndroidSimpleBufferQueueItf bq_, void *context){
    (*(this->bq))->Enqueue(this->bq, this->soundbuffer, sizeof(this->soundbuffer));}), NULL);
  if(result != SL_RESULT_SUCCESS) return false;
//  result = (*bq)->RegisterCallback(bq, &SLAudio::bqPlayerCallback, NULL);
//  result = (*bq)->RegisterCallback(bq, this->bqPlayerCallback, NULL);
//  result = (*bq)->RegisterCallback(bq, [this](SLAndroidSimpleBufferQueueItf bq_, void *context)
//    {
//      (*bq)->Enqueue(this->bq, this->soundbuffer, sizeof(this->soundbuffer));
//    } , NULL);
//  result = (*bq)->RegisterCallback(bq, bqPlayerCallback, NULL);

  (*player)->GetInterface(player, SL_IID_EFFECTSEND, &effect);

  (*player)->GetInterface(player, SL_IID_VOLUME, &volume);

  result = (*play)->SetPlayState(play, SL_PLAYSTATE_PLAYING);
  if(result != SL_RESULT_SUCCESS) return false;

  return true;
}
/*
void SLAudio::bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq_, void *context)
{
  result = (*bq)->Enqueue(bq, soundbuffer, sizeof(soundbuffer));
}
*/
MyAudio::MyAudio()
{
  sampleRate = MY_AUDIO_SAMPLE_RATE;
  noiseReminder = 0;
  NextNoiseValue();
  a = new SLAudio();
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
