#include<unistd.h>
#include<cstdlib>
#include<GLES2/gl2.h>
#include<jni.h>

//#include <JNIHelp.h>
//#include <android_runtime/AndroidRuntime.h>
//#define  LOG_TAG    "libJNIExInterface"  
//#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static JavaVM *jvm = NULL;
static JNIEnv *jnienv = NULL;
jobject jo = NULL;
static int sWindowWidth  = 720;
static int sWindowHeight = 1280;
static int sStopped      = 0;
GLuint program           = 0; //class?//fixme
GLint pos                = 0;
GLint color              = 0;
GLint yoffset            = 0;
GLint radius             = 0;
GLint textureCoords      = 0;
GLuint vb;
GLfloat yOffs            = 0;
GLfloat speed            = 0;
struct timeval lastTime;
int impulse              = 0;

struct Vertex {
    GLfloat pos[3];
//    GLubyte rgba[4];
    GLfloat txtcrds[2];
};


const Vertex data[] = {
  {{ -0.05f, -0.05f,  0.0f }, {0.0f, 0.0f}},
  {{ -0.05f,  0.05f,  0.0f }, {0.0f, 1.0f}},
  {{  0.05f, -0.05f,  0.0f }, {1.0f, 0.0f}},
  {{  0.05f,  0.05f,  0.0f }, {1.0f, 0.0f}}, //fixme: why texture crds are float? :)
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
  "attribute vec4 vTextureCoordinate;\n"
  "uniform vec4 vOffset;\n"
  "varying vec4 vp;\n"
  "varying vec2 tc;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vPosition + vOffset;\n"
  "  vp = vOffset;\n"
  "  tc = vTextureCoordinate.xy;\n"
  "}\n"
;

static const char * squareFragmentShader =
  "precision mediump float;\n"
  "varying vec4 vp;\n"
  "varying vec2 tc;\n"
  "uniform vec4 vColor;\n"
  "uniform float vRadius;\n"
  "void main()\n"
  "{\n"
  "  gl_FragColor = vecr(vColor.x,vColor.y,tc.x,tc.y);\n"
  "//  if(vColor == vec4(1,1,1,1)\n"
  "//  {\n"
  "//  float distanceFromCenter = distance(vec2(0.5, vp.y/2+0.5), tc);\n"
  "//  float checkForPresenceWithinCircle = step(distanceFromCenter, vRadius);\n"
  "//  gl_FragColor = vColor * checkForPresenceWithinCircle;\n"
  "//  }\n"
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

void Quit(const char *message)
{
/*
  if(jclass Toast = jnienv->FindClass("android/widget/Toast"))
  {
    if(jmethodID makeText = jnienv->GetStaticMethodID(Toast, "makeText", "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;"))
    {
      if(jobject toast = jnienv->CallStaticObjectMethod(Toast, makeText, jo, jnienv->NewStringUTF(message), 3000))
      {
        if(jmethodID show = jnienv->GetMethodID(Toast,"show","()V"))
        {
          jnienv->CallVoidMethod(toast,show);
          return; // toast launched and will deleted in time
        }
        delete toast;
      }
      delete makeText;
    }
    delete Toast;
  }
*/
  if(!jo) return;
  if(!jnienv) return;
  if(jclass c = jnienv->FindClass("com.kay27.Glukalo.MainActivity"))
//  if(jclass c = jnienv->GetObjectClass(jo))
    if(jmethodID m = jnienv->GetStaticMethodID(c, "ErrorCallback", "(Ljava/lang/String;)V"))
      jnienv->CallStaticVoidMethod(c, m, jnienv->NewStringUTF(message));
//  return;

//  if(!jvm) { if(!jnienv) return; jnienv->GetJavaVM(&jvm); if(!jvm) return; }



//  JNIEnv *env;

//  if(jvm->AttachCurrentThread(&env, NULL) < 0) return;
//  if(jclass c = env->GetObjectClass(jo))
//    if(jmethodID m = env->GetMethodID(c, "ErrorCallback", "(Ljava/lang/String;)V"))
//      env->CallVoidMethod(c, m, env->NewStringUTF(message));
//  jvm->DetachCurrentThread();
}

extern "C"
{

  JNIEXPORT jint JNI_Onload(JavaVM *vm, void *reserved)
  {
    jvm = vm;
    return JNI_VERSION_1_4;
  }

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

  JNIEXPORT void Java_com_kay27_Glukalo_MyRenderer_nativeInit(JNIEnv* env, jobject obj)
  {
    jo = obj;
    jnienv = env;

    gettimeofday(&lastTime, NULL);

    program = glCreateProgram();
    if(!program) return Quit("Could not create OpenGL ES program: " + glGetError());

    GLint s = glCreateShader(GL_VERTEX_SHADER);
    if(!s) return Quit("Could not create vertex shader: " + glGetError());

    glShaderSource(s, 1, &squareVertexShader, NULL);
    glCompileShader(s);
    glAttachShader(program, s);

    s = glCreateShader(GL_FRAGMENT_SHADER);
    if(!s) return Quit("Could not create fragment shader: " + glGetError());

    glShaderSource(s, 1, &squareFragmentShader, NULL);
    glCompileShader(s);
    glAttachShader(program, s);

    glLinkProgram(program);
    if(!program) return Quit("GL program error: " + glGetError());

    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);

    pos = glGetAttribLocation(program, "vPosition");
    textureCoords = glGetAttribLocation(program, "vTextureCoordinate");
    color = glGetUniformLocation(program, "vColor");
    yoffset = glGetUniformLocation(program, "vOffset");
    radius = glGetUniformLocation(program, "vRadius");

    yOffs            = 0;
    speed            = 0;
    impulse          = 0;
Quit("Test error message just for fun");
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
    glEnableVertexAttribArray(textureCoords);
//    glEnableVertexAttribArray(color);
//   glEnableVertexAttribArray(yoffset);
//    glEnableVertexAttribArray(radius);
    glVertexAttribPointer(pos, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex,pos));
    glVertexAttribPointer(textureCoords, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex,txtcrds));
//    glAttrib2f(textureCoords, 0.5, 0.4);

    if(impulse) {impulse=0; speed=-0.06;}
    speed+=delta/5000000;
    yOffs-=delta/27000*speed;
    if(yOffs<-0.8) { yOffs=-0.8; speed=0; /* echo BANG!!! */ }
    if(yOffs>1.5) { yOffs=1.5; speed=0; /* echo BANG!!! */ }

    glUniform4f(color, 0, 0.6, 1, 1);
    glUniform4f(yoffset, 0, yOffs, 0, 0);
    glUniform1f(radius, 0.5);
//    glVertexAttribPointer(color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex) /*stride*/, (void*)offsetof(Vertex,rgba));

//    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

//    glDisableVertexAttribArray(1);
//    glDisableVertexAttribArray(0);

//    glDisableClientState(GL_VERTEX_ARRAY);

  }
}
