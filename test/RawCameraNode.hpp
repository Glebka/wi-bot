#ifndef RAWCAMERANODE_HPP
#define RAWCAMERANODE_HPP

#include <QSGGeometryNode>
#include <QSGTextureMaterial>
#include <QSGOpaqueTextureMaterial>
#include <QQuickWindow>
#include <QOpenGLContext>
#include <QByteArray>
#include <QMutex>
#include <QMutexLocker>
#include <QSize>

class QQuickWindow;
class StreamClient;

class RawCameraNode : public QSGGeometryNode
{
public:

    RawCameraNode();
    virtual ~RawCameraNode();

    virtual void preprocess();
    void setQuickWindow( QQuickWindow* window );
    void update( StreamClient* stream );
    void setSize( const QSize& size );


private:

    QSGGeometry m_geometry;
    QSGTextureMaterial m_material;
    QSGOpaqueTextureMaterial m_materialO;
    QSGTexture* m_texture;
    QOpenGLContext* m_currentContext;
    GLuint m_GlTexture;
    QQuickWindow* mWindow;
    QSize mSize;
    QImage mFrame;
    QMutex mBufferGuard;

};

#endif // RAWCAMERANODE_HPP
