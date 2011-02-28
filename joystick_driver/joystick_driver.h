// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 joystick driver header file
 *
 *      The joystick driver provides an interface to the five-way joystick on
 *      the DB101 board. The drivers hides the underlying polling operation and
 *      debouncing of the joystick "buttons" and provides functions to read the
 *      current state of the joystick, change delay settings for double-click
 *      sensing and press-and-hold conditions, and register event handler
 *      callback functions.
 *
 *      When an event handler is associated with the joystick handler, it is
 *      possible to be notified immediately whenever the joystick state changes,
 *      be it a click, release, double-click or press-and-hold condition. With
 *      the use of an event handler, there is no need for the user application
 *      to continuously poll the joystick state manually.
 *
 *      The joystick driver is not interrupt driven, since not all applications
 *      can afford to allocate five interrupt inputs for a joystick. Therefore,
 *      the joystick driver implements a polling scheme where the joystick
 *      inputs are read at regular intervals. The joystick state is then updated
 *      from these regular readings. In order to make the driver as flexible as
 *      possible, the user is responsible for calling the polling routine, named
 *      JOYSTICK_PollingHandler. In most cases this would be implemented using a
 *      timer. For the DB101 firmware, this is implemented using the Timing
 *      Library, which in turn is driven by the RTC Driver.
 *
 *      In addition to the polling operation, the driver can also enter wakeup
 *      mode. This is used when the application is idle and wants to reduce
 *      power consumption. The driver reconfigures the joystick IO-lines and
 *      uses one external interrupt line configured to wake up the AVR from any
 *      sleep mode on any movement. When it wakes up, the driver automatically
 *      enters polling mode again.
 *
 *      The polling handler must be called at invervals longer than maximum
 *      mechanical bounce time for the joystick. Datasheets specify this time
 *      to approximately 5 milliseconds. Therefore the user must implement a
 *      timer interrupt (or other schemes provided by an operative system) that
 *      calls the handler at regular intervals. The handler is not re-entrant,
 *      so care should be taken not to call it from e.g. different threads or
 *      from application code and interrupt handlers at the same time.
 *
 *      The press-and-hold and double-click delays are based on the interval at
 *      which the polling handler is called.
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
 * $Revision: 2403 $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/joystick_driver/joystick_driver.h $
 * $Date: 2007-09-17 00:38:51 -0600 (ma, 17 sep 2007) $  \n
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

#ifndef JOYSTICK_DRIVER_H
#define JOYSTICK_DRIVER_H

#include <stdint.h>
#include <cal.h>



/********************************
 * Joystick bitmask definitions.
 ********************************/

#define JOYSTICK_UP    (1 << PC5)
#define JOYSTICK_DOWN  (1 << PC4)
#define JOYSTICK_LEFT  (1 << PC6)
#define JOYSTICK_RIGHT (1 << PC3)
#define JOYSTICK_ENTER (1 << PC7)



/**********************
 * Types and typedefs.
 **********************/

//! All info regarding one joystick event.
typedef struct JOYSTICK_event_struct
{
	uint8_t state;     //!< Current button state.
	uint8_t clicked;   //!< Buttons clicked in this event.
	uint8_t released;  //!< Buttons released in this event.
	uint8_t held;      //!< Buttons held down just long enough to regard as press-and-hold in this event.
	uint8_t dclick;    //!< Buttons double-clicked in this event. Note that these will also read as "clicked".
} JOYSTICK_event_t;


//! Event handler callback type. Is called with current state and bitmasks for clicked and released buttons.
typedef void (* JOYSTICK_EventHandler_t)( JOYSTICK_event_t const * event );
//! Variable type used for delay counts for press-and-hold and double-click features.
typedef uint8_t JOYSTICK_delay_t;




/***********************
 * Function prototypes.
 ***********************/

void JOYSTICK_Init( JOYSTICK_delay_t pressAndHoldDelay, JOYSTICK_delay_t doubleClickDelay ); //!< Init driver.
void JOYSTICK_PrepareForSleep( void ); //!< Prepare driver for entering sleep mode.
void JOYSTICK_SetEventHandler( JOYSTICK_EventHandler_t eventHandler ); //!< Set new event handler, overriding old handler reference.
JOYSTICK_EventHandler_t JOYSTICK_GetEventHandler( void ); //!< Get event handler reference, in case you want to chain handlers.
void JOYSTICK_ReleaseEventHandler( void ); //!< Remove event handler reference.
uint8_t JOYSTICK_GetState( void ); //!< Get current joystick state.
uint8_t JOYSTICK_GetHoldState( void ); //!< Get current joystick hold state.
void JOYSTICK_PollingHandler( void ); //!< Must be called at intervals longer than maximum bounce time.



/***********************************************************
 * Convenience macros for quick access to joystick buttons.
 ***********************************************************/

#define JOYSTICK_IsUpPressed()    ((JOYSTICK_GetState() & JOYSTICK_UP) != 0x00)
#define JOYSTICK_IsDownPressed()  ((JOYSTICK_GetState() & JOYSTICK_DOWN) != 0x00)
#define JOYSTICK_IsLeftPressed()  ((JOYSTICK_GetState() & JOYSTICK_LEFT) != 0x00)
#define JOYSTICK_IsRightPressed() ((JOYSTICK_GetState() & JOYSTICK_RIGHT) != 0x00)
#define JOYSTICK_IsEnterPressed() ((JOYSTICK_GetState() & JOYSTICK_ENTER) != 0x00)

#endif
