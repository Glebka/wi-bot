#include <Wire.h>
#include "Timer.h"
#include <Adafruit_PWMServoDriver.h>

#define MAX_PWM_SERVO 500
#define MIN_PWM_SERVO 140
#define STEP 5

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

Timer timer;
int timeoutEvent = 0;

uint16_t rotX = MIN_PWM_SERVO;
uint16_t rotZ = MIN_PWM_SERVO;
int cmd = 0;

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  pwm.setPWM(0, 0, rotZ);
  pwm.setPWM(1, 0, rotX);
  timer.oscillate(9, 100, LOW, 10);
}

void loop() {
  if (Serial.available() > 0) 
  {
    timer.oscillate(9, 50, LOW, 2);
    cmd = Serial.read();
    switch(cmd)
    {
      case 'W':
        if (rotX - STEP >= MIN_PWM_SERVO)
          rotX -= STEP;
        break;
      case 'S':
        if (rotX + STEP <= MAX_PWM_SERVO)
          rotX += STEP;
        break;
      case 'A':
        if (rotZ + STEP <= MAX_PWM_SERVO)
          rotZ += STEP;
        break;
      case 'D':
        if (rotZ - STEP >= MIN_PWM_SERVO)
          rotZ -= STEP;
        break;
      default:
        break;
    }
  }
  pwm.setPWM(0, 0, rotZ);
  pwm.setPWM(1, 0, rotX);
  timer.update();
}
