#ifndef H_GAME_GLUKALO
# define H_GAME_GLUKALO

# include <unistd.h>
# include <cstdlib>
# include <GLES2/gl2.h>

  class Game;

# include "config.h"
# include "shader.h"
# include "jni2game.h"
# include "sound.h"

  enum BonusType { GUN, SLOWDOWN, SPEEDUP };

  struct Bonus
  {
    BonusType type;
    float x,y;
  };

  class Game
  {
    public:
      void AddScore();
      void ChangeLevel();
      float Clamp(float x, float a, float b);
      bool Collision(float x0, float x1, float y0, float gapHalfSize);
      Game();
      ~Game();
      void GameOver();
      int GetLevel(int newScore);
      int GetNumberLength(int number);
      void Init();
      void Pause();
      void PrintNumber(float xcrd, float ycrd, float r, float g, float b, int number);
      void PrintScore();
      void Tap();
      float Rand();
      void Render();
      void Resize(int w, int h);
      void Restart();
      void Resume();
      void Untap();
      void UpdateHighScore();

    protected:

      int firstRun, gameStarted, gameOver, gameLooped, scoreRestarted;

      struct timeval lastTime;

      float gameOverTime;

      GLfloat speed, x, y, speedVect, yMulValue,
        blockPos, floorOffset, swingSpeed, jawsSpeed,
        charWidth;

      GLfloat gaps[MAX_COLUMNS], swingVectors[MAX_COLUMNS], gapHalfSizes[MAX_COLUMNS], jawsVectors[MAX_COLUMNS];

      int impulse, pause, score, highScore, level, maxLevel;

      GLuint vb;
      GLint vPosition, vTextureCoordinate, vColor, vOffset, vEyeY, vRadius, vMul,
        vGapPosition, vGapTextureCoordinate, vGap, vLevel, vOffsetX, vHalfSize,
        vFloorPosition, vFloorOffset, vFontPosition, vFontOffset, vFontCharCode,
        vFontColor, vCharMul, vFontLineSize, vCharOffset;
     
      GLuint birdProgram, floorProgram, gapProgram, fontProgram;

      MyAudio *audio;
  };

#endif // #ifndef H_GAME_GLUKALO
