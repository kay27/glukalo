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
      bool CreatePlayer();

    protected:
      void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context);

      short soundbuffer[MY_AUDIO_BUFFER_FRAMES];

      SLAndroidSimpleBufferQueueItf bq;
      SLObjectItf engine, mix, player;
      SLEngineItf itf;
      SLPlayItf play;
      SLEnvironmentalReverbItf reverb;
      SLEffectSendItf effect;
      SLVolumeItf volume;
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
