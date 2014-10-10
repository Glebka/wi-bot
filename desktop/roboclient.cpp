#include <QHostAddress>
#include <QDebug>
#include "roboclient.h"

const char RoboClient::commandsMappings[] =
{
  '\0',
  'W',
  'S',
  'A',
  'D'
};

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
    mSocket.disconnectFromHost();
}

void RoboClient::sendCommand(RoboCommand::RoboCmd command)
{
    if (mSocket.state() == QTcpSocket::ConnectedState)
    {
        qDebug() << commandsMappings[command];
        mSocket.write(&commandsMappings[command], 1);
        mSocket.flush();
    }
}
