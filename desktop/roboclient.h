#ifndef ROBOCLIENT_H
#define ROBOCLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>

namespace RoboCommand
{
    enum RoboCmd
    {
        NONE,
        CAM_UP,
        CAM_DOWN,
        CAM_LEFT,
        CAM_RIGTH
    };

    enum RoboCmdCount
    {
        COMMANDS_COUNT = 5
    };
}

class RoboClient : public QObject
{
    Q_OBJECT
public:
    explicit RoboClient(QObject *parent = 0);

    void connectToHost(const QString &address, quint16 port);
    void sendCommand(RoboCommand::RoboCmd command);

public slots:
    void disconnectFromHost();

signals:
    void connected();
    void disconnected();

private:
    QTcpSocket mSocket;

    static const char commandsMappings[RoboCommand::COMMANDS_COUNT];
};

#endif // ROBOCLIENT_H
