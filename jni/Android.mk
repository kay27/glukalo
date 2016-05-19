LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := glukalo

LOCAL_SRC_FILES := \
  bonus.cxx \
  column.cxx \
  column_preview.cxx \
  game.cxx \
  game_audio.cxx \
  icon.cxx \
  jni2game.cxx \
  missile.cxx \
  shader.cxx \
  system.cxx \

LOCAL_CPPFLAGS := -std=c++11

LOCAL_LDLIBS := -lGLESv2 -lOpenSLES -ldl

include $(BUILD_SHARED_LIBRARY)
