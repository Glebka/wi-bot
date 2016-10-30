#ifndef MOTORCONTROLLER_HPP
#define MOTORCONTROLLER_HPP

#include <QObject>

class CommunicationController;

class MotorController : public QObject
{
   Q_OBJECT

   Q_PROPERTY(QObject * communicator READ getCommunicator WRITE setCommunicator)

public:
   explicit MotorController(QObject *parent = 0);
   ~MotorController();

   void setCommunicator( QObject* communicator );
   QObject* getCommunicator() const;

public slots:
   void upPressed();
   void upReleased();
   void leftPressed();
   void leftReleased();
   void rightPressed();
   void rightReleased();

private:
   void syncState();

private:
   enum KeyState
   {
      NONE = 0,
      UP = 1,
      LEFT = 2,
      UP_LEFT = 3,
      RIGHT = 4,
      UP_RIGHT = 5
   };

   int mKeyState;
   CommunicationController* mCommunicator;
};

#endif // MOTORCONTROLLER_HPP
