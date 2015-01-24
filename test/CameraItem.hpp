#ifndef CAMERAITEM_HPP
#define CAMERAITEM_HPP

#include <QQuickItem>

class StreamClient;

class CameraItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY( QObject *stream READ stream WRITE setStream)
public:
    explicit CameraItem(QQuickItem *parent = 0);

    QObject* stream();
    void setStream( QObject* stream );

    // QQuickItem interface
protected:
    virtual QSGNode *updatePaintNode(QSGNode *oldnode, UpdatePaintNodeData *);

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *);

private:
    StreamClient* mStream;
};

#endif // CAMERAITEM_HPP
