#ifndef ROBOCLIENT_HPP
#define ROBOCLIENT_HPP

#include <QObject>
#include <QString>
#include <QTcpSocket>

class RoboClient : public QObject
{
    Q_OBJECT
public:
    explicit RoboClient(QObject *parent = 0);
    ~RoboClient();

    void connectToHost( const QString &address, quint16 port );

public slots:
    void disconnectFromHost();
    void bytesAvailable();

signals:
    void connected();
    void disconnected();

private:
    QTcpSocket mSocket;
};

#endif // ROBOCLIENT_HPP
