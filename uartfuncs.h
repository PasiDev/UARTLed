/*
 * uartfuncs.h
 *
 * UART functions
 *
 *  Version:    1.0
 *  Created on: 29.12.2024
 *  Author:     Pasi
 */

#ifndef __UARTFUNCS_H__
#define __UARTFUNCS_H__

#define CMDBUFSIZE 100  // Max UART buffer size

//*****************************************************************************
//
// Functions from uartfuncs.c
//
//*****************************************************************************

int32_t ReadFrame(int32_t* UARTBuffer);
void SendSetResponse(int32_t result);
void SendGetResponse(const uint32_t* pui100RGBColor);
void SendPayload(const int32_t* CommandBuffer, uint32_t length);

#endif // __UARTFUNCS_H__
