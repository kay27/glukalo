#include "game.h"

void Game::InitAudio()
{
/*
  audio = new MyAudio();

  if(audio!=nullptr)
    if(!audio->Play())
      audio = nullptr;

  if(audio==nullptr)
    MyCallback::Toast("Failed to start audio");
*/

  audio = nullptr;
}

void Game::PauseAudio()
{
  if(audio != nullptr)
    audio->Pause();
}

void Game::ResumeAudio()
{
  if(audio != nullptr)
    audio->Continue();
}

void Game::PlayAudio()
{
  if(audio!=nullptr)
  {
    if(!gameOver) audio->MakeNoise((unsigned)((y+1.0)/2*14000));
    else audio->Clear(); //fixme (iteration)
  }
}
