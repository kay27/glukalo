#ifndef H_JNI2GAME_GLUKALO
# define H_JNI2GAME_GLUKALO

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
      void Toast(const char *message) { Call("ToastCallBack", message); }
      void Quit(const char *message) { Call("ErrorCallback", message); }

    protected:
      static void Call(const char *javaStaticMethod, const char *message);
  }

#endif // #ifndef H_JNI2GAME_GLUKALO
