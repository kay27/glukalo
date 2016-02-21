#include<unistd.h>
#include<cstdlib>
#include<GLES2/gl2.h>
#include<jni.h>

static int sWindowWidth  = 720;
static int sWindowHeight = 1280;
static int sStopped      = 0;
GLuint program           = 0; //class?//fixme
GLint pos                = 0;
GLint color              = 0;
GLint yoffset            = 0;
GLint pixradius          = 0;
GLuint vb;
GLfloat yOffs            = 0;
GLfloat speed            = 0;
struct timeval lastTime;
int impulse              = 0;

struct Vertex {
    GLfloat pos[3];
    GLubyte rgba[4];
};


const Vertex data[] = {
  {{  0.0f,  0.5f,  0.0f }, {0, 160, 255, 255}},
  {{ -0.5f, -0.5f,  0.0f }, {0, 160, 255, 255}},
  {{  0.5f, -0.5f,  0.0f }, {0, 160, 255, 255}}
};

const char font[12*14*1] =
{
 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
 1, 2, 2, 2, 2, 2, 2, 2, 1, 1,
 1, 2, 2, 2, 2, 2, 2, 2, 1, 1,
 1, 2, 2, 2, 2, 2, 2, 2, 1, 1,
 1, 2, 2, 2, 1, 2, 2, 2, 1, 1,
 1, 2, 2, 2, 1, 2, 2, 2, 1, 1,
 1, 2, 2, 2, 1, 2, 2, 2, 1, 1,
 1, 2, 2, 2, 1, 2, 2, 2, 1, 1,
 1, 2, 2, 2, 1, 2, 2, 2, 1, 1,
 1, 2, 2, 2, 1, 2, 2, 2, 1, 1,
 1, 2, 2, 2, 2, 2, 2, 2, 1, 1,
 1, 2, 2, 2, 2, 2, 2, 2, 1, 1,
 1, 2, 2, 2, 2, 2, 2, 2, 1, 1,
 0, 1, 1, 1, 1, 1, 1, 1, 1, 1
};


static const char * squareVertexShader =
  "attribute vec4 vPosition;\n"
  "uniform vec4 vOffset;\n"
  "varying vec4 vp;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vPosition + vOffset;\n"
  "  vp = vOffset;\n"
  "}\n"
;

static const char * squareFragmentShader =
  "precision mediump float;\n"
  "varying vec4 vp;\n"
  "uniform vec4 vColor;\n"
  "uniform float vPixRadius;\n"
  "void main()\n"
  "{\n"
  " // if(sqrt(pow(gl_PointCoord.x,2)+pow(gl_PointCoord.x-vOffset.y,2)) >= vPixRadius)\n"
  " //   gl_FragColor = vColor;\n"
  " // else\n"
  "// if(distance(gl_PointCoord.xy,vp.xy) <= 20)\n"
  "  // gl_FragColor = vColor;\n"
  " //else discard;\n"
  "//  if(vp.y>0.000001*vPixRadius)\n"
  " //   gl_FragColor = vColor;\n"
  " // else\n"
  " //   gl_FragColor = vec4(1,0,0,1);\n"
  " //   gl_FragColor = vColor;\n"
  "  gl_FragColor = vec4(vp.y,gl_PointCoord.y,vColor.z,1);\n"
  "//  gl_FragColor = vec4(vOffset.x,gl_PointCoord.y,vColor.z,1);\n"
  "}\n"
;


class MyQuad
{
  typedef struct Vec2   { GLfloat x, y; } Vec2;
  typedef struct Color4 { GLbyte r, g, b, a; } Color4;
  typedef struct QuadVertex
  {
    Vec2    vec;
    Color4  color;
    Vec2    txtrcrds;
  } QuadVertex;

  typedef QuadVertex Quad[4];
  Quad q;

public:

  MyQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLbyte r, GLbyte g, GLbyte b)
  {
    if(w<0) w=0; //else w--;
    if(h<0) h=0; //else h--;
    q[0].vec = (Vec2){x  , y  };
    q[1].vec = (Vec2){x+w, y  };
    q[2].vec = (Vec2){x+w, y+h};
    q[3].vec = (Vec2){x  , y+h};
    for(int i=0; i<4; i++)
    {
      q[i].color    = (Color4) {r, g, b, 0xff};
      q[i].txtrcrds = (Vec2)   {( i^(i>>1) ) & 1, i>>1}; // 00 > 10 > 11 > 01
    }
  }
};

