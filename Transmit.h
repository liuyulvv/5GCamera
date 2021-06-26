#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <QDebug>
#include <QObject>
#include <QString>
#include "MvCameraControl.h"
#include "opencv.hpp"

using namespace cv;

class Transmit : public QObject
{
  Q_OBJECT
public:
  explicit Transmit(QObject *parent = nullptr);
  ~Transmit();
signals:

public slots:

  int startSend(int bitrate, int fps, int width, int height);
  void send(unsigned char *data, MV_FRAME_OUT_INFO_EX *frameInfo);
  int stopSend();

private:
  QString rtmp;
  Mat *frame;
  VideoWriter *videoWriter;
};

#endif // TRANSMIT_H
