#include <QDebug>
#include <QTimer>
#include <QUdpSocket>
#include <QScopedPointer>
#include <QEventLoop>

#include "BotDiscovery.hpp"

const quint16 DEFAULT_DISCOVERY_PORT = 779;
const int DEFAULT_INTERVAL_MS = 500;
const int DEFAULT_TIMEOUT_MS = 5 * 1000;


BotDiscovery::BotDiscovery(QObject *parent)
    : mDiscoveryPort( DEFAULT_DISCOVERY_PORT )
{

}

BotDiscovery::BotDiscovery(const QJsonObject &config)
    : mDiscoveryPort( config["discoveryPort"].toInt() )
{

}

BotDiscovery::~BotDiscovery()
{

}

bool BotDiscovery::findWiBot(QString &wiBotHost)
{
    QScopedPointer<BotDiscoveryImpl> impl( new BotDiscoveryImpl( mDiscoveryPort ) );
    QEventLoop loop;

    impl->moveToThread( &mDiscoveryThread );
    impl->connect( &mDiscoveryThread, &QThread::started, impl.data(), &BotDiscovery::BotDiscoveryImpl::startDiscovery );
    impl->connect( &mDiscoveryThread, &QThread::finished, impl.data(), &BotDiscovery::BotDiscoveryImpl::stopDiscovery );
    impl->connect( impl.data(), &BotDiscovery::BotDiscoveryImpl::foundWiBot, &mDiscoveryThread, &QThread::quit );
    loop.connect( impl.data(), &BotDiscovery::BotDiscoveryImpl::foundWiBot, &loop, &QEventLoop::quit );
    qDebug() << "Starting discovery timer";
    QTimer::singleShot( DEFAULT_TIMEOUT_MS, &loop, &QEventLoop::quit );
    qDebug() << "Starting discovery thread...";
    mDiscoveryThread.start();
    qDebug() << "Waiting for events...";
    loop.exec();
    mDiscoveryThread.quit();
    if ( !impl->getBotAddress().isNull() )
    {
        QString host = impl->getBotAddress().toString();
        wiBotHost.swap( host );
    }
}


BotDiscovery::BotDiscoveryImpl::BotDiscoveryImpl(quint16 port, QObject *parent)
    : QObject( parent )
    , mSocketSender( NULL )
    , mTimer( NULL )
    , mPort( port )
{

}

BotDiscovery::BotDiscoveryImpl::~BotDiscoveryImpl()
{

}

QHostAddress BotDiscovery::BotDiscoveryImpl::getBotAddress() const
{
    return mAddress;
}

void BotDiscovery::BotDiscoveryImpl::startDiscovery()
{
    qDebug() << "Starting discovery";
    mSocketSender = new QUdpSocket(this);
    mTimer = new QTimer(this);
    mSocketReceiver = new QUdpSocket(this);
    mSocketReceiver->bind( mPort, QUdpSocket::ShareAddress );

    connect( mTimer, &QTimer::timeout, this, &BotDiscoveryImpl::sendBroadcastQuery );
    connect( mSocketReceiver, &QUdpSocket::readyRead, this, &BotDiscoveryImpl::processPendingDatagram );

    sendBroadcastQuery();
    mTimer->start( DEFAULT_INTERVAL_MS );
}

void BotDiscovery::BotDiscoveryImpl::stopDiscovery()
{
    qDebug() << "Stopping discovery";
    mTimer->stop();
}

void BotDiscovery::BotDiscoveryImpl::processPendingDatagram()
{
    while (mSocketReceiver->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(mSocketReceiver->pendingDatagramSize());
        QHostAddress botAddress;
        mSocketReceiver->readDatagram(datagram.data(), datagram.size(), &botAddress);
        qDebug() << "Datagram received: " << datagram.data();
        if ( QString( datagram.data() ) == "wibot" )
        {
            qDebug() << "Have found WiBot on " << botAddress.toString();
            mAddress = botAddress;
            emit foundWiBot( botAddress );
            stopDiscovery();
        }
    }
}

void BotDiscovery::BotDiscoveryImpl::sendBroadcastQuery()
{
    qDebug() << "Sending broadcast query";
    QByteArray data( "whereis wi-bot" );
    mSocketSender->writeDatagram( data, QHostAddress::Broadcast, mPort );
}
