#include <jni.h>
#include <string.h>
#include <utils/jniLog.h>
extern "C"
{
#include <libavformat/avformat.h>
#include <libavfilter/version.h>
}


//#define LOG_TAG ("JNI-DDLog")
//#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO   , LOG_TAG, __VA_ARGS__))
//#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR  , LOG_TAG, __VA_ARGS__))

extern "C" JNIEXPORT jstring

JNICALL
Java_com_example_administrator_ffmpegandroiddemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    av_register_all();
    LOGI("ffmpeg配置为:%s\n", avcodec_configuration);

    return env->NewStringUTF("hello is me ");
}
