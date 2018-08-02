//
// Created by Administrator on 2018/8/2/002.
//
#include "android/log.h"
#ifndef FFMPEGANDROIDDEMO_JNILOG_H
#define FFMPEGANDROIDDEMO_JNILOG_H

#define LOG_TAG ("JNI-DDLog")

#define LOGV(...) ((void)__android_log_print(ANDROID_LOG_VERBOSE  , LOG_TAG, __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG  , LOG_TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO   , LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN  , LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR  , LOG_TAG, __VA_ARGS__))




#endif //FFMPEGANDROIDDEMO_JNILOG_H
