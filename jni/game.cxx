#include <unistd.h>
#include <cstdlib>
#include <GLES2/gl2.h>
#include <jni.h>

static int sWindowWidth  = 720;
static int sWindowHeight = 1280;
static int sStopped      = 0;

/* filled circle
glEnable(GL_POINT_SMOOTH);
glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
glPointSize(radius << 1);
glPoint(x, y, z);
*/

static const char VERTEX_SHADER[] =
  "#version 100\n"
  "uniform mat2 scaleRot;\n"
  "uniform vec2 offset;\n"
  "attribute vec2 pos;\n"
  "attribute vec4 color;\n"
  "varying vec4 vColor;\n"
  "void main() {\n"
  "    gl_Position = vec4(scaleRot*pos + offset, 0.0, 1.0);\n"
  "    vColor = color;\n"
  "}\n";

static const char FRAGMENT_SHADER[] =
    "#version 100\n"
    "precision mediump float;\n"
    "varying vec4 vColor;\n"
    "void main() {\n"
    "    gl_FragColor = vColor;\n"
    "}\n";


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
    sStopped = 0;
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyRenderer_nativeInit(JNIEnv* env)
  {
    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if(!mProgram) { sStopped = 1; return; }

    mPosAttrib = glGetAttribLocation(mProgram, "pos");
    mColorAttrib = glGetAttribLocation(mProgram, "color");
    mScaleRotUniform = glGetUniformLocation(mProgram, "scaleRot");
    mOffsetUniform = glGetUniformLocation(mProgram, "offset");

    glGenBuffers(1, &mVB);
    glBindBuffer(GL_ARRAY_BUFFER, mVB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD), &QUAD[0], GL_STATIC_DRAW);

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
    glClearColor(myrand(), myrand(), myrand(), 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}
