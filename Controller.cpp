#include "Controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
  deviceInfoList = new MV_CC_DEVICE_INFO_LIST;
  device = new Device();
  transmit = new Transmit();
  client = nullptr;
  server = new QTcpServer();

  bitrate = 3584000;
  fps = 30;
  width = 1920;
  height = 1080;

  connect(device, &Device::getOneFrame, transmit, &Transmit::send);
  connect(server, &QTcpServer::newConnection, this,
          &Controller::newTcpConnection);

  server->listen(QHostAddress::LocalHost, 4001);

  //  openDevice();
  //  device->startGrab();
  //  transmit->startSend(bitrate, fps, width, height);
}

Controller::~Controller()
{
  delete client;
  delete server;
  delete transmit;
  delete device;
  delete deviceInfoList;
}

int Controller::enumDevices()
{
  if (MV_OK != MV_CC_EnumDevices(MV_USB_DEVICE, deviceInfoList))
  {
    return -1;
  }
  return 0;
}

int Controller::openDevice()
{
  enumDevices();
  if (MV_OK == device->open(deviceInfoList->pDeviceInfo[0]))
  {
    return 0;
  }
  return -1;
}

int Controller::closeDevice()
{
  if (MV_OK != device->close())
  {
    return -1;
  }
  return 0;
}

int Controller::updateParameters(int bitrate, int fps, int width, int height)
{
  if (MV_OK == device->setIntValue("Width", width))
  {
    return -1;
  }
  if (MV_OK == device->setIntValue("Height", height))
  {
    return -1;
  }
  if (MV_OK == device->setFrameRate(fps))
  {
    return -1;
  }
  if (MV_OK == transmit->startSend(bitrate, fps, width, height))
  {
    return 0;
  }
  return -1;
}

void Controller::newTcpConnection()
{
  client = server->nextPendingConnection();
  connect(client, &QTcpSocket::readyRead, this, &Controller::receiveTcpData);
  QJsonObject obj;
  obj.insert("type", "info");
  obj.insert("width", width);
  obj.insert("height", height);
  obj.insert("fps", fps);
  obj.insert("bitrate", bitrate);
  auto doc = QJsonDocument(obj);
  client->write(doc.toJson());
  qDebug() << "----------new tcp connection----------";
  qDebug() << "----------";
}

void Controller::receiveTcpData()
{
  qDebug() << "**********new data**********";
  auto data = client->readAll();
  auto doc = QJsonDocument::fromJson(data);
  auto json = doc.object();
  auto type = json.value("type").toString();
  if ("openDevice" == type)
  {
    if (MV_OK == openDevice())
    {
      client->write("Open device successfully.");
      qDebug() << "----------Open device successfully----------";
      qDebug() << "----------";
    }
    else
    {
      client->write("Open device failed.");
      qDebug() << "----------Open device failed.----------";
      qDebug() << "----------";
    }
  }
  else if ("startGrab" == type)
  {
    if (MV_OK == device->startGrab() &&
        MV_OK == transmit->startSend(bitrate, fps, width, height))
    {
      client->write("Start grab successfully.");
      qDebug() << "----------Start grab successfully----------";
      qDebug() << "----------";
    }
    else
    {
      client->write("Start grab failed.");
      qDebug() << "----------Start grab failed----------";
      qDebug() << "----------";
    }
  }
  else if ("stopGrab" == type)
  {
    if (MV_OK == device->stopGrab())
    {
      client->write("Stop grab successfully.");
      qDebug() << "----------Stop grab successfully----------";
      qDebug() << "----------";
      transmit->stopSend();
    }
    else
    {
      client->write("Stop grab failed.");
      qDebug() << "----------Stop grab failed----------";
      qDebug() << "----------";
    }
  }
  else if ("updateParameters" == type)
  {
    if (MV_OK == updateParameters(json.value("bitrate").toInt(),
                                  json.value("fps").toInt(),
                                  json.value("width").toInt(),
                                  json.value("height").toInt()))
    {
      bitrate = json.value("bitrate").toInt();
      fps = json.value("fps").toInt();
      width = json.value("width").toInt();
      height = json.value("height").toInt();
      client->write("Update parameters successfully.");
      qDebug() << "----------Update parameters successfully----------";
      qDebug() << "----------";
    }
    else
    {
      client->write("Update parameters failed.");
      qDebug() << "----------Update parameters failed----------";
      qDebug() << "----------";
    }
  }
}
