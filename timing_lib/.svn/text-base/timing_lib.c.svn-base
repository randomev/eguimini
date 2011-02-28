// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  Generic event timing library source file.
 *
 *      The timing and scheduling library provides functions for a timing and
 *      scheduling infrastructure to which other models can subscribe to
 *      periodic or one-shot timed events. The time granularity is referred to
 *      as a tick, which typically corresponds with the tick period from the
 *      RTC Driver. The DB101 firmware registers this library's
 *      TIMING_TickHandler function as the tick handler for the RTC driver.
 *
 *      The timing events can be implemented as either callbacks or counter
 *      updates. With callback events, a function is called when the timing
 *      event occurs. For counter updates, the user registers a pointer to a
 *      counter variable, which gets incremented once every time the timing
 *      event occurs.
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
 * $Revision: 2391 $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/timing_lib/timing_lib.c $
 * $Date: 2007-09-13 12:55:27 -0600 (to, 13 sep 2007) $  \n
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
#include "timing_lib.h"


/*********************************
 * Private variables and typedefs
 *********************************/

typedef uint16_t TIMING_queueSize_t;  //!< Size type for holding number of events in queue.

volatile TIMING_queueSize_t TIMING_queueSize;  //!< Number of events in queue.
TIMING_event_t * TIMING_firstEvent;   //!< Head of event queue, if any.
volatile TIMING_time_t TIMING_currentTime;     //!< Current timestamp. Updated by TIMING_TickHandler.
volatile TIMING_time_t TIMING_nextEventTime;   //!< The timestamp at which next event will occur, if any.



/*******************************
 * Internal function prototypes
 *******************************/

//! Insert event into queue, sort by event trigger time.
static void TIMING_AddEvent( TIMING_event_t * event );
//! Perform event operation based on event type. Updates next TIMING_nextEventTime if required.
static void TIMING_DoEvent( TIMING_event_t const * event );



/***************************
 * Function implementations
 ***************************/

/*!
 *  This function must be called before doing anything with the library,
 *  even before any calls to TIMING_TickHandler.
 */
void TIMING_Init( void )
{
	// Initialize necessary state variables.
	TIMING_queueSize = 0;
	TIMING_currentTime = 0;
}


/*!
 *  This function performs all the work for us, performing all event operations
 *  and managing periodic events. This function must not be called from several
 *  places. It should only be called from a dedicated timing source, e.g. a
 *  timer overflow interrupt. Note that this function will disable all interrupts
 *  during execution. This means that all event operations should be kept as
 *  short as possible. Longer event operations should be implemented as a flag
 *  signaling to other parts of the application that things should be done.
 *
 *  The state of the interrupt flag is restore to its original state when
 *  this function exits.
 */
void TIMING_TickHandler( void )
{
	// Wait need to operate on the queue undisturbed.
	uint8_t const savedSREG = SREG;
	CAL_disable_interrupt();

	// Time is increment in any case.
	++TIMING_currentTime;
	
	// Something to do only if there are events in the queue and timestamps match.
	///kmr while ((TIMING_queueSize > 0) && (TIMING_currentTime == TIMING_nextEventTime)) {
	///kmr Rework while() statement to explictly set order of volatile variable access
	// mt further rework to avoid "break"
	uint8_t goon = 1;
	while ( ( TIMING_queueSize > 0 ) && goon ) {
		TIMING_time_t next_event = TIMING_nextEventTime;
		if (TIMING_currentTime != next_event) {
			goon = 0;
		}
		else {
			// Remove first event from queue and update TIMING_nextEventTime if necessary.
			TIMING_event_t * event = TIMING_firstEvent;
			TIMING_firstEvent = event->next;
			--TIMING_queueSize;
			if (TIMING_queueSize > 0) {
				TIMING_nextEventTime = TIMING_firstEvent->triggerTime;
			}
		
			// Perform operation depending on event type.
			TIMING_DoEvent( event );
		
			// Decrement event repeat count if not TIMING_INFINITE_REPEAT.
			if (event->repeatCount != TIMING_INFINITE_REPEAT) {
				--(event->repeatCount);
			}
		
			// Re-insert event if there are repetitions left.
			if (event->repeatCount > 0) {
				event->triggerTime += event->period;
				TIMING_AddEvent( event );
			}
		}
	}

	// Restore status register value, including Global Interrupt Flag.
	SREG = savedSREG;
}


