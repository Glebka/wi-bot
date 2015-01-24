#include "RawCameraNode.hpp"
#include <QOpenGLFunctions>
#include <QQuickWindow>
#include <QDebug>
#include "StreamClient.hpp"
#include <QImage>

RawCameraNode::RawCameraNode()
    : QSGGeometryNode()
    , m_geometry( QSGGeometry::defaultAttributes_TexturedPoint2D(), 4 )
    , m_texture( 0 )
    , m_currentContext( QOpenGLContext::currentContext() )
    , m_GlTexture( 0 )
    , mWindow( 0 )
    , mSize( 160, 120 )
    , mFrame( mSize, QImage::Format_Mono )
{
    setMaterial( &m_material );
    setOpaqueMaterial( &m_materialO );
    setGeometry( &m_geometry );
    setFlag( UsePreprocess );
}

RawCameraNode::~RawCameraNode()
{
    delete m_texture;
}


void RawCameraNode::preprocess()
{
    QMutexLocker lock( &mBufferGuard );

    delete m_texture;
    m_texture = 0;
    /*m_currentContext->functions()->glDeleteTextures( 1, &m_GlTexture );

    m_currentContext->functions()->glGenTextures( 1, &m_GlTexture );
    m_currentContext->functions()->glBindTexture( GL_TEXTURE_2D, m_GlTexture );

    m_currentContext->functions()->glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, mWidth, mHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, ( uchar* )mFrame.data() );*/

    m_texture = mWindow->createTextureFromImage( mFrame );
    m_material.setTexture( m_texture );
    m_materialO.setTexture( m_texture );
    QSGGeometry::updateTexturedRectGeometry( &m_geometry,
                                                QRectF(0, 0, mSize.width(), mSize.height() ),
                                                QRectF(0, 0, 1, 1));

}

void RawCameraNode::setQuickWindow( QQuickWindow* window )
{
    mWindow = window;
}

void RawCameraNode::update(StreamClient *stream)
{
    if ( !stream->isEmptyFrame() )
    {
        QMutexLocker lock( &mBufferGuard );
        mFrame = stream->getFrame().scaled( mSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
        /*QImage img( ( uchar * )mFrame.data(), 160, 120, QImage::Format_RGB888, 0, 0 );
        img.save( "raw.bmp" );*/
        //exit( -1 );
    }
}

void RawCameraNode::setSize(const QSize &size)
{
    mSize = size;
}
