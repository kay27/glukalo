#ifndef H_SOUND_GLUKALO
# define H_SOUND_GLUKALO

# include <SLES/OpenSLES.h>
# include <SLES/OpenSLES_Android.h>

# include "config.h"

  class SLAudio
  {
    protected:
      SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
  };

  class MyAudio
  {
    public:

      void NextNoiseValue()
      {
        noiseValue = (rand() % (MY_AUDIO_NOISE_VOLUME<<1)) - MY_AUDIO_NOISE_VOLUME;
      }

      MyAudio()
      {
        sampleRate = MY_AUDIO_SAMPLE_RATE;
        noiseReminder = 0;
        NextNoiseValue();
        a = new SLAudio();
      }

      ~MyAudio()
      {
        if(a) delete a;
        a = NULL;
      }

      void Noise(short *buffer, unsigned length, unsigned freq)
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

    protected:
      SLAudio *a;
      short soundbuffer[MY_AUDIO_BUFFER_FRAMES];
      short noiseValue;
      int sampleRate, noiseReminder;
  };

#endif // #ifndef H_SOUND_GLUKALO
