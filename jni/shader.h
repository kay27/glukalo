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
    {{ -BIRD_RADIUS*2.0f,-BIRD_RADIUS*1.5f, 0.0f }, {-2.0f,-1.5f}}, // bird
    {{ -BIRD_RADIUS*2.0f, BIRD_RADIUS*1.5f, 0.0f }, {-2.0f, 1.5f}},
    {{  BIRD_RADIUS*2.0f,-BIRD_RADIUS*1.5f, 0.0f }, { 2.0f,-1.5f}},
    {{  BIRD_RADIUS*2.0f, BIRD_RADIUS*1.5f, 0.0f }, { 2.0f, 1.5f}},
    {{ -COLUMN_HALFWIDTH,            -1.0,  0.0f }, { 0.0f, 0.0f}}, // gap
    {{ -COLUMN_HALFWIDTH,             1.0,  0.0f }, { 0.0f, 2.0f}},
    {{  COLUMN_HALFWIDTH,            -1.0,  0.0f }, { 2.0f, 0.0f}},
    {{  COLUMN_HALFWIDTH,             1.0,  0.0f }, { 2.0f, 2.0f}},
    {{ -1               ,            -1.0,  0.0f }, { 0.0f, 0.0f}}, // floor
    {{ -1               ,  FLOOR_HEIGHT-1,  0.0f }, { 0.0f, 2.0f}},
    {{  1               ,            -1.0,  0.0f }, { 2.0f, 0.0f}},
    {{  1               ,  FLOOR_HEIGHT-1,  0.0f }, { 2.0f, 2.0f}},
    {{ -CHAR_HALFWIDTH  ,-CHAR_HALFWIDTH ,  0.0f }, {-1.0f,-1.0f}}, // char
    {{ -CHAR_HALFWIDTH  , CHAR_HALFWIDTH ,  0.0f }, {-1.0f, 1.0f}},
    {{  CHAR_HALFWIDTH  ,-CHAR_HALFWIDTH ,  0.0f }, { 1.0f,-1.0f}},
    {{  CHAR_HALFWIDTH  , CHAR_HALFWIDTH ,  0.0f }, { 1.0f, 1.0f}},
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
    "uniform float vLevel;\n"
    "varying vec4 vp;\n"
    "void main()\n"
    "{\n"
    "  if(abs(vp.y-vGap)+sin(vp.x*(99.0+sin(vLevel*333.0)*33.0))/(70.0-vLevel) < vHalfSize) discard;\n"
    "  if(vLevel<0.1)\n"
    "    gl_FragColor = vec4(sin(vp.x*88.0)/2.0+0.5, 0.0, sin(vp.y*41.0)/20.0+0.05, 1.0);\n"
    "  else if(vLevel<1.1)\n"
    "    gl_FragColor = vec4(0.2, sin(vp.y*31.0)/2.0+0.5, 0.4, 1.0);\n"
    "  else if(vLevel<2.1)\n"
    "    gl_FragColor = vec4(0.0, 0.0, cos(vp.y*vp.x*99.0)/2.0+0.5, 1.0);\n"
    "  else if(vLevel<3.1)\n"
    "    gl_FragColor = vec4(abs(cos(vp.x*vp.y*111.0)), abs(vp.x), abs(vp.y), 1.0);\n"
    "  else if(vLevel<4.1)\n"
    "    gl_FragColor = vec4(1.0-fract(sin(vp.x*vp.y*555.1)*55.4), 0.5, 0.5, 1.0);\n"
    "  else if(vLevel<5.1)\n"
    "    gl_FragColor = vec4(abs(vp.x*vp.y), abs(vp.x+vp.y)/2.0, abs(cos(vp.x*vp.y*239.0)), 1.0);\n"
    "  else if(vLevel<6.1)\n"
    "    { float q=fract(sin((vp.x*vp.x)*(vp.y+vp.x)*1299.7132)*11.54); gl_FragColor = vec4(q,q,q,1.0); }\n"
    "  else if(vLevel<7.1)\n"
    "    { float q=fract(sin((vp.x*vp.x)*(vp.y+vp.x)*1299.7132)*11.54); gl_FragColor = vec4(q,0.0,0.0,1.0); }\n"
    "  else if(vLevel<8.1)\n"
//    "    gl_FragColor = vec4(fract(vp.x*999.1), fract(vp.y*949.3), fract(vp.x*vp.y*198.19), 1.0);\n"
    "    gl_FragColor = vec4(0.0,fract(vp.x*999.1)*fract(vp.y*949.3), 0.0, 1.0);\n"
    "  else if(vLevel<9.1)\n" //bricks:
    "    { float q=vp.x; if(fract(vp.y*6.0)>=0.5)q+=0.35; if((fract(vp.y*12.0)>0.2)&&(fract(q*7.0)>0.1)) gl_FragColor=vec4(1.0,0.0,0.0,1.0); else gl_FragColor=vec4(0.0,0.41,0.2,1.0); }\n"
    "  else if(vLevel<10.1)\n" //nice blue:
    "    gl_FragColor = vec4(0.0,0.0,1.0-abs(sin(vp.x+vp.y+vp.x*vp.y)),1.0);\n"
    "  else if(vLevel<11.1)\n" // red with 2 blue stripes:
