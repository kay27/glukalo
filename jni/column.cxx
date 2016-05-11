#include "game.h"

GLint Column::gapProgram            =  0;
GLint Column::vGapPosition          =  0;
GLint Column::vGapTextureCoordinate =  0;
GLint Column::vGap                  =  0;
GLint Column::vOffsetX              =  0;
GLint Column::vHalfSize             =  0;
GLint Column::vLevel                =  0;
GLint Column::lastSwingVector       = -1;
int   Column::gapCount              =  0;
int   Column::lastLevel             = -1;

bool Column::Collision(float x0, float y0, float yMulValue)
{
  float closestX = Clamp(x0, x - COLUMN_HALFWIDTH, x + COLUMN_HALFWIDTH);
  float distanceX = x0 - closestX;

  if(distanceX < BIRD_RADIUS/yMulValue) distanceX *= yMulValue;

  float dx2 = distanceX * distanceX;

  float closestY = Clamp(y0, -2, y - halfSize);
  float distanceY = y0 - closestY;
  if (dx2 + distanceY*distanceY < BIRD_RADIUS*BIRD_RADIUS)
    return true;

  closestY = Clamp(y0, y + halfSize, 2);
  distanceY = y0 - closestY;
  return dx2 + distanceY*distanceY < BIRD_RADIUS*BIRD_RADIUS;
}

int Column::Collision(float x0, float y0, float r0, float yMulValue)
{
  float closestX = Clamp(x0, x - COLUMN_HALFWIDTH, x + COLUMN_HALFWIDTH);
  float distanceX = x0 - closestX;

  if(distanceX < r0/yMulValue) distanceX *= yMulValue;

  float dx2 = distanceX * distanceX;

  float closestY = Clamp(y0, -2, y - halfSize);
  float distanceY = y0 - closestY;
  if (dx2 + distanceY*distanceY < r0*r0)
    return -1;

  closestY = Clamp(y0, y + halfSize, 2);
  distanceY = y0 - closestY;
  if(dx2 + distanceY*distanceY < r0*r0)
    return 1;

  return 0;
}

void Column::OnCreate()
{
  y = (RandFloat() * GAP_MAX_OFFSET * 2 - GAP_MAX_OFFSET) * (1 - 2 * BIRD_RADIUS);

  level %= NUMBER_OF_LEVELS;

  if(level != lastLevel)
  {
    gapCount = 0;
    lastLevel = level;
  }
  else
    if((++gapCount) >= NEXT_LEVEL_SCORE/2)
      jawsVector = -1;

  swingSpeed=((float)(score/3))*0.001;
  if(swingSpeed>0.13)
    swingSpeed=0.13;

  jawsSpeed=((float)(score/7))*0.002;
  if(jawsSpeed>0.11)
    jawsSpeed=0.11;

  lastSwingVector = swingVector;

}

void Column::Restart(float x_, int score_)
{
  x           = x_;
  score       = score_;
  level       = score / NEXT_LEVEL_SCORE;
  passed      = 0;
  freeze      = 0;
  swingVector = -lastSwingVector;
  jawsVector  = 1;
  halfSize    = GAP_HALFSIZE;
  OnCreate();
}

float Column::Move(float dx)
{
  x -= dx;

  float delta = dx / H_SPEED;
  if(freeze)
    delta /= 2.7;

  y += swingVector * swingSpeed * delta / 500000;
  if(y > GAP_MAX_OFFSET*(1-2*BIRD_RADIUS)) swingVector = -1;
  else if(y < -GAP_MAX_OFFSET*(1-2*BIRD_RADIUS)) swingVector = 1;

  halfSize += jawsVector * jawsSpeed * delta / 400000;
  if(halfSize > GAP_HALFSIZE_MAX) jawsVector = -1;
  else if(halfSize < GAP_HALFSIZE_MIN) jawsVector = 1;

  return x;
}

void Column::Render()
{
  glUseProgram(gapProgram);
  glUniform1f(vLevel, level);
  glUniform1f(vOffsetX, x);
  glUniform1f(vGap, y);
  glUniform1f(vHalfSize, halfSize);
  glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
}

void Column::Init()
{
  gapProgram = MyShader::CreateProgram();
  MyShader::AttachVertexShader(gapProgram, gapVertexShader);
  MyShader::AttachFragmentShader(gapProgram, gapFragmentShader);
  MyShader::LinkProgram(gapProgram);
  glUseProgram(gapProgram);
  vGapPosition          = glGetAttribLocation(gapProgram, "vPosition");
  vGapTextureCoordinate = glGetAttribLocation(gapProgram, "vTextureCoordinate");
  vOffsetX              = glGetUniformLocation(gapProgram, "vOffsetX");
  vGap                  = glGetUniformLocation(gapProgram, "vGap");
  vHalfSize             = glGetUniformLocation(gapProgram, "vHalfSize");
  vLevel                = glGetUniformLocation(gapProgram, "vLevel");
  glEnableVertexAttribArray(vGapPosition);
  glEnableVertexAttribArray(vGapTextureCoordinate);
  glVertexAttribPointer(vGapPosition, 3, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,pos));
  glVertexAttribPointer(vGapTextureCoordinate, 2, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,txtcrds));
  glUseProgram(0);
}

bool Column::Pass()
{
  if (passed) return false;
  passed = (x <= -FLY_BACK);
  return passed;
}
