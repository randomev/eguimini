// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 joystick driver source file
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
 * $Revision: 2436 $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/joystick_driver/joystick_driver.c $
 * $Date: 2007-09-17 15:18:48 -0600 (ma, 17 sep 2007) $  \n
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
#include <common.h>

#include "joystick_driver.h"
#include <power_driver.h>




/*************************************
 * Hardware configuration definitons.
 *************************************/

#define JOYSTICK_DDR_REG DDRC    //!< Data Direction register for joystick IO port.
#define JOYSTICK_PORT_REG PORTC  //!< Output register for joystick IO port.
#define JOYSTICK_PIN_REG PINC    //!< Input register for joystick IO port.

#define JOYSTICK_COM_DDR_REG  DDRB
#define JOYSTICK_COM_PORT_REG PORTB
#define JOYSTICK_COM_PIN_REG  PINB
#define JOYSTICK_COM_PIN      PB4

#define JOYSTICK_COM_INTVECT PCINT0_vect
#define JOYSTICK_COM_PCIE_BIT PCIE0
#define JOYSTICK_COM_PCIF_BIT PCIF0
#define JOYSTICK_COM_PCMSK_REG PCMSK0
#define JOYSTICK_COM_PCINT_BIT PCINT4


/*********************
 * Private variables.
 *********************/

volatile uint8_t JOYSTICK_currentState = 0;      //!< Stores the current debounced state.
volatile uint8_t JOYSTICK_currentHoldState = 0;  //!< Stores the current hold state.
volatile JOYSTICK_delay_t JOYSTICK_pressAndHoldDelay = 0;  //!< Delay in number of calls to polling handler. 0 = disabled.
volatile JOYSTICK_delay_t JOYSTICK_doubleClickDelay = 0;   //!< Delay in number of calls to polling handler. 0 = disabled.
JOYSTICK_EventHandler_t JOYSTICK_EventHandler = NULL;  //!< Event handler callback.



/****************************
 * Function implementations.
 ****************************/

/*! \brief Enter Wakeup Mode
 *
 * Set the joystick to power consumption save-mode.
 * It turns the pull-up off for the direction pins.
 * And sets the common pin to input with pull-up
 *
 */
static void JOYSTICK_SetWakeupMode( void )
{
	// Direction pins to output low.
	JOYSTICK_PORT_REG &= ~(JOYSTICK_UP | JOYSTICK_DOWN | JOYSTICK_LEFT | JOYSTICK_RIGHT | JOYSTICK_ENTER);
	JOYSTICK_DDR_REG |= (JOYSTICK_UP | JOYSTICK_DOWN | JOYSTICK_LEFT | JOYSTICK_RIGHT | JOYSTICK_ENTER);
	// Common pin to input with pull-up.
	JOYSTICK_COM_DDR_REG &= ~(1 << JOYSTICK_COM_PIN);
	JOYSTICK_COM_PORT_REG |= (1 << JOYSTICK_COM_PIN);
}

/*! \brief Leave WakeUp mode and enter active mode
 * 
 * Leave power-saving mode and enter active mode
 * Sets the direction pins to input with pull-up
 * and set common pin to output
 *
 */
static void JOYSTICK_SetActiveMode( void )
{
	// Direction pins to input with pull-up.
	JOYSTICK_DDR_REG &= ~(JOYSTICK_UP | JOYSTICK_DOWN | JOYSTICK_LEFT | JOYSTICK_RIGHT | JOYSTICK_ENTER);
	JOYSTICK_PORT_REG |= (JOYSTICK_UP | JOYSTICK_DOWN | JOYSTICK_LEFT | JOYSTICK_RIGHT | JOYSTICK_ENTER);
	// Common pin to output low.
	JOYSTICK_COM_PORT_REG &= ~(1 << JOYSTICK_COM_PIN);
	JOYSTICK_COM_DDR_REG |= (1 << JOYSTICK_COM_PIN);
}

/*! \brief Enables external interrupt from the joystick
 *
 * Makes the board wake up from sleep on joystick movement
 * Uses the common pin for the external interrupt, so it has to be set as a input with pull-up
 * (which is done by JOYSTICK_SetWakeupMode(void))
 *
 */
static void JOYSTICK_EnableWakeupInterrupt( void )
{
	uint8_t savedSREG = SREG;
	CAL_disable_interrupt();
	
	JOYSTICK_COM_PCMSK_REG |= (1 << JOYSTICK_COM_PCINT_BIT); // Enable joystick common line for interrupts.
	PCIFR = (1 << JOYSTICK_COM_PCIF_BIT); // Clear pending flags.
	PCICR |= (1 << JOYSTICK_COM_PCIE_BIT); // Enable Pin-change interrupts for correct pin group.
	
	SREG = savedSREG;
}

/*! \brief Disables external interrupt from the joystick
 *
 * Disables the external interrupt running on the joystick's common pin
 *
 */
static void JOYSTICK_DisableWakeupInterrupt( void )
{
	uint8_t savedSREG = SREG;
	CAL_disable_interrupt();
	
	JOYSTICK_COM_PCMSK_REG &= ~(1 << JOYSTICK_COM_PCINT_BIT); // Disable joystick common line for interrupts.
	PCIFR = (1 << JOYSTICK_COM_PCIF_BIT); // Clear pending flags.
	
	SREG = savedSREG;
}

/*! \brief Interrupt handling the external interrupt from the joystick
 *
 * When in WakeUp mode and joystick is moved, return to active mode
 *
 */

CAL_ISR( JOYSTICK_COM_INTVECT )
{
	POWER_WakeupCallback();
	JOYSTICK_SetActiveMode();
	JOYSTICK_DisableWakeupInterrupt();
}


