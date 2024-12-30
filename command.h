/*
 * command.h
 *
 * UART functions
 *
 *  Version:    1.0
 *  Created on: 29.12.2024
 *  Author:     Pasi
 */

#ifndef __COMMAND_H__
#define __COMMAND_H__

#define WIDEINTENSITY (float)1.0  // Always max common intensity

// Duty cycle multiplier for LED
// LED PWM = 0-0xffff, Command argument is 0-100
// 655*100 = 65500 (max would be 65535). Close enough.
#define DUTYSCALE 655

// Command codes
#define STARTOF 0xff
#define ENDOF   0xfe
#define ESCCHAR 0xfd
#define MSG_REQUEST 0x00
#define MSG_RESPONSE 0x01
#define OPCODE_SET 0x00
#define OPCODE_GET 0x01

// Command indexes
#define MSG_TYPE_INDEX  1
#define OPCODE_INDEX    2

// Request packet indexes
#define REQ_START_INDEX  0
#define REQ_TYPE_INDEX   1
#define REQ_OPCODE_INDEX 2
#define REQ_RED_INDEX    3
#define REQ_GREEN_INDEX  4
#define REQ_BLUE_INDEX   5
#define REQ_END_INDEX    6

// Get response packet indexes
#define GRESP_START_INDEX 0
#define GRESP_TYPE_INDEX  1
#define GRESP_RED_INDEX   2
#define GRESP_GREEN_INDEX 3
#define GRESP_BLUE_INDEX  4
#define GRESP_END_INDEX   5

// Set response packet indexes
#define SRESP_START_INDEX  0
#define SRESP_TYPE_INDEX   1
#define SRESP_RESULT_INDEX 2
#define SRESP_END_INDEX    3

// Set result
#define SUCCESS 0
#define FAIL    1

//*****************************************************************************
//
// Functions from commands.c
//
//*****************************************************************************

int32_t CommandExe(const int32_t* CMDBuffer, int32_t result);

#endif // __COMMAND_H__
