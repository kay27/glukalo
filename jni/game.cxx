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
  impulse     =  0;
  gameStarted =  0;
  vect        = -0.0023;

  for (int i = gaps.size()-1; i>=0; i--)
  {
    Gap *gap = gaps[i];

    delete gaps[i];
  }
  gaps.clear();

  gettimeofday(&lastTime, NULL);
}

void Game::Resume()
{
  impulse = 0;
  gettimeofday(&lastTime, NULL);
}

void Game::Tap()
{
  impulse = 1;

  if(gameOver)
    Restart(); 
  else
    started = 1;
}

void Game::Render()
{
  struct timeval now;
  gettimeofday(&now, NULL);
  float delta = (now.tv_sec - lastTime.tv_sec) * 1000000 + ((int)now.tv_usec - (int)lastTime.tv_usec);
  lastTime = now;

  glClearColor(myrand()/50, myrand()/20, myrand()/8, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(impulse) {impulse=0; speed=-0.06;}
    if(started)
    {
      speed+=delta/5000000;
      if(xOffs>-0.33)
      {
        xOffs=xOffs-delta/2000000;
        if(xOffs<-0.34) xOffs=-0.34;
      }
    }
    if(!gameOver) yOffs-=delta/27000*speed;
    if(!(started||gameOver))
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
