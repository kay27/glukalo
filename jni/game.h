#ifndef H_GAME_GLUKALO
# define H_GAME_GLUKALO

# include <unistd.h>
# include <string>
# include <math.h>
# include <GLES2/gl2.h>

  class Game;

# include "config.h"
# include "shader.h"
# include "jni2game.h"
# include "sound.h"
# include "system.h"

  enum BonusType { GUN, SLOWDOWN, SPEEDUP };

  struct Bonus
  {
    BonusType type;
    float x,y;
  };

  class Missile
  {
    public:
//      Missile() : phase(0) {};
      Missile() : phase(0), vm(1) {};
      void Explode();
      inline float GetPhase(){ return phase; }
      inline float GetR(){ return MISSILE_RADIUS; }
      inline float GetX(){ return x; }
      inline float GetY(){ return y; }
      static void Init();
      void Move(float delta, int antiGravity);
      void Render();
//      static void Resize(float newMulValue);
      void Resize(float newMulValue);
      void Start(float x_, float y_);
    private:
      static GLint program, vPos, vTC, vOffs, vMul;
      int phase;
      float a, x, y, ec;

float vm;
  };

  class Column
  {
    public:
      static void Init();

      bool Collision(float x0, float y0, float yMulValue);

      bool Collision(float x0, float y0, float r0, float yMulValue);

      inline const float GetY(){ return y; }

      inline const float GetHalfSize(){ return halfSize; }

      float Move(float dx);
     
      void OnCreate();

      void Render();

      void Restart(float x_, int score_);

//      inline void SetY(float y_){ y = y_; }

//      inline void SetHalfSize(float halfSize_){ halfSize = halfSize_; }

      inline void Freeze(float y_, float halfSize_){  y = y_; halfSize = halfSize_; freeze = 1; }

      void SetSpeed(const float ss, const float js);

      bool Pass();

    private:
      int passed, freeze;
      float x, y, halfSize;
      int level, score;
      GLfloat swingVector, jawsVector, swingSpeed, jawsSpeed;

      static GLint gapProgram, vGapPosition, vGapTextureCoordinate, vGap, vOffsetX, vHalfSize, vLevel,
        lastSwingVector;
      static int gapCount, lastLevel;
  };

  class Game
  {
    public:
      void AddScore();
      void ChangeLevel();
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

      GLfloat speed, x, y, speedVect, yMulValue, floorOffset, charWidth;

      Column gaps[MAX_COLUMNS];

      int impulse, pause, score, highScore, level, maxLevel;

      GLuint vb;
      GLint vPosition, vTextureCoordinate, vColor, vOffset, vEyeY, vRadius, vMul,
        vFloorPosition, vFloorOffset, vFontPosition, vFontOffset, vFontCharCode,
        vFontColor, vCharMul, vFontLineSize, vCharOffset;
     
      GLuint birdProgram, floorProgram, fontProgram;

      MyAudio *audio;

      Missile m;
  };

#endif // #ifndef H_GAME_GLUKALO
