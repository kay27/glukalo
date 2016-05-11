#include "game.h"

GLint Missile::program = 0;
GLint Missile::vPos = 0;
GLint Missile::vTC = 0;
GLint Missile::vOffs = 0;
GLint Missile::vMul = 0;

void Missile::Explode()
{
  if(phase != 1) return;
  phase = 2;
}

void Missile::Init()
{
  program = MyShader::CreateProgram();
  MyShader::AttachVertexShader  (program, missileVertexShader);
  MyShader::AttachFragmentShader(program, missileFragmentShader);
  MyShader::LinkProgram(program);
  glUseProgram(program);
  vPos  = glGetAttribLocation (program, "vPos");
  vTC   = glGetAttribLocation (program, "vTC");
  vOffs = glGetUniformLocation(program, "vOffs");
  glEnableVertexAttribArray(vPos);
  glEnableVertexAttribArray(vTC);
  glVertexAttribPointer(vPos, 3, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,pos));
  glVertexAttribPointer(vTC,  2, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,txtcrds));
}

void Missile::Move(float delta, int antiGravity)
{
  if(antiGravity) y += delta/GRAVITY_TUNE*a*0.9;
    else y -= delta/GRAVITY_TUNE*a*0.9;
  a += delta*ACCELERATION;

  if(phase == 2)
  {
    x -= delta * H_SPEED;
    ec+=delta;
    if(ec >= EXPLODE_TIMEOUT)
    {
      phase = 0;
      ec = 0;
      return;
    }
  }
  else if(phase == 1) x += delta * H_MISSILE_SPEED;

  if((x > 1 + MISSILE_RADIUS) || (x < -1 - MISSILE_RADIUS) || (y < -1 - MISSILE_RADIUS))
  {
    phase = 0;
    return;
  }
}

void Missile::Resize(float newMulValue)
{
  vm = newMulValue;
}

void Missile::Render()
{
  if(!phase) return;
  glUseProgram(program);
  glUniform4f(vOffs, x, y, vm, phase);
  glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
}


void Missile::Start(float x_, float y_)
{
  if(phase>0) return;
  ec=0;
  phase = 1;
  x = x_;
  y = y_;

  a = -TAP_IMPULSE*1.5;
}
