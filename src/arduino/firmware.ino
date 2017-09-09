/*************************************************************************
 * WiBot Arduino Pro Mini firmware
 *************************************************************************
 * @file    firmware.ino
 * @date
 * @author  Hlieb Romanov
 * @brief   Main firmware file for Arduino Pro Mini board
 ************************************************************************/
#include <Timer.h>
#include <Servo.h>

#include "common.h"
#include "commands.h"
#include "promini_pins.h"

Servo servoX;         // servo motor that rotates camera around X axis (up-down)
Servo servoZ;         // servo motor that rotates camera around Z axis (left-right)
Timer timer;          // timer for oscillating the status LED and for timeout handling
int timeoutEvent = 0; // timeout event ID for handling situations when HU ocasionally disconnects

byte isInitialized = 0;

Command cmd;

/**
 * Callback function that turns off motors.
 * This callback is called when HU doesn't send commands during 500 ms
 * @param context - not used 
 */
void resetMotors(void *context)
{
  analogWrite(LEFT_MOTOR_PIN, 0);
  analogWrite(RIGHT_MOTOR_PIN, 0);
}

/**
 * Setup function for initializing the board's hardware
 */
void setup(void)
{
  Serial.begin(DEFAULT_BAUD_RATE);
  servoX.attach(X_AXIS_SERVO_PIN, MIN_PWM_SERVO, MAX_PWM_SERVO);
  servoZ.attach(Z_AXIS_SERVO_PIN, MIN_PWM_SERVO, MAX_PWM_SERVO);
  resetMotors(NULL);  
}

void loop(void)
{
  if (Serial.available() > 0)
  {
    if (Serial.readBytes((char *)&cmd, sizeof(Command)) == 0)
    {
      timer.oscillate(INDICATOR_PIN, 50, LOW, 2);
    }

    if (!isInitialized)
    {
      if (memcmp(&cmd, &INIT_CMD, sizeof(struct Command)) == 0)
      {
        isInitialized = 1;
        timer.oscillate(INDICATOR_PIN, 100, LOW, 10);
      }
      goto upd;
    }

    switch(cmd.code) {
      case CMD_CODE_CAM_ROTATE:
        {
          servoX.write(cmd.hiParam);
          servoZ.write(cmd.loParam);
        }
        break;
      case CMD_CODE_MOTOR_CONTROL:
        {
          timer.stop(timeoutEvent);
          analogWrite(LEFT_MOTOR_PIN, cmd.hiParam);
          analogWrite(RIGHT_MOTOR_PIN, cmd.loParam);
          timeoutEvent = timer.after(NEXT_CMD_TIMEOUT_MS, resetMotors, NULL);
        }        
        break;
      default:
        break;
    }
  }
upd:
  timer.update();
}
