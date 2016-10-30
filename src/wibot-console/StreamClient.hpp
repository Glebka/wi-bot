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

class StreamClient : public QObject
{
    Q_OBJECT

    StreamClient( const StreamClient& );
    StreamClient& operator=( const StreamClient& );

public:
    StreamClient( const QString& host, quint16 port, QObject* parent );
    ~StreamClient();

    QImage getFrame();
    bool isEmptyFrame();

signals:
    void decompress( QByteArray frameBuffer );

public slots:
    void init();
    void teardown();

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onDecompress( QByteArray frameBuffer );

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
