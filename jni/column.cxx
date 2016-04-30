#include "config.h"
#include "game.h"

GLint Column::gapProgram            =  0;
GLint Column::vGapPosition          =  0;
GLint Column::vGapTextureCoordinate =  0;
GLint Column::vGap                  =  0;
GLint Column::vOffsetX              =  0;
GLint Column::vHalfSize             =  0;
GLint Column::vLevel                =  0;
GLint Column::lastSwingVector       =  0;
int   Column::gapCount              =  0;
int   Column::lastLevel             = -1;

Column::Column(float x_, int level_)
: x(x_), level(level_), passed(0), swingVector(-lastSwingVector), jawsVector(1)
{
  y = (RandFloat() * GAP_MAX_OFFSET * 2 - GAP_MAX_OFFSET) * (1 - 2 * BIRD_RADIUS);

  lastSwingVector = swingVector;

  if(level != lastLevel)
  {
    gapCount = 0;
    lastLevel = level;
  }
  else
    if((++gapCount) >= NEXT_LEVEL_SCORE/2)
      jawsVector = -1;
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
  passed = (x <= -0.34);
  return passed;
}
