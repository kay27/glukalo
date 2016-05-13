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

  class Icon
  {
    public:
      static void Init();
      void Render();
      static void Resize(float newMulValue);
    private:
      static GLint progLevel, vPosLevel, vTCLevel, vOffsLevel;
      static GLint progSound, vPosSound, vTCSound, vOffsSound, vState;
      static float vm;
  };

  enum BonusType { MUSHROOM_MISSILE, SLOWDOWN, SPEEDUP };

  class Bonus
  {
    public:
      Bonus() : angle(0) {}
      bool Collision(float x0, float y0);
      static void Init();
      void Move(float delta, int antiGravity, int direction);
      void Set(float y_);
      void Set(float x_, float y_);
      void Set(float x_, float y_, BonusType type_);
      void Render();
      static void Resize(float newMulValue);
    private:
      static GLint program, vPos, vTC, vOffs, vType;
      static float vm;
      BonusType type;
      float x,y,angle;
  };

  class Missile
  {
    public:
      Missile() : phase(0) {};
      void Explode();
      inline float GetPhase(){ return phase; }
      inline float GetR(){ return MISSILE_RADIUS; }
      inline float GetX(){ return x; }
      inline float GetY(){ return y; }
      static void Init();
      void Move(float delta, int antiGravity, int direction);
      void Render();
      static void Resize(float newMulValue);
      void Start(float x_, float y_);
    private:
      static GLint program, vPos, vTC, vOffs/*, vMul*/;
      int phase;
      float a, x, y, ec;
      static float vm;
  };

  class Column
  {
    public:
      static void Init();

      bool Collision(float x0, float y0, float yMulValue);

      int Collision(float x0, float y0, float r0, float yMulValue);

      inline void Freeze(float y_, float halfSize_){  y = y_; halfSize = halfSize_; freeze = 1; solid = 0; }

      inline const float GetX(){ return x; }

      inline const float GetY(){ return y; }

      inline const float GetHalfSize(){ return halfSize; }

      inline const float GetSolid(){ return solid; }

      float Move(float dx);
     
      void OnCreate();

      void Render();

      void Restart(float x_, int score_);

      void Restart(float x_, int score_, int level_);

//      inline void SetY(float y_){ y = y_; }

//      inline void SetHalfSize(float halfSize_){ halfSize = halfSize_; }

      inline const float MakeSolid(){ solid = 1; y = -3; }

      void SetSpeed(const float ss, const float js);

      bool Pass(int direction);

    private:
      int passed, freeze, solid;
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
      Game();
      ~Game();
      void AddScore();
      void ChangeLevel();
      void CheckBonus();
      void CheckColumns();
      void CheckMissiles();
      void DemoAnimation(float delta);
      void GameOver();
      static int GetLevel(int newScore);
      int GetNumberLength(int number);
      float GetTimeInterval();
      void FlyAway(float deltaX);
      void Init();
      void MoveBonus();
      void MoveColumnsCheckPass(float deltaX);
      void OnNewColumn(Column * c, float cx, int cScore, int cLevel);
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

      void InitAudio();
      void PauseAudio();
      void PlayAudio();
      void ResumeAudio();

    protected:

      Icon icon;
      Bonus b;
      Column * bonusColumn;
      int bonus;

      int firstRun, gameStarted, gameOver, gameLooped, scoreRestarted, direction;

      struct timeval lastTime;

      float gameOverTime;

      GLfloat speed, x, y, speedVect, yMulValue, floorOffset, charWidth;

      Column gaps[MAX_COLUMNS];

      int impulse, pause, score, highScore, level, maxLevel, tapFire;

      GLuint vb;
      GLint vPosition, vTextureCoordinate, vColor, vOffset, vEye, vRadius, vMul,
        vFloorPosition, vFloorOffset, vFontPosition, vFontOffset, vFontCharCode,
        vFontColor, vCharMul, vFontLineSize, vCharOffset;
     
      GLuint birdProgram, floorProgram, fontProgram;

      MyAudio *audio;

      Missile m;
  };

#endif // #ifndef H_GAME_GLUKALO
