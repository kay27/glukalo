package com.kay27.Glukalo;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.content.res.Configuration;
import android.content.SharedPreferences;
import android.graphics.Point;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;
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
      Display display = getWindowManager().getDefaultDisplay();
      Point size = new Point();
      display.getSize(size);
      mGLView = new MyGLSurfaceView(this, size.x, size.y);
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
    Display display = getWindowManager().getDefaultDisplay();
    Point size = new Point();
    display.getSize(size);
    mGLView.Resize(size.x, size.y);
  }

  @Override
  public void onBackPressed()
  {
    if(nativeOnBackPressed() == 0)
      activity.finish();
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
  public static final int PREFS_KEY = 5553;
  public static void CBSetHighScore(final int hs)
  {
    SharedPreferences.Editor editor = activity.getSharedPreferences(PREFS_ID, MODE_PRIVATE).edit();
    editor.putInt("hs", hs);
    editor.putInt("hh", PREFS_KEY);
    editor.commit();
  }
  public static int CBGetHighScore()
  {
    if(activity.getSharedPreferences(PREFS_ID, MODE_PRIVATE).getInt("hh",-1) != PREFS_KEY)
      return -1;
    return activity.getSharedPreferences(PREFS_ID, MODE_PRIVATE).getInt("hs", -1);
  }
  public static void CBSetMuteState(final int ms)
  {
    SharedPreferences.Editor editor = activity.getSharedPreferences(PREFS_ID, MODE_PRIVATE).edit();
    editor.putInt("ms", ms);
    editor.commit();
  }
  public static int CBGetMuteState()
  {
    return activity.getSharedPreferences(PREFS_ID, MODE_PRIVATE).getInt("ms", -1);
  }

  private MyGLSurfaceView mGLView;

  static { System.loadLibrary("glukalo"); }

  private static native int nativeOnBackPressed();
}

class MyGLSurfaceView extends GLSurfaceView
{
  public MyGLSurfaceView(Context context, int w, int h)
  {
    super(context);
    halfH=w/2; halfW=h/2;
    setEGLContextClientVersion(2);
    mRenderer = new MyRenderer();
    setRenderer(mRenderer);
    requestRender(); //temp
  }

  public void Resize(int w, int h)
  {
    halfH=w/2; halfW=h/2;
  }

  public boolean onTouchEvent(final MotionEvent event)
  {
    int action = event.getAction();
    if(action == MotionEvent.ACTION_DOWN) nativeKeyPress(event.getX()/halfH-1, 1-event.getY()/halfW);
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
  private float halfW, halfH;

  private static native void nativePause();
  private static native void nativeResume();
  private static native void nativeKeyPress(float x, float y);

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
