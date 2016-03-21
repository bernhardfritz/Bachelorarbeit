//
//  Utils.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 20/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <stdio.h>
extern "C" {
    #include <libavcodec/avcodec.h>
}

class Utils {
private:
    static unsigned char* g_video_memory_start;
    static unsigned char* g_video_memory_ptr;
    static int g_video_seconds_total;
    static int g_video_fps;
    static bool dump_video;
    static double video_timer;
    static double video_dump_timer;
    static double frame_time;
    static int frame_number;
    
    static void reserve_video_memory();
    static void grab_video_frame();
    static bool dump_video_frame();
    static bool dump_video_frames();
    
    static void FlipFrame(AVFrame* frame);
public:
    static void screenshot();
    static void recordVideo();
    static void captureFrame(double elapsedSeconds);
    static bool video_encode_example(const char *filename, int codec_id);
};

#endif /* Utils_hpp */
