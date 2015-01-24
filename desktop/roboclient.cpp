#include <QHostAddress>
#include <QDebug>
#include "roboclient.h"

RoboClient::RoboClient(QObject *parent) :
    QObject(parent)
{
    connect(&mSocket, &QTcpSocket::connected, this, &RoboClient::connected);
    connect(&mSocket, &QTcpSocket::disconnected, this, &RoboClient::disconnected);
}

void RoboClient::connectToHost(const QString &address, quint16 port)
{
    if (mSocket.state() != QTcpSocket::ConnectedState)
    {
        QHostAddress addr(address);
        mSocket.connectToHost(addr, port);
    }
}

void RoboClient::disconnectFromHost()
{
    sendCommand( RoboCommand::EXIT );
    mSocket.disconnectFromHost();
}

void RoboClient::sendCommand(quint8 command)
{
    if (mSocket.state() == QTcpSocket::ConnectedState)
    {
        mSocket.write( (const char *)&command, 1);
        mSocket.flush();
    }
}
