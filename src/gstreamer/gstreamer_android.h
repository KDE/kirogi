#ifndef GSTREAMER_ANDROID_H
#define GSTREAMER_ANDROID_H

#include <gst/gst.h>

G_BEGIN_DECLS

void gst_android_init(JNIEnv *env, jclass klass, jobject context);

G_END_DECLS

#endif
