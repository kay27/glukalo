#include "sound.h"

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
