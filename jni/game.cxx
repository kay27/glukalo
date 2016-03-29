#include "game.h"
#include <mutex>
//#include <string>

Game::Game()
{
  firstRun = 1;

  yMulValue = 1;

  maxLevel = 0;

  Init();

//  audio = new MyAudio();
  audio = nullptr;
  if(audio!=nullptr) if(!audio->Play()) audio = nullptr;
  if(audio==nullptr) MyCallback::Toast("Failed to start audio");
}

Game::~Game()
{
  if(audio != nullptr)
  {
    audio->Stop();
    delete audio;
    audio = nullptr;
  }
}

void Game::Init()
{
  pause = 0;

  if(firstRun)
    MyCallback::Toast("Tap to play");
  firstRun = 0;

  glGenBuffers(1, &vb);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

  birdProgram = MyShader::CreateProgram();
  MyShader::AttachVertexShader(birdProgram, birdVertexShader);
  MyShader::AttachFragmentShader(birdProgram, birdFragmentShader);
  MyShader::LinkProgram(birdProgram);

  glUseProgram(birdProgram);

  vPosition          = glGetAttribLocation(birdProgram, "vPosition");
  vTextureCoordinate = glGetAttribLocation(birdProgram, "vTextureCoordinate");

  vColor  = glGetUniformLocation(birdProgram, "vColor");
  vOffset = glGetUniformLocation(birdProgram, "vOffset");
  vRadius = glGetUniformLocation(birdProgram, "vRadius");
  vMul    = glGetUniformLocation(birdProgram, "vMul");
  vSpeed  = glGetUniformLocation(birdProgram, "vSpeed");

  glEnableVertexAttribArray(vPosition);
  glEnableVertexAttribArray(vTextureCoordinate);
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,pos));
  glVertexAttribPointer(vTextureCoordinate, 2, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,txtcrds));
  glUniform4f(vColor, 0, 0.6, 1, 1);
  glUniform1f(vRadius, (float)1.0);
  glUseProgram(0);

  gapProgram = MyShader::CreateProgram();
  MyShader::AttachVertexShader(gapProgram, gapVertexShader);
  MyShader::AttachFragmentShader(gapProgram, gapFragmentShader);
  MyShader::LinkProgram(gapProgram);

  glUseProgram(gapProgram);

  vGapPosition          = glGetAttribLocation(gapProgram, "vPosition");
  vGapTextureCoordinate = glGetAttribLocation(gapProgram, "vTextureCoordinate");

  vOffsetX              = glGetUniformLocation(gapProgram, "vOffsetX");
  vGap                  = glGetUniformLocation(gapProgram, "vGap");
  vHalfSize             = glGetUniformLocation(gapProgram, "vHalfSize");
  vLevel                = glGetUniformLocation(gapProgram, "vLevel");

  glEnableVertexAttribArray(vGapPosition);
  glEnableVertexAttribArray(vGapTextureCoordinate);
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,pos));
  glVertexAttribPointer(vTextureCoordinate, 2, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,txtcrds));
  glUniform1f(vHalfSize, GAP_HALFSIZE);
  glUseProgram(0);

  floorProgram = MyShader::CreateProgram();
  MyShader::AttachVertexShader(floorProgram, floorVertexShader);
  MyShader::AttachFragmentShader(floorProgram, floorFragmentShader);
  MyShader::LinkProgram(floorProgram);
  glUseProgram(floorProgram);
  vFloorPosition = glGetAttribLocation(floorProgram, "vPosition");
  glEnableVertexAttribArray(vFloorPosition);
  glVertexAttribPointer(vFloorPosition, 3, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,pos));
  glUseProgram(0);

  glClearColor(0.0, 0.0, 0.0, 1.0);

  Restart();
}

void Game::Restart()
{
  y           =  0;
  x           =  0;
  speed       =  0;
  speedVect   = -0.0023;
  gameOver    =  0;
  gameStarted =  0;
  impulse     =  0;
  blockPos    =  1.5;
  score       =  0;
  level       =  START_LEVEL;

  for(int i=0; i<MAX_COLUMNS; i++)
    gaps[i]=Rand()-0.5; // easy at start

  glUseProgram(birdProgram);
  glUniform1f(vRadius, (float)1.0);
  glUseProgram(0);

//  glUseProgram(gapProgram);
//  glUniform1f(vLevel, (float)level);
//  glUseProgram(0);

  gettimeofday(&lastTime, NULL);

}

void Game::Pause()
{
  if(audio != nullptr) audio->Pause();
  pause = 1;
}

void Game::Resume()
{
  if(audio != nullptr) audio->Continue();
  Untap();
  gettimeofday(&lastTime, NULL);
  pause = 0;
}

void Game::Resize(int w, int h)
{
  glViewport(0, 0, w, h);
  if(h>0)
  {
    yMulValue = float(w)/h;
    glUseProgram(birdProgram);
    glUniform1f(vMul, yMulValue);
    glUseProgram(0);
  }
}

void Game::Tap()
{
  impulse = 1;

  if(!gameOver)
    gameStarted=1;
}

inline void Game::Untap()
{
  impulse = 0;
}