/*!
 *  This function searches through the event queue and removes
 *  the event if it finds it. If it is not in the queue, this function
 *  leaves the queue untouched.
 *
 *  \param  event  Pointer to event control block to remove.
 */
void TIMING_RemoveEvent( TIMING_event_t const * event )
{
	// Wait need to operate on the queue undisturbed.
	uint8_t const savedSREG = SREG;
	CAL_disable_interrupt();

	// Set up event queue iteration variables.
	TIMING_queueSize_t eventsLeft = TIMING_queueSize;
	TIMING_event_t * currentEvent = TIMING_firstEvent;
	TIMING_event_t * lastEvent = 0;
	bool eventFound = false;
	
	// Search for matching event pointer.
	while ((eventsLeft > 0) && (eventFound == false)) {
		if (currentEvent == event) {
			eventFound = true;
		} else {
			lastEvent = currentEvent;
			currentEvent = currentEvent->next;
			--eventsLeft;		
		}
	}
	
	// If eventsLeft is untouched, we haven't iterated in the queue, and we
	// need to remove the very first event in the queue. Otherwise, remove
	// from within the queue. If the event was found, that is.
	if (eventFound == true) {
		if (eventsLeft == TIMING_queueSize) {
			TIMING_firstEvent = currentEvent->next;
		} else {
			lastEvent->next = currentEvent->next;
		}
		--TIMING_queueSize;
	}

	// Restore status register value, including Global Interrupt Flag.
	SREG = savedSREG;
}


/*!
 *  This function allocates and adds a local event to be fired
 *  after a delay, and waits for it. This function will not return
 *  until the "delay" amount of ticks has occurred. Therefore the user
 *  must make sure that the tick handler gets called by an interrupt handler
 *  in order to make this function work.
 *
 *  \param  delay  Delay to wait, given in ticks.
 */
void TIMING_WaitFor( TIMING_time_t delay )
{
	// Allocate counter and event on local stack, add event and wait for counter update.
	TIMING_counter_t volatile counter = 0;
	TIMING_event_t event;
	TIMING_AddCounterEventAfter( delay, &counter, &event );
	do {} while (counter == 0);
}


/*!
 *  This function allocates and adds a local event to be fired
 *  a given timestamp, and waits for it. This function will not return
 *  until the current time equals the timestamp. Therefore the user
 *  must make sure that the tick handler gets called by an interrupt handler
 *  in order to make this function work.
 *
 *  \param  timestamp  The timestamp at which the event should fire.
 */
void TIMING_WaitUntil( TIMING_time_t timestamp )
{
	// Allocate counter and event on local stack, add event and wait for counter update.
	TIMING_counter_t volatile counter = 0;
	TIMING_event_t event;
	TIMING_AddCounterEventAt( timestamp, &counter, &event );
	do {} while (counter == 0);
}


/*!
 *  Note that the timestamp value wraps back to zero when it reaches
 *  the maximum value for the TIMING_time_t datatype. Therefore, take care
 *  when comparing timestamps.
 */
TIMING_time_t TIMING_GetTime( void )
{
	return TIMING_currentTime;
}


/*!
 *  This function registers a one-shot event to trigger at a given timestamp.
 *  The caller must supply a pointer to an event control block pointing to
 *  a valid block of memory. The user must also make sure that the memory
 *  block is valid as long as the event is active.
 *
 *  \param  timestamp   Timestamp to trigger event.
 *  \param  counterPtr  Pointer to counter variable to increment when event triggers.
 *  \param  event       Pointer to unintialized event control block.
 */
