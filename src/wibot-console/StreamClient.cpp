#include "StreamClient.hpp"
#include <QDebug>
#include <QtEndian>
#include <jpeglib.h>
#include <setjmp.h>

struct my_error_mgr {
  struct jpeg_error_mgr pub;    /* "public" fields */

  jmp_buf setjmp_buffer;        /* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

static void my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

StreamClient::StreamClient(const QString &host, quint16 port, QObject *parent)
    : QObject( 0 )
    , mHost( host )
    , mPort( port )
    , mFrameSize( 0 )

{

}

StreamClient::~StreamClient()
{

}

QImage StreamClient::getFrame()
{
    QMutexLocker lock( &mDecompressedLocker );
    return QImage( ( uchar* ) mDecompressedFrameBuffer->data(), mFrameWidth, mFrameHeight, QImage::Format_RGB888, 0, 0 );
}

bool StreamClient::isEmptyFrame()
{
    QMutexLocker lock( &mDecompressedLocker );
    return mDecompressedFrameBuffer->isEmpty();
}

void StreamClient::init()
{
    mSocketPtr.reset( new QTcpSocket() );
    mFrameBuffer.reset( new QByteArray() );
    mDecompressedFrameBuffer.reset( new QByteArray() );

    QTcpSocket* sock = mSocketPtr.data();

    connect( sock, &QTcpSocket::connected, this, &StreamClient::onConnected );
    connect( sock, &QTcpSocket::disconnected, this, &StreamClient::onDisconnected );
    connect( sock, &QTcpSocket::readyRead, this, &StreamClient::onReadyRead );
    connect( this, &StreamClient::decompress, this, &StreamClient::onDecompress );

    mSocketPtr->connectToHost( mHost, mPort );

}

void StreamClient::teardown()
{
   mSocketPtr->disconnectFromHost();
}

void StreamClient::onConnected()
{
    qDebug() << "Connected to robo video server on "
             <<  mSocketPtr->peerName()
             << ":" << mSocketPtr->peerPort();
}

void StreamClient::onDisconnected()
{
    qDebug() << "Disconnected from robo video server";
}

void StreamClient::onReadyRead()
{
    quint64 bytesAvail = mSocketPtr->bytesAvailable();
    while ( bytesAvail )
    {
        if ( mFrameSize == 0 )
        {
            if ( mFrameBuffer->size() > 0 )
            {
                emit decompress( *mFrameBuffer.data() );
                mFrameBuffer->clear();
            }
            if ( bytesAvail >= 4 )
            {
                int read = mSocketPtr->read( ( char* )&mFrameSize, 4 );
                mFrameSize = qFromBigEndian( mFrameSize );
                bytesAvail -= read;
            }
        }
        if ( bytesAvail )
        {
            mFrameBuffer->append( mSocketPtr->read( mFrameSize ) );
            mFrameSize -= ( bytesAvail - mSocketPtr->bytesAvailable() );
            bytesAvail = mSocketPtr->bytesAvailable();
        }
    }

}

void StreamClient::onDecompress(QByteArray frameBuffer)
{
    jpeg_decompress_struct cinfo;
    my_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    if ( setjmp(jerr.setjmp_buffer) )
    {
        jpeg_destroy_decompress( &cinfo );
        return;
    }

    jpeg_create_decompress( &cinfo );
    jpeg_mem_src( &cinfo, ( unsigned char* )frameBuffer.data(), frameBuffer.size() );
    jpeg_read_header( &cinfo, TRUE );
    jpeg_start_decompress( &cinfo );

    int row_stride = cinfo.output_width * cinfo.output_components;
    JSAMPARRAY buffer;
    buffer = (*cinfo.mem->alloc_sarray)
                    ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    QMutexLocker sharedBufferGuard( &mDecompressedLocker );
    mDecompressedFrameBuffer->clear();

    mFrameWidth = cinfo.output_width;
    mFrameHeight = cinfo.output_height;

    while (cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        mDecompressedFrameBuffer->append( ( const char* ) buffer[0], row_stride );

    }
    jpeg_finish_decompress(&cinfo);

}
