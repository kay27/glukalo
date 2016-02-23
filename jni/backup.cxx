#include<unistd.h>
#include<cstdlib>
#include<GLES2/gl2.h>
#include<jni.h>
//#include<string.h>
//#include<cmath>

using namespace std;

//#include <JNIHelp.h>
//#include <android_runtime/AndroidRuntime.h>
//#define  LOG_TAG    "libJNIExInterface"  
//#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static JavaVM *jvm = NULL;
static JNIEnv *jnienv = NULL;
jobject jo = NULL;
static int sWindowWidth  = 333;
static int sWindowHeight = 155;
static int sPaused      = 0;
GLuint program           = 0; //class?//fixme
GLint pos                = 0;
GLint color              = 0;
GLint yoffset            = 0;
GLint radius             = 0;
GLint textureCoords      = 0;
GLint ymul               = 0;
GLint vSpeed             = 0;
float yMulValue          = 1;
GLuint vb;
GLfloat yOffs            = 0, xOffs = 0;
int started = 0;
int gameOver = 0;
int firstTime = 1;
GLfloat speed            = 0;
struct timeval lastTime;
int impulse              = 0;
float vect=-0.01;

struct Vertex {
    GLfloat pos[3];
//    GLubyte rgba[4];
    GLfloat txtcrds[2];
};

