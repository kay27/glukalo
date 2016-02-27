#include <unistd.h>
#include <cstdlib>
#include "gles2game.h"

void Game::Game()
{
  firstRun = 1;
  Init();
}

void Game::Init()
{
  if(firstRun)
    Toast("Tap to play");
  firstRun = 0;

  Restart();
}

void Game::Restart()
{
  gameOver    =  0;
  yOffs       =  0;
  xOffs       =  0;
  speed       =  0;
  gameStarted =  0;
  vect        = -0.0023;

  {
    lock_guard<mutex> lock(gapListMutex);
    for (int i = 0; i < gaps.size(); i++)
      delete gaps[i];
    gaps.clear();
  }

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

void Game::Tap()
{
  bird.Tap();

  if(gameOver)
    Restart(); 
  else
    started = 1;
}

void Game::Render()
{
  if(pause) return;

  struct timeval now;
  gettimeofday(&now, NULL);
  float delta = (now.tv_sec - lastTime.tv_sec) * 1000000 + ((int)now.tv_usec - (int)lastTime.tv_usec);
  lastTime = now;

  glClearColor(0.0, 0.0, (yOffs+1)/3.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  bird.Render(delta);

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
}

bool Game::Collision(Gap &gap)
{ // based on http://stackoverflow.com/a/1879223/5920627

  // Find the closest point to the circle within the rectangle
  float closestX = clamp(bird.xOffs, gap.x - GAP_HALF_WIDTH, gap.x + GAP_HALF_WIDTH);
  float closestY = clamp(bird.yOffs, gap.y - GAP_HALF_HEIGHT, gap.y + GAP_HALF_HEIGHT);

  // Calculate the distance between the circle's center and this closest point
  float distanceX = bird.xOffs - closestX;
  float distanceY = bird.yOffs - closestY;

  // If the distance is less than the circle's radius, an intersection occurs
  return (distanceX * distanceX) + (distanceY * distanceY)  <  (circle.Radius * circle.Radius);
}

inline bool Game::Clamp(float x, float a, float b)
{ // http://www.gamedev.net/topic/473207-clamping-a-value-to-a-range-in-c-quickly/#entry4105200
  return x < a ? a : (x > b ? b : x);
}

inline void Bird::Tap()
{
  impulse = 1;
}

inline void Bird::Untap()
{
  impulse = 0;
}

void Bird::Render()
{
  if(impulse) {impulse=0; speed=-0.06;}
  if(gameSttarted)
  {
    speed+=delta/5000000;
    if(xOffs>-0.33)
    {
      xOffs=xOffs-delta/2000000;
      if(xOffs<-0.34) xOffs=-0.34;
    }
  }
  if(!gameOver) yOffs-=delta/27000*speed;
  if(!(gameStarted||gameOver))
  {
    yOffs+=vect*delta/253427; // more: slower initial animation
    if(yOffs>0.008) {yOffs=0.008; vect=-0.0095;}
    if(yOffs<-0.006) {yOffs=-0.006; vect=0.011;}
  }

  if(yOffs<-0.8998) if(!gameOver) { yOffs=-0.8998; gameOver=1; Toast("Game over"); }
  if(yOffs>1.09) { yOffs=1.09; speed=0; }

    glUniform4f(yoffset, xOffs, yOffs, 0, 0);
    glUniform1f(vSpeed, speed);
//    glVertexAttribPointer(color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex) /*stride*/, (void*)offsetof(Vertex,rgba));

//    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

//    glDisableVertexAttribArray(1);
//    glDisableVertexAttribArray(0);

//    glDisableClientState(GL_VERTEX_ARRAY);



}