#include "soundplayer.h"

short SoundPlayer::buffer[NEXT_LEVEL_SCORE*MISS_SAMPLE_LENGTH];
short SoundPlayer::bufferd[D_SAMPLE_LENGTH];

void SoundPlayer::Init()
{
  createEngine();
  createBufferQueueAudioPlayer();

  {
    float r = (float(rand()) / float(RAND_MAX)-0.5) * 2.0;
    for(int j=0; j<D_SAMPLE_LENGTH; j++)
    {
//    bufferd[j]=((rand()-(RAND_MAX/2)) * 2) % (32768 * (D_SAMPLE_LENGTH-j) / D_SAMPLE_LENGTH);

      float a = (float(D_SAMPLE_LENGTH)-float(j)-1.0) / float(D_SAMPLE_LENGTH);
//      float mult = (exp(a)-1)*19000.0;
      float mult = (exp(a)-1)*8000.0;

      bufferd[j] = r*mult;

      if(j % (8 + j * 29 / D_SAMPLE_LENGTH) == 4)
        r = (float(rand()) / float(RAND_MAX)-0.5) * 2.0;
    }
  }

  for(int i=0; i<NEXT_LEVEL_SCORE; i++)
    for(int j=0; j<MISS_SAMPLE_LENGTH; j++)
    {
      float twice = 1;
//      if(j>=MISS_SAMPLE_LENGTH>>2) twice=2;

      float a = (float(MISS_SAMPLE_LENGTH)-float(j)-1.0) / float(MISS_SAMPLE_LENGTH);

      float slide = 0.999 + 0.001 * a;

//      float tune = pow(1.0594630943592952645618252949463,i);
//      float tune = pow(1.0293022366434920287823718007739,i);
      float tune = pow(1.01,i);
      float step = (float)(j) / float(SAMPLE_RATE);

      float sine = sin(2.0 * PI * MISS_BASE_FREQ * tune * step * twice * slide);
//      float mult = 32767.0*(MISS_SAMPLE_LENGTH-j)/MISS_SAMPLE_LENGTH;

//      float a = (float(MISS_SAMPLE_LENGTH)-float(j)-1.0) * 22025.0 / float(MISS_SAMPLE_LENGTH) + 1;
//      float mult = 3276.7 * log(a);

//      float a = (float(MISS_SAMPLE_LENGTH)-float(j)-1.0) / float(MISS_SAMPLE_LENGTH) + 1;
//      float mult = 47272 * log(a);

      float mult = (exp(a)-1)*19000.0;
 
      buffer[i*MISS_SAMPLE_LENGTH + j] = (short)(sine * mult);
    }
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
void SoundPlayer::PlayMiss(int tune)
{
  tune = (tune + NEXT_LEVEL_SCORE - 1) % NEXT_LEVEL_SCORE;
  selectClip((short*)&buffer[tune*MISS_SAMPLE_LENGTH], MISS_SAMPLE_LENGTH*sizeof(short));
//  MyCallback::Toast("PLAY");
}

void SoundPlayer::PlayD()
{
  selectClip(bufferd, D_SAMPLE_LENGTH*sizeof(short));
//  MyCallback::Toast("PLAY");
}
