#include "game.h"

GLint Column::gapProgram            =  0;
GLint Column::vGapTextureCoordinate =  0;
GLint Column::vOffs                 =  0;
GLint Column::vLevel                =  0;

void ColumnPreview::Set(float x_, float y_, int level_)
{
  x           = x_;
  score       = score_;
  level       = level_;
  passed      = 0;
  freeze      = 0;
  swingVector = -lastSwingVector;
  jawsVector  = 1;
  halfSize    = GAP_HALFSIZE;
  OnCreate();
}

void ColumnPreview::Render()
{
  glUseProgram(gapProgram);
  glUniform1f(vLevel, level);
  glUniform4f(vOffs, x, y, 0, 0);
  glDrawArrays(GL_TRIANGLE_STRIP, 24, 4);
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

bool Column::Pass(int direction)
{
  if (passed) return false;
  if(direction == 1)
    passed = (x <= -FLY_BACK);
  else
    passed = (x >= FLY_BACK);
  return passed;
}