void TIMING_AddCounterEventAt( TIMING_time_t timestamp, TIMING_counter_t volatile * counterPtr, TIMING_event_t * event )
{
	TIMING_AddRepCounterEventAt( timestamp, 1, 0, counterPtr, event );
}


/*!
 *  This function registers a one-shot event to trigger after a given delay.
 *  If the delay value is zero, the event will trigger when the current time
 *  wraps back to zero and reaches its current value again.
 *  The caller must supply a pointer to an event control block pointing to
 *  a valid block of memory. The user must also make sure that the memory
 *  block is valid as long as the event is active.
 *
 *  \param  delay       Delay in ticks until event triggers.
 *  \param  counterPtr  Pointer to counter variable to increment when event triggers.
 *  \param  event       Pointer to unintialized event control block.
 */
void TIMING_AddCounterEventAfter( TIMING_time_t delay, TIMING_counter_t volatile * counterPtr, TIMING_event_t * event )
{
	TIMING_AddCounterEventAt( TIMING_currentTime + delay, counterPtr, event );
}


/*!
 *  This function registers a one-shot event to trigger at a given timestamp.
 *  The caller must supply a pointer to an event control block pointing to
 *  a valid block of memory. The user must also make sure that the memory
 *  block is valid as long as the event is active.
 *
 *  \param  timestamp  Timestamp to trigger event.
 *  \param  Callback   Pointer to function to be called when event triggers.
 *  \param  event      Pointer to unintialized event control block.
 */
void TIMING_AddCallbackEventAt( TIMING_time_t timestamp, TIMING_Callback_t Callback, TIMING_event_t * event )
{
	TIMING_AddRepCallbackEventAt( timestamp, 1, 0, Callback, event );
}


/*!
 *  This function registers a one-shot event to trigger after a given delay.
 *  If the delay value is zero, the event will trigger when the current time
 *  wraps back to zero and reaches its current value again.
 *  The caller must supply a pointer to an event control block pointing to
 *  a valid block of memory. The user must also make sure that the memory
 *  block is valid as long as the event is active.
 *
 *  \param  delay     Delay in ticks until event triggers.
 *  \param  Callback  Pointer to function to be called when event triggers.
 *  \param  event     Pointer to unintialized event control block.
 */
void TIMING_AddCallbackEventAfter( TIMING_time_t delay, TIMING_Callback_t Callback, TIMING_event_t * event )
{
	TIMING_AddCallbackEventAt( TIMING_currentTime + delay, Callback, event );
}


/*!
 *  This function registers a periodic event. First trigger time is after
 *  one period time has passed. If repeat count is zero, no event will trigger.
 *  If repeat count is TIMING_INFINITE_REPEAT, it will repeat until manually removed.
 *  The caller must supply a pointer to an event control block pointing to
 *  a valid block of memory. The user must also make sure that the memory
 *  block is valid as long as the event is active.
 *
 *  \param  repeatCount  Number of repetitions, possibly zero or infinite.
 *  \param  period       Event period in number of ticks.
 *  \param  counterPtr   Pointer to counter variable to increment when event triggers.
 *  \param  event        Pointer to unintialized event control block.
 */
void TIMING_AddRepCounterEvent( TIMING_repeatCount_t repeatCount, TIMING_time_t period, TIMING_counter_t volatile * counterPtr, TIMING_event_t * event )
{
	TIMING_AddRepCounterEventAfter( period, repeatCount, period, counterPtr, event );
}


/*!
 *  This function registers a periodic event. First trigger time is after
 *  one period time has passed. If repeat count is zero, no event will trigger.
 *  If repeat count is TIMING_INFINITE_REPEAT, it will repeat until manually removed.
 *  The caller must supply a pointer to an event control block pointing to
 *  a valid block of memory. The user must also make sure that the memory
 *  block is valid as long as the event is active.
 *
 *  \param  repeatCount  Number of repetitions, possibly zero or infinite.
 *  \param  period       Event period in number of ticks.
 *  \param  Callback     Pointer to function to be called when event triggers.
 *  \param  event        Pointer to unintialized event control block.
 */
