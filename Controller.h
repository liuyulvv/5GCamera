#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QArrayData>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUrl>
#include "Device.h"
#include "MvCameraControl.h"
#include "Transmit.h"

class Controller : public QObject
{
  Q_OBJECT
public:
  explicit Controller(QObject *parent = nullptr);
  ~Controller();

signals:

public slots:

  int enumDevices();
  int openDevice();
  int closeDevice();
  int updateParameters(int bitrate, int fps, int width, int height);

  void newTcpConnection();
  void receiveTcpData();

private:
  MV_CC_DEVICE_INFO_LIST *deviceInfoList;
  Device *device;
  Transmit *transmit;
  QTcpServer *server;
  QTcpSocket *client;
  int bitrate;
  int fps;
  int width;
  int height;
};

#endif // CONTROLLER_H
