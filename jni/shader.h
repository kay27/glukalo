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
    {{   -BIRD_RADIUS*2.0f,   -BIRD_RADIUS*1.5f,  0.0f }, {-2.0f,-1.5f}}, // bird
    {{   -BIRD_RADIUS*2.0f,    BIRD_RADIUS*1.5f,  0.0f }, {-2.0f, 1.5f}},
    {{    BIRD_RADIUS*2.0f,   -BIRD_RADIUS*1.5f,  0.0f }, { 2.0f,-1.5f}},
    {{    BIRD_RADIUS*2.0f,    BIRD_RADIUS*1.5f,  0.0f }, { 2.0f, 1.5f}},
    {{   -COLUMN_HALFWIDTH,                -1.0,  0.0f }, { 0.0f, 0.0f}}, // gap
    {{   -COLUMN_HALFWIDTH,                 1.0,  0.0f }, { 0.0f, 2.0f}},
    {{    COLUMN_HALFWIDTH,                -1.0,  0.0f }, { 2.0f, 0.0f}},
    {{    COLUMN_HALFWIDTH,                 1.0,  0.0f }, { 2.0f, 2.0f}},
    {{                  -1,                -1.0,  0.0f }, { 0.0f, 0.0f}}, // floor
    {{                  -1,      FLOOR_HEIGHT-1,  0.0f }, { 0.0f, 2.0f}},
    {{                   1,                -1.0,  0.0f }, { 2.0f, 0.0f}},
    {{                   1,      FLOOR_HEIGHT-1,  0.0f }, { 2.0f, 2.0f}},
    {{        -CHAR_HEIGHT,  -CHAR_HEIGHT*0.75f,  0.0f }, {-2.0f,-1.5f}}, // char
    {{        -CHAR_HEIGHT,   CHAR_HEIGHT*0.75f,  0.0f }, {-2.0f, 1.5f}},
    {{         CHAR_HEIGHT,  -CHAR_HEIGHT*0.75f,  0.0f }, { 2.0f,-1.5f}},
    {{         CHAR_HEIGHT,   CHAR_HEIGHT*0.75f,  0.0f }, { 2.0f, 1.5f}},
    {{-MISSILE_RADIUS*2.0f,-MISSILE_RADIUS*1.5f,  0.0f }, {-2.0f,-1.5f}}, // missile
    {{-MISSILE_RADIUS*2.0f, MISSILE_RADIUS*1.5f,  0.0f }, {-2.0f, 1.5f}},
    {{ MISSILE_RADIUS*2.0f,-MISSILE_RADIUS*1.5f,  0.0f }, { 2.0f,-1.5f}},
    {{ MISSILE_RADIUS*2.0f, MISSILE_RADIUS*1.5f,  0.0f }, { 2.0f, 1.5f}},
    {{  -BONUS_RADIUS*2.0f,  -BONUS_RADIUS*1.5f,  0.0f }, {-2.0f,-1.5f}}, // bonus
    {{  -BONUS_RADIUS*2.0f,   BONUS_RADIUS*1.5f,  0.0f }, {-2.0f, 1.5f}},
    {{   BONUS_RADIUS*2.0f,  -BONUS_RADIUS*1.5f,  0.0f }, { 2.0f,-1.5f}},
    {{   BONUS_RADIUS*2.0f,   BONUS_RADIUS*1.5f,  0.0f }, { 2.0f, 1.5f}},
    {{     -ICON_SIZE/2.0f,     -ICON_SIZE*1.5f,  0.0f }, {-2.0f,-1.5f}}, // icon
    {{     -ICON_SIZE/2.0f,      ICON_SIZE/1.5f,  0.0f }, {-2.0f, 1.5f}},
    {{      ICON_SIZE/2.0f,     -ICON_SIZE/1.5f,  0.0f }, { 2.0f,-1.5f}},
    {{      ICON_SIZE/2.0f,      ICON_SIZE/1.5f,  0.0f }, { 2.0f, 1.5f}},
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
    "uniform vec2 vEye;\n"
    "void main()\n"
    "{\n" // based on http://stackoverflow.com/a/11457353/5920627
    "  float distanceFromCenter = distance(vec2(vp.x*vMul, vp.y), vec2(tc.x*vMul,tc.y));\n"
    "  if(distanceFromCenter > 1.0*vRadius) discard;\n"
    "  float distanceFromEye = distance(vec2(vp.x*vMul + vEye.x*vRadius, vp.y + vEye.y*vRadius), vec2(tc.x*vMul,tc.y));\n"
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
    "uniform vec4 vOM;\n"
    "void main()\n"
    "{\n"
    "  float x=vp.x+vOM.x;\n"
