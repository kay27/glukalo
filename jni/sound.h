#ifndef H_SOUND_GLUKALO
# define H_SOUND_GLUKALO

# include <SLES/OpenSLES.h>
# include <SLES/OpenSLES_Android.h>

# define MY_AUDIO_BUFFER_FRAMES 441
# define MY_AUDIO_NOISE_VOLUME 3000

  class SLAudio
  {
    protected:
      SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
  };

  class MyAudio
  {
    public:

      NextNoiseValue()
      {
        noiseValue = (rand() % (MY_AUDIO_NOISE_VOLUME<<1)) - MY_AUDIO_NOISE_VOLUME;
      }

      MyAudio()
      {
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
      short soundbuffer[BUFFER_FRAMES];

  };

#endif // #ifndef H_SOUND_GLUKALO