/*
const Vertex data[] = {
  {{ -0.05f, -0.05f,  0.0f }, {0.0f, 0.0f}},
  {{ -0.05f,  0.05f,  0.0f }, {0.0f, 1.0f}},
  {{  0.05f, -0.05f,  0.0f }, {1.0f, 0.0f}},
  {{  0.05f,  0.05f,  0.0f }, {1.0f, 1.0f}}, //fixme: why texture crds are float? :)
};
*/
const Vertex data[] = {
  {{ -0.10f, -0.10f,  0.0f }, {0.0f, 0.0f}},
  {{ -0.10f,  0.10f,  0.0f }, {0.0f, 1.0f}},
  {{  0.10f, -0.10f,  0.0f }, {1.0f, 0.0f}},
  {{  0.10f,  0.10f,  0.0f }, {1.0f, 1.0f}}, //fixme: why texture crds are float? :)
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
  "{\n"
  "  float distanceFromCenter = distance(vec2(vp.x/2.0, vp.y/2.0), vec2((tc.x-0.5)*vMul,tc.y-0.5));\n"
  "  float checkForPresenceWithinCircle = 1.0 - smoothstep(vRadius-0.05, vRadius+0.05, distanceFromCenter);\n"
  "  if(checkForPresenceWithinCircle<0.01) discard;\n"
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

float myrand()
{
  return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void Quit(const char *message)
{
  if(!jo) return;
  if(!jnienv) return;
  if(jclass c = jnienv->FindClass("com.kay27.Glukalo.MainActivity"))
    if(jmethodID m = jnienv->GetStaticMethodID(c, "ErrorCallback", "(Ljava/lang/String;)V"))
      jnienv->CallStaticVoidMethod(c, m, jnienv->NewStringUTF(message));
}

void Toast(const char *message)
{
  if(!jo) return;
  if(!jnienv) return;
  if(jclass c = jnienv->FindClass("com.kay27.Glukalo.MainActivity"))
    if(jmethodID m = jnienv->GetStaticMethodID(c, "ToastCallback", "(Ljava/lang/String;)V"))
      jnienv->CallStaticVoidMethod(c, m, jnienv->NewStringUTF(message));
}

void Restart()
{
  gameOver         = 0;
  yOffs            = 0;
  xOffs            = 0;
  speed            = 0;
  impulse          = 0;
  started          = 0;
  if(firstTime) Toast("Tap to play");
  firstTime = 0;
  vect = -0.0023;
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
    if(gameOver) Restart(); 
    else started = 1;
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyGLSurfaceView_nativeKeyRelease(JNIEnv* env)
  {
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyGLSurfaceView_nativePause(JNIEnv* env)
  {
    sPaused = 1;
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyGLSurfaceView_nativeResume(JNIEnv* env)
  {
    gettimeofday(&lastTime, NULL);
    sPaused = 0;
    impulse  = 0;
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyRenderer_nativeInit(JNIEnv* env, jobject obj)
  {
    jo = obj;
    jnienv = env;
    firstTime = 1;

    gettimeofday(&lastTime, NULL);

    program = glCreateProgram();
    if(!program) return Quit("Could not create OpenGL ES program: " + glGetError());

    GLint tmp[1];
    char l[512];

    GLint s = glCreateShader(GL_VERTEX_SHADER);
    if(!s) return Quit("Could not create vertex shader: " + glGetError());
    glShaderSource(s, 1, &birdVertexShader, NULL);
    glCompileShader(s);
    glGetShaderiv(s, GL_COMPILE_STATUS, tmp);
    if(!tmp[0])
    {
      glGetShaderInfoLog(s,512,tmp,l);
      Toast("Could not compile vertex shader:"); Quit(l);
      return;
    }
    glAttachShader(program, s);

    s = glCreateShader(GL_FRAGMENT_SHADER);
    if(!s) { Quit("Could not create fragment shader: " + glGetError()); return; }
    glShaderSource(s, 1, &birdFragmentShader, NULL);
    glCompileShader(s);
    glGetShaderiv(s, GL_COMPILE_STATUS, tmp);
    if(!tmp[0])
    {
      glGetShaderInfoLog(s,512,tmp,l);
      Toast("Could not compile fragment shader:"); Quit(l);
      return;
    }
    glAttachShader(program, s);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, tmp);
    if(tmp[0] != GL_TRUE)
    {
      glGetProgramInfoLog(program, 512, tmp, l);
      glDeleteProgram(program);
      Toast("GL program link error:"); Quit(l);
      return;
    }

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
//    glUniform1f(ymul, yMulValue);

    Restart();
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyRenderer_nativeResize(JNIEnv* env, jobject thiz, jint w, jint h)
  {
    sWindowWidth  = w;
    sWindowHeight = h;
    if(h>0)
    {
      yMulValue = float(w)/h;
//      glUseProgram(program);
      glUniform1f(ymul, yMulValue);
    }
    glViewport(0, 0, w, h); //???FIX
  //  __android_log_print(ANDROID_LOG_INFO, "Glukalo", "resize w=%d h=%d", w, h);
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyRenderer_nativeRender(JNIEnv* env)
  {
//    if(sPaused) return;
    struct timeval now;
    gettimeofday(&now, NULL);
    float delta = (now.tv_sec - lastTime.tv_sec) * 1000000 + ((int)now.tv_usec - (int)lastTime.tv_usec);
//    float progress = float(now.tv_sec)*1000 + float(now.tv_usec)/1000;
    lastTime = now;

    glClearColor(myrand()/50, myrand()/20, myrand()/8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glTranslatef(0.0f, 0.0f, -0.5f);
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glVertexPointer(3, GL_FLOAT, sizeof(float)*3, vertices);
//    glEnableVertexAttribArray(color);
//   glEnableVertexAttribArray(yoffset);
//    glEnableVertexAttribArray(radius);
//    glAttrib2f(textureCoords, 0.5, 0.4);

/*
    glUseProgram(program);
    glEnableVertexAttribArray(pos);
    glEnableVertexAttribArray(textureCoords);
    glVertexAttribPointer(pos, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex,pos));
    glVertexAttribPointer(textureCoords, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex,txtcrds));
    glUniform4f(color, 0, 0.6, 1, 1);
    glUniform1f(radius, 0.25);
    glUniform1f(ymul, yMulValue);
*/

    if(impulse) {impulse=0; speed=-0.06;}
    if(started)
    {
      speed+=delta/5000000;
      if(xOffs>-0.33)
      {
        xOffs=xOffs-delta/2000000;
        if(xOffs<-0.34) xOffs=-0.34;
      }
    }
    if(!gameOver) yOffs-=delta/27000*speed;
    if(!(started||gameOver))
    {
      yOffs+=vect*delta/253427; // more: slower initial animation
      if(yOffs>0.008) {yOffs=0.008; vect=-0.0095;}
      if(yOffs<-0.006) {yOffs=-0.006; vect=0.011;}
    }

    if(yOffs<-0.8998) if(!gameOver) { yOffs=-0.8998; gameOver=1; Toast("Game over"); }
    if(yOffs>1.09) { yOffs=1.09; speed=0; }

    glUniform4f(yoffset, xOffs, yOffs, 0, 0);
    glUniform1f(vSpeed, speed);
//    glVertexAttribPointer(color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex) /*stride*/, (void*)offsetof(Vertex,rgba));

//    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

//    glDisableVertexAttribArray(1);
//    glDisableVertexAttribArray(0);

//    glDisableClientState(GL_VERTEX_ARRAY);

  }
}
