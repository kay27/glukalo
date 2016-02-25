#include "gles2game.h"

void Game::Game()
{
  firstRun = 1;
  Init();
}

void Game::Init()
{
  gettimeofday(&lastTime, NULL);

}