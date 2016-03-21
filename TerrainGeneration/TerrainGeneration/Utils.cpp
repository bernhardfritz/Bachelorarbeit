//
//  Utils.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 20/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include "Utils.hpp"
#include <sys/time.h>
#include <sys/stat.h>
#include <GLFW/glfw3.h> // GLFW helper library
extern "C" {
    #include <libavutil/opt.h>
    #include <libavutil/imgutils.h>
    #include <libswscale/swscale.h>
}
#include "stb_image_write.h"

unsigned char* Utils::g_video_memory_start = NULL;
unsigned char* Utils::g_video_memory_ptr = NULL;
int Utils::g_video_seconds_total = 5;
int Utils::g_video_fps = 25;
bool Utils::dump_video = false;
double Utils::video_timer = 0.0;
double Utils::video_dump_timer = 0.0;
double Utils::frame_time = 1.0/Utils::g_video_fps;
int Utils::frame_number = 0;

void Utils::screenshot() {
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    unsigned char* buffer = new unsigned char[width * height * 3];
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    char name[1024];
    long int t = time(NULL);
    mkdir("screenshots", 0775);
    sprintf(name, "screenshots/screenshot%ld.png", t);
    unsigned char* last_row = buffer + (width * 3 * (height - 1));
    if(!stbi_write_png(name, width, height, 3, last_row, -3 * width)) {
        fprintf(stderr, "ERROR: could not write screenshot file %s\n", name);
    }
    delete [] buffer;
}

void Utils::reserve_video_memory() {
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    Utils::g_video_memory_ptr = new unsigned char[width * height * 3 * Utils::g_video_fps * Utils::g_video_seconds_total];
    Utils::g_video_memory_start = Utils::g_video_memory_ptr;
}

void Utils::grab_video_frame() {
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, Utils::g_video_memory_ptr);
    Utils::g_video_memory_ptr += width * height * 3;
}

bool Utils::dump_video_frame() {
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    char name[1024];
    mkdir("videos", 0775);
    sprintf(name, "videos/frame%03d.png", Utils::frame_number);
    unsigned char* last_row = Utils::g_video_memory_ptr + (width * 3 * (height - 1));
    if(!stbi_write_png(name, width, height, 3, last_row, -3 * width)) {
        fprintf(stderr, "ERROR: could not write frame file %s\n", name);
        return false;
    }
    Utils::frame_number++;
    return true;
}

bool Utils::dump_video_frames() {
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    Utils::frame_number = 0;
    Utils::g_video_memory_ptr = Utils::g_video_memory_start;
    for(int i = 0; i < Utils::g_video_seconds_total * Utils::g_video_fps; i++) {
        if(!Utils::dump_video_frame()) return false;
        Utils::g_video_memory_ptr += width * height * 3;
    }
    delete [] Utils::g_video_memory_start;
    Utils::g_video_memory_start = NULL;
    Utils::g_video_memory_ptr = NULL;
    Utils::dump_video = false;
    printf("VIDEO IMAGES DUMPED\n");
    return true;
}

void Utils::recordVideo() {
    if(!Utils::dump_video) {
        Utils::dump_video = true;
        Utils::video_timer = 0.0;
        Utils::video_dump_timer = 0.0;
        Utils::reserve_video_memory();
    }
}

void Utils::captureFrame(double elapsedSeconds) {
    if(Utils::dump_video) {
        Utils::video_timer += elapsedSeconds;
        Utils::video_dump_timer += elapsedSeconds;
        while(Utils::video_dump_timer > Utils::frame_time) {
            Utils::grab_video_frame();
            Utils::video_dump_timer -= Utils::frame_time;
        }
        if(Utils::video_timer > Utils::g_video_seconds_total) {
            mkdir("videos", 0775);
            /* register all the codecs */
            avcodec_register_all();
            Utils::video_encode_example("videos/video.mpg", AV_CODEC_ID_MPEG2VIDEO);//Utils::dump_video_frames(); // also sets dump_video to false
        }
    }
}

void Utils::FlipFrame(AVFrame* frame) {
    for (int i = 0; i < 4; i++) {
        if (i) {
            frame->data[i] += frame->linesize[i] * ((frame->height >> 1)-1);
        }
        else {
            frame->data[i] += frame->linesize[i] * (frame->height-1);
        }
        frame->linesize[i] = -frame->linesize[i];
    }
}

