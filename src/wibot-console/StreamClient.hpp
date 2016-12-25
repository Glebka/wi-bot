#ifndef STREAMCLIENT_HPP
#define STREAMCLIENT_HPP

#include <QObject>
#include <QTcpSocket>
#include <QScopedPointer>
#include <QByteArray>
#include <QMutex>
#include <QMutexLocker>
#include <QSharedPointer>
#include <QImage>
#include <QJsonObject>

class StreamClient : public QObject
{
    Q_OBJECT

    StreamClient( const StreamClient& );
    StreamClient& operator=( const StreamClient& );

public:
    StreamClient( const QString& host, quint16 port, QObject* parent );
    explicit StreamClient( const QJsonObject& config );
    ~StreamClient();

    QImage getFrame();
    bool isEmptyFrame();

signals:
    void decompress( QByteArray frameBuffer );
    void error( const QString& errorMessage );

public slots:
    void init();
    void teardown();

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onDecompress( QByteArray frameBuffer );
    void onConnectionError( QAbstractSocket::SocketError socketError );

private:

    QString mHost;
    quint16 mPort;

    QScopedPointer<QTcpSocket> mSocketPtr;
    QScopedPointer<QByteArray> mFrameBuffer;
    QSharedPointer<QByteArray> mDecompressedFrameBuffer;
    quint32 mFrameSize;
    QMutex mDecompressedLocker;
    int mFrameWidth;
    int mFrameHeight;
};

#endif // STREAMCLIENT_HPP
