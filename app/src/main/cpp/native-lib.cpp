#include <jni.h>
#include <string.h>


extern "C"
{
#include <libavformat/avformat.h>
#include <libavfilter/version.h>
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavutil/imgutils.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include <utils/DDLog.h>
#include <android/window.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>


};



//#define LOG_TAG ("JNI-DDLog")
//#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO   , LOG_TAG, __VA_ARGS__))
//#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR  , LOG_TAG, __VA_ARGS__))

extern "C" JNIEXPORT jstring

JNICALL
Java_com_example_administrator_ffmpegandroiddemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    av_register_all();
    DDLogI("ffmpeg配置为:%s\n", avcodec_configuration);

    return env->NewStringUTF("hello is me ");
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_administrator_ffmpegandroiddemo_MainActivity_playVideo(JNIEnv *env,
                                                                        jobject instance,
                                                                        jstring path_,
                                                                        jobject surfaceView) {


    // sd卡中的视频文件地址,可自行修改或者通过jni传入
    const char *filepath = env->GetStringUTFChars(path_, 0);

    av_register_all();

    AVFormatContext *pFormatCtx = avformat_alloc_context();

    // Open video file
    if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0) {

        DDLogI("Couldn't open file:%s\n", filepath);
        return -1; // Couldn't open file
    }

    // Retrieve stream information
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        DDLogI("Couldn't find stream information.");
        return -1;
    }

    // Find the first video stream
    int videoStream = -1, i;
    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO
            && videoStream < 0) {
            videoStream = i;
        }
    }

    if (videoStream == -1) {
        DDLogI("Didn't find a video stream.");
        return -1; // Didn't find a video stream
    }

    // Get a pointer to the codec context for the video stream
    AVCodecContext *pCodecCtx = pFormatCtx->streams[videoStream]->codec;

    // Find the decoder for the video stream
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL) {
        DDLogI("Codec not found.");
        return -1; // Codec not found
    }

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        DDLogI("Could not open codec.");
        return -1; // Could not open codec
    }

    // 获取native window
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surfaceView);

    // 获取视频宽高
    int videoWidth = pCodecCtx->width;
    int videoHeight = pCodecCtx->height;

    DDLogI("video resolution: %d , %d ",videoWidth,videoHeight);

    // 设置native window的buffer大小,可自动拉伸
    ;


    if (0 > ANativeWindow_setBuffersGeometry(nativeWindow, videoWidth, videoHeight,
                                             WINDOW_FORMAT_RGBX_8888)){
        DDLogE("Couldn't set buffers geometry.\n");
        ANativeWindow_release(nativeWindow);
        return -1;
    }


    ANativeWindow_Buffer windowBuffer;
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        DDLogI("Could not open codec.");
        return -1; // Could not open codec
    }

    // Allocate video frame
    AVFrame *pFrame = av_frame_alloc();
    // 用于渲染
    AVFrame *pFrameRGBA = av_frame_alloc();
    if (pFrameRGBA == NULL || pFrame == NULL) {
        DDLogI("Could not allocate video frame.");
        return -1;
    }



    // Determine required buffer size and allocate buffer
    // buffer中数据就是用于渲染的,且格式为RGBA
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, pCodecCtx->width, pCodecCtx->height,
                                            1);
    uint8_t *buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(pFrameRGBA->data, pFrameRGBA->linesize, buffer, AV_PIX_FMT_RGBA,
                         pCodecCtx->width, pCodecCtx->height, 1);

    // 由于解码出来的帧格式不是RGBA的,在渲染之前需要进行格式转换
    struct SwsContext *sws_ctx = sws_getContext(pCodecCtx->width,
                                                pCodecCtx->height,
                                                pCodecCtx->pix_fmt,
                                                pCodecCtx->width,
                                                pCodecCtx->height,
                                                AV_PIX_FMT_RGBA,
                                                SWS_BILINEAR,
                                                NULL,
                                                NULL,
                                                NULL);


    int frameFinished;
    AVPacket packet;
    while (av_read_frame(pFormatCtx, &packet) >= 0) {
        // Is this a packet from the video stream?
        if (packet.stream_index == videoStream) {

            // Decode video frame
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

            // 并不是decode一次就可解码出一帧
            if (frameFinished) {

                // lock native window buffer
                ANativeWindow_lock(nativeWindow, &windowBuffer, 0);

                // 格式转换
                sws_scale(sws_ctx, (uint8_t const *const *) pFrame->data,
                          pFrame->linesize, 0, pCodecCtx->height,
                          pFrameRGBA->data, pFrameRGBA->linesize);

                // 获取stride
                uint8_t *dst = (uint8_t *) windowBuffer.bits;
                int dstStride = windowBuffer.stride * 4;
                uint8_t *src = (pFrameRGBA->data[0]);
                int srcStride = pFrameRGBA->linesize[0];

                // 由于window的stride和帧的stride不同,因此需要逐行复制
                int h;
                for (h = 0; h < videoHeight; h++) {
                    memcpy(dst + h * dstStride, src + h * srcStride, srcStride);
                }

                ANativeWindow_unlockAndPost(nativeWindow);
            }

        }
        av_packet_unref(&packet);
    }

    av_free(buffer);
    av_free(pFrameRGBA);

    // Free the YUV frame
    av_free(pFrame);

    // Close the codecs
    avcodec_close(pCodecCtx);

    // Close the video file
    avformat_close_input(&pFormatCtx);


    return 0;

}
