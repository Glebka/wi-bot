TEMPLATE = app

QT += qml quick

SOURCES += main.cpp \
    RawCameraNode.cpp \
    CameraItem.cpp \
    StreamClient.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    RawCameraNode.hpp \
    CameraItem.hpp \
    StreamClient.hpp

LIBS += -ljpeg
