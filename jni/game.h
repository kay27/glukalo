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
# include "system.h"
# include "SoundPlayer.h"

  class ColumnPreview
  {
    public:
      static void Init();

      void Render(float x, float y, int level, bool portrait);

    private:
      static GLint gapProgram, vGapPosition, vGapTextureCoordinate, vGap, vOffs, vHalfSize, vLevel;
  };

  class Icon
  {
    public:
      void Init(const char * vs, const char * fs, float x_, float y_, float state_);
      void Init(const char * vs, const char * fs, float x_, float y_);
      void Move(float x_, float y_);
      void Render();
      static void Resize(float newMulValue);
      int Tap(float tx, float ty);
      void SetState(const float _state){state=_state;}
    private:
      float x, y, state;
      GLint prog, vPos, vTC, vOffs;
      static float vm;
  };

  enum BonusType { NO_BONUS, MUSHROOM_MISSILE, AUTOPILOT, RANDOM_BONUS };

  class Bonus
  {
    public:
      Bonus() : angle(0) {}
      bool Collision(float x0, float y0);
      static void Init();
      void Move(float delta, int antiGravity, int direction, float deltaX);
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
      void Move(float delta, int antiGravity, int direction, float deltaX);
      void Render(float radius);
      static void Render(float x, float y);
      static void Resize(float newMulValue);
      void Start(float x_, float y_);
    private:
      static GLint program, vPos, vTC, vOffs, vRadius;
      int phase;
      float a, x, y, ec;
      static float vm;
  };

  class Column
  {
    public:
      static void Init();

      int Collision(float x0, float y0, float r0, float yMulValue);

      inline void Freeze(float y_, float halfSize_){  y = y_; halfSize = halfSize_; freeze = 1; solid = 0; }

      inline int GetPassed(){ return passed; }

      inline const float GetX(){ return x; }

      inline const float GetY(){ return y; }

      inline const float GetHalfSize(){ return halfSize; }

      inline const float GetSolid(){ return solid; }

      float Move(float dx);
     
      void OnCreate();

      void Render();

      void Restart(float x_, int score_);

      void Restart(float x_, int score_, int level_);

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
      void Logic(const float delta);
      void MoveBonus();
      void MoveColumnsCheckPass(float deltaX);
      int OnBackPressed();
      void OnNewColumn(Column * c, float cx, int cScore, int cLevel);
      void OnDirectionChange();
      void Pause();
      void PrintNumber(float xcrd, float ycrd, float r, float g, float b, int number);
      void PrintScore();
      void Tap(float x, float y);
      void Render();
      void RenderMenu(float delta);
      void Resize(int w, int h);
      void Restart();
      void Resume();
      void SelectLevel(float x, float y);
      void Untap();
      void UpdateHighScore();
      void AutoPilot(const float delta);

    protected:

      static int blockMode;

      Icon levelIcon, soundIcon;
      Bonus b;
      Column * bonusColumn;
      ColumnPreview cp;
      BonusType bonus;

      int firstRun, gameStarted, gameOver, gameLooped, scoreRestarted, direction, verticalMenu, muteState;
      int antiGravity, autoPilot, randomTime, changeRadius;
      float menu_x[NUMBER_OF_LEVELS<<1], menu_y[NUMBER_OF_LEVELS<<1];

      struct timeval lastTime;

      float gameOverTime, changeSpeed, addToRadius;

      GLfloat speed, x, y, speedVect, yMulValue, floorOffset, charWidth;

      Column gaps[MAX_COLUMNS];

      int impulse, pause, score, highScore, level, maxLevel, tapFire, showMenu, selectedLevelScore;

      GLuint vb;
      GLint vPosition, vTextureCoordinate, vColor, vOffset, vState, vRadius, vMul,
        vFloorPosition, vFloorOffset, vFontPosition, vFontOffset, vFontCharCode,
        vFontColor, vCharMul, vFontLineSize, vCharOffset;
     
      GLuint birdProgram, floorProgram, fontProgram;

      Missile m;

      int codePass, codePass2, codePass3;
  };

#endif // #ifndef H_GAME_GLUKALO
