#include "Device.h"

Device::Device(QObject *parent) : QObject(parent)
{
  status = -1;
  handle = new void *;
  intValue = new MVCC_INTVALUE;
  floatValue = new MVCC_FLOATVALUE;
  enumValue = new MVCC_ENUMVALUE;
  openFlag = false;
}

Device::~Device()
{
  close();
  delete intValue;
  delete floatValue;
  delete enumValue;
}

bool Device::isOpen() { return openFlag; }

int Device::open(MV_CC_DEVICE_INFO *info)
{
  if (openFlag)
  {
    return 0;
  }
  status = MV_CC_CreateHandle(&handle, info);
  if (MV_OK != status)
  {
    qDebug() << status;
    return -1;
  }
  status = MV_CC_OpenDevice(handle);
  if (MV_OK != status)
  {
    qDebug() << status;
    return -1;
  }
  status = MV_CC_RegisterImageCallBackForBGR(handle, frameCallback, this);
  if (MV_OK != status)
  {
    qDebug() << status;
    return -1;
  }
  openFlag = true;
  qDebug() << "----------Device information----------";
  qDebug() << "VendorName:"
           << (char *)info->SpecialInfo.stUsb3VInfo.chVendorName;
  qDebug() << "ModelName:" << (char *)info->SpecialInfo.stUsb3VInfo.chModelName;
  qDebug() << "DeviceVersion:"
           << (char *)info->SpecialInfo.stUsb3VInfo.chDeviceVersion;
  qDebug() << "SerialNumber:"
           << (char *)info->SpecialInfo.stUsb3VInfo.chSerialNumber;
  qDebug() << "GUID:" << (char *)info->SpecialInfo.stUsb3VInfo.chDeviceGUID;
  qDebug() << "----------";
  setIntValue("Width", 1920);
  setIntValue("Height", 1080);
  MV_CC_SetEnumValue(handle, "PixelFormat", 0x02180014);
  MV_CC_SetEnumValue(handle, "ExposureAuto", 2);
  setFrameRate(30);
  return 0;
}

unsigned Device::getIntValue(QString key)
{
  if (openFlag)
  {
    status = MV_CC_GetIntValue(handle, key.toLatin1().data(), intValue);
    if (MV_OK == status)
    {
      return intValue->nCurValue;
    }
  }
  qDebug() << status;
  return 0;
}

unsigned Device::setIntValue(QString key, unsigned value)
{
  if (openFlag)
  {
    status = MV_CC_SetIntValue(handle, key.toLatin1().data(), value);
    if (MV_OK == status)
    {
      return value;
    }
  }
  qDebug() << status;
  return 0;
}

float Device::getFloatValue(QString key)
{
  if (openFlag)
  {
    status = MV_CC_GetFloatValue(handle, key.toLatin1().data(), floatValue);
    if (MV_OK == status)
    {
      return floatValue->fCurValue;
    }
  }
  qDebug() << status;
  return 0;
}

float Device::setFloatValue(QString key, float value)
{
  if (openFlag)
  {
    status = MV_CC_SetFloatValue(handle, key.toLatin1().data(), value);
    if (MV_OK == status)
    {
      return value;
    }
  }
  qDebug() << status;
  return 0;
}

float Device::getFrameRate()
{
  if (openFlag)
  {
    status = MV_CC_GetFrameRate(handle, floatValue);
    if (MV_OK == status)
    {
      return floatValue->fCurValue;
    }
  }
  qDebug() << status;
  return 0;
}

float Device::setFrameRate(float value)
{
  if (openFlag)
  {
    status = MV_CC_SetFrameRate(handle, value);
    if (MV_OK == status)
    {
      return value;
    }
  }
  qDebug() << status;
  return 0;
}

int Device::startGrab()
{
  if (openFlag)
  {
    status = MV_CC_StartGrabbing(handle);
    if (MV_OK == status)
    {
      qDebug() << "----------Start grab----------";
      qDebug() << "----------";
      grabParameters();
      return 0;
    }
  }
  qDebug() << status;
  return -1;
}

int Device::stopGrab()
{
  if (openFlag)
  {
    status = MV_CC_StopGrabbing(handle);
    if (MV_OK == status)
    {
      qDebug() << "----------Stop grab----------";
      qDebug() << "----------";
      return 0;
    }
  }
  qDebug() << status;
  return -1;
}

void Device::frameCallback(unsigned char *data, MV_FRAME_OUT_INFO_EX *frameInfo,
                           void *pUser)
{
  emit((Device *)(pUser))->getOneFrame(data, frameInfo);
}

int Device::close()
{
  if (openFlag)
  {
    MV_CC_CloseDevice(handle);
    status = MV_CC_DestroyHandle(handle);
    if (MV_OK == status)
    {
      openFlag = false;
      return 0;
    }
  }
  qDebug() << status;
  return -1;
}

void Device::grabParameters()
{
  qDebug() << "----------Grab parameters----------";
  qDebug() << "Width" << getIntValue("Width");
  qDebug() << "Height" << getIntValue("Height");
  qDebug() << "FPS" << getFrameRate();
  qDebug() << "----------";
}
