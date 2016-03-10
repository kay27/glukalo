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

  class Game
  {
    public:
      float Clamp(float x, float a, float b);
      bool Collision(float x0, float x1, float y0);
      Game();
      ~Game();
      void GameOver();
      void Init();
      void Pause();
      void Tap();
      float Rand();
      void Render();
      void Resize(int w, int h);
      void Restart();
      void Resume();
      void Untap();

    protected:

      int firstRun, gameStarted, gameOver;

      struct timeval lastTime;

      float gameOverTime;

      GLfloat speed, x, y, speedVect, yMulValue,
        blockPos;

      GLfloat gaps[MAX_COLUMNS];

      int impulse, pause, score;

      GLuint vb;
      GLint vPosition, vTextureCoordinate, vColor, vOffset, vSpeed, vRadius, vMul,
        vGapPosition, vGapTextureCoordinate, vGap, vOffsetX, vHalfSize,
        vFloorPosition;
     
      GLuint birdProgram, floorProgram, gapProgram;

      MyAudio *audio;
  };

#endif // #ifndef H_GAME_GLUKALO
