#ifndef H_SHADER_GLUKALO
# define H_SHADER_GLUKALO

# include "config.h"

  class MyShader
  {
    public:
      static GLuint CreateProgram();
      static void LinkProgram(GLuint program);
      static void AttachVertexShader(GLuint program, const char *shaderCode);
      static void AttachFragmentShader(GLuint program, const char *shaderCode);
      static void AttachShader(GLuint program, GLenum shaderType, GLint shader, const char *shaderCode);
  };

  struct MyVertex
  {
    GLfloat pos[3];
    GLfloat txtcrds[2];
  };

  static const MyVertex vertices[] =
  {
    {{ -BIRD_RADIUS*2.0, -BIRD_RADIUS*1.5,  0.0f }, {-2.0f,-1.5f}}, // bird
    {{ -BIRD_RADIUS*2.0,  BIRD_RADIUS*1.5,  0.0f }, {-2.0f, 1.5f}},
    {{  BIRD_RADIUS*2.0, -BIRD_RADIUS*1.5,  0.0f }, { 2.0f,-1.5f}},
    {{  BIRD_RADIUS*2.0,  BIRD_RADIUS*1.5,  0.0f }, { 2.0f, 1.5f}},
    {{ -COLUMN_HALFWIDTH,            -1.0,  0.0f }, { 0.0f, 0.0f}}, // gap
    {{ -COLUMN_HALFWIDTH,             1.0,  0.0f }, { 0.0f, 2.0f}},
    {{  COLUMN_HALFWIDTH,            -1.0,  0.0f }, { 2.0f, 0.0f}},
    {{  COLUMN_HALFWIDTH,             1.0,  0.0f }, { 2.0f, 2.0f}},
    {{ -1               ,            -1.0,  0.0f }, { 0.0f, 0.0f}}, // floor
    {{ -1               ,  FLOOR_HEIGHT-1,  0.0f }, { 0.0f, 2.0f}},
    {{  1               ,            -1.0,  0.0f }, { 2.0f, 0.0f}},
    {{  1               ,  FLOOR_HEIGHT-1,  0.0f }, { 2.0f, 2.0f}},
  };

  static const char * birdVertexShader =
    "attribute vec4 vPosition;\n"
    "attribute vec4 vTextureCoordinate;\n"
    "uniform vec4 vOffset;\n"
    "varying vec4 vp;\n"
    "varying vec2 tc;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vPosition + vOffset;\n"
    "  vp = vPosition;\n"
    "  tc = vTextureCoordinate.xy;\n"
    "}\n"
  ;

  static const char * birdFragmentShader =
    "precision mediump float;\n"
    "varying vec4 vp;\n"
    "varying vec2 tc;\n"
    "uniform vec4 vColor;\n"
    "uniform float vRadius;\n"
    "uniform float vMul;\n"
    "uniform float vSpeed;\n"
    "void main()\n"
    "{\n" // based on http://stackoverflow.com/a/11457353/5920627
    "  float distanceFromCenter = distance(vec2(vp.x*vMul, vp.y), vec2(tc.x*vMul,tc.y));\n"
    "  if(distanceFromCenter > 1.0*vRadius) discard;\n"
    "  float distanceFromEye = distance(vec2(vp.x*vMul + 0.5*vRadius, vp.y + 0.5*vRadius), vec2(tc.x*vMul,tc.y));\n"
    "  if(distanceFromEye < 0.1*vRadius) gl_FragColor=vec4(0.0,0.0,0.0,1.0);\n"
    "  else if(distanceFromEye < 0.3*vRadius) gl_FragColor=vec4(1.0,1.0,0.99,1.0);\n"
    "  else if(distanceFromEye < 0.32*vRadius) gl_FragColor=vec4(0.0,0.0,0.0,1.0);\n"
    "  else gl_FragColor = vColor * (1.0-distanceFromCenter/3.0);\n"
    "}\n"
  ;

  static const char * gapVertexShader =
    "attribute vec4 vPosition;\n"
    "uniform float vOffsetX;\n"
    "varying vec4 vp;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(vPosition.x+vOffsetX,vPosition.yzw);\n"
    "  vp = vPosition;\n"
    "}\n"
  ;

  static const char * gapFragmentShader =
    "precision mediump float;\n"
    "uniform float vGap;\n"
    "uniform float vHalfSize;\n"
    "varying vec4 vp;\n"
    "void main()\n"
    "{\n"
    "  if(abs(vp.y-vGap)+sin(vp.x*99.0)/70.0 < vHalfSize) discard;\n"
    "  gl_FragColor = vec4(sin(vp.x*88.0)/2.0+0.5, 0.0, sin(vp.y*41.0)/20.0+0.05, 1.0);\n"
    "}\n"
  ;

  static const char * floorVertexShader =
    "attribute vec4 vPosition;\n"
    "varying vec4 vp;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vPosition;\n"
    "  vp = vPosition;\n"
    "}\n"
  ;

  static const char * floorFragmentShader =
    "precision mediump float;\n"
    "varying vec4 vp;\n"
    "void main()\n"
    "{\n"
    "  gl_FragColor = vec4((sin(vp.y*191.1+vp.x/0.001)+1.0)/2.0, 0.0, 0.0, 1.0);\n"
    "}\n"
  ;

#endif // #ifndef H_SHADER_GLUKALO
