#include <Wire.h>
#include <Timer.h>
#include <Adafruit_PWMServoDriver.h>

#define MAX_PWM_SERVO 500
#define MIN_PWM_SERVO 140
#define STEP 5
#define INDICATOR_PIN 9
#define LEFT_MOTOR_PIN 3
#define RIGHT_MOTOR_PIN 5

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

Timer timer;
int timeoutEvent = 0;

uint16_t rotX = MIN_PWM_SERVO; // up-down servo
uint16_t rotZ = MIN_PWM_SERVO; // left-right servo
uint16_t motorL = 0;
uint16_t motorR = 0;

byte isInitialized = 0;

struct Command
{
  uint16_t code;
  uint16_t hiParam;
  uint16_t loParam;
};

Command cmd;

void resetMotors( void )
{
  analogWrite( LEFT_MOTOR_PIN, 0 );
  analogWrite( RIGHT_MOTOR_PIN, 0 );
}

void setup( void ) 
{
  Serial.begin( 115200 );
  pwm.begin();
  pwm.setPWMFreq( 60 );  // Analog servos run at ~60 Hz updates
  pwm.setPWM( 0, 0, rotZ );
  pwm.setPWM( 1, 0, rotX );
  analogWrite( LEFT_MOTOR_PIN, 0 );
  analogWrite( RIGHT_MOTOR_PIN, 0 );
  //timer.oscillate( INDICATOR_PIN, 100, LOW, 10 );
}

void loop( void ) 
{
  if (Serial.available() > 0) 
  {
    if ( Serial.readBytes( ( char* )&cmd, sizeof( Command ) ) == 0 )
    {
      timer.oscillate( INDICATOR_PIN, 50, LOW, 2 );
    }
    
    if ( !isInitialized )
    {
      if ( cmd.code == 0xFF && cmd.hiParam == 0x1 && cmd.loParam == 0xFF )
      {
        isInitialized = 1;
        timer.oscillate( INDICATOR_PIN, 100, LOW, 10 );
      }
      goto upd;
    }
    
    if ( cmd.code == 0xFA ) // servo control
    {
      //timer.oscillate( INDICATOR_PIN, 50, LOW, 2 );
      rotX = cmd.hiParam;      
      rotZ = cmd.loParam;
      
      if ( rotX > MAX_PWM_SERVO )
      {
        timer.oscillate( INDICATOR_PIN, 50, LOW, 2 );
        rotX = MAX_PWM_SERVO;
      }
      if ( rotZ > MAX_PWM_SERVO )
      {
        timer.oscillate( INDICATOR_PIN, 50, LOW, 2 );
        rotZ = MAX_PWM_SERVO;
      }
      if ( rotX < MIN_PWM_SERVO )
      {
        timer.oscillate( INDICATOR_PIN, 50, LOW, 2 );
        rotX = MIN_PWM_SERVO;
      }
      if ( rotZ < MIN_PWM_SERVO )
      {
        timer.oscillate( INDICATOR_PIN, 50, LOW, 2 );
        rotZ = MIN_PWM_SERVO;
      }
      pwm.setPWM( 0, 0, rotZ );
      pwm.setPWM( 1, 0, rotX );
    }
    else
    {
      if ( cmd.code == 0xFB ) // motor control
      {
        timer.stop( timeoutEvent );
        motorL = cmd.hiParam;
        motorR = cmd.loParam;
        analogWrite( LEFT_MOTOR_PIN, motorL );
        analogWrite( RIGHT_MOTOR_PIN, motorR );
        timeoutEvent = timer.after( 500, resetMotors, NULL );
      }
    }
  }
  upd:
  timer.update();
  /*rotX = 140;
  while ( true )
  {
    while( rotX !=500 )
    {
      pwm.setPWM( 0, 0, rotX );
      pwm.setPWM( 1, 0, rotX );
      delay(10);
      ++rotX;
    }
    while ( rotX != 140 )
    {
      pwm.setPWM( 0, 0, rotX );
      pwm.setPWM( 1, 0, rotX );
      delay(10);
      --rotX;
    }
  }*/
}
