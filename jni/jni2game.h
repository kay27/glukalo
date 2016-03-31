#ifndef H_JNI2GAME_GLUKALO
# define H_JNI2GAME_GLUKALO

# include <jni.h>
# include "game.h"

  static JavaVM *jvm       = NULL;
  static JNIEnv *jnienv    = NULL;
  static jobject jo        = NULL;

  static int sWindowWidth  = 333;
  static int sWindowHeight = 155;
  static int sPaused       = 0;

  static Game *game        = NULL;


  class MyCallback
  {
    public:
      static void Toast(const char *message) { Call("ToastCallback", message); }
      static void Quit(const char *message) { Call("ErrorCallback", message); }
      static void SetHighScore(const int hs) { Call("SetHighScore", hs); }
      static int GetHighScore() { return Call("GetHighScore"); }

    protected:
      static void Call(const char *javaStaticMethod, const char *message);
      static void Call(const char *javaStaticMethod, const int n);
      static int Call(const char *javaStaticMethod);
  };

#endif // #ifndef H_JNI2GAME_GLUKALO
