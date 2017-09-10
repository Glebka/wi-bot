#ifndef COMMANDS_H
#define COMMANDS_H
/*************************************************************************
 * WiBot project
 *************************************************************************
 * @file    commands.h
 * @date
 * @author  Hlieb Romanov
 * @brief   Command definition and constants
 ************************************************************************/
#include <inttypes.h>

/**
 * Command structure that is transferred between 
 * HU and ARDU via UART.
 */
struct Command
{
 uint16_t code;    /// command type
 uint16_t hiParam; /// high byte of optional parameter
 uint16_t loParam; /// low byte of optional parameter
};

const uint16_t CMD_CODE_INIT = 0xFF;            /// HU -> ARDU
const uint16_t CMD_CODE_HEARTBEAT = 0xFE;       /// ARDU -> HU
const uint16_t CMD_CODE_VBAT = 0xFD;            /// ARDU -> HU
const uint16_t CMD_CODE_MOTOR_CONTROL = 0xFB;   /// HU -> ARDU
const uint16_t CMD_CODE_CAM_ROTATE = 0xFA;      /// HU -> ARDU

const Command INIT_CMD = {CMD_CODE_INIT, 0x1, 0xFF};            /// initialization command from HU
const Command HEARTBEAT_CMD = {CMD_CODE_HEARTBEAT, 0x00, 0x00}; /// initialization of heartbeat notification for HU

#endif
