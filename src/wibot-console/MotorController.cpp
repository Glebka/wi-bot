#include <QDebug>
#include "MotorController.hpp"
#include "CommunicationController.hpp"

static const quint8 MOTOR_OFF = 0;
static const quint8 MOTOR_LOW_SPEED = 80;
static const quint8 MOTOR_NORM_SPEED = 128;

MotorController::MotorController(QObject *parent)
 : QObject(parent)
 , mKeyState( NONE )
 , mCommunicator( 0 )
{

}

MotorController::~MotorController()
{

}

void MotorController::setCommunicator(QObject* communicator)
{
   mCommunicator = qobject_cast<CommunicationController*>( communicator );
}

QObject* MotorController::getCommunicator() const
{
   return mCommunicator;
}

void MotorController::upPressed()
{
   mKeyState |= UP;
   syncState();
}

void MotorController::upReleased()
{
   mKeyState ^= UP;
   syncState();
}

void MotorController::leftPressed()
{
   mKeyState |= LEFT;
   syncState();
}

void MotorController::leftReleased()
{
   mKeyState ^= LEFT;
   syncState();
}

void MotorController::rightPressed()
{
   mKeyState |= RIGHT;
   syncState();
}

void MotorController::rightReleased()
{
   mKeyState ^= RIGHT;
   syncState();
}

void MotorController::syncState()
{
   switch( mKeyState )
   {
   case UP:
      qDebug() << "Up";
      break;
   case LEFT:
      qDebug() << "Left";
      break;
   case RIGHT:
      qDebug() << "Right";
      break;
   case UP_LEFT:
      qDebug() << "Up-Left";
      break;
   case UP_RIGHT:
      qDebug() << "Up-Right";
      break;
   default:
      qDebug() << "None";
      break;
   }
}

