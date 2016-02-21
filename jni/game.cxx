#include <unistd.h>
#include <cstdlib>
#include <GLES2/gl2.h>
#include <jni.h>

static int sWindowWidth  = 720;
static int sWindowHeight = 1280;
static int sStopped      = 0;
GLuint program           = 0; //class?//fixme
GLint pos                = 0;
GLint color              = 0;
GLuint vb;

struct Vertex {
    GLfloat pos[3];
    GLubyte rgba[4];
};

const Vertex data[] = {
  {{  0.0f,  0.5f,  0.0f }, {0, 160, 255, 255}},
  {{ -0.5f, -0.5f,  0.0f }, {0, 160, 255, 255}},
  {{  0.5f, -0.5f,  0.0f }, {0, 160, 255, 255}}
};

//static const char vertShader[] =
static const char * squareVertexShader =
  "attribute vec4 vPosition;\n"
  "void main()\n"
  "{\n"
  "  gl_Position = vPosition;\n"
  "}\n"
;

static const char * squareFragmentShader =
  "precision mediump float;\n"
  "uniform vec4 vColor;\n"
  "void main()\n"
  "{\n"
  "  gl_FragColor = vColor;\n"
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

    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD), &QUAD[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data[0], GL_STATIC_DRAW);

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

    glUseProgram(program);
//    glTranslatef(0.0f, 0.0f, -0.5f);
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glVertexPointer(3, GL_FLOAT, sizeof(float)*3, vertices);
    glEnableVertexAttribArray(pos);
    glEnableVertexAttribArray(color);
    glVertexAttribPointer(pos, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex,pos));

    glUniform4f(color, 0, 0.6, 1, 1);
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