/*!
 * Sets the joystick to active mode and sets values
 * for press-and-hold and double-click
 *
 * \param  pressAndHoldDelay  How long do you have to press (in mumber of pollings) before it is counted as a hold
 * \param  doubleClickDelay   Max time between clicks (in number of pollings) to call it a double-click
 *
 */
void JOYSTICK_Init( JOYSTICK_delay_t pressAndHoldDelay, JOYSTICK_delay_t doubleClickDelay )
{
	// Setup pins and interrupts.
	JOYSTICK_SetActiveMode();
	JOYSTICK_DisableWakeupInterrupt();

	// Delay configuration.
	JOYSTICK_pressAndHoldDelay = pressAndHoldDelay;
	JOYSTICK_doubleClickDelay = doubleClickDelay;
}


void JOYSTICK_PrepareForSleep( void )
{
	JOYSTICK_SetWakeupMode();
	JOYSTICK_EnableWakeupInterrupt();
}

/*!
 * \param  eventHandler  New joystick event handler
 */
void JOYSTICK_SetEventHandler( JOYSTICK_EventHandler_t eventHandler )
{
	JOYSTICK_EventHandler = eventHandler;
}


/*
 * \return  Current joystick event handler
 */
JOYSTICK_EventHandler_t JOYSTICK_GetEventHandler( void )
{
	return JOYSTICK_EventHandler;
}


void JOYSTICK_ReleaseEventHandler( void )
{
	JOYSTICK_EventHandler = NULL;
}


/*
 * \return  Current state of the joystick
 */
uint8_t JOYSTICK_GetState( void )
{
	return JOYSTICK_currentState;
}


/*
 * \return  Current hold state of the joystick
 */
uint8_t JOYSTICK_GetHoldState( void )
{
	return JOYSTICK_currentHoldState;
}


/*!
 * Checks the state of the buttons,
 * sees if there are any double-clicks or holds
 *
 * It debounces the joystick by only continuing if the current sample is the same as last one
 * If an event handler is registered, it is called if necessary
 * It must not be called more often than maximum mechanical bounce time,
 * it's also not reentrant
 *
 */
void JOYSTICK_PollingHandler( void )
{
	// Internal state variables.
	// NOTE: This function is not re-entrant and should only be called from
	// a controlled and timed loop (or timer interrupt).
	static uint8_t lastSample = 0;
	static JOYSTICK_delay_t pressAndHoldCountdown[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	static JOYSTICK_delay_t doubleClickCountdown[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	// Read joystick state and mask out all but the joystick signals. Buttons are active low, but we invert to active high.
	uint8_t newState = ~JOYSTICK_PIN_REG;
	newState &= (JOYSTICK_UP | JOYSTICK_DOWN | JOYSTICK_LEFT | JOYSTICK_RIGHT | JOYSTICK_ENTER);
	
	// Compare to last sample. If equal, process it. If not, store and wait for next sample.
	if (newState != lastSample) {
		// Store sample for debouncing next time polling handler is called.
		lastSample = newState;

	} else {
		// Extract information about button state.
		uint8_t changedButtons = JOYSTICK_currentState ^ newState;
		uint8_t clickedButtons = changedButtons & newState;
		uint8_t releasedButtons = changedButtons & JOYSTICK_currentState;
		uint8_t stillPressed = ~changedButtons & newState;
		
		// Check for press-and-hold and double-click and update countdown values.
		uint8_t pressAndHoldMask = 0x00;
		uint8_t doubleClickMask = 0x00;
		uint8_t buttonMask = 0x01; // Used for selecting the one bit we process every iteration below.
		for (uint8_t i = 0; i < 8; ++i) { // Walk through all eight button bits, even if not all bits are joystick buttons.
			// Count down double-click delay regardless of events and states.
			if (doubleClickCountdown[i] > 0) {
				--doubleClickCountdown[i];
			}
			
			// Handle clicked buttons.
			if (clickedButtons & buttonMask) {
				// Reset hold delay upon each new button press.
				pressAndHoldCountdown[i] = JOYSTICK_pressAndHoldDelay;

				// Check for double-click.
				if (doubleClickCountdown[i] > 0) {
					doubleClickMask |= buttonMask;
				}

				// Reset double-click counter value since was just clicked.
				doubleClickCountdown[i] = JOYSTICK_doubleClickDelay;
			}
			
			// Check for press-and-hold.
			if (stillPressed & buttonMask) {
				// Count down if not reached zero yet. If already reached zero, we won't repeat the hold event.
				if (pressAndHoldCountdown[i] > 0x00) {
					// Count down and set bit in press-and-hold mask when counter reaches zero.
					--pressAndHoldCountdown[i];
					if (pressAndHoldCountdown[i] == 0x00) {
						pressAndHoldMask |= buttonMask;
					}
				}
			}
			
			// Prepare bitmask for next button bit.
			buttonMask <<= 1;
		} // end: for (uint8_t i = 0; i < 8; ++i) ...
		
		// Update state variables.
		JOYSTICK_currentHoldState |= pressAndHoldMask;
		JOYSTICK_currentHoldState &= ~releasedButtons;
		JOYSTICK_currentState = newState;
		
		// Call event handler if registered and events found.
		if ((JOYSTICK_EventHandler != NULL) && ((changedButtons | pressAndHoldMask | doubleClickMask) != 0x00)) {
			// Prepare event struct.
			JOYSTICK_event_t event;
			event.state = newState;
			event.clicked = clickedButtons;
			event.released = releasedButtons;
			event.held = pressAndHoldMask;
			event.dclick = doubleClickMask;
			
			// Call event handler.
			JOYSTICK_EventHandler( &event );
		}
	} // end: if (newState != lastSample) ... else ...
}