/*
 * Video encoding example
 */
bool Utils::video_encode_example(const char *filename, int codec_id)
{
    Utils::g_video_memory_ptr = Utils::g_video_memory_start;
    
    AVCodec *codec;
    AVCodecContext *c= NULL;
    int i, ret, got_output;
    FILE *f;
    AVFrame *frame;
    AVPacket pkt;
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    
    printf("Encode video file %s\n", filename);
    
    /* find the mpeg1 video encoder */
    codec = avcodec_find_encoder((enum AVCodecID) codec_id);
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        return false;
    }
    
    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        return false;
    }
    
    /* put sample parameters */
    c->bit_rate = 4000000;
    /* resolution must be a multiple of two */
    glfwGetFramebufferSize(glfwGetCurrentContext(), &c->width, &c->height);
    
    SwsContext * ctx = sws_getContext(c->width, c->height,
                                      AV_PIX_FMT_RGB24,
                                      c->width, c->height,
                                      AV_PIX_FMT_YUV420P,
                                      SWS_BICUBIC, 0, 0, 0);
    /* frames per second */
    c->time_base = (AVRational){1,g_video_fps};
    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    c->gop_size = 10;
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;
    
    if (codec_id == AV_CODEC_ID_H264)
        av_opt_set(c->priv_data, "preset", "slow", 0);
    
    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        return false;
    }
    
    f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", filename);
        return false;
    }
    
    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        return false;
    }
    frame->format = c->pix_fmt;
    frame->width  = c->width;
    frame->height = c->height;
    
    /* the image can be allocated by any means and av_image_alloc() is
     * just the most convenient way if av_malloc() is to be used */
    ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height,
                         c->pix_fmt, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate raw picture buffer\n");
        return false;
    }
    
    /* encode video */
    for(i = 0; i < Utils::g_video_seconds_total * Utils::g_video_fps; i++) {
        av_init_packet(&pkt);
        pkt.data = NULL;    // packet data will be allocated by the encoder
        pkt.size = 0;
        
        fflush(stdout);
        
        /* convert RGB to YCbCr */
        uint8_t *inData[1]     = { g_video_memory_ptr };
        int      inLinesize[1] = { 3 * c->width };
        sws_scale(ctx, inData, inLinesize, 0, c->height,
                  frame->data, frame->linesize);
        
        Utils::FlipFrame(frame);
        
        frame->pts = i;
        
        /* encode the image */
        ret = avcodec_encode_video2(c, &pkt, frame, &got_output);
        if (ret < 0) {
            fprintf(stderr, "Error encoding frame\n");
            return false;
        }
        
        if (got_output) {
            printf("Write frame %3d (size=%5d)\n", i, pkt.size);
            fwrite(pkt.data, 1, pkt.size, f);
            av_packet_unref(&pkt);
        }
        Utils::g_video_memory_ptr += c->width * c->height * 3;
    }

    /* get the delayed frames */
    for (got_output = 1; got_output; i++) {
        fflush(stdout);
        
        ret = avcodec_encode_video2(c, &pkt, NULL, &got_output);
        if (ret < 0) {
            fprintf(stderr, "Error encoding frame\n");
            exit(1);
        }
        
        if (got_output) {
            printf("Write frame %3d (size=%5d)\n", i, pkt.size);
            fwrite(pkt.data, 1, pkt.size, f);
            av_packet_unref(&pkt);
        }
    }
    
    /* add sequence end code to have a real mpeg file */
    fwrite(endcode, 1, sizeof(endcode), f);
    fclose(f);
    
    // Undo flip
    Utils::FlipFrame(frame);
    
    avcodec_close(c);
    av_free(c);
    av_freep(&frame->data[0]);
    av_frame_free(&frame);
    printf("\n");
    
    delete [] Utils::g_video_memory_start;
    Utils::g_video_memory_start = NULL;
    Utils::g_video_memory_ptr = NULL;
    Utils::dump_video = false;
    return true;
}

// ffmpeg -framerate 25 -i frame%03d.png -s:v 1280x960 -c:v libx264 \
-profile:v high -crf 20 -pix_fmt yuv420p video.mp4

