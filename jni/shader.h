#ifndef H_SHADER_GLUKALO
# define H_SHADER_GLUKALO

  struct BirdVertex
  {
    GLfloat pos[3];
    GLfloat txtcrds[2];
  };

  static const BirdVertex birdVertices[] =
  {
    {{ -0.10f, -0.10f,  0.0f }, {0.0f, 0.0f}}, // triangle to quad
    {{ -0.10f,  0.10f,  0.0f }, {0.0f, 1.0f}},
    {{  0.10f, -0.10f,  0.0f }, {1.0f, 0.0f}},
    {{  0.10f,  0.10f,  0.0f }, {1.0f, 1.0f}},
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
    "  float distanceFromCenter = distance(vec2(vp.x/2.0, vp.y/2.0), vec2((tc.x-0.5)*vMul,tc.y-0.5));\n"
    "  float checkForPresenceWithinCircle = 1.0 - smoothstep(vRadius-0.05, vRadius+0.05, distanceFromCenter);\n"
    "  if(checkForPresenceWithinCircle<0.01) discard;\n"
    "\n"
    "  float distanceFromEye = distance(vec2(vp.x/2.0 + 0.12, vp.y/2.0 + 0.1), vec2((tc.x-0.5)*vMul,tc.y-0.5));\n"
    "  float checkForPresenceWithinEye = (1.0 - smoothstep(0.0001, 0.089, distanceFromEye));\n"
  //  "  if(checkForPresenceWithinEye>1.0) gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
  //  "  else if(checkForPresenceWithinEye>0.15) gl_FragColor = vec4(0.8, 0.9, 0.86, 1.0);\n"
  //  "  else if(checkForPresenceWithinEye>0.001) gl_FragColor = vec4(0.0, 0.0, 0.01, 1.0);\n"
    "  if(checkForPresenceWithinEye > 0.00001) gl_FragColor = abs(checkForPresenceWithinEye*2.0-1.0) * vec4(0.8, 0.9, 0.86, 1.0);\n"
    "  else gl_FragColor = vec4(distanceFromCenter, 1.0-distanceFromCenter, vColor.z, vColor.w) * checkForPresenceWithinCircle;\n"
    "}\n"
  ;

  static const char * blockVertexShader =
    "attribute vec4 vPosition;\n"
    "float vOffsetX;\n"
    "varying vec4 vp;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(vPosition.x+vOffsetX,vPosition.yzw);\n"
    "  vp = gl_Position;\n"
    "}\n"
  ;

  static const char * blockFragmentShader =
    "precision mediump float;\n"
    "varying vec4 vp;\n"
    "void main()\n"
    "{\n"
    "  gl_FragColor = sin(vp)/2+0.5; gl_FragColor.w=1.0;\n"
    "}\n"
  ;

#endif // #ifndef H_SHADER_GLUKALO
