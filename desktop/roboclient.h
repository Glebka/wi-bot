#ifndef ROBOCLIENT_H
#define ROBOCLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>

namespace RoboCommand
{
    enum RoboCmd
    {
        NONE = 0,
        EXIT = 1,
        CAM_UP = 2,
        CAM_DOWN = 4,
        CAM_LEFT = 8,
        CAM_RIGTH = 16,
        WHEEL_LEFT = 32,
        WHEEL_RIGHT = 64
    };
}

class RoboClient : public QObject
{
    Q_OBJECT
public:
    explicit RoboClient(QObject *parent = 0);

    void connectToHost(const QString &address, quint16 port);
    void sendCommand(quint8 command);

public slots:
    void disconnectFromHost();

signals:
    void connected();
    void disconnected();

private:
    QTcpSocket mSocket;
};

#endif // ROBOCLIENT_H
