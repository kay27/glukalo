#ifndef H_SOUND_GLUKALO
# define H_SOUND_GLUKALO

# include <cstdlib>

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
      MyAudio();
      ~MyAudio();
      void NextNoiseValue();
      void Noise(short *buffer, unsigned length, unsigned freq);

    protected:
      SLAudio *a;
      short soundbuffer[MY_AUDIO_BUFFER_FRAMES];
      short noiseValue;
      int sampleRate, noiseReminder;
  };

#endif // #ifndef H_SOUND_GLUKALO
