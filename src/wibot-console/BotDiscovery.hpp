#ifndef BOTDISCOVERY_HPP
#define BOTDISCOVERY_HPP

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QThread>
#include <QHostAddress>
#include <QTimer>
#include <QUdpSocket>

class BotDiscovery : public QObject
{
   Q_OBJECT
public:
    explicit BotDiscovery(QObject *parent = 0);
    explicit BotDiscovery( const QJsonObject& config );
    ~BotDiscovery();

    bool findWiBot( QString& wiBotHost );

private:
    quint16 mDiscoveryPort;
    QThread mDiscoveryThread;
    class BotDiscoveryImpl;
    BotDiscoveryImpl* mBotDiscovery;
    QHostAddress mBotAddress;
};

class BotDiscovery::BotDiscoveryImpl : public QObject
{
    Q_OBJECT

public:
    explicit BotDiscoveryImpl(quint16 port, QObject *parent = 0);
    ~BotDiscoveryImpl();

    QHostAddress getBotAddress() const;

signals:

    void foundWiBot( QHostAddress address );

public slots:

    void startDiscovery();
    void stopDiscovery();
    void processPendingDatagram();
    void sendBroadcastQuery();

private:
    QTimer* mTimer;
    QUdpSocket* mSocketSender;
    QUdpSocket* mSocketReceiver;
    quint16 mPort;
    QHostAddress mAddress;
};

#endif // COMMUNICATIONCONTROLLER_HPP
