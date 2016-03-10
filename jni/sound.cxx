#include "sound.h"

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
