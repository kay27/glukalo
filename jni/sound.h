#ifndef H_SOUND_GLUKALO
# define H_SOUND_GLUKALO

# include <cstdlib>
//# include <functional>

# include <SLES/OpenSLES.h>
# include <SLES/OpenSLES_Android.h>

# include "config.h"

  static void* globalsoundbuffer;

  class SLAudio
  {
    public:
      SLAudio();
      ~SLAudio();
      bool CreateEngine();
      bool CreatePlayer();
      inline SLAndroidSimpleBufferQueueItf & GetBQ() { return bq; }

    protected:
//      void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq_, void *context);

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
      bool Play();
      void NextNoiseValue();
      void Noise(short *buffer, unsigned length, unsigned freq);
      void MakeNoise(unsigned freq);

    protected:
      SLAudio *a;
      short noiseValue;
      int sampleRate, noiseReminder;
//      std::function<void(SLAndroidSimpleBufferQueueItf bq, void *context)> callback;
  };

#endif // #ifndef H_SOUND_GLUKALO
