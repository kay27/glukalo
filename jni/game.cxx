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

  glGenBuffers(2, vb);

  glBindBuffer(GL_ARRAY_BUFFER, vb[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(birdVertices), &birdVertices[0], GL_STATIC_DRAW);

  birdProgram = MyShader::CreateProgram();
  MyShader::AttachVertexShader(birdProgram, birdVertexShader);
  MyShader::AttachFragmentShader(birdProgram, birdFragmentShader);
  MyShader::LinkProgram(birdProgram);

  vPosition          = glGetAttribLocation(birdProgram, "vPosition");
  vTextureCoordinate = glGetAttribLocation(birdProgram, "vTextureCoordinate");

  vColor  = glGetUniformLocation(birdProgram, "vColor");
  vOffset = glGetUniformLocation(birdProgram, "vOffset");
  vRadius = glGetUniformLocation(birdProgram, "vRadius");
  vMul    = glGetUniformLocation(birdProgram, "vMul");
  vSpeed  = glGetUniformLocation(birdProgram, "vSpeed");

  glEnableVertexAttribArray(vPosition);
  glEnableVertexAttribArray(vTextureCoordinate);
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, false, sizeof(BirdVertex), (void*)offsetof(BirdVertex,pos));
  glVertexAttribPointer(vTextureCoordinate, 2, GL_FLOAT, false, sizeof(BirdVertex), (void*)offsetof(BirdVertex,txtcrds));
  glUniform4f(vColor, 0, 0.6, 1, 1);
  glUniform1f(vRadius, (float)BIRD_RADIUS);
//  glDisableVertexAttribArray(vTextureCoordinate);
//  glDisableVertexAttribArray(vPosition);

  glBindBuffer(GL_ARRAY_BUFFER, vb[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(gapVertices), &gapVertices[0], GL_STATIC_DRAW);

  gapProgram = MyShader::CreateProgram();
  MyShader::AttachVertexShader(gapProgram, gapVertexShader);
  MyShader::AttachFragmentShader(gapProgram, gapFragmentShader);
  MyShader::LinkProgram(gapProgram);

  vGapPosition          = glGetAttribLocation(gapProgram, "vPosition");
  vGapTextureCoordinate = glGetAttribLocation(gapProgram, "vTextureCoordinate");

  vOffsetX              = glGetUniformLocation(birdProgram, "vOffsetX");
  vGap                  = glGetUniformLocation(birdProgram, "vGap");

//  glUseProgram(gapProgram);
  glEnableVertexAttribArray(vGapPosition);
  glEnableVertexAttribArray(vGapTextureCoordinate);
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, false, sizeof(BirdVertex), (void*)offsetof(BirdVertex,pos));
  glVertexAttribPointer(vTextureCoordinate, 2, GL_FLOAT, false, sizeof(BirdVertex), (void*)offsetof(BirdVertex,txtcrds));
//  glDisableVertexAttribArray(vTextureCoordinate);
//  glDisableVertexAttribArray(vPosition);
//  glUseProgram(0);

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

/*
  {
    lock_guard<mutex> lock(gapListMutex);
    for (int i = 0; i < gaps.size(); i++)
      delete gaps[i];
    gaps.clear();
  }
*/
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
    if(x>-0.33)
    {
      x=x-delta/2000000;
      if(x<-0.34) x=-0.34;
    }
  }
  if(!gameOver) y-=delta/27000*speed;
  if(!(gameStarted||gameOver))
  {
    y += speedVect * delta / 253427; // greater -> slower initial animation
    if(y>0.008) {y=0.008; speedVect=-0.0095;}
    if(y<-0.006) {y=-0.006; speedVect=0.011;}
  }

  if(y<-0.8998) if(!gameOver) { y=-0.8998; gameOver=1; MyCallback::Toast("Game over"); }
  if(y>1.09) { y=1.09; speed=0; }

  glUseProgram(birdProgram);
//  glEnableVertexAttribArray(vPosition);
//  glEnableVertexAttribArray(vTextureCoordinate);
  glUniform4f(vOffset, x, y, 0.0, 0.0);
  glUniform1f(vSpeed, speed);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//  glUseProgram(0);

  glUseProgram(gapProgram);
  glUniform1f(vOffsetX, 0.3);
  glUniform1f(vGap, 0.44);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


/*

  {
    lock_guard<mutex> lock(gapListMutex);
    for (int i = 0; i < gaps.size(); i++)
      if(gaps[i])
      {
        gaps[i]->Render(delta);
        if(Collision(*(gaps[i])))
          gameOver = 1;
      }
  }
*/

}
/*
bool Game::Collision(Gap &gap)
{ // based on http://stackoverflow.com/a/1879223/5920627
  float closestX = Clamp(bird.x, gap.x - GAP_HALF_WIDTH, gap.x + GAP_HALF_WIDTH);
  float distanceX = bird.x - closestX;
  float dx2 = distanceX*distanceX;

  float closestY = Clamp(bird.y, 2, gap.y + GAP_HALF_HEIGHT);
  float distanceY = bird.y - closestY;
  if (dx2+distanceY*distanceY  <  BIRD_RADIUS*BIRD_RADIUS)
    return true;

  closestY = Clamp(bird.y, gap.y - GAP_HALF_HEIGHT, -2);
  distanceY = bird.y - closestY;
  return
    dx2+distanceY*distanceY  <  BIRD_RADIUS*BIRD_RADIUS;
}
*/
inline bool Game::Clamp(float x, float a, float b)
{ // http://www.gamedev.net/topic/473207-clamping-a-value-to-a-range-in-c-quickly/#entry4105200
  return x < a ? a : (x > b ? b : x);
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