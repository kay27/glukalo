LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := glukalo

LOCAL_SRC_FILES := \
    game.cxx \

#LOCAL_CXXFLAGS := -std=c++11

LOCAL_LDLIBS := -lGLESv2 -ldl -llog

include $(BUILD_SHARED_LIBRARY)
