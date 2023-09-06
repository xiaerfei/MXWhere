//
//  MXExternalSourceParse.cpp
//  MXWhere
//
//  Created by sharexia on 9/4/23.
//

#include "MXExternalSourceParse.hpp"

MXExternalSourceParse::~MXExternalSourceParse() {
    printf("will delete~~~ \n");
}

int MXExternalSourceParse::Start(const char *path, int index) {
    m_filePath = path;
    m_index    = index;
    if (openFile() < 0) {
        return -1;
    }
    
    
    
    m_running  = true;
    m_thread = std::thread(&MXExternalSourceParse::sourceParseThread, this);
    
    return 0;
}

int MXExternalSourceParse::Stop() {
    m_running = false;
    m_videoStream = -1;
    m_audioStream = -1;
    m_thread.join();
    return 0;
}

bool MXExternalSourceParse::Running() {
    return m_running;
}

void MXExternalSourceParse::sourceParseThread() {
    while (m_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        printf("thread running \n");
    }
    printf("thread will stop running \n");
}

int MXExternalSourceParse::openFile() {
    AVFormatContext *formatContext = NULL;
    AVDictionary *opts             = NULL;
    
    formatContext = avformat_alloc_context();
    if (!formatContext) {
        return -1;
    }
    if (avformat_open_input(&formatContext, m_filePath, NULL, &opts) < 0) {
        if (formatContext)
            avformat_free_context(formatContext);
        return -1;
    }
    
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        /// 已经包含 free formatContext
        avformat_close_input(&formatContext);
        return -1;
    }
    
    m_formatContext = formatContext;
    return 0;
}

int MXExternalSourceParse::openVideoStream() {
    for (int i = 0; i < m_formatContext->nb_streams; i ++) {
        if (m_formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_videoStream = i;
        }
    }
    
    if (m_videoStream == -1) {
        return -1;
    }
    /**
     *  关于 AV_DISPOSITION_ATTACHED_PIC 的含义，可以参考这里：
     *  https://segmentfault.com/a/1190000018373504
     */
    if ((m_formatContext->streams[m_videoStream]->disposition & AV_DISPOSITION_ATTACHED_PIC) == 0) {
        AVCodecID codec_id = m_formatContext->streams[m_videoStream]->codecpar->codec_id;
        const AVCodec *codec = avcodec_find_decoder(codec_id);
        if (!codec) {
            return -1;
        }
        AVCodecContext *codecContext = avcodec_alloc_context3(codec);
        
        int err = avcodec_parameters_to_context(codecContext,
                                                m_formatContext->streams[m_videoStream]->codecpar);
        if (err < 0) {
            avcodec_free_context(&codecContext);
            return -1;
        }
        
    }
    
    
    return 0;
}
