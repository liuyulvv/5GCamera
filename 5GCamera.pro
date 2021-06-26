QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    Transmit.cpp \
    Device.cpp \
    Controller.cpp

HEADERS += \
    Transmit.h \
    Device.h \
    Controller.h

unix:!macx: LIBS += \
-L$$PWD/lib/aarch64/ -lMvCameraControl \
-L$$PWD/lib/aarch64/ -lGCBase_gcc48_v3_0 \
-L$$PWD/lib/aarch64/ -lGenApi_gcc48_v3_0 \
-L$$PWD/lib/aarch64/ -lXmlParser_gcc48_v3_0

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include


unix:!macx: LIBS += \
-L$$PWD/../../../../usr/lib/aarch64-linux-gnu/ -lopencv_core -lopencv_highgui -lopencv_video -lopencv_videoio

INCLUDEPATH += \
$$PWD/../../../../usr/include/opencv4 \
$$PWD/../../../../usr/include/opencv4/opencv2

DEPENDPATH += \
$$PWD/../../../../usr/include/opencv4 \
$$PWD/../../../../usr/include/opencv4/opencv2
