#include "config.h"
#include "jni2game.h"
#include "game.h"

void MyCallBack::Call(const char *javaStaticMethod, const char *message)
{
  if(jo && jnienv && jclass c = jnienv->FindClass(ACTIVITY_NAME))
    if(jmethodID m = jnienv->GetStaticMethodID(c, javaStaticMethod, "(Ljava/lang/String;)V"))
      jnienv->CallStaticVoidMethod(c, m, jnienv->NewStringUTF(message));
}


extern "C"
{

  JNIEXPORT void Java_com_kay27_Glukalo_MyGLSurfaceView_nativeKeyPress(JNIEnv* env)
  {
    if(game)
      game->Tap();
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyGLSurfaceView_nativePause(JNIEnv* env)
  {
    sPaused = 1;
    if(game)
      game->Pause();
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyGLSurfaceView_nativeResume(JNIEnv* env)
2  {
    sPaused = 0;
    if(game)
      game->Resume();
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyRenderer_nativeInit(JNIEnv* env, jobject obj)
  {
    jo = obj;
    jnienv = env;

    if(game)
    {
      delete game;
      game = NULL;
    }
    game = new Game();
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyRenderer_nativeResize(JNIEnv* env, jobject thiz, jint w, jint h)
  {
    sWindowWidth  = w;
    sWindowHeight = h;
    if(game)
      game->Resize(w, h);
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyRenderer_nativeRender(JNIEnv* env)
  {
    if(game)
      game->Render();
  }

} // extern "C"
