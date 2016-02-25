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
  gameOver  =  0;
  yOffs     =  0;
  xOffs     =  0;
  speed     =  0;
  impulse   =  0;
  started   =  0;
  vect      = -0.0023;

  gettimeofday(&lastTime, NULL);

  if(firstTime)
    Toast("Tap to play");

  firstTime = 0;
}

void Game::Resume()
{
  impulse  = 0;
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

