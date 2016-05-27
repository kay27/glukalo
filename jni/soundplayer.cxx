#include "soundplayer.h"

short SoundPlayer::buffer[NEXT_LEVEL_SCORE*MISS_SAMPLE_LENGTH];
short SoundPlayer::bufferd[D_SAMPLE_LENGTH];
short SoundPlayer::bonusBuffer[BONUS_SAMPLE_LENGTH];
short SoundPlayer::gameOverBuffer[GAMEOVER_SAMPLE_LENGTH];
short SoundPlayer::endOfautoPilotBuffer[ENDOFAUTOPILOT_SAMPLE_LENGTH];
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
    float tune = pow(1.02,i);
    float freq = MISS_BASE_FREQ * tune;
    freq = Clamp(freq, 20, 8000);
    for(int j=0; j<MISS_SAMPLE_LENGTH; j++)
    {
      float volume = (8020-freq) / 8000;
      float pos = BackNormalize(j, MISS_SAMPLE_LENGTH); //0...1-
      float fade = (exp(pos) - 1) * 19000.0;
      float step = (float)(j) / float(SAMPLE_RATE);
      float vibr = sin(2.0 * PI * pos * 2) * 0.01 + 1;
      float sine = sin(2.0 * PI * freq * step * vibr);
      buffer[i*MISS_SAMPLE_LENGTH + j] = (short)(sine * volume * fade);
    }
  }

  {
    float freq = 300;
    for(int j=0; j<BONUS_SAMPLE_LENGTH; j++)
    {
      float pos = BackNormalize(j, BONUS_SAMPLE_LENGTH); //0...1-
      float fade = (exp(pos) - 1) * 19000.0;
      float step = (float)(j) / float(SAMPLE_RATE);
      float vibr = sin(2.0 * PI * pos * 8) * 0.07 + 1;
      float sine = sin(2.0 * PI * freq * step * vibr);
      bonusBuffer[j] = (short)(sine * fade);
    }
  }

  {
    for(int j=0; j<GAMEOVER_SAMPLE_LENGTH; j++)
    {
      float pos = BackNormalize(j, GAMEOVER_SAMPLE_LENGTH); //0...1-
//      float fade = (exp(pos) - 1) * 9500.0;
      float fade = (exp(pos) - 1) * 1992.7;
      float step = (float)(j) / float(SAMPLE_RATE);
      float sine = sin(2.0 * PI * 2992.7 * step * (1.0-pos/33));
      gameOverBuffer[j] = (short)(fade * (RandFloatN()*0.2 + sine*0.8));
    }
  }

  {
    for(int j=0, i=64; j<ENDOFAUTOPILOT_SAMPLE_LENGTH; j++,j^=(i&192))
    {
//      endOfautoPilotBuffer[j] = (short)((j<<(j&7))&65535);
      endOfautoPilotBuffer[j] = (short)((j&i)<<7) - 4096;
//      i=i^192;
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

void SoundPlayer::PlayBonus()
{
  if(mute) return;
  selectClip(bonusBuffer, BONUS_SAMPLE_LENGTH*sizeof(short));
}

void SoundPlayer::PlayGameOver()
{
  if(mute) return;
  selectClip(gameOverBuffer, GAMEOVER_SAMPLE_LENGTH*sizeof(short));
}

void SoundPlayer::PlayEndOfAutoPilot()
{
  if(mute) return;
  selectClip(endOfautoPilotBuffer, ENDOFAUTOPILOT_SAMPLE_LENGTH*sizeof(short));
}
