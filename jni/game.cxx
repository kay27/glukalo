#include <GLES2/gl2.h>
#include <jni.h>

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

JNIEXPORT void Java_com_android_glukalo_MyRenderer_nativeInit(JNIEnv* env)
{
}

int main()
{
  MyQuad q(30, 30, 40, 40, 255, 0, 255);

  
}