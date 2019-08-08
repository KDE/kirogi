#include <QtAndroid>
#include <QAndroidJniEnvironment>
#include <gst/gst.h>
#include "gstreamer_android.h"

extern "C" gboolean gst_qt_android_init (GError ** error)
{
  QAndroidJniEnvironment jniEnv;
  JNIEnv *env = jniEnv;
  QAndroidJniObject QJObj = QtAndroid::androidActivity();
  jobject context = QJObj.object<jobject> ();

  gst_android_init (jniEnv, NULL, context);

  if (env->ExceptionCheck()) {
    jclass throwable_class = env->FindClass("java/lang/Throwable");
    jmethodID throwable_to_string_id = env->GetMethodID(throwable_class, "toString", "()Ljava/lang/String;");
    jstring msg_obj = (jstring) env->CallObjectMethod (context, throwable_to_string_id);
    const char* msg_str = env->GetStringUTFChars (msg_obj, 0);

    g_set_error (error, GST_LIBRARY_ERROR, GST_LIBRARY_ERROR_INIT, "%s", msg_str);

    env->ReleaseStringUTFChars (msg_obj, msg_str);
    env->DeleteLocalRef (throwable_class);
    env->DeleteLocalRef (msg_obj);

    return FALSE;
  }

  return TRUE;
}
