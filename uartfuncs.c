/*
 * uartfuncs.c
 *
 * Main file
 *
 *  Version:    1.0
 *  Created on: 29.12.2024
 *  Author:     Pasi
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/uart.h"

#include "uartfuncs.h"
#include "command.h"
#include "rgb.h"

//*****************************************************************************
//
// Read UART data until a frame is found.
// Read a whole frame until end of frame is found and return it in the buffer.
// Remove escape characters
//
// \param  UARTBuffer for received frame. Max size is CMDBUFSIZE.
//
// \return SUCCESS if frame was read, FAIL if something went wrong
//
//*****************************************************************************
int32_t ReadFrame(int32_t* UARTBuffer)
{
    int32_t ThisChar;
    int32_t index = 0;

	// Find the beginning of a frame
	do
	{
		// Read a character using the blocking read function.
		ThisChar = UARTCharGet(UART1_BASE);

		// Ignore next char if escape
		if(ThisChar == ESCCHAR)
		{
			UARTCharGet(UART1_BASE);
		}

	}
	while (ThisChar != STARTOF);

	index = 0;
	UARTBuffer[index++] = ThisChar;

	// receive a frame and copy it in a buffer
	do
	{
		ThisChar = UARTCharGet(UART1_BASE);

		// Packet was interrupted and transmitter reset if received another start character
		// Discard previous data and start over
		if(ThisChar == STARTOF)
		{
			index = 0;
		}
		// Do not save escape character and do not check if the next one is end of frame
		else if(ThisChar == ESCCHAR)
		{
			ThisChar = UARTCharGet(UART1_BASE);
			UARTBuffer[index++] = ThisChar;
			ThisChar = UARTCharGet(UART1_BASE);
		}

		UARTBuffer[index++] = ThisChar;
	}
	while (ThisChar != ENDOF && index < CMDBUFSIZE);

	// Return failure if buffer overrun
	if(index < CMDBUFSIZE)
	{
		return (SUCCESS);
	}
	else
	{
		return (FAIL);
	}
}

//*****************************************************************************
//
// Create a set response frame payload.
//
// \param  result - 0 = request was executed, 1 = otherwise
//
//*****************************************************************************
void SendSetResponse(int32_t result)
{
	int32_t CommandBuffer[CMDBUFSIZE];

	// Set response packet payload
	CommandBuffer[SRESP_START_INDEX] = STARTOF;
	CommandBuffer[SRESP_TYPE_INDEX] = MSG_RESPONSE;
	CommandBuffer[SRESP_RESULT_INDEX] = result;
	CommandBuffer[SRESP_END_INDEX] = ENDOF;

	SendPayload(CommandBuffer, SRESP_END_INDEX);
}

//*****************************************************************************
//
// Create a get response frame payload.
//
// \param  pui100RGBColor - Colors in 0-100 scale
//
//*****************************************************************************
void SendGetResponse(const uint32_t* pui100RGBColor)
{
	int32_t CommandBuffer[CMDBUFSIZE];

	// Get response packet payload
	CommandBuffer[GRESP_START_INDEX] = STARTOF;
	CommandBuffer[GRESP_TYPE_INDEX] = MSG_RESPONSE;
	CommandBuffer[GRESP_RED_INDEX] = pui100RGBColor[RED];
	CommandBuffer[GRESP_GREEN_INDEX] = pui100RGBColor[GREEN];
	CommandBuffer[GRESP_BLUE_INDEX] = pui100RGBColor[BLUE];
	CommandBuffer[GRESP_END_INDEX] = ENDOF;

	SendPayload(CommandBuffer, (uint32_t)GRESP_END_INDEX);
}

//*****************************************************************************
//
// Send a get response frame.
// Add esc characters
//
// \param  CommandBuffer - Buffer with data payload. Max size is CMDBUFSIZE.
// \param  length - Data length
//
//*****************************************************************************
void SendPayload(const int32_t* CommandBuffer, uint32_t length)
{
	int32_t UARTBuffer[CMDBUFSIZE];
    int32_t UARTIndex = 0;
    int32_t PayloadIndex = 0;
    int32_t SendIndex = 0;

    // Start character
    UARTBuffer[UARTIndex++] = CommandBuffer[PayloadIndex++];

    // Add escape characters to the frame
	do
	{
		if(CommandBuffer[PayloadIndex] == STARTOF || CommandBuffer[PayloadIndex] == ENDOF || CommandBuffer[PayloadIndex] == ESCCHAR)
		{
		    UARTBuffer[UARTIndex++] = ESCCHAR;
		}
	    UARTBuffer[UARTIndex++] = CommandBuffer[PayloadIndex++];
	}
	while (PayloadIndex < length && UARTIndex < CMDBUFSIZE);
    // End character
    UARTBuffer[UARTIndex] = CommandBuffer[PayloadIndex];

    // Send frame
    SendIndex = 0;
	do
	{
		UARTCharPut(UART1_BASE, (unsigned char)UARTBuffer[SendIndex++]);
	}
	while (SendIndex <= UARTIndex);
}

