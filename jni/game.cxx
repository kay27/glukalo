#include "game.h"

Game::Game()
{
  firstRun = 1;

  yMulValue = 1;

  Init();
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
  glUniform1f(vRadius, (float)BIRD_RADIUS);
//  glDisableVertexAttribArray(vTextureCoordinate);
//  glDisableVertexAttribArray(vPosition);
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

  glEnableVertexAttribArray(vGapPosition);
  glEnableVertexAttribArray(vGapTextureCoordinate);
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,pos));
  glVertexAttribPointer(vTextureCoordinate, 2, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,txtcrds));
  glUniform1f(vHalfSize, (float)BIRD_RADIUS*2.0);
//  glDisableVertexAttribArray(vTextureCoordinate);
//  glDisableVertexAttribArray(vPosition);
  glUseProgram(0);

  floorProgram = MyShader::CreateProgram();
  MyShader::AttachVertexShader(floorProgram, floorVertexShader);
  MyShader::AttachFragmentShader(floorProgram, floorFragmentShader);
  MyShader::LinkProgram(floorProgram);
  glUseProgram(floorProgram);
  vFloorPosition = glGetAttribLocation(floorProgram, "vPosition");
  glEnableVertexAttribArray(vFloorPosition);
  glVertexAttribPointer(vFloorPosition, 3, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,pos));
//  glDisableVertexAttribArray(vFloorPosition);
  glUseProgram(0);

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
  for(int i=0; i<MAX_COLUMNS; i++)
    gaps[i]=Rand()-0.5; // easy at start

  gettimeofday(&lastTime, NULL);
}

void Game::Pause()
{
  pause = 1;
}

void Game::Resume()
{
  Untap();
  gettimeofday(&lastTime, NULL);
  pause = 0;
}

void Game::Resize(int w, int h)
{
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

  if(gameOver)
    Restart(); 
  else
    gameStarted = 1;
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

  glClearColor(0.0, 0.0, (y+1.0)/5.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(impulse) {impulse=0; speed=-0.06;}
  if(gameStarted)
  {
    speed+=delta/5000000;
    float deltaX = delta/2000000;
    if(x>-0.33)
    {
      x=x-deltaX;
      if(x<-0.34) x=-0.34;
    }
    if(!gameOver)
    {
      blockPos -= deltaX;
      if(blockPos < -1-COLUMN_HALFWIDTH)
      {
        blockPos += SEGMENT;
        for(int i=0;i<MAX_COLUMNS-1;i++)
          gaps[i]=gaps[i+1];
        gaps[MAX_COLUMNS-1]=(Rand()-0.5)*2*(1-2*BIRD_RADIUS*1.02);
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
//  if(y > 1) { y = 1; speed=0; }

  if(y < BIRD_RADIUS+FLOOR_HEIGHT-1)
  {
    y = BIRD_RADIUS+FLOOR_HEIGHT-1;
    GameOver();
  }

  if(!gameOver)
    for(int i=0; i<MAX_COLUMNS-1; i++)
      if(Collision(blockPos + SEGMENT * i, blockPos + SEGMENT * i + 2*COLUMN_HALFWIDTH, gaps[i]))
        GameOver();

  glUseProgram(birdProgram);
  glUniform4f(vOffset, x, y, 0.0, 0.0);
  glUniform1f(vSpeed, speed);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glUseProgram(floorProgram);
  glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

  glUseProgram(gapProgram);
  for(int i=0; i<MAX_COLUMNS; i++)
  {
    glUniform1f(vOffsetX, blockPos + SEGMENT * i);
    glUniform1f(vGap, gaps[i]);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
  }
}

bool Game::Collision(float x0, float x1, float y0)
{ // based on http://stackoverflow.com/a/1879223/5920627
  float closestX = Clamp(x, x0, x1);
  float distanceX = x - closestX;
  float dx2 = distanceX*distanceX;

  float closestY = Clamp(y, -2, y0-2*BIRD_RADIUS);
  float distanceY = y - closestY;
  if (dx2+distanceY*distanceY < BIRD_RADIUS*BIRD_RADIUS)
    return true;

  closestY = Clamp(y, y0+2*BIRD_RADIUS, 2);
  distanceY = y - closestY;
  return dx2+distanceY*distanceY < BIRD_RADIUS*BIRD_RADIUS;
}

inline bool Game::Clamp(float x, float a, float b)
{ // http://www.gamedev.net/topic/473207-clamping-a-value-to-a-range-in-c-quickly/#entry4105200
  return x < a ? a : (x > b ? b : x);
}

inline float Game::Rand()
{
  return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void Game::GameOver()
{
  gameOver=1;
  MyCallback::Toast("Game over");
}

/*
inline void Bird::Tap()
{
  impulse = 1;
}

inline void Bird::Untap()
{
  impulse = 0;
}
*/
/*
void Bird::Render()
{
  if(impulse) {impulse=0; speed=-0.06;}
  if(gameStarted)
  {
    speed+=delta/5000000;
    if(x>-0.33)
    {
      x=x-delta/2000000;
      if(x<-0.34) x=-0.34;
    }
  }
  if(!gameOver) y-=delta/27000*speed;
  if(!(gameStarted||gameOver))
  {
    y+=speedVect*delta/253427; // more: slower initial animation
    if(y>0.008) {y=0.008; speedVect=-0.0095;}
    if(y<-0.006) {y=-0.006; speedVect=0.011;}
  }

  if(y<-0.8998) if(!gameOver) { y=-0.8998; gameOver=1; Toast("Game over"); }
  if(y>1.09) { y=1.09; speed=0; }

  glUniform4f(vOffset, x, y, 0.0, 0.0);
  glUniform1f(vSpeed, speed);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


}
*/
/*
void Bird::Init()
{
}

Bird::Bird()
{
  Init();
}
*/