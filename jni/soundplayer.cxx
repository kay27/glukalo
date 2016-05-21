#include "soundplayer.h"

short SoundPlayer::buffer[NEXT_LEVEL_SCORE*MISS_SAMPLE_LENGTH];
short SoundPlayer::bufferd[D_SAMPLE_LENGTH];
int SoundPlayer::mute = 0;

void SoundPlayer::Init()
{
  createEngine();
  createBufferQueueAudioPlayer();

  {
    float r = RandFloatN();
    for(int j=0; j<D_SAMPLE_LENGTH; j++)
    {
      float a = BackNormalize(j, D_SAMPLE_LENGTH);
      float mult = (exp(a)-1)*5777.77;

      bufferd[j] = r*mult;

      if(j % (120 + j * 199 / D_SAMPLE_LENGTH) == 99)
        r = RandFloatN();
    }
  }

  for(int i=0; i<NEXT_LEVEL_SCORE; i++)
  {
    float tune = pow(1.01,i);
    float twice = 1;
    for(int j=0; j<MISS_SAMPLE_LENGTH; j++)
    {
      float a = BackNormalize(j, MISS_SAMPLE_LENGTH);

      if(a<=0.5) if(twice<2.0) twice += 0.00001;
      if(twice>2.0) twice = 2.0;
      float mult = (exp(a)-1)*19000.0;
      float slide = 0.999 + 0.001 * a;

      float step = (float)(j) / float(SAMPLE_RATE);

      float sine = sin(2.0 * PI * MISS_BASE_FREQ * tune * step * twice * slide);
 
      buffer[i*MISS_SAMPLE_LENGTH + j] = (short)(sine * mult);
    }
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
  if(mute) return;
  selectClip(buffer, count);
}

void SoundPlayer::PlayMiss(int tune)
{
  if(mute) return;
  tune = (tune + NEXT_LEVEL_SCORE - 1) % NEXT_LEVEL_SCORE;
  selectClip((short*)&buffer[tune*MISS_SAMPLE_LENGTH], MISS_SAMPLE_LENGTH*sizeof(short));
}

void SoundPlayer::PlayD()
{
  if(mute) return;
  selectClip(bufferd, D_SAMPLE_LENGTH*sizeof(short));
}