void TIMING_AddRepCallbackEvent( TIMING_repeatCount_t repeatCount, TIMING_time_t period, TIMING_Callback_t Callback, TIMING_event_t * event )
{
	TIMING_AddRepCallbackEventAfter( period, repeatCount, period, Callback, event );
}


/*!
 *  This function registers a periodic event. First trigger time is at a time given
 *  by "firstTimestamp", then every "period" ticks.
 *  If repeat count is zero, no event will trigger.
 *  If repeat count is TIMING_INFINITE_REPEAT, it will repeat until manually removed.
 *  The caller must supply a pointer to an event control block pointing to
 *  a valid block of memory. The user must also make sure that the memory
 *  block is valid as long as the event is active.
 *
 *  \param  firstTimestamp  Timestamp to trigger first event.
 *  \param  repeatCount     Number of repetitions, possibly zero or infinite.
 *  \param  period          Event period in number of ticks.
 *  \param  counterPtr      Pointer to counter variable to increment when event triggers.
 *  \param  event           Pointer to unintialized event control block.
 */
void TIMING_AddRepCounterEventAt( TIMING_time_t firstTimestamp, TIMING_repeatCount_t repeatCount, TIMING_time_t period, TIMING_counter_t volatile * counterPtr, TIMING_event_t * event )
{
	// Repeat count 0 is interpreted "don't run"
	if (repeatCount > 0) {
		// Prepare event structure and add it to queue.
		event->triggerTime = firstTimestamp;
		event->period = period;
		event->repeatCount = repeatCount;
		event->type = TIMING_COUNTER_EVENT;
		event->counterPtr = counterPtr;
		TIMING_AddEvent( event );
	}
}


/*!
 *  This function registers a periodic event. First trigger time is after given delay,
 *  then every "period" ticks.
 *  If repeat count is zero, no event will trigger.
 *  If repeat count is TIMING_INFINITE_REPEAT, it will repeat until manually removed.
 *  The caller must supply a pointer to an event control block pointing to
 *  a valid block of memory. The user must also make sure that the memory
 *  block is valid as long as the event is active.
 *
 *  \param  firstDelay   Delay in ticks until first event triggers.
 *  \param  repeatCount  Number of repetitions, possibly zero or infinite.
 *  \param  period       Event period in number of ticks.
 *  \param  counterPtr   Pointer to counter variable to increment when event triggers.
 *  \param  event        Pointer to unintialized event control block.
 */
void TIMING_AddRepCounterEventAfter( TIMING_time_t firstDelay, TIMING_repeatCount_t repeatCount, TIMING_time_t period, TIMING_counter_t volatile * counterPtr, TIMING_event_t * event )
{
	TIMING_AddRepCounterEventAt( TIMING_currentTime + firstDelay, repeatCount, period, counterPtr, event );
}


/*!
 *  This function registers a periodic event. First trigger time is at a time given
 *  by "firstTimestamp", then every "period" ticks.
 *  If repeat count is zero, no event will trigger.
 *  If repeat count is TIMING_INFINITE_REPEAT, it will repeat until manually removed.
 *  The caller must supply a pointer to an event control block pointing to
 *  a valid block of memory. The user must also make sure that the memory
 *  block is valid as long as the event is active.
 *
 *  \param  firstTimestamp  Timestamp to trigger first event.
 *  \param  repeatCount     Number of repetitions, possibly zero or infinite.
 *  \param  period          Event period in number of ticks.
 *  \param  Callback        Pointer to function to be called when event triggers.
 *  \param  event           Pointer to unintialized event control block.
 */
void TIMING_AddRepCallbackEventAt( TIMING_time_t firstTimestamp, TIMING_repeatCount_t repeatCount, TIMING_time_t period, TIMING_Callback_t Callback, TIMING_event_t * event )
{
	// Repeat count 0 is interpreted "don't run"
	if (repeatCount > 0) {
		// Prepare event structure and add it to queue.
		event->triggerTime = firstTimestamp;
		event->period = period;
		event->repeatCount = repeatCount;
		event->type = TIMING_CALLBACK_EVENT;
		event->Callback = Callback;
		TIMING_AddEvent( event );
	}
}


