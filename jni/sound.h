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
      void Clear();
      void Pause(bool pause);
      void Start();
      bool CreateEngine();
      bool CreatePlayer();
      void DestroyEngine();
      void DestroyPlayer();
      inline SLAndroidSimpleBufferQueueItf & GetBQ() { return bq; }

    protected:
      short soundbuffer[MY_AUDIO_BUFFER_FRAMES];
      SLAndroidSimpleBufferQueueItf bq;
      SLObjectItf engine, mix, player;
      SLEngineItf itf;
      SLPlayItf play;
      SLEnvironmentalReverbItf reverb;
      SLEffectSendItf effect;
      SLVolumeItf volume;
      SLresult result;
      bool paused;
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
