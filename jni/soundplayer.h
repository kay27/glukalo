#ifndef H_SOUNDPLAYER_GLUKALO
# define H_SOUNDPLAYER_GLUKALO

# include <math.h>
# include "config.h"
# include "sound.h"

  class SoundPlayer
  {
    public:
      static void Init();
      static void Destroy();
      static void Pause();
      static void Resume();
      static void Play(short * buffer, int count);
      static void PlayMiss(int tune);
      static void PlayD();
    protected:
      static short buffer[NEXT_LEVEL_SCORE*MISS_SAMPLE_LENGTH];
      static short bufferd[D_SAMPLE_LENGTH];

  };


#endif // #ifndef H_SOUNDPLAYER_GLUKALO