float myrand()
{
  return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

extern "C"
{
  JNIEXPORT void Java_com_kay27_Glukalo_MyGLSurfaceView_nativeKeyPress(JNIEnv* env)
  {
    impulse = 1;
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyGLSurfaceView_nativeKeyRelease(JNIEnv* env)
  {
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyGLSurfaceView_nativePause(JNIEnv* env)
  {
    sStopped = 1;
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyGLSurfaceView_nativeResume(JNIEnv* env)
  {
    gettimeofday(&lastTime, NULL);
    sStopped = 0;
    impulse  = 0;
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyRenderer_nativeInit(JNIEnv* env)
  {
    gettimeofday(&lastTime, NULL);

    program = glCreateProgram(); // handle error //fixme // callback and toast?
    if(!program) return;

    GLint s = glCreateShader(GL_VERTEX_SHADER); // handle error //fixme // callback and toast? AND BELOW!!!
    if(!s) return;
    glShaderSource(s, 1, &squareVertexShader, NULL); glCompileShader(s); glAttachShader(program, s);

    s = glCreateShader(GL_FRAGMENT_SHADER);
    if(!s) return;
    glShaderSource(s, 1, &squareFragmentShader, NULL); glCompileShader(s); glAttachShader(program, s);

    glLinkProgram(program);

    if(!program) return;

    pos = glGetAttribLocation(program, "vPosition");
    color = glGetUniformLocation(program, "vColor");
    yoffset = glGetUniformLocation(program, "vOffset");
    pixradius = glGetUniformLocation(program, "vPixRadius");

    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD), &QUAD[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);

    yOffs            = 0;
    speed            = 0;
    impulse          = 0;
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyRenderer_nativeResize(JNIEnv* env, jobject thiz, jint w, jint h)
  {
    sWindowWidth  = w;
    sWindowHeight = h;
    glViewport(0, 0, w, h); //???FIX
  //  __android_log_print(ANDROID_LOG_INFO, "Glukalo", "resize w=%d h=%d", w, h);
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyRenderer_nativeRender(JNIEnv* env)
  {
    struct timeval now;
    gettimeofday(&now, NULL);
    float delta = (now.tv_sec - lastTime.tv_sec) * 1000000 + ((int)now.tv_usec - (int)lastTime.tv_usec);
    lastTime = now;

    glClearColor(myrand()/50, myrand()/20, myrand()/8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);
//    glTranslatef(0.0f, 0.0f, -0.5f);
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glVertexPointer(3, GL_FLOAT, sizeof(float)*3, vertices);
    glEnableVertexAttribArray(pos);
//    glEnableVertexAttribArray(color);
//   glEnableVertexAttribArray(yoffset);
//    glEnableVertexAttribArray(pixradius);
    glVertexAttribPointer(pos, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex,pos));

    if(impulse) {impulse=0; speed=-0.06;}
    speed+=delta/5000000;
    yOffs-=delta/27000*speed;
    if(yOffs<-0.8) { yOffs=-0.8; speed=0; /* echo BANG!!! */ }
    if(yOffs>1.5) { yOffs=1.5; speed=0; /* echo BANG!!! */ }

    glUniform4f(color, 0, 0.6, 1, 1);
    glUniform4f(yoffset, 0, yOffs, 0, 0);
    glUniform1f(pixradius, sWindowHeight/10);
//    glVertexAttribPointer(color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex) /*stride*/, (void*)offsetof(Vertex,rgba));

//    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

//    glDisableVertexAttribArray(1);
//    glDisableVertexAttribArray(0);

//    glDisableClientState(GL_VERTEX_ARRAY);

/*
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, false, 0, &vertices);
    glUniform4fv(color, 1, clr);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(pos);
*/
//    glBindBuffer(GL_ARRAY_BUFFER, vb);

/*
    GLfloat vertices[] = {-0.5, -0.5, 0, // bottom left corner
                          -0.5,  0.5, 0, // top left corner
                           0.5,  0.5, 0, // top right corner
                           0.5, -0.5, 0}; // bottom right corner

    GLubyte indices[] = {0,1,2, // first triangle (bottom left - top left - top right)
                         0,2,3}; // second triangle (bottom left - top right - bottom right)

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
*/
  }
}
