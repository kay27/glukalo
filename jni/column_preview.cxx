#include "game.h"

GLint ColumnPreview::gapProgram            =  0;
GLint ColumnPreview::vGapPosition          =  0;
GLint ColumnPreview::vGapTextureCoordinate =  0;
GLint ColumnPreview::vGap                  =  0;
GLint ColumnPreview::vHalfSize             =  0;
GLint ColumnPreview::vOffs                 =  0;
GLint ColumnPreview::vLevel                =  0;

void ColumnPreview::Render(float x, float y, int level, bool portrait)
{
  glUseProgram(gapProgram);
  glUniform1f(vLevel, level);
  glUniform4f(vOffs, x, y, 0, 0);
  glUniform1f(vGap, RandFloat());
  glUniform1f(vHalfSize, 0.2);
  if(portrait)
    glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);
  else
    glDrawArrays(GL_TRIANGLE_STRIP, 32, 4);
}

void ColumnPreview::Init()
{
  gapProgram = MyShader::CreateProgram();
  MyShader::AttachVertexShader(gapProgram, gapBoxVertexShader);
  MyShader::AttachFragmentShader(gapProgram, gapFragmentShader);
  MyShader::LinkProgram(gapProgram);
  glUseProgram(gapProgram);
  vGapPosition          = glGetAttribLocation(gapProgram, "vPosition");
  vGapTextureCoordinate = glGetAttribLocation(gapProgram, "vTextureCoordinate");
  vOffs                 = glGetUniformLocation(gapProgram, "vOffs");
  vGap                  = glGetUniformLocation(gapProgram, "vGap");
  vHalfSize             = glGetUniformLocation(gapProgram, "vHalfSize");
  vLevel                = glGetUniformLocation(gapProgram, "vLevel");
  glEnableVertexAttribArray(vGapPosition);
  glEnableVertexAttribArray(vGapTextureCoordinate);
  glVertexAttribPointer(vGapPosition, 3, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,pos));
  glVertexAttribPointer(vGapTextureCoordinate, 2, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,txtcrds));
  glUseProgram(0);
}
