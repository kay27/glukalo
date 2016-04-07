package com.kay27.Glukalo;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.content.res.Configuration;
import android.content.SharedPreferences;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;
//import java.lang.Thread;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;

public class MainActivity extends Activity
{
  private static Activity activity;

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    activity = this;
    ActivityManager am = (ActivityManager)getSystemService(Context.ACTIVITY_SERVICE);
    ConfigurationInfo info = am.getDeviceConfigurationInfo();
    boolean supportES2 = (info.reqGlEsVersion >= 0x20000);
    if(supportES2)
    {
      requestWindowFeature(Window.FEATURE_NO_TITLE);
      getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
      mGLView = new MyGLSurfaceView(this);
      setContentView(mGLView);
    }
    else
      Toast.makeText(this, "Your device doesn't support ES2. (" + info.reqGlEsVersion + ")", Toast.LENGTH_LONG).show();
  }

  @Override
  protected void onPause()
  {
    super.onPause();
    mGLView.onPause();
  }

  @Override
  protected void onResume()
  {
    super.onResume();
    mGLView.onResume();
  }

  @Override
  public void onConfigurationChanged(Configuration newConfig)
  {
    super.onConfigurationChanged(newConfig);
    getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
  }

  public static void ErrorCallback(final String message)
  {
    activity.runOnUiThread(new Runnable() {
      public void run(){ Toast.makeText(activity, message, Toast.LENGTH_LONG).show(); } } );
    activity.finish();
  }

  public static void ToastCallback(final String message)
  {
    activity.runOnUiThread(new Runnable() {
      public void run(){ Toast.makeText(activity, message, Toast.LENGTH_SHORT).show(); } } );
  }

  public static final String PREFS_ID = "com_kay27_Glukalo_store"; 
  public static void CBSetHighScore(final int hs)
  {
    SharedPreferences.Editor editor = activity.getSharedPreferences(PREFS_ID, MODE_PRIVATE).edit();
    editor.putInt("hs", hs);
    editor.commit();
  }
  public static int CBGetHighScore()
  {
    return activity.getSharedPreferences(PREFS_ID, MODE_PRIVATE).getInt("hs", 0);
  }

  private MyGLSurfaceView mGLView;

  static { System.loadLibrary("glukalo"); }

}

class MyGLSurfaceView extends GLSurfaceView
{
  public MyGLSurfaceView(Context context)
  {
    super(context);
    setEGLContextClientVersion(2);
    mRenderer = new MyRenderer();
    setPreserveEGLContextOnPause(true); //???fix
    setRenderer(mRenderer);
    requestRender(); //temp
  }

  public boolean onTouchEvent(final MotionEvent event)
  {
    int action = event.getAction();
    if(action == MotionEvent.ACTION_DOWN) nativeKeyPress();
//    else if(action == MotionEvent.ACTION_UP) nativeKeyRelease();
    return true;
  }

  @Override
  public void onPause()
  {
    super.onPause();
    nativePause();
  }

  @Override
  public void onResume()
  {
    super.onResume();
    nativeResume();
  }

  private MyRenderer mRenderer;

  private static native void nativePause();
  private static native void nativeResume();
  private static native void nativeKeyPress();
  private static native void nativeKeyRelease();

}

class MyRenderer implements GLSurfaceView.Renderer
{
  public void onSurfaceCreated(GL10 gl, EGLConfig config) { nativeInit(); }
  public void onSurfaceChanged(GL10 gl, int w, int h) { nativeResize(w, h); }
  public void onDrawFrame(GL10 gl) { nativeRender(); }

  private static native void nativeInit();
  private static native void nativeResize(int w, int h);
  private static native void nativeRender();
}
