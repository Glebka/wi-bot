#ifndef COMMON_H
#define COMMON_H
/*************************************************************************
 * WiBot Project
 *************************************************************************
 * @file    common.h
 * @date
 * @author  Hlieb Romanov
 * @brief   Common definitions and constants
 ************************************************************************/

#define DEFAULT_BAUD_RATE 115200
#define MAX_PWM_SERVO 500 // rotation "angle" should be less than this value in order to not overload servo motor
#define MIN_PWM_SERVO 150 // rotation "angle" should be greater than this value in order to not overload servo motor
#define STEP 5
#define NEXT_CMD_TIMEOUT_MS 500 // next command timeout in ms
#define HEART_BEAT_TIMEOUT_MS 1000

#endif
