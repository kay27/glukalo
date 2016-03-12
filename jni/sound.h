#ifndef H_SOUND_GLUKALO
# define H_SOUND_GLUKALO

# include <cstdlib>

# include <SLES/OpenSLES.h>
# include <SLES/OpenSLES_Android.h>

# include "config.h"

  class SLAudio
  {
    public:
      bool CreateEngine();
    protected:
      void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context);

      short soundbuffer[MY_AUDIO_BUFFER_FRAMES];
      SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
      SLObjectItf engine, mix;
      SLEngineItf itf;
      SLEnvironmentalReverbItf reverb;
      SLresult result;
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
      short noiseValue;
      int sampleRate, noiseReminder;
  };

#endif // #ifndef H_SOUND_GLUKALO
