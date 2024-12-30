/*
 * uartled.c
 *
 * Main file
 *
 *  Version:    1.0
 *  Created on: 28.12.2024
 *  Author:     Pasi
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/rom_map.h"

#include "rgb.h"
#include "uartfuncs.h"
#include "command.h"

//*****************************************************************************
//
// Control RGB LED PWM signals based on UART commands
//
//*****************************************************************************
int main(void)
{
	int32_t CommandBuffer[CMDBUFSIZE];

	//RGB LED color intensities. Initialized to 0 level.
	const uint32_t pui32RGBColor[3] = {0,0,0};

	// RGB is using float
    ROM_FPUEnable();
	// Set the clocking to run directly from the crystal.
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    // Enable RGB LED and timers
    // Wide timer is not needed
    RGBInit(1);
    RGBSet(pui32RGBColor, WIDEINTENSITY);

    // Initialize UART U1 in pins PB0 and PB1
    // Enable Peripheral Clocks
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    // Enable pins PB0 and PB1 for UART1 U1RX and U1TX
    ROM_GPIOPinConfigure(GPIO_PB0_U1RX);
    ROM_GPIOPinConfigure(GPIO_PB1_U1TX);
    ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Enable processor interrupts.
    ROM_IntMasterEnable();

    // Configure the UART for 9600, 8-N-1 operation.
    ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), 9600,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    // Enter infinite loop
    do
    {
    	int32_t OpSuccess;
        // Find and return a frame from the UART
    	OpSuccess = ReadFrame(CommandBuffer);

    	// Process command
    	CommandExe(CommandBuffer, OpSuccess);
    }
    while(1);

}
