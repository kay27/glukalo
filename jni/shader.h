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
    {{ -BIRD_RADIUS*1.5, -BIRD_RADIUS*1.5,  0.0f }, {0.0f, 0.0f}}, // bird
    {{ -BIRD_RADIUS*1.5,  BIRD_RADIUS*1.5,  0.0f }, {0.0f, 2.0f}},
    {{  BIRD_RADIUS*1.5, -BIRD_RADIUS*1.5,  0.0f }, {2.0f, 0.0f}},
    {{  BIRD_RADIUS*1.5,  BIRD_RADIUS*1.5,  0.0f }, {2.0f, 2.0f}},
    {{ -BIRD_RADIUS*2  ,             -1.0,  0.0f }, {0.0f, 0.0f}}, // gap
    {{ -BIRD_RADIUS*2  ,              1.0,  0.0f }, {0.0f, 2.0f}},
    {{  BIRD_RADIUS*2  ,             -1.0,  0.0f }, {2.0f, 0.0f}},
    {{  BIRD_RADIUS*2  ,              1.0,  0.0f }, {2.0f, 2.0f}},
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
//    "  vp = gl_Position;\n"
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
    "  float distanceFromCenter = distance(vec2(vp.x*vMul, vp.y), vec2((tc.x-1.0)*vMul,tc.y-1.0));\n"
    "  float checkForPresenceWithinCircle = 1.0 - smoothstep(vRadius-0.05, vRadius+0.05, distanceFromCenter);\n"
    "  if(checkForPresenceWithinCircle<0.01) discard;\n"
    "\n"
    "  float distanceFromEye = distance(vec2(vp.x*vMul + vRadius/2.0, vp.y + vRadius/2.0), vec2((tc.x-1.0)*vMul,tc.y-1.0));\n"
    "  float checkForPresenceWithinEye = (1.0 - smoothstep(0.0001, 0.089, distanceFromEye));\n"
  //  "  if(checkForPresenceWithinEye>1.0) gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
  //  "  else if(checkForPresenceWithinEye>0.15) gl_FragColor = vec4(0.8, 0.9, 0.86, 1.0);\n"
  //  "  else if(checkForPresenceWithinEye>0.001) gl_FragColor = vec4(0.0, 0.0, 0.01, 1.0);\n"
    "  if(checkForPresenceWithinEye > 0.00001) gl_FragColor = 1.0 - abs(checkForPresenceWithinEye*2.0-1.0) * vec4(0.8, 0.9, 0.86, 1.0);\n"
    "  else gl_FragColor = vec4(distanceFromCenter, 1.0-distanceFromCenter, vColor.z, vColor.w) * checkForPresenceWithinCircle;\n"
    "}\n"
  ;

  static const char * gapVertexShader =
    "attribute vec4 vPosition;\n"
    "float vOffsetX;\n"
    "varying vec4 vp;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(vPosition.x+vOffsetX,vPosition.yzw);\n"
    "  vp = vPosition;\n"
    "}\n"
  ;

  static const char * gapFragmentShader =
    "precision mediump float;\n"
    "float vGap;\n"
    "varying vec4 vp;\n"
    "void main()\n"
    "{\n"
    "  if(abs(vp.y-vGap)<0.3) discard;\n"
    "  gl_FragColor = sin(vp)/2.0+0.5; gl_FragColor.w=1.0;\n"
    "}\n"
  ;

#endif // #ifndef H_SHADER_GLUKALO
