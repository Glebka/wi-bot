#include <QDebug>
#include "CommunicationController.hpp"
#include "CameraController.hpp"

CameraController::CameraController(QObject* parent)
   : QObject( parent )
   , mSensitivity( 50 )
   , mPwmX( MIN_SERVO + ( MAX_SERVO - MIN_SERVO ) / 2 )
   , mPwmZ( MIN_SERVO + ( MAX_SERVO - MIN_SERVO ) / 2 )
   , mPrevX( -1 )
   , mPrevY( -1 )
   , mCommunicator( 0 )
{

}

CameraController::~CameraController()
{

}

void CameraController::setCommunicator(QObject* communicator)
{
   mCommunicator = qobject_cast<CommunicationController*>( communicator );
   mCommunicator->changeCameraPosition( mPwmX, mPwmZ );
}

QObject* CameraController::getCommunicator() const
{
   return mCommunicator;
}

void CameraController::setSensitivity(int value)
{
   mSensitivity = value;
}

int CameraController::getSensitivity() const
{
   return mSensitivity;
}

void CameraController::mouseMoved(int x, int y)
{
   int deltaX = 0;
   int deltaY = 0;

   if ( mPrevX > -1 && mPrevY > -1 )
   {
      deltaX = ( x - mPrevX ) * ( ( double ) mSensitivity / 100 );
      deltaY = ( y - mPrevY ) * ( ( double ) mSensitivity / 100 );
   }

   mPrevX = x;
   mPrevY = y;

   mPwmX += deltaY;
   mPwmZ += deltaX;

   if ( mPwmX < MIN_SERVO )
      mPwmX = MIN_SERVO;
   if ( mPwmX > MAX_SERVO )
      mPwmX = MAX_SERVO;
   if ( mPwmZ < MIN_SERVO )
      mPwmZ = MIN_SERVO;
   if ( mPwmZ > MAX_SERVO )
      mPwmZ = MAX_SERVO;

   if ( mCommunicator )
   {
      mCommunicator->changeCameraPosition( mPwmX, mPwmZ );
   }
}

void CameraController::mouseReleased()
{
   mPrevX = -1;
   mPrevY = -1;
}

