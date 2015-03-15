#include <QHostAddress>
#include "RoboClient.hpp"

RoboClient::RoboClient(QObject *parent) :
    QObject(parent)
{
    connect(&mSocket, &QTcpSocket::connected, this, &RoboClient::connected);
    connect(&mSocket, &QTcpSocket::disconnected, this, &RoboClient::disconnected);
    connect(&mSocket, &QTcpSocket::readyRead, this, &RoboClient::bytesAvailable);
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

void RoboClient::bytesAvailable()
{
    mSocket.readAll();
    qDebug() << "Wake up";
}

void RoboClient::sendCommand(quint8 command)
{
    if (mSocket.state() == QTcpSocket::ConnectedState)
    {
        mSocket.write( (const char *)&command, 1);
        mSocket.flush();
    }
}