//    "    gl_FragColor = vec4(1.0-abs(sin(vp.x+vp.y+vp.x*vp.y)),0.0,fract(vp.x*99.1)*abs(sin(vp.x*999.1)),1.0);\n"
    "    gl_FragColor = vec4(1.0-abs(sin(vp.x+vp.y+vp.x*vp.y)),0.0,fract(vp.x*9.1)*abs(sin(vp.x*3.1)),1.0);\n"
    "  else if(vLevel<12.1)\n" // pink:
    "    gl_FragColor = vec4(dot(vp,sin(vp)),0.7,0.8,1.0);\n"
    "  else if(vLevel<13.1)\n" // jeans:
    "    gl_FragColor = vec4(dot(vp,sin(vp*333.0)),0.5,dot(vp,cos(vp*333.0)),1.0);\n"
    "  else if(vLevel<14.1)\n" // rainbow:
    "    { float q=fract((vp.x+vp.y)*3.0)*2.0*3.1415926536; gl_FragColor = vec4(abs(sin(q)),abs(sin(q+3.1415926536/3.0)),abs(sin(q+3.1415926536/1.5)),1.0); }\n"
    "  else if(vLevel<15.1)\n" // :
    "    { float q=fract((vp.x+vp.y)*5.0)*2.0*3.1415926536; gl_FragColor = vec4(abs(cos(q)),0.2,abs(sin(q)),1.0); }\n"
    "  else if(vLevel<16.1)\n" // :
    "    { float q=fract(vp.x*vp.y*5.0)*3.1415926536; gl_FragColor = vec4(0.0,sin(q),0.0,1.0); }\n"
    "  else if(vLevel<17.1)\n" // :
    "    { float q=fract(vp.x*19.0); if(q>=0.5) q=fract(vp.y*19.0); if(q>0.5) gl_FragColor = vec4(0.0,0.0,q,1.0); else gl_FragColor = vec4(0.0,fract(vp.y*111.0*cos(vp.x)),0.0,1.0); }\n"
    "  else if(vLevel<18.1)\n" // gray:
    "    gl_FragColor = vec4(0.7,0.7,0.7,1.0);\n"
    "  else if(vLevel<19.1)\n" // :
    "    gl_FragColor = vec4(fract(vp.x*11.39),fract(vp.x*12.37),fract(vp.x*13.44),1.0);\n"
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
    "uniform float vOffset;\n"
    "void main()\n"
    "{\n"
//    "  gl_FragColor = vec4((sin(vp.y*191.1+(vp.x+vOffset)/0.001)+1.0)/2.0, abs((vp.x+vOffset)*vp.y), abs((vp.x+vOffset)*vp.y), 1.0);\n"
    "  float x=vp.x+vOffset;\n"
//    "  gl_FragColor = vec4((sin(vp.y*191.1+x/0.001)+1.0)/2.0*fract(x*991.1), (sin(vp.y*19.1+x/0.001)+1.0)/2.0*fract(x*121.1), (sin(vp.y*331.09+x/0.01)+1.0)/2.0*fract(x*221.77), 1.0);\n"
    "  gl_FragColor = vec4(fract(x*8.11+vp.y*11.3), fract(x*8.21+vp.y*11.9), fract(x*8.31+vp.y*12.19), 1.0);\n"
    "}\n"
  ;

  static const char * fontVertexShader =
    "attribute vec4 vCharPos;\n"
    "attribute vec4 vCharTC;\n"
    "varying vec4 vp;\n"
    "varying vec4 tc;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vCharPos;\n"
    "  vp = vCharPos;\n"
    "  tc = vCharTC;\n"
    "}\n"
  ;

  static const char * fontFragmentShader =
    "precision mediump float;\n"
    "uniform float vCharMul;\n"
    "uniform float vLineSize;\n"
    "uniform vec4 vCharColor;\n"
    "uniform int vCharCode;\n"
    "varying vec4 vp;\n"
    "varying vec4 tc;\n"
    "#define OVAL 1\n"
    "#define LINE 2\n"
    "#define OTOP 3\n"
    "#define ORIG 4\n"
    "#define OLEF 5\n"
    "bool draw(int what,vec4 where)\n"
    "{\n"
    "  if(what == OVAL)\n"
    "  {\n"
    "    float dFC=distance(where.xy, vec2(tc.x*where.z/where.w,tc.y));\n"
    "    if(dFC <= where.w+vLineSize/2.0)\n"
    "      if(dFC >= where.w-vLineSize/2.0)\n"
    "      { gl_FragColor = vCharColor; return true; }\n"
    "  }\n"
    "  else if(what == LINE)\n"
    "  {\n"
    "    vec2 lineDir = where.xy - where.zw;\n"
    "    vec2 perpDir = vec2(lineDir.y, -lineDir.x);\n"
    "    vec2 dirToPt1 = where.zw - tc.xy;\n"
    "    if(abs(dot(normalize(perpDir), dirToPt1)) <= vLineSize/2.0)\n"
    "    { gl_FragColor = vCharColor; return true; }\n"
    "  }\n"
    "  else if(what == OTOP)\n"
    "  {\n"
    "    if(vp.y >= where.y)\n"
    "    {\n"
    "      float dFC=distance(where.xy, vec2(tc.x*where.z/where.w,tc.y));\n"
    "      if(dFC <= where.w + vLineSize/2.0)\n"
    "        if(dFC >= where.w - vLineSize/2.0)\n"
    "        { gl_FragColor = vCharColor; return true; }\n"
    "    }\n"
    "  }\n"
    "  return false;\n"
    "}\n"
    "\n"
    "void main()\n"
    "{\n"
    "  if(vCharCode==48)\n"
    "  {\n"
    "    if(draw(OVAL,vec4(0.0*vCharMul, 0.0, 0.8*vCharMul, 1.0))) discard;\n"
    "  }\n"
    "  else if(vCharCode==49)\n"
    "  {\n"
    "    if(draw(LINE,vec4(-0.5*vCharMul, 0.0, 0.5*vCharMul,-1.0))) discard;\n"
    "    if(draw(LINE,vec4(0.5*vCharMul, 1.0, 0.5*vCharMul,-1.0))) discard;\n"
    "  }\n"
    "}\n"
  ;

#endif // #ifndef H_SHADER_GLUKALO
