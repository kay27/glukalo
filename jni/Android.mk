LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := glukalo

LOCAL_SRC_FILES := \
  column.cxx \
  game.cxx \
  game_audio.cxx \
  jni2game.cxx \
  missile.cxx \
  shader.cxx \
  sound.cxx \
  system.cxx \

LOCAL_CPPFLAGS := -std=c++11

LOCAL_LDLIBS := -lGLESv2 -lOpenSLES -ldl

include $(BUILD_SHARED_LIBRARY)
