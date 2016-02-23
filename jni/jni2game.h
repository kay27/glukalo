#ifndef H_JNI2GAME_GLUKALO
# define H_JNI2GAME_GLUKALO

# include "game.h"

  static JavaVM *jvm       = NULL;
  static JNIEnv *jnienv    = NULL;
  static jobject jo        = NULL;
  static int sWindowWidth  = 333;
  static int sWindowHeight = 155;
  static int sPaused       = 0;

  static Game *game        = NULL;

  extern "C"
  {

    JNIEXPORT void Java_com_kay27_Glukalo_MyGLSurfaceView_nativeKeyPress(JNIEnv* env)
    {
      if(game) game->Tap();
    }

    JNIEXPORT void Java_com_kay27_Glukalo_MyGLSurfaceView_nativePause(JNIEnv* env)
    {
      sPaused = 1;

      if(game) game->Pause();
    }

    JNIEXPORT void Java_com_kay27_Glukalo_MyGLSurfaceView_nativeResume(JNIEnv* env)
    {
      sPaused = 0;

      if(game) game->Resume();
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
      if(game) game->Resize(w, h);
    }

    JNIEXPORT void Java_com_kay27_Glukalo_MyRenderer_nativeRender(JNIEnv* env)
    {
      if(game) game->Render();
    }

  } // extern "C"

#endif // #ifndef H_JNI2GAME_GLUKALO
