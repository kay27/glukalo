#include "game.h"

GLint Missile::program = 0;
GLint Missile::vPos = 0;
GLint Missile::vRadius = 0;
GLint Missile::vTC = 0;
GLint Missile::vOffs = 0;
float Missile::vm = 1;

void Missile::Explode()
{
  if(phase != 1) return;
  SoundPlayer::PlayD();
  phase = 2;
}

void Missile::Init()
{
  program = MyShader::CreateProgram();
  MyShader::AttachVertexShader  (program, missileVertexShader);
  MyShader::AttachFragmentShader(program, missileFragmentShader);
  MyShader::LinkProgram(program);
  glUseProgram(program);
  vPos    = glGetAttribLocation (program, "vPos");
  vTC     = glGetAttribLocation (program, "vTC");
  vOffs   = glGetUniformLocation(program, "vOffs");
  vRadius = glGetUniformLocation (program, "vRadius");
  glEnableVertexAttribArray(vPos);
  glEnableVertexAttribArray(vTC);
  glVertexAttribPointer(vPos, 3, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,pos));
  glVertexAttribPointer(vTC,  2, GL_FLOAT, false, sizeof(MyVertex), (void*)offsetof(MyVertex,txtcrds));
}

void Missile::Move(float delta, int antiGravity, int direction, float deltaX)
{
  if(phase == 2)
  {
    if(antiGravity) y += delta/GRAVITY_TUNE*a*0.4;
      else y -= delta/GRAVITY_TUNE*a*0.4;
    a += delta*ACCELERATION;

    x -= deltaX;
    ec += delta;
    if(ec >= EXPLODE_TIMEOUT)
    {
      phase = 0;
      ec = 0;
      return;
    }
  }
  else
  {
    if(antiGravity) y += delta/GRAVITY_TUNE*a*0.9;
      else y -= delta/GRAVITY_TUNE*a*0.9;
    a += delta*ACCELERATION;

    if(phase == 1) x += delta * H_MISSILE_SPEED * direction;
  }

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

void Missile::Render(float radius)
{
  if(!phase) return;
  glUseProgram(program);
  glUniform4f(vOffs, x, y, vm, phase);
  glUniform1f(vRadius, radius);
  glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
}

void Missile::Render(float x, float y)
{
  glUseProgram(program);
  glUniform4f(vOffs, x, y, vm, 33);
  glUniform1f(vRadius, 1.0);
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
