#ifndef H_GAME_GLUKALO
# define H_GAME_GLUKALO

# include <unistd.h>
# include <cstdlib>
//# include <mutex>
//# include <vector>
# include <GLES2/gl2.h>

  class Game;

# include "config.h"
# include "shader.h"
# include "jni2game.h"

//  using namespace std;

//  class Bird;
//  class Gap;

  class Game
  {
    public:
      bool Clamp(float x, float a, float b);
//      bool Collision(Gap &gap);
      Game();
//      ~Game();
      void Init();
      void Pause();
      void Tap();
      void Render();
      void Resize(int w, int h);
      void Restart();
      void Resume();
      void Untap();

    protected:
//      Bird & bird;
//      vector<Gap*> gaps;
//      mutex gapListMutex;

      int firstRun, gameStarted, gameOver;

      struct timeval lastTime;

      GLfloat speed, x, y, speedVect, yMulValue;
      int impulse, pause;

      GLuint vb;
      GLint vPosition, vTextureCoordinate, vColor, vOffset, vSpeed, vRadius, vMul,
        vGapPosition, vGapTextureCoordinate, vGap, vOffsetX;
     
      GLuint birdProgram, gapProgram;
  };
/*
  class Bird
  {
    public:
      Bird();
      ~Bird();
      void Tap();
      void Init();
      void Render();
  };

  class Gap
  {
    public:
      Gap();
      ~Gap();
  };
*/

#endif // #ifndef H_GAME_GLUKALO
