/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 slideshow demo application.
 *
 *         This application runs a picture slideshow that you can control
 *         with the joystick.
 *
 *         The application changes picture automatic after 3 seconds.
 *         Pressing joystick right or left will change picture in that
 *         direction and also change in which direction the timer will
 *         change picture afterwards. Pressing up is the same as pressing
 *         left and down is the same as right. Pressing enter will quit
 *         the application.
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
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/production_demo_rev_A/slideshow.c $
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


#define SLIDESHOW_MAXEVENTS 4
#define SLIDESHOW_MAXPICS 10
#define SLIDESHOW_DELAYSECONDS 3

typedef enum Slideshow_Event_enum
{
	SLIDESHOW_EXIT,
	SLIDESHOW_TIMER,
	SLIDESHOW_NEXT,
	SLIDESHOW_PREV
} Slideshow_event_t;

static FIFO_handle_t * Slideshow_eventFifo;  //!< FIFO containing slideshow app events.


static void Slideshow_TimerCallback( void )
{
	Slideshow_event_t const timerEvent = SLIDESHOW_TIMER;
	FIFO_PutData( Slideshow_eventFifo, &timerEvent );
}


static void Slideshow_JoystickCallback( JOYSTICK_event_t const * event )
{
	if ((event->clicked & (JOYSTICK_UP | JOYSTICK_LEFT)) != 0x00) {
		Slideshow_event_t const timerEvent = SLIDESHOW_PREV;
		FIFO_PutData( Slideshow_eventFifo, &timerEvent );
	}
	else if ((event->clicked & (JOYSTICK_DOWN | JOYSTICK_RIGHT)) != 0x00) {
		Slideshow_event_t const timerEvent = SLIDESHOW_NEXT;
		FIFO_PutData( Slideshow_eventFifo, &timerEvent );
	}
	else if ((event->clicked & JOYSTICK_ENTER) != 0x00) {
		Slideshow_event_t const timerEvent = SLIDESHOW_EXIT;
		FIFO_PutData( Slideshow_eventFifo, &timerEvent );
	}
}


static void Slideshow_UpdateIndex( int8_t * picIdx, int8_t direction )
{
	*picIdx += direction;
	if (*picIdx < 0) {
		*picIdx = SLIDESHOW_MAXPICS - 1;
	}
	else if (*picIdx >= SLIDESHOW_MAXPICS) {
		*picIdx = 0;
	}
}


void Slideshow( void )
{
	LCD_SetScreen( 0x00 );

	FIFO_data_t * fifoBuffer = MEM_ALLOC_ARRAY( Slideshow_event_t, SLIDESHOW_MAXEVENTS );
	
	if (fifoBuffer == NULL) {
		POPUP_MsgBox( 10, 2, 6, "Not enough\r\nmemory!", NULL );
	} else {
		TIMING_event_t timerEvent;
		FIFO_handle_t fifo;
		
		// Init FIFO.
		Slideshow_eventFifo = &fifo;
		FIFO_Init( &fifo, fifoBuffer, SLIDESHOW_MAXEVENTS );
		
		// Init slideshow data and state.
		static uint8_t const CAL_PGM_DEF(* const pictures[SLIDESHOW_MAXPICS]) = {
			FLASHPICS_ECARS_DEMO_0,
			FLASHPICS_ECARS_DEMO_1,
			FLASHPICS_ECARS_DEMO_2,
			FLASHPICS_ECARS_DEMO_3,
			FLASHPICS_ECARS_DEMO_4,
			FLASHPICS_ECARS_DEMO_5,
			FLASHPICS_ECARS_DEMO_6,
			FLASHPICS_ECARS_DEMO_7,
			FLASHPICS_ECARS_DEMO_8,
			FLASHPICS_ECARS_DEMO_9
		};
		int8_t picIdx = 0;
		int8_t direction = 1;
		
		// Install joystick handler.
		CAL_disable_interrupt();
		JOYSTICK_EventHandler_t oldHandler = JOYSTICK_GetEventHandler();
		JOYSTICK_SetEventHandler( Slideshow_JoystickCallback );
		CAL_enable_interrupt();
		
		// Run main slideshow loop.
		bool exit = false;
		do {
			// Show current picture.
			PICTURE_CopyFullscreenFlashToLcd( CAL_pgm_read_puint8(&pictures[picIdx]) );
			
			// Register timer event.
			TIMING_RemoveEvent( &timerEvent );
			TIMING_AddCallbackEventAfter( RTC_TICKS_PER_SECOND * SLIDESHOW_DELAYSECONDS,
					Slideshow_TimerCallback,
					&timerEvent );
			
			// Wait for event from timer or joystick.
			Slideshow_event_t event;
			while (FIFO_HasData( &fifo, Slideshow_event_t ) == false) {
				POWER_EnterIdleSleepMode();
			}

			FIFO_GetData( &fifo, &event );
			
			// Process event.
			switch (event) {
				case SLIDESHOW_EXIT : exit = true; break;
				
				case SLIDESHOW_TIMER :
					Slideshow_UpdateIndex( &picIdx, direction );
				break;
		
				case SLIDESHOW_NEXT :
					direction = 1;
					Slideshow_UpdateIndex( &picIdx, direction );
				break;
		
				case SLIDESHOW_PREV :
					direction = -1;
					Slideshow_UpdateIndex( &picIdx, direction );
				break;
				
				default : break;
			}
		} while (exit == false);
		
		// Clean up.
		CAL_disable_interrupt();
		TIMING_RemoveEvent( &timerEvent );
		JOYSTICK_SetEventHandler( oldHandler );
		CAL_enable_interrupt();

		MEM_FREE( fifoBuffer );
	}
}
