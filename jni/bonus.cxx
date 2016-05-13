#include "game.h"

GLint Bonus::program = 0;
GLint Bonus::vPos = 0;
GLint Bonus::vTC = 0;
GLint Bonus::vType = 0;
GLint Bonus::vOffs = 0;
float Bonus::vm = 1;

void Bonus::Init()
{
  program = MyShader::CreateProgram();
  MyShader::AttachVertexShader  (program, bonusVertexShader);
  MyShader::AttachFragmentShader(program, bonusFragmentShader);
  MyShader::LinkProgram(program);
  glUseProgram(program);
  vPos  = glGetAttribLocation (program, "vPos");
  vTC   = glGetAttribLocation (program, "vTC");
  vOffs = glGetUniformLocation(program, "vOffs");
  vType = glGetUniformLocation(program, "vType");
  glEnableVertexAttribArray(vPos);
  glEnableVertexAttribArray(vTC);
  glVertexAttribPointer(vPos, 3, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,pos));
  glVertexAttribPointer(vTC,  2, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,txtcrds));
}

void Bonus::Move(float delta, int antiGravity, int direction)
{
  angle += delta / 1000000;
  if(angle > 4) angle -= 2*3.1415926;
}

void Bonus::Resize(float newMulValue)
{
  vm = newMulValue;
}

void Bonus::Render()
{
  x = 0.5; y=0.5; //angle = 0.5;

  glUseProgram(program);
  glUniform4f(vOffs, x, y, vm, angle);
  glUniform1i(vType, 1);
  glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

  glUniform4f(vOffs, x, y-0.5, vm, angle);
  glUniform1i(vType, 2);
  glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

  glUniform4f(vOffs, x, y-1.0, vm, angle);
  glUniform1i(vType, 3);
  glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

}
