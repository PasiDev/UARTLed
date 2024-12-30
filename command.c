/*
 * command.c
 *
 * Main file
 *
 *  Version:    1.0
 *  Created on: 29.12.2024
 *  Author:     Pasi
 */

#include <stdint.h>
#include <stdbool.h>
#include "uartfuncs.h"
#include "command.h"
#include "rgb.h"

//*****************************************************************************
//
// Process command. Escape characters have been removed.
//
// \param  CMDBuffer - Buffer containing command frame
// \param  Result - 0 = UARTSUCCESS or 1 = UARTFAIL
//
// \return SUCCESS if frame was read, FAIL if something went wrong
//
//*****************************************************************************
int32_t CommandExe(const int32_t* CMDBuffer, int32_t result)
{
	uint32_t pui32RGBColor[3] = {0,0,0};

	// Check MSG_TYPE
	if(CMDBuffer[MSG_TYPE_INDEX] != MSG_REQUEST)
	{
		// Only requests accepted
		return (FAIL);
	}

	// Check OPCODE
	if(CMDBuffer[OPCODE_INDEX] == OPCODE_SET)
	{
		// Set command with sanity checks
		if ((CMDBuffer[REQ_RED_INDEX]   >= 0 && CMDBuffer[REQ_RED_INDEX]   <= 100) ||
			(CMDBuffer[REQ_GREEN_INDEX] >= 0 && CMDBuffer[REQ_GREEN_INDEX] <= 100) ||
			(CMDBuffer[REQ_BLUE_INDEX ] >= 0 && CMDBuffer[REQ_BLUE_INDEX]  <= 100))
		{
			// Duty cycle value was wrong
			result = FAIL;
		}
		else
		{
			// Scale from 0-100 to 0-0xFFFF
			pui32RGBColor[RED] = CMDBuffer[REQ_RED_INDEX] * DUTYSCALE;
			pui32RGBColor[GREEN] = CMDBuffer[REQ_GREEN_INDEX] * DUTYSCALE;
			pui32RGBColor[BLUE] = CMDBuffer[REQ_BLUE_INDEX] * DUTYSCALE;
		    RGBSet(pui32RGBColor, WIDEINTENSITY);
		}
	    SendSetResponse(result);
	}
	else if(CMDBuffer[OPCODE_INDEX] == OPCODE_GET)
	{
		// Get command
		RGBColorGet(pui32RGBColor);
		// Scale from 0-0xFFFF to 0-100. Not caring about rounding
		pui32RGBColor[RED] /= DUTYSCALE;
		pui32RGBColor[GREEN] /= DUTYSCALE;
        pui32RGBColor[BLUE] /= DUTYSCALE;
		SendGetResponse(pui32RGBColor);
	}
	else
	{
		// Unknown OPCODE
		return (FAIL);
	}

	return (SUCCESS);
}



