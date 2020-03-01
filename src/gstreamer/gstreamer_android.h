#pragma once

#include <gst/gst.h>
#include <jni.h>

G_BEGIN_DECLS

void gst_android_init(JNIEnv *env, jclass klass, jobject context);

G_END_DECLS
