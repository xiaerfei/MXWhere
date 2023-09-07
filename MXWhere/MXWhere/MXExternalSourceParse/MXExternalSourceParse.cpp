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
        release();
        return -1;
    }
    
    if (openVideoStream() < 0) {
        release();
        return -1;
    }
    
    if (openAudioStream() < 0) {
        release();
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
    release();
    
    return 0;
}

bool MXExternalSourceParse::Running() {
    return m_running;
}

void MXExternalSourceParse::sourceParseThread() {
    AVPacket *packet;
    while (m_running) {
        packet = av_packet_alloc();
        
        int size = av_read_frame(m_formatContext, packet);
        if (size < 0 || packet->size < 0) {
            /// 视频播放完成了
            break;
        }
        
        if (packet->stream_index == m_videoStream) {
            
        } else if (packet->stream_index == m_audioStream) {
            
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        printf("thread running \n");
        
        av_packet_free(&packet);
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
            avformat_close_input(&formatContext);
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
    m_videoStream = -1;
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
     *  AV_DISPOSITION_ATTACHED_PIC 是一个标志。如果一个流中含有这个标志的话，
     *  那么就是说这个流是 *.mp3 文件中的一个 Video Stream 。并且该流只有一个
     *  AVPacket ，也就是 attached_pic 。这个 AVPacket 中所存储的内容就是这个 *.mp3 文件的封面图片。
     *
     *  这里可以回答，是因为，如果这个流中包含这个标志的话，说明这个流是 *.mp3 文件中的 Video Stream 。不是传统意义上的视频流。
     *  它只存放了封面信息，在播放或者导出时，不需要这个数据。因此我们可以使用这个标志很好的区分这个特殊的 Video Stream 。
     *  并且通过判断，屏蔽该流，不对其进行操作。
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
            return -1;
        }
        
        if (avcodec_open2(codecContext, codec, NULL) < 0) {
            return -1;
        }
        m_videoCodecContext = codecContext;
        return 0;
    }
    return -1;
}

int MXExternalSourceParse::openAudioStream() {
    m_audioStream = -1;
    for (int i = 0; i < m_formatContext->nb_streams; i ++) {
        if (m_formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            m_audioStream = i;
        }
    }
    
    if (m_audioStream == -1) {
        return -1;
    }
    
    AVCodecID codec_id = m_formatContext->streams[m_audioStream]->codecpar->codec_id;
    const AVCodec *codec = avcodec_find_decoder(codec_id);
    if (!codec) {
        return -1;
    }
    AVCodecContext *codecContext = avcodec_alloc_context3(codec);
    
    int err = avcodec_parameters_to_context(codecContext,
                                            m_formatContext->streams[m_audioStream]->codecpar);
    if (err < 0) {
        return -1;
    }
    
    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        return -1;
    }
    m_audioCodecContext = codecContext;
    
    m_audioFrame = av_frame_alloc();
    if (!m_audioFrame) {
        return -1;
    }
    
    return 0;
}

void MXExternalSourceParse::release() {
    if (m_audioCodecContext) {
        avcodec_close(m_audioCodecContext);
        avcodec_free_context(&m_audioCodecContext);
        m_audioCodecContext = NULL;
    }
    
    if (m_videoCodecContext) {
        avcodec_send_packet(m_videoCodecContext, NULL);
        avcodec_flush_buffers(m_videoCodecContext);
        
        avcodec_close(m_videoCodecContext);
        avcodec_free_context(&m_videoCodecContext);
        m_videoCodecContext = NULL;
    }
    
    if (m_formatContext) {
        /// 已经包含 free formatContext
        avformat_close_input(&m_formatContext);
        m_formatContext = NULL;
    }
    
    if (m_audioFrame) {
        av_free(m_audioFrame);
        m_audioFrame = NULL;
    }
    
    if (m_videoFrame) {
        av_free(m_videoFrame);
        m_videoFrame = NULL;
    }
}
