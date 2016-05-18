#ifndef H_SOUND_GLUKALO
# define H_SOUND_GLUKALO

# include <cstdlib>
//# include <functional>

# include <SLES/OpenSLES.h>
# include <SLES/OpenSLES_Android.h>

# include "config.h"

  class SLAudio
  {
    public:
//      static SLAudio();
//      static ~SLAudio();
      static void Clear();
      static void Pause(bool pause);
      static void Start();
      static bool CreateEngine();
      static bool CreatePlayer();
      static void DestroyEngine();
      static void DestroyPlayer();
      static inline SLAndroidSimpleBufferQueueItf & GetBQ() { return bq; }
      static void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context);

      static void* globalsoundbuffer;
    protected:
      static SLInterfaceID ids[1];
      static SLboolean     req[1];
//      static SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
//      static SLboolean     req[1] = {SL_BOOLEAN_FALSE};


      static short soundbuffer[MY_AUDIO_BUFFER_FRAMES];
      static SLAndroidSimpleBufferQueueItf bq;
      static SLObjectItf engine, mix, player;
      static SLEngineItf itf;
      static SLPlayItf play;
      static SLEnvironmentalReverbItf reverb;
      static SLEffectSendItf effect;
      static SLVolumeItf volume;
      static SLresult result;
      static bool paused;
  };

  class MyAudio
  {
    public:
      MyAudio();
      ~MyAudio();
      bool Play();
      void Stop();
      void Pause();
      void Clear();
      void Continue();
      void NextNoiseValue();
      void Noise(short *buffer, unsigned length, unsigned freq);
      void MakeNoise(unsigned freq);

    protected:
      SLAudio *a;
      short noiseValue;
      int sampleRate, noiseReminder;
  };

#endif // #ifndef H_SOUND_GLUKALO