/*!
 *  This function registers a periodic event. First trigger time is after given delay,
 *  then every "period" ticks.
 *  If repeat count is zero, no event will trigger.
 *  If repeat count is TIMING_INFINITE_REPEAT, it will repeat until manually removed.
 *  The caller must supply a pointer to an event control block pointing to
 *  a valid block of memory. The user must also make sure that the memory
 *  block is valid as long as the event is active.
 *
 *  \param  firstDelay   Delay in ticks until first event triggers.
 *  \param  repeatCount  Number of repetitions, possibly zero or infinite.
 *  \param  period       Event period in number of ticks.
 *  \param  Callback     Pointer to function to be called when event triggers.
 *  \param  event        Pointer to unintialized event control block.
 */
void TIMING_AddRepCallbackEventAfter( TIMING_time_t firstDelay, TIMING_repeatCount_t repeatCount, TIMING_time_t period, TIMING_Callback_t Callback, TIMING_event_t * event )
{
	TIMING_AddRepCallbackEventAt( TIMING_currentTime + firstDelay, repeatCount, period, Callback, event );
}


/*******************************
 * Internal function prototypes
 *******************************/

/*!
 * This function adds an event into the timing queue.
 *
 * \param  event  Pointer to initialized event.
 */
static void TIMING_AddEvent( TIMING_event_t * event )
{
	// Wait need to operate on the queue undisturbed.
	uint8_t const savedSREG = SREG;
	CAL_disable_interrupt();
	
	// Set up event queue iteration variables.
	TIMING_queueSize_t eventsLeft = TIMING_queueSize;
	TIMING_event_t * currentEvent = TIMING_firstEvent;
	TIMING_event_t * lastEvent = 0;
	
	// If event timestamp is less than or equal to current timestamp, it means
	// that the time variable needs to wrap to zero before this event will trigger.
	// Therefore we need to iterate through the queue until we find the first
	// event that also has trigger time less than the current timestamp, and
	// then start to compare.
	if (event->triggerTime <= TIMING_currentTime) {
		///mt 20080301 while ((eventsLeft > 0) && (currentEvent->triggerTime >= TIMING_currentTime)) {
		while ((currentEvent->triggerTime >= TIMING_currentTime) && (eventsLeft > 0)) {
			lastEvent = currentEvent;
			currentEvent = currentEvent->next;
			--eventsLeft;
		}
	}
	
	// Now, find the first event that has trigger time equal or larger than the
	// one we are about to add to the queue.
	while ((eventsLeft > 0) && (currentEvent->triggerTime < event->triggerTime)) {
		lastEvent = currentEvent;
		currentEvent = currentEvent->next;
		--eventsLeft;
	}
	
	// If eventsLeft is untouched, we haven't iterated in the queue, and we
	// need to insert our event at the head of the queue and correct the
	// TIMING_nextEventTime variable. If not, we insert in between
	// two other events.
	if (eventsLeft == TIMING_queueSize) {
		event->next = TIMING_firstEvent;
		TIMING_firstEvent = event;
		TIMING_nextEventTime = event->triggerTime;
	} else {
		event->next = currentEvent;
		lastEvent->next = event;
	}
	++TIMING_queueSize;
		
	// Restore status register value, including Global Interrupt Flag.
	SREG = savedSREG;
}


/*!
 * This function looks at the event type and performs action.
 *
 * \param  event  Pointer to event.
 */
static void TIMING_DoEvent( TIMING_event_t const * event )
{
	switch (event->type) {
		case TIMING_COUNTER_EVENT :
			++(*(event->counterPtr)); // Increment variable pointed to by counterPtr.
		break;

		case TIMING_CALLBACK_EVENT :
			event->Callback();  // Call callback function.
		break;
		
		default :
			// Ignore unsupported event types. Should never happen.
		break;
	}
}


// end of file
