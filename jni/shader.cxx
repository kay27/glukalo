#include <stdio.h>
#include <GLES2/gl2.h>

GLuint MyShader::CreateProgram()
{
  GLuint program = glCreateProgram();
  if(!program)
  {
    char msg[64];
    sprintf(msg, "Could not create OpenGL ES program: %d", glGetError());
    MyCallback::Quit(msg);
  }
  return program;
}

void MyShader::LinkProgram(GLuint program)
{
  GLint tmp[1];
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, tmp);

  if(tmp[0] != GL_TRUE)
  {
    char l[512];
    glGetProgramInfoLog(program, 512, tmp, l);
    glDeleteProgram(program);
    Toast("GL program link error:");
    Quit(l);
  }
}

void MyShader::AttachVertexShader(GLuint program, const char *shaderCode)
{
  if(GLint s = glCreateShader(GL_VERTEX_SHADER))
  {
    AttachShader(program, GL_VERTEX_SHADER, s, shaderCode);
    return;
  }

  char msg[64];
  sprintf(msg, "Could not create vertex shader: %d", glGetError());
  MyCallback::Quit(msg);
}

void MyShader::AttachFragmentShader(GLuint program, const char *shaderCode)
{
  if(GLint s = glCreateShader(GL_FRAGMENT_SHADER))
  {
    AttachShader(program, GL_FRAGMENT_SHADER, s, shaderCode);
    return;
  }

  char msg[64];
  sprintf(msg, "Could not create fragment shader: %d", glGetError());
  MyCallback::Quit(msg);
}


void MyShader::AttachShader(GLuint program, GLenum shaderType, GLint shader, const char *shaderCode)
{
  glShaderSource(shader, 1, &shaderCode, NULL);
  glCompileShader(shader);

  GLint tmp[1];
  glGetShaderiv(shader, GL_COMPILE_STATUS, tmp);
  if(tmp[0])
  {
    glAttachShader(program, shader);
    return;
  }

  switch (shaderType)
  {
    case GL_VERTEX_SHADER:    Toast("Could not compile vertex shader:"  ); break;
    case GL_FRAGMENT_SHADER:  Toast("Could not compile fragment shader:"); break;
  }
  char l[512]; glGetShaderInfoLog(shader, 512, tmp, l);
  Quit(l);
}    

void MyShader::Init()
{
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
}