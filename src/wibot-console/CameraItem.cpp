#include "CameraItem.hpp"
#include "RawCameraNode.hpp"
#include "StreamClient.hpp"
#include <QDebug>

CameraItem::CameraItem(QQuickItem *parent)
    : QQuickItem(parent)
    , mStream( 0 )

{
    setFlag( ItemHasContents );
    setSmooth( false );
    startTimer( 0 );
}

QObject *CameraItem::stream()
{
    return mStream;
}

void CameraItem::setStream( QObject *stream )
{
    mStream = qobject_cast<StreamClient*> ( stream );
}


QSGNode *CameraItem::updatePaintNode(QSGNode * oldnode, UpdatePaintNodeData *)
{
    if ( width() <= 0 || height() <= 0 )
    {
        delete oldnode;
        return 0;
    }

    RawCameraNode *node = static_cast<RawCameraNode *>(oldnode);
    if ( !node )
    {
        node = new RawCameraNode();
        node->setQuickWindow( window() );
    }
    node->setSize( QSize( width(), height() ) );
    node->update( mStream );
    node->markDirty( QSGNode::DirtyMaterial | QSGNode::DirtyGeometry );

    return node;
}


void CameraItem::timerEvent(QTimerEvent *)
{
    update();
}
