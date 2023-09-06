//
//  MXExternalSourceParse.hpp
//  MXWhere
//
//  Created by sharexia on 9/4/23.
//

#ifndef MXExternalSourceParse_hpp
#define MXExternalSourceParse_hpp

#include <stdio.h>
#include <thread>
#include <iostream>
#include <atomic>
extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/opt.h"
}
class MXExternalSourceParse {
private:
    AVFormatContext *m_formatContext;
    AVCodecContext  *m_audioCodecContext;
    AVCodecContext  *m_videoCodecContext;
    
    int m_videoStream = -1;
    int m_audioStream = -1;
    
    const char *m_filePath;
    int m_index;
    
    std::thread m_thread;
    void sourceParseThread();
    std::atomic<bool> m_running = false;
    
    int openFile();
    int openVideoStream();
    int openAudioStream();
    
public:
    ~MXExternalSourceParse();
    int Start(const char *path, int index);
    int Stop();
    bool Running();
};





#endif /* MXExternalSourceParse_hpp */
