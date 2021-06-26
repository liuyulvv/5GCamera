#include "Transmit.h"

Transmit::Transmit(QObject *parent) : QObject(parent)
{
  rtmp = "rtmp://14.18.87.27:1935/live/rtmp";
  frame = new Mat();
  videoWriter = new VideoWriter();
}

Transmit::~Transmit()
{
  videoWriter->release();
  delete frame;
  delete videoWriter;
}

int Transmit::startSend(int bitrate, int fps, int width, int height)
{
  QString src = QString(
      "appsrc ! videoconvert ! "
      "nvvidconv ! nvv4l2h264enc maxperf-enable=1 "
      "bitrate=" +
      QString::number(bitrate) +
      " ! h264parse ! queue ! "
      "flvmux ! rtmpsink location=" +
      rtmp);
  if (videoWriter->open(src.toStdString(), 0, (double)fps, Size(width, height),
                        true))
  {
    qDebug() << "Bitrate:" << bitrate << "FPS:" << fps << "Width:" << width
             << "Height:" << height;
    return 0;
  }
  return -1;
}

void Transmit::send(unsigned char *data, MV_FRAME_OUT_INFO_EX *frameInfo)
{
  delete frame;
  frame = new Mat(frameInfo->nHeight, frameInfo->nWidth, CV_8UC3, data);
  (*videoWriter) << (*frame);
}

int Transmit::stopSend()
{
  videoWriter->release();
  return 0;
}
