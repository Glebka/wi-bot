#ifndef CAMERACONTROLLER_HPP
#define CAMERACONTROLLER_HPP

#include <QObject>

const quint16 MAX_SERVO = 500;
const quint16 MIN_SERVO = 140;

class CommunicationController;

class CameraController : public QObject
{
   Q_OBJECT

   Q_PROPERTY(QObject * communicator READ getCommunicator WRITE setCommunicator)
   Q_PROPERTY(int sensitivity READ getSensitivity WRITE setSensitivity)

public:
   explicit CameraController(QObject *parent = 0);
   ~CameraController();

   void setCommunicator( QObject* communicator );
   QObject* getCommunicator() const;

   void setSensitivity( int value );
   int getSensitivity() const;

public slots:

   void mouseMoved( int x, int y );
   void mouseReleased();

private:
   int mSensitivity;
   quint16 mPwmX;
   quint16 mPwmZ;
   int mPrevX;
   int mPrevY;
   CommunicationController* mCommunicator;
};

#endif // CAMERACONTROLLER_HPP
