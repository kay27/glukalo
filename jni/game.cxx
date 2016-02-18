#include <unistd.h>
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

class MyQuad
{
  typedef struct Vec2   { GLint x, y; } Vec2;
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

  MyQuad(GLint x, GLint y, GLint w, GLint h, GLbyte r, GLbyte g, GLbyte b)
  {
    if(w<0) w=0; else w--;
    if(h<0) h=0; else h--;
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
  //                  com.kay27.Glukalo.MyRenderer.nativeInit
  {
  //  MyQuad q(30, 30, 40, 40, 255, 0, 255);
    usleep(5000000);
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyRenderer_nativeResize(JNIEnv* env, jobject thiz, jint w, jint h)
  {
    sWindowWidth  = w;
    sWindowHeight = h;
  //  __android_log_print(ANDROID_LOG_INFO, "Glukalo", "resize w=%d h=%d", w, h);
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyRenderer_nativeRender(JNIEnv* env)
  {
  }
}