void Game::Render()
{
  if(pause) return;

  struct timeval now;
  gettimeofday(&now, NULL);
  float delta = (now.tv_sec - lastTime.tv_sec) * 1000000 + ((int)now.tv_usec - (int)lastTime.tv_usec);
  lastTime = now;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(impulse) {impulse = 0; speed = -TAP_IMPULSE;}
  if(gameStarted)
  {
    speed+=delta*ACCELERATION;
    float deltaX = delta*H_SPEED;
    if(x>-0.33)
    {
      x=x-deltaX;
      if(x<-0.34) x=-0.34;
    }
    if(!gameOver)
    {
      blockPos -= deltaX;
      if(score==0 && blockPos <= -0.34) score = 1;
      if(blockPos < -1-COLUMN_HALFWIDTH)
      {
        blockPos += SEGMENT;
        for(int i=0;i<MAX_COLUMNS-1;i++)
          gaps[i]=gaps[i+1];
//        gaps[MAX_COLUMNS-1]=(Rand()*1.8-0.9)*(1-2*BIRD_RADIUS);
//        gaps[MAX_COLUMNS-1]=Rand()-0.5;
        gaps[MAX_COLUMNS-1]=(Rand()*1.4-0.7)*(1-2*BIRD_RADIUS);
        score++;
        if(GetLevel(score) != level)
          ChangeLevel();
//        if (score % 2 == 0) ChangeLevel();
      }
    }
  }
  if(!gameOver) y-=delta/27000*speed;
  if(!(gameStarted||gameOver))
  {
    y += speedVect * delta / 253427; // greater -> slower initial animation
    if(y>0.008) {y=0.008; speedVect=-0.0095;}
    if(y<-0.006) {y=-0.006; speedVect=0.011;}
  }

  if(y > 1+BIRD_RADIUS*0.9) { y = 1+BIRD_RADIUS*0.9; speed=0; }

  if(y < BIRD_RADIUS+FLOOR_HEIGHT-1)
  {
    y = BIRD_RADIUS+FLOOR_HEIGHT-1;
    GameOver();
  }

  if(!gameOver)
    for(int i=0; i<MAX_COLUMNS-1; i++)
      if(Collision(blockPos+SEGMENT*i-COLUMN_HALFWIDTH, blockPos+SEGMENT*i+COLUMN_HALFWIDTH, gaps[i]))
        GameOver();

  glUseProgram(birdProgram);
  float deltaGameOver = 1.0;
  if(gameOver)
  {
    gameOverTime += delta;
    deltaGameOver = (1000000.0 - gameOverTime)/1000000.0;
    if(deltaGameOver<=0)
    {
      glClearColor(0.0, 0.0, 0.0, 1.0);
      deltaGameOver = 1.0;
      Restart();
    }
    else glClearColor((1.0-deltaGameOver)/1.2, 0.0, 0.0, 1.0);
    glUniform1f(vRadius,deltaGameOver);
  }
  glUniform4f(vOffset, x, y, 0.0, 0.0);
  glUniform1f(vSpeed, speed);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glUseProgram(gapProgram);
  for(int i=0; i<MAX_COLUMNS; i++)
  {
    glUniform1f(vLevel, (float)GetLevel(score+i-1));
    glUniform1f(vOffsetX, blockPos + SEGMENT * i);
    glUniform1f(vGap, gaps[i]);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
  }

  glUseProgram(floorProgram);
  glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

  if(audio!=nullptr)
  {
    if(!gameOver) audio->MakeNoise((unsigned)((y+1.0)/2*14000));
    else audio->Clear(); //fixme (iteration)
  }
}

bool Game::Collision(float x0, float x1, float y0)
{
  // based on http://stackoverflow.com/a/1879223/5920627

  float closestX = Clamp(x, x0, x1);
  float distanceX = x - closestX;

  if(distanceX < BIRD_RADIUS/yMulValue) distanceX*=yMulValue;

  float dx2 = distanceX*distanceX;

  float closestY = Clamp(y, -2, y0-GAP_HALFSIZE);
  float distanceY = y - closestY;
  if (dx2+distanceY*distanceY < BIRD_RADIUS*BIRD_RADIUS)
    return true;

  closestY = Clamp(y, y0+GAP_HALFSIZE, 2);
  distanceY = y - closestY;
  return dx2+distanceY*distanceY < BIRD_RADIUS*BIRD_RADIUS;
}

inline float Game::Clamp(float x, float a, float b)
{
  // http://www.gamedev.net/topic/473207-clamping-a-value-to-a-range-in-c-quickly/#entry4105200

  return x < a ? a : (x > b ? b : x);
}

inline float Game::Rand()
{
  return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void Game::GameOver()
{
  gameOverTime = 0;
  gameOver=1;
  char msg[40];
  sprintf(msg, "Game over: %d", score);
  MyCallback::Toast(msg);
}

void Game::ChangeLevel()
{
  level=GetLevel(score);
  if(level>maxLevel) maxLevel = level;
  if(level==0)
    MyCallback::Toast("You win!");
  else
  {
    char msg[32];
    sprintf(msg, "Level %d", level + 1);
    MyCallback::Toast(msg);
  }
  glUseProgram(gapProgram);
  glUniform1f(vLevel, (float)level);
  glUseProgram(0);
}

inline int Game::GetLevel(int newScore)
{
  int newLevel = newScore / NEXT_LEVEL_SCORE;
  if(newLevel>=20) newLevel = 0;
  return newLevel;
}