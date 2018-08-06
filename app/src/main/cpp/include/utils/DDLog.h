#ifndef LOGJAM_H
#define LOGJAM_H

#include <android/log.h> 

#define LOGTAG "DDLog"

#define DDLogV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOGTAG, __VA_ARGS__)
#define DDLogD(...) __android_log_print(ANDROID_LOG_DEBUG  , LOGTAG, __VA_ARGS__)
#define DDLogI(...) __android_log_print(ANDROID_LOG_INFO   , LOGTAG, __VA_ARGS__)
#define DDLogW(...) __android_log_print(ANDROID_LOG_WARN   , LOGTAG, __VA_ARGS__)
#define DDLogE(...) __android_log_print(ANDROID_LOG_ERROR  , LOGTAG, __VA_ARGS__)
	
#endif