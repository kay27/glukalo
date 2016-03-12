#include "sound.h"

bool SLAudio::CreatePlayer()
{ // https://habrahabr.ru/post/235795/#anc5
    // configure audio source
/*
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_8,
        SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};
    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    // create audio player
    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND,
     //SL_IID_MUTESOLO,
 SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE,
            //SL_BOOLEAN_TRUE,
 SL_BOOLEAN_TRUE};
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &bqPlayerObject, &audioSrc, &audioSnk,
            3, ids, req);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // realize the player
    result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // get the play interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // get the buffer queue interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_BUFFERQUEUE,
            &bqPlayerBufferQueue);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // register callback on the buffer queue
    result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, bqPlayerCallback, NULL);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // get the effect send interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_EFFECTSEND,
            &bqPlayerEffectSend);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

#if 0   // mute/solo is not supported for sources that are known to be mono, as this is
    // get the mute/solo interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_MUTESOLO, &bqPlayerMuteSolo);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;
#endif

    // get the volume interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_VOLUME, &bqPlayerVolume);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // set the player's state to playing
    result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;
  */
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

void SLAudio::bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
  result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, soundbuffer, sizeof(soundbuffer));
}

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
