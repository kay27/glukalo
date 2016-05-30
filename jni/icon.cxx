#include "game.h"

float Icon::vm = 1;

void Icon::Init(const char * vs, const char * fs, float x_, float y_, float state_)
{
  x = x_;
  y = y_;
  state = state_;
  prog = MyShader::CreateProgram();
  MyShader::AttachVertexShader(prog, vs);
  MyShader::AttachFragmentShader(prog, fs);
  MyShader::LinkProgram(prog);
  glUseProgram(prog);
  vPos  = glGetAttribLocation(prog, "vPos");
  vTC   = glGetAttribLocation(prog, "vTC");
  vOffs = glGetUniformLocation(prog, "vOffs");
  glEnableVertexAttribArray(vPos);
  glEnableVertexAttribArray(vTC);
  glVertexAttribPointer(vPos, 3, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,pos));
  glVertexAttribPointer(vTC, 2, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,txtcrds));
  glUseProgram(0);
}

void Icon::Init(const char * vs, const char * fs, float x_, float y_)
{
  Init(vs, fs, x_, y_, 0);
}

void Icon::Move(float x_, float y_)
{
  x = x_;
  y = y_;
}

void Icon::Render()
{
  glUseProgram(prog);
  glUniform4f(vOffs, x, y, vm, state);
  glDrawArrays(GL_TRIANGLE_STRIP, 24, 4);
}

void Icon::Resize(float newMulValue)
{
  vm = newMulValue;
}

int Icon::Tap(float tx, float ty)
{
  return (tx>=x-ICON_SIZE/vm/2) && (tx<=x+ICON_SIZE/vm/2) && (ty>=y-ICON_SIZE/2) && (ty<=y+ICON_SIZE/2);
}
