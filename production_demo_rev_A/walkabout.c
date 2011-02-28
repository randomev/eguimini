/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 picture walkabout demo application.
 *
 *         This application lets you look at a big picture (ie, more
 *         pixels than the screen) by scrolling it with the joystick.
 *         Pressing enter will quit the application.
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
 * $Revision: 2353 $
 * $Date: 2007-09-11 07:29:06 -0600 (ti, 11 sep 2007) $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/production_demo_rev_A/walkabout.c $
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

#include <cal.h>

#include <picture_lib.h>
#include <timing_lib.h>
#include <fifo_lib.h>
#include <joystick_driver.h>
#include <memblock_lib.h>
#include <rtc_driver.h>
#include <power_driver.h>
#include <popup_lib.h>

#include "flashpics.h"


#define WALKABOUT_MAXEVENTS 4


static FIFO_handle_t * Walkabout_eventFifo;  //!< FIFO containing walkabout app events.


static void Walkabout_JoystickCallback( JOYSTICK_event_t const * event )
{
	FIFO_PutData( Walkabout_eventFifo, event );
}


void Walkabout( void )
{
	POPUP_MsgBox( 16, 3, 6,
	              "Move joystick to"
	              "scroll picture. "
	              "ENTER to exit.", NULL );
	
	FIFO_data_t * fifoBuffer = MEM_ALLOC_ARRAY( JOYSTICK_event_t, WALKABOUT_MAXEVENTS );
	
	if (fifoBuffer == NULL) {
		POPUP_MsgBox( 10, 2, 6, "Not enough\r\nmemory!", NULL );
	} else {
		FIFO_handle_t fifo;
		
		// Init FIFO.
		Walkabout_eventFifo = &fifo;
		FIFO_Init( &fifo, fifoBuffer, sizeof(JOYSTICK_event_t) * WALKABOUT_MAXEVENTS );
		
		// Init picture and state.
		uint8_t const CAL_PGM(* const picture) = FLASHPICS_db101pcb;
		uint16_t startX = 0;
		uint8_t startPage = 0;
		
		// Install joystick handler.
		CAL_disable_interrupt();
		JOYSTICK_EventHandler_t oldHandler = JOYSTICK_GetEventHandler();
		JOYSTICK_SetEventHandler( Walkabout_JoystickCallback );
		CAL_enable_interrupt();
		
		// Run main walkabout loop.
		bool exit = false;
		do {
			// Show current picture window.
			PICTURE_CopyFlashToLcd( picture,
					FLASHPICS_HUGE_WIDTH,
					startX,
					startPage,
					0,
					0,
					LCD_WIDTH,
					LCD_HEIGHT / LCD_PAGE_HEIGHT );
			
			// Wait for event joystick.
			JOYSTICK_event_t event;
			while (FIFO_HasData( &fifo, JOYSTICK_event_t ) == false) {
				POWER_EnterIdleSleepMode();
			}
				
			FIFO_GetData( &fifo, &event );
			
			// Process event.
			if ((event.clicked & JOYSTICK_ENTER) != 0x00) {
				exit = true;
			}
			if ((event.clicked & JOYSTICK_UP) != 0x00) {
				if (startPage > 0) {
					--startPage;
				} else {
					startPage = 0;
				}
			}
			if ((event.clicked & JOYSTICK_DOWN) != 0x00) {
				if (startPage < (((FLASHPICS_HUGE_HEIGHT - LCD_HEIGHT) / LCD_PAGE_HEIGHT) - 1)) {
					++startPage;
				} else {
					startPage = (FLASHPICS_HUGE_HEIGHT - LCD_HEIGHT) / LCD_PAGE_HEIGHT;
				}
			}
			if ((event.clicked & JOYSTICK_LEFT) != 0x00) {
				if (startX > LCD_PAGE_HEIGHT) {
					startX -= LCD_PAGE_HEIGHT;
				} else {
					startX = 0;
				}
			}
			if ((event.clicked & JOYSTICK_RIGHT) != 0x00) {
				if (startX < (FLASHPICS_HUGE_WIDTH - LCD_WIDTH - LCD_PAGE_HEIGHT)) {
					startX += LCD_PAGE_HEIGHT;
				} else {
					startX = FLASHPICS_HUGE_WIDTH - LCD_WIDTH;
				}
			}
		} while (exit == false);
		
		// Clean up.
		CAL_disable_interrupt();
		JOYSTICK_SetEventHandler( oldHandler );
		CAL_enable_interrupt();

		MEM_FREE( fifoBuffer );						
	}
}
