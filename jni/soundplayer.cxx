#include "soundplayer.h"

short SoundPlayer::buffer[MISS_SAMPLE_LENGTH];

void SoundPlayer::Init()
{
  createEngine();
  createBufferQueueAudioPlayer();

  for(int j=0; j<MISS_SAMPLE_LENGTH; j++)
    buffer[j] = 32767*(MISS_SAMPLE_LENGTH-j)/MISS_SAMPLE_LENGTH*sin(2.0*3.14159*float(j)/SAMPLE_RATE*400);
/*
  float freq = 200;
  for(int i=0; i<NEXT_LEVEL_SCORE; i++)
  {
    short * buffer = &missSound[i];
    for(int j=0; j<MISS_SAMPLE_LENGTH; j++,buffer++)
      *buffer = sin(2.0*3.14159*float(j)*freq/MISS_SAMPLE_TIME
  }
*/
}

void SoundPlayer::Destroy()
{
  shutdown();
}

void SoundPlayer::Pause()
{
  Destroy();
}

void SoundPlayer::Resume()
{
  Init();
}

void SoundPlayer::Play(short * buffer, int count)
{
  selectClip(buffer, count);
}

//#include "jni2game.h"
void SoundPlayer::PlayMiss()
{
  selectClip(buffer, MISS_SAMPLE_LENGTH);
//  MyCallback::Toast("PLAY");
}
