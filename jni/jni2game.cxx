#include "jni2game.h"

void MyCallback::Call(const char *javaStaticMethod, const char *message)
{
  if(jo && jnienv)
    if(jclass c = jnienv->FindClass(ACTIVITY_NAME))
      if(jmethodID m = jnienv->GetStaticMethodID(c, javaStaticMethod, "(Ljava/lang/String;)V"))
        jnienv->CallStaticVoidMethod(c, m, jnienv->NewStringUTF(message));
}

void MyCallback::Call(const char *javaStaticMethod, const int n)
{
  if(jo && jnienv)
    if(jclass c = jnienv->FindClass(ACTIVITY_NAME))
      if(jmethodID m = jnienv->GetStaticMethodID(c, javaStaticMethod, "(I)V"))
        jnienv->CallStaticVoidMethod(c, m, n);
}

int MyCallback::Call(const char *javaStaticMethod)
{
  if(jo && jnienv)
    if(jclass c = jnienv->FindClass(ACTIVITY_NAME))
      if(jmethodID m = jnienv->GetStaticMethodID(c, javaStaticMethod, "()I"))
        return jnienv->CallStaticIntMethod(c, m);
  return -1;
}


extern "C"
{

  JNIEXPORT void Java_com_kay27_Glukalo_MyGLSurfaceView_nativeKeyPress(JNIEnv* env, jobject thiz, float x, float y)
  {
    if(game)
      game->Tap(x, y);
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyGLSurfaceView_nativePause(JNIEnv* env)
  {
    sPaused = 1;
    if(game)
      game->Pause();
  }

  JNIEXPORT void Java_com_kay27_Glukalo_MyGLSurfaceView_nativeResume(JNIEnv* env)
  {
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
