package com.kay27.Glukalo;

import android.app.Activity;
import android.app.ActivityManager;
import android.os.Bundle;
import android.opengl.GLSurfaceView;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.view.MotionEvent;
import android.view.WindowManager;
import android.util.Log;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;
import android.widget.Toast;

public class MainActivity extends Activity
{
  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    ActivityManager am = (ActivityManager)getSystemService(Context.ACTIVITY_SERVICE);
    ConfigurationInfo info = am.getDeviceConfigurationInfo();
    boolean supportES2 = (info.reqGlEsVersion >= 0x20000);
    if(supportES2)
    {
      Toast.makeText(this, "Your device supports ES2! (" + info.reqGlEsVersion + ")", Toast.LENGTH_LONG).show();
//      getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
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

  private void ErrorCallback(int depth) //??????????????????????
  {
    System.exit(0);
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
    else if(action == MotionEvent.ACTION_UP) nativeKeyRelease();
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
