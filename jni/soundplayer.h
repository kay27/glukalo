#ifndef H_SOUNDPLAYER_GLUKALO
# define H_SOUNDPLAYER_GLUKALO

# include <math.h>
# include "config.h"
# include "sound.h"
# include "system.h"

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
      static void PlayBonus();
      static void PlayGameOver();
      static void PlayEndOfAutoPilot();
      static void Mute(){mute = 1;}
      static void Unmute(){mute = 0;}
    protected:
      static short buffer[NEXT_LEVEL_SCORE*MISS_SAMPLE_LENGTH];
      static short bufferd[D_SAMPLE_LENGTH];
      static short bonusBuffer[BONUS_SAMPLE_LENGTH];
      static short gameOverBuffer[GAMEOVER_SAMPLE_LENGTH];
      static short endOfautoPilotBuffer[ENDOFAUTOPILOT_SAMPLE_LENGTH];

      static int mute;
  };


#endif // #ifndef H_SOUNDPLAYER_GLUKALO
