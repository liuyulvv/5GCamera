#ifndef DEVICE_H
#define DEVICE_H

#include <QDebug>
#include <QObject>
#include <QString>
#include "MvCameraControl.h"

class Device : public QObject
{
  Q_OBJECT
public:
  explicit Device(QObject *parent = nullptr);
  ~Device();

signals:
  void getOneFrame(unsigned char *data, MV_FRAME_OUT_INFO_EX *frameInfo);

public slots:
  bool isOpen();
  int open(MV_CC_DEVICE_INFO *info);
  unsigned getIntValue(QString key);
  unsigned setIntValue(QString key, unsigned value);
  float getFloatValue(QString key);
  float setFloatValue(QString key, float value);
  float getFrameRate();
  float setFrameRate(float value);
  int startGrab();
  int stopGrab();
  int close();
  void grabParameters();

private:
  int status;
  bool openFlag;
  void *handle;
  MVCC_INTVALUE *intValue;
  MVCC_FLOATVALUE *floatValue;
  MVCC_ENUMVALUE *enumValue;

  void static frameCallback(unsigned char *data,
                            MV_FRAME_OUT_INFO_EX *frameInfo, void *pUser);
};

#endif // DEVICE_H
