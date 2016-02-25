#include <unistd.h>
#include <cstdlib>
#include "gles2game.h"

void Game::Game()
{
  firstRun = 1;
  Init();
}

void Game::Init()
{
  gameOver  =  0;
  yOffs     =  0;
  xOffs     =  0;
  speed     =  0;
  impulse   =  0;
  started   =  0;
  vect      = -0.0023;

  gettimeofday(&lastTime, NULL);

  if(firstTime)
    Toast("Tap to play");

  firstTime = 0;


    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);

    pos = glGetAttribLocation(program, "vPosition");
    textureCoords = glGetAttribLocation(program, "vTextureCoordinate");
    color = glGetUniformLocation(program, "vColor");
    yoffset = glGetUniformLocation(program, "vOffset");
    radius = glGetUniformLocation(program, "vRadius");
    ymul = glGetUniformLocation(program, "vMul");
    vSpeed = glGetUniformLocation(program, "vSpeed");

    glUseProgram(program);
    glEnableVertexAttribArray(pos);
    glEnableVertexAttribArray(textureCoords);
    glVertexAttribPointer(pos, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex,pos));
    glVertexAttribPointer(textureCoords, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex,txtcrds));
    glUniform4f(color, 0, 0.6, 1, 1);
    glUniform1f(radius, 0.25);
//    glUniform1f(ymul, yMulValue);

    Restart();


}

void Game::Resume()
{
  impulse  = 0;
  gettimeofday(&lastTime, NULL);
}

void Game::Tap()
{
  impulse = 1;

  if(gameOver)
    Restart(); 
  else
    started = 1;
}