//    "  gl_FragColor = vec4(fract(x*vOM.y+vp.y*11.3)*0.9, fract(x*vOM.y*4.0+vp.y*11.9)*0.6, fract(x*2.0*vOM.y+vp.y*12.19)*0.2, 1.0);\n"
//    "  gl_FragColor = vec4(fract(x*vOM.y+vp.y*11.3+sin(x*4.0*vOM.y*2.0*3.14159265))*0.9, 0.0, fract(x*2.0*vOM.y+vp.y*12.19)*0.2, 1.0);\n"
//    "  gl_FragColor = vec4(fract(vp.y*11.3+sin(x*4.0*vOM.y*2.0*3.14159265))*0.9, 0.0, fract(x*2.0*vOM.y+vp.y*12.19)*0.2, 1.0);\n"
//    "  if(vOM.z < 0.9) gl_FragColor = vec4(fract(vp.y*11.3+sin(x*4.0*vOM.y*2.0*3.14159265))*0.9, 0.0, 0.0, 1.0);\n"
//    "  else gl_FragColor = vec4(fract(vp.y*11.3+sin(x*4.0*vOM.y*2.0*3.14159265))*0.6, 0.0, fract(sin(x*32.0*vOM.y*2.0*3.14159265)*29.1)*0.8, 1.0);\n"
    "  float q = fract(vp.y*11.3+sin(x*4.0*vOM.y*2.0*3.14159265));\n"
    "  if(vOM.z < 0.9) gl_FragColor = vec4(q*0.8, q*0.2, q*0.19, 1.0);\n"
    "  else gl_FragColor = vec4((1.0-q)*0.2, q*0.2, q*fract(sin(x*32.0*vOM.y*2.0*3.14159265)*29.1), 1.0);\n"
    "}\n"
  ;

  static const char * fontVertexShader =
    "attribute vec4 vCharPos;\n"
    "attribute vec4 vCharTC;\n"
    "uniform vec4 vCharOffset;\n"
    "varying vec4 vp;\n"
    "varying vec4 tc;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vCharPos + vCharOffset;\n"
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
    "#define LINE   1\n"
    "#define OVAL   2\n"
    "#define OTOP   3\n"
    "#define OBOT   4\n"
    "#define OLEF   5\n"
    "#define ORIG   6\n"
    "#define OTOP3  7\n"
    "#define OBOT3  8\n"
    "#define OLEF3  9\n"
    "#define ORIG3 10\n"
    "#define LINO  11\n"
    "bool draw(int what,vec4 where)\n"
    "{\n"
    "  vec4 t = tc * vec4(vCharMul, 1.0, vCharMul, 1.0);\n"
    "  if(what == LINO)\n"
    "  {\n"
    "    if(abs(distance(where.xy,t.xy)+distance(where.zw,t.xy)-distance(where.xy,where.zw)) > vLineSize/2.0) return false;\n"
    "    gl_FragColor = vCharColor; return true;\n"
    "  }\n"
    "  if(what == LINE)\n"
    "  {\n"
    "    float ac=distance(where.xy,t.xy);\n"
    "    float bc=distance(where.zw,t.xy);\n"
    "    float ac2=pow(ac,2.0);\n"
    "    float ab=distance(where.xy,where.zw);\n"
    "    if(abs(ac+bc-ab) > vLineSize/2.0) return false;\n"
    "    float ad=(ac2-pow(bc,2.0)+pow(ab,2.0)) * 0.5 / ab;\n"
    "    if(ad<0.0) return false;\n"
    "    float h=sqrt(ac2-pow(ad,2.0));\n"
    "    if(h > vLineSize) return false;\n"
    "    gl_FragColor = vCharColor; return true;\n"
    "  }\n"
    "  else if(what == OTOP) { if(tc.y < where.y) return false; what=OVAL; }\n"
    "  else if(what == ORIG) { if(tc.x < where.x/vCharMul) return false; what=OVAL; }\n"
    "  else if(what == OLEF) { if(tc.x > where.x/vCharMul) return false; what=OVAL; }\n"
    "  else if(what == OBOT) { if(tc.y > where.y) return false; what=OVAL; }\n"
    "  else if(what == OTOP3) { if(tc.y < where.y-where.w*0.333) return false; what=OVAL; }\n"
    "  else if(what == ORIG3) { if(tc.x < (where.x-where.z*0.333)/vCharMul) return false; what=OVAL; }\n"
    "  else if(what == OLEF3) { if(tc.x > (where.x+where.z*0.333)/vCharMul) return false; what=OVAL; }\n"
    "  else if(what == OBOT3) { if(tc.y > where.y+where.w*0.333) return false; what=OVAL; }\n"
    "\n"
    "  if(what == OVAL)\n"
    "  {\n"
    "    if(abs(sqrt(pow(t.x-where.x,2.0)/pow(where.z,2.0) + pow(t.y-where.y,2.0)/pow(where.w,2.0)) - 1.0) <= vLineSize)\n"
    "    { gl_FragColor = vCharColor; return true; }\n"
    "    return false;\n"
    "  }\n"
    "  return false;\n"
    "}\n"
    "\n"
    "void main()\n"
    "{\n"
    "  if(vCharCode==48)\n" //0
    "  {\n"
    "    if(!draw(OVAL,vec4(0.0, 0.0, 0.8, 1.0))) discard;\n"
    "  }\n"
    "  else if(vCharCode==49)\n" //1
    "  {\n"
    "    if(!draw(LINE,vec4(-0.5,0.0,0.6,1.0))) if(!draw(LINE,vec4(0.6,1.0,0.1,-1.0))) discard;\n"
    "  }\n"
    "  else if(vCharCode==50)\n" //2
    "  {\n"
    "    if(!draw(OTOP3,vec4(0.0,0.5,0.8,0.5))) if(!draw(LINE,vec4(0.8,0.5,-0.8,-1.0))) if(!draw(LINE,vec4(-0.8,-1.0,0.8,-1.0)))discard;\n"
    "  }\n"
    "  else if(vCharCode==51)\n" //3
    "  {\n"
    "    if(!draw(ORIG,vec4(-0.8,0.5,1.6,0.5))) if(!draw(ORIG,vec4(-0.8,-0.5,1.6,0.5))) discard;\n"
    "  }\n"
    "  else if(vCharCode==52)\n" //4
    "  {\n"
    "    if(!draw(LINE,vec4(0.0,1.0,-0.8,0.0))) if(!draw(LINE,vec4(-0.8,0.0,0.8,0.0))) if(!draw(LINE,vec4(0.8,1.0,0.2,-1.0))) discard;\n"
    "  }\n"
    "  else if(vCharCode==53)\n" //5
    "  {\n"
    "    if(!draw(ORIG,vec4(-0.8,-0.2,1.6,0.8))) if(!draw(LINE,vec4(-0.8,0.6,-0.8,1.0))) if(!draw(LINE,vec4(-0.8,1.0,0.8,1.0)))discard;\n"
    "  }\n"
    "  else if(vCharCode==54)\n" //6
    "  {\n"
    "    if(!draw(OVAL,vec4(0.0,-0.5,0.8,0.5))) if(!draw(OLEF,vec4(0.6,0.0,1.4,1.0))) discard;\n"
    "  }\n"
    "  else if(vCharCode==55)\n" //7
    "  {\n"
    "    if(!draw(LINE,vec4(-0.8,1.0,0.8,1.0))) if(!draw(LINE,vec4(0.8,1.0,-0.1,-1.0))) discard;\n"
    "  }\n"
    "  else if(vCharCode==56)\n" //8
    "  {\n"
    "    if(!draw(OVAL,vec4(0.0,-0.5,0.8,0.5))) if(!draw(OVAL,vec4(0.0,0.5,0.8,0.5))) discard;\n"
    "  }\n"
    "  else if(vCharCode==57)\n" //9
    "  {\n"
    "    if(!draw(OVAL,vec4(0.0,0.5,0.8,0.5))) if(!draw(ORIG,vec4(-0.6,0.0,1.4,1.0))) discard;\n"
    "  }\n"
    "  else discard;\n"
    "}\n"
  ;

  static const char * missileVertexShader =
    "attribute vec4 vPos;\n"
    "attribute vec4 vTC;\n"
    "uniform vec4 vOffs;\n"
    "varying vec4 vp;\n"
    "varying vec2 tc;\n"
    "varying float phase;\n"
    "varying float vMul;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vPos + vec4(vOffs.xy, 0.0, 0.0);\n"
    "  vMul = vOffs.z;\n"
    "  phase = vOffs.w;\n"
    "  vp = vPos;\n"
    "  tc = vTC.xy;\n"
    "}\n"
  ;

  static const char * missileFragmentShader =
    "precision mediump float;\n"
    "varying vec4 vp;\n"
    "varying vec2 tc;\n"
    "varying float phase;\n"
    "varying float vMul;\n"
    "void main()\n"
    "{\n"
    "  if(phase > 1.999) { gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); return; }\n"
    "  float dfc = distance(vec2(vp.x*vMul, vp.y), vec2(tc.x*vMul,tc.y));\n"
    "  if(dfc > 1.0) discard;\n"
    "  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0) * fract(vp.x*sin(vp.y)+tc.x*1.117);\n"
    "}\n"
  ;

  static const char * bonusVertexShader =
    "attribute vec4 vPos;\n"
    "attribute vec4 vTC;\n"
    "uniform vec4 vOffs;\n"
    "varying vec4 vp;\n"
    "varying vec2 tc;\n"
    "varying float vMul;\n"
    "varying float angle;\n"
    "void main()\n"
    "{\n"
    "  vMul = vOffs.z;\n"
    "  angle = vOffs.w;\n"
    "  mat2 rotationMatrix = mat2(\n"
    "    cos(angle), -sin(angle),\n"
    "    sin(angle),  cos(angle));\n"
    "  gl_Position = vec4((vPos.xy*vec2(vMul,1.0) * rotationMatrix) / vec2(vMul, 1.0) + vOffs.xy, vPos.zw);\n"
    "  vp = vPos;\n"
    "  tc = vTC.xy;\n"
    "}\n"
  ;

  static const char * bonusFragmentShader =
    "precision mediump float;\n"
    "uniform int vType;\n"
    "varying vec4 vp;\n"
    "varying vec2 tc;\n"
    "varying float vMul;\n"
    "varying float angle;\n"
    "void main()\n"
    "{\n"
    "  float dfc = distance(vec2(vp.x*vMul, vp.y), vec2(tc.x*vMul,tc.y));\n"
    "  if(dfc > 1.0) discard;\n"
    "  float dft = distance(vec2(0.0, 2.0), vec2(tc.x*vMul,tc.y));\n"
    "  if(dft < 2.13)\n"
    "  {\n"
    "    if(vType == 1)\n"
    "      gl_FragColor = vec4(abs(sin(tc.y*tc.x*137.1+0.7211)), 0.1, 1.0-abs(sin(tc.y*tc.x*137.1+0.7111)), 1.0);\n"
    "    else if(vType == 2)\n"
    "      gl_FragColor = vec4(fract(angle*13.1+0.7211), fract(angle*13.11+0.7212), 1.0 - fract(angle*13.12+0.7213), 1.0);\n"
    "    else\n"
    "      gl_FragColor = vec4(abs(sin(tc.x*999.0))*0.2, abs(sin(tc.x*999.1))*0.3, 0.95, 1.0);\n"
    "    return;\n"
    "  }\n"
    "  else if(abs(tc.x*vMul) > 0.3) discard;\n"
    "  if(vType == 1)\n"
    "    gl_FragColor = vec4(abs(sin(tc.y*tc.x*33.1)), 0.1, 0.1, 1.0);\n"
    "  else if(vType == 2)\n"
    "    gl_FragColor = vec4(fract(tc.y*11.1), fract(tc.y*11.11+0.01), 1.0 - fract(tc.y*11.12+0.02), 1.0);\n"
    "  else\n"
    "    gl_FragColor = vec4(0.8, 0.5, 0.9, 1.0);\n"
    "}\n"
  ;

  static const char * iconVertexShader =
    "attribute vec4 vPos;\n"
    "attribute vec4 vTC;\n"
    "uniform vec4 vOffs;\n"
    "varying vec4 vp;\n"
    "varying vec2 tc;\n"
    "varying float vMul;\n"
    "void main()\n"
    "{\n"
    "  vMul = vOffs.z;\n"
    "  gl_Position = vec4(vPos.xy + vOffs.xy, vPos.zw);\n"
    "  vp = vPos;\n"
    "  tc = vTC.xy;\n"
    "}\n"
  ;

  static const char * iconLevelFragmentShader =
    "precision mediump float;\n"
    "varying vec4 vp;\n"
    "varying vec2 tc;\n"
    "varying float vMul;\n"
    "void main()\n"
    "{\n"
    "  vec2 p = vec2(tc.x*vMul, tc.y);\n"
    "  vec2 a = abs(p);\n"
    "  if( (a.x>0.8)||(a.y>0.8)||((a.x>0.2)&&(a.x<0.4))||((a.y>0.2)&&(a.y<0.4)) )\n"
    "  {\n"
    "    gl_FragColor = vec4(0.0, 1.0, 0.0, 0.3);\n" //hash
    "  }\n"
    "  else\n"
    "  {\n"
    "    if( (p.y>0.2)||((p.y>-0.2)&&(p.x<-0.2)) )\n" //completed levels
    "    {\n"
    "      gl_FragColor = vec4(abs(sin(tc.y*tc.x*3.3)), abs(sin(tc.y*tc.x*4.5)), abs(sin(tc.y*tc.x*5.8)), 1.0);\n"
    "    }\n"
    "    else\n"
    "    {\n"
    "      gl_FragColor = vec4(0.7, 0.7, 0.8, 0.9);\n"
    "    }\n"
    "  }\n"
    "}\n"
  ;

  static const char * iconSoundFragmentShader =
    "precision mediump float;\n"
    "uniform int vState;\n"
    "varying vec4 vp;\n"
    "varying vec2 tc;\n"
    "varying float vMul;\n"
    "void main()\n"
    "{\n"
    "  vec2 p = vec2(tc.x*vMul, tc.y);\n"
    "  if( (abs(p.y)<0.4)&&(p.x>-0.6)&&(p.x<-0.2) )\n"
    "  {\n"
    "    gl_FragColor = vec4(abs(sin(tc.x*tc.y*2.9)), 0.3, 0.3, 1.0);\n" //magnet
    "  }\n"
    "  else if( (p.x>-0.2)&&(p.x<0.6)&&(abs(p.y)-0.75*x < 0.55) )\n"
    "  {\n"                                                              //diffuser
    "    if(vState==1) gl_FragColor = vec4(abs(sin(tc.x*tc.y*3.9)), abs(sin(tc.x*tc.y*4.9)), abs(sin(tc.x*tc.y*5.9)), 1.0);\n"
    "    else gl_FragColor = vec4(0.9, 0.6, 0.6, 0.7);\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "    gl_FragColor = vec4(0.0, 1.0, 0.0, 0.3);\n" // frame
    "  }\n"
    "}\n"
  ;


#endif // #ifndef H_SHADER_GLUKALO
