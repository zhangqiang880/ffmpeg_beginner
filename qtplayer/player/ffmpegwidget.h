#ifndef FFMPEGWIDGET_H
#define FFMPEGWIDGET_H

#include <QImage>
#include <QWidget>
#include <QPaintEvent>
#include <QThread>
#include <QPainter>
#include <QDebug>

#include <string>

extern "C"{
#include <libavcodec/avcodec.h>

#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/frame.h>
#include <libavutil/samplefmt.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libavutil/parseutils.h>
#include <libavutil/mem.h>

#include <libswscale/swscale.h>

#include <libavformat/avformat.h>
}

using namespace std;

class FFmpegVideo : public QThread
{
    Q_OBJECT
public:
    explicit FFmpegVideo();
    ~FFmpegVideo();

    void ffmpeg_init_variables();
    void ffmpeg_free_variables();
    int open_input_file();

protected:
    void run();

signals:
    void sendQImage(QImage);

private:
    AVFormatContext *fmtCtx       =NULL;
    AVCodec         *videoCodec   =NULL;
    AVCodec         *audioCodec   =NULL;
    AVCodecContext  *videoCodecCtx=NULL;
    AVCodecContext  *audioCodecCtx=NULL;
    AVPacket        *pkt          = NULL;
    AVFrame         *yuvFrame     = NULL;
    AVFrame         *rgbFrame     = NULL;
    AVFrame         *audioFrame   = NULL;
    AVCodecParserContext *audioParser  = NULL;

    struct SwsContext *img_ctx=NULL;

    unsigned char *out_buffer;
    uint8_t *audioOutData;
    size_t audioOutDataSize;

    int videoStreamIndex =-1;
    int audioStreamIndex = -1;
    int numBytes = -1;

    bool initFlag=false,openFlag=false,runFlag=false;
};

class FFmpegAudio : public QThread
{
    explicit FFmpegAudio();
    ~FFmpegAudio();
};

class FFmpegWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FFmpegWidget(QWidget *parent = nullptr);
    ~FFmpegWidget();

    void play();
    void pause();
    void stop();

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void receiveQImage(const QImage &rImg);

private:
    FFmpegVideo *ffmpeg;

    QImage img;
};

#endif // FFMPEGWIDGET_H