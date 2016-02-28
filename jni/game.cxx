#include "game.h"

Game::Game()
{
  firstRun = 1;
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(birdVertices), &birdVertices[0], GL_STATIC_DRAW);

  vPosition          = glGetAttribLocation(birdProgram, "vPosition");
  vTextureCoordinate = glGetAttribLocation(birdProgram, "vTextureCoordinate");

  vColor             = glGetUniformLocation(birdProgram, "vColor");
  vOffset            = glGetUniformLocation(birdProgram, "vOffset");
  vRadius            = glGetUniformLocation(birdProgram, "vRadius");
  vMul               = glGetUniformLocation(birdProgram, "vMul");
  vSpeed             = glGetUniformLocation(birdProgram, "vSpeed");

  glUseProgram(birdProgram);
  glEnableVertexAttribArray(vPosition);
  glEnableVertexAttribArray(vTextureCoordinate);
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, false, sizeof(BirdVertex), (void*)offsetof(BirdVertex,pos));
  glVertexAttribPointer(vTextureCoordinate, 2, GL_FLOAT, false, sizeof(BirdVertex), (void*)offsetof(BirdVertex,txtcrds));
  glUniform4f(vColor, 0, 0.6, 1, 1);
  glUniform1f(vRadius, (float)BIRD_RADIUS);
  glDisableVertexAttribArray(vTextureCoordinate);
  glDisableVertexAttribArray(vPosition);
  glUseProgram(0);

  Restart();
}

void Game::Restart()
{
  y           =  0;
  x           =  0;
  speed       =  0;
  vect        = -0.0023;
  gameOver    =  0;
  gameStarted =  0;

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
  bird->Untap();
  gettimeofday(&lastTime, NULL);
  pause = 0;
}

void Game::Resize(int w, int h)
{
  if(h>0)
  {
    yMulValue = float(w)/h;
    glUseProgram(birdProgram);
    glUniform1f(ymul, yMulValue);
    glUseProgram(0);
  }
}

void Game::Tap()
{
  impulse = 1;

  if(gameOver)
    Restart(); 
  else
    started = 1;
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

  glClearColor(0.0, 0.0, (y+1)/3.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//  bird.Render(delta);

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
    y+=vect*delta/253427; // more: slower initial animation
    if(y>0.008) {y=0.008; vect=-0.0095;}
    if(y<-0.006) {y=-0.006; vect=0.011;}
  }

  if(y<-0.8998) if(!gameOver) { y=-0.8998; gameOver=1; Toast("Game over"); }
  if(y>1.09) { y=1.09; speed=0; }

  glUseProgram(birdProgram);
  glUniform4f(vOffset, x, y, 0.0, 0.0);
  glUniform1f(vSpeed, speed);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glUseProgram(0);


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

  closestY = clamp(bird.y, gap.y - GAP_HALF_HEIGHT, -2);
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
    y+=vect*delta/253427; // more: slower initial animation
    if(y>0.008) {y=0.008; vect=-0.0095;}
    if(y<-0.006) {y=-0.006; vect=0.011;}
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