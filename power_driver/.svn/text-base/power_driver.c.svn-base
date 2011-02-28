// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 power management driver source file
 *
 *      The power management driver provides an interface to the power
 *      management and sleep system of the AVR microcontroller. The driver hides
 *      the details of preparing for and entering sleep modes.
 *
 *      When the deeper sleep modes are used, for instance Power Save sleep
 *      mode, some interrupts are still in operation. However, the user might
 *      not want to wake up and resume the main application every time the
 *      real-time clock ticks. The clock driver should process the tick and then
 *      enter Power Save sleep mode again. In order to allow the user to resume
 *      the main application on selected conditions only, the driver provides a
 *      callback function, named POWER_WakeupCallback, which tells the power
 *      management driver to resume the main application instead of re-entering
 *      sleep mode. The DB101 firmware uses this feature to resume the main
 *      application upon joystick movement and not after every clock tick.
 *
 *      Note that the driver initializes the AVR microcontroller with all
 *      peripherals in Power Reduction Mode. This means that the user must
 *      manually disable Power Reduction Mode for individual peripherals when
 *      required. Typically, the driver for the peripheral should take care of
 *      this for you.
 *
 * \par Application note:
 *      AVR482: DB101 Software
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 *
 * $Revision: 2404 $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/power_driver/power_driver.c $
 * $Date: 2007-09-17 00:44:48 -0600 (ma, 17 sep 2007) $  \n
 *
 * Copyright (c) 2006, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include <stdbool.h>
#include <cal.h>

#include "power_driver.h"

#include <rtc_driver.h>
#include <s6b1713_driver.h>
#include <joystick_driver.h>
#include <backlight_driver.h>
#include <sound_driver.h>
#include <lcd_lib.h>


/*********************
 * Private variables.
 *********************/

//! if true, start the whole application after an interrupt instead of just returning to sleep
static bool volatile POWER_wakeupRequested = false;



/****************************
 * Function implementations.
 ****************************/

/*! 
 * When an interrupt wakes up the controller, and wants the controller to be awake
 * after the interrupt is done, it should call this function. Otherwise the power-driver 
 * will make the controller sleep again after the interrupt is done
 */
void POWER_WakeupCallback( void )
{
	POWER_wakeupRequested = true;
}

void POWER_EnterIdleSleepMode( void )
{
	SMCR = (0 << SM2) | (0 << SM1) | (0 << SM0) | (1 << SE);
	CAL_sleep();
	SMCR = (0 << SE);
}


/*! 
 * Stops some modules and the lcd, disables jtag and enters powersave mode.
 * If waken by an interrupt, it will enter sleep mode again unless the interrupt
 * called POWER_WakeupCallback(void)
 * 
 * It enables the disabled modules and jtag again when doing a full wakeup
 */
void POWER_EnterPowerSaveMode( void )
{
	// Stop some modules.
	RTC_TickHandler_t oldTickHandler = RTC_GetTickHandler();
	RTC_RemoveTickHandler();
	SOUND_StopAudio();
	S6B1713_PowerSave();
	BACKLIGHT_Off();
	
	// Disable JTAG interface if enabled.
	uint8_t savedSREG = SREG;
	CAL_disable_interrupt();
	uint8_t newMCUCR = MCUCR | (1 << JTD);
	MCUCR = newMCUCR;
	MCUCR = newMCUCR;
	SREG = savedSREG;
	
	// Sleep until someone requests a wakeup.
	SMCR = (0 << SM2) | (1 << SM1) | (1 << SM0) | (1 << SE);
	POWER_wakeupRequested = false;
	while (POWER_wakeupRequested == false) {
		CAL_sleep();
	}
	SMCR = (0 << SE);
	
	// Enable JTAG interface again, if previously enabled.
	CAL_disable_interrupt();
	newMCUCR = MCUCR & ~(1 << JTD);
	MCUCR = newMCUCR;
	MCUCR = newMCUCR;
	SREG = savedSREG;
	
	// Enable some modules.
	BACKLIGHT_Init();
	S6B1713_PowerSaveOff();
	LCD_Init(); // Need this, because for some reason, the LCD won't wake up properly.
	RTC_SetTickHandler( oldTickHandler );
}


/*! 
 * Initialize the power-driver. This shuts down most functions of the AVR.
 * The seperate drivers will then have to start the modules they need.
 */
void POWER_Init( void )
{
	// NOTE: Modules are turned on/off by driver.
        /*! TWI, timers 0-2, SPI, USART0 and ADC is shut down */
	PRR0 = (1 << PRTWI) |
	       (1 << PRTIM2) |
	       (1 << PRTIM0) |
	       // bit 4 reserved
	       (1 << PRTIM1) |
	       (1 << PRSPI) |
	       (1 << PRUSART0) |
	       (1 << PRADC);
	
        /*! Timers 3-5 and USART1 is shut down */
	PRR1 = // bit 7 reserved
	       // bit 6 reserved
	       (1 << PRTIM5) |
	       (1 << PRTIM4) |
	       (1 << PRTIM3) |
	       // bit 2 reserved
	       // bit 1 reserved
	       (1 << PRUSART1);
	
        
	ACSR |= (1 << ACD); //!< Disable Analog Comparator.
	
	DIDR0 = (1 << ADC7D) | //!< Disable digital input buffer for analog input pins.
	        (1 << ADC6D) |
	        (1 << ADC5D) |
	        (1 << ADC4D);
}
