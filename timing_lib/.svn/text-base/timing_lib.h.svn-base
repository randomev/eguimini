// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  Generic event timing library header file.
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
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/timing_lib/timing_lib.h $
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
#ifndef TIMING_LIB_H
#define TIMING_LIB_H

#include <stdint.h>



/************************
 * Constants and defines
 ************************/

//! Regardless of data type, this will result in a value with all '1'-bits.
#define TIMING_INFINITE_REPEAT ((TIMING_repeatCount_t) (-1))



/*********************
 * Types and typedefs
 *********************/

//! Timestamp type given in ticks. Used for timestamps and delay values.
typedef uint32_t TIMING_time_t;
//! Event repeat count type. Used for periodic events.
typedef uint8_t TIMING_repeatCount_t;
//! Event counter type. Used by events that increment counter variables.
typedef uint8_t TIMING_counter_t;
//! Event handler callback type. Is called when timing events occur.
typedef void (* TIMING_Callback_t)( void );

//! Available event types. Do not use directly. Initialization functions are provided.
typedef enum TIMING_eventType_enum
{
	TIMING_COUNTER_EVENT,
	TIMING_CALLBACK_EVENT
} TIMING_eventType_t;

//! Timing event control block. Do not use directly. Initialization functions are provided.
typedef struct TIMING_event_struct
{
	TIMING_time_t triggerTime;  //!< When to trigger this event.
	TIMING_time_t period;  //!< Period for periodic events.
	TIMING_repeatCount_t repeatCount;  //!< Repetitions left before removing from queue. -1 = infinite.
	
	TIMING_eventType_t type;  //!< This variable decides the interpretation of the anonymous union.
	union {
		TIMING_counter_t volatile * counterPtr;  //!< Pointer to counter to increment for TIMING_COUNTER_EVENT type.
		TIMING_Callback_t Callback;  //!< Callback function for TIMING_CALLBACK_EVENT type.
	};
	
	struct TIMING_event_struct * next;  //!< Next node in linked list of events.	
} TIMING_event_t;



/**********************
 * Function prototypes
 **********************/

//! Initialize timing library.
void TIMING_Init( void );
//! The tick handler to be called from the timing source.
void TIMING_TickHandler( void );
//! Remove an event from the queue if found.
void TIMING_RemoveEvent( TIMING_event_t const * event );
//! Shortcut to wait for a certain amount of ticks.
void TIMING_WaitFor( TIMING_time_t delay );
//! Shortcut to wait until current time matches the given timestamp.
void TIMING_WaitUntil( TIMING_time_t timestamp );
//! Return current time in number of ticks.
TIMING_time_t TIMING_GetTime( void );
//! Register counter event at given time.
void TIMING_AddCounterEventAt( TIMING_time_t timestamp, TIMING_counter_t volatile * counterPtr, TIMING_event_t * event );
//! Register counter event after a given delay.
void TIMING_AddCounterEventAfter( TIMING_time_t delay, TIMING_counter_t volatile * counterPtr, TIMING_event_t * event );
//! Register callback event at given time.
void TIMING_AddCallbackEventAt( TIMING_time_t timestamp, TIMING_Callback_t Callback, TIMING_event_t * event );
//! Register callback event after a given delay.
void TIMING_AddCallbackEventAfter( TIMING_time_t delay, TIMING_Callback_t Callback, TIMING_event_t * event );
//! Register periodic counter event.
void TIMING_AddRepCounterEvent( TIMING_repeatCount_t repeatCount, TIMING_time_t period, TIMING_counter_t volatile * counterPtr, TIMING_event_t * event );
//! Register periodic callback event.
void TIMING_AddRepCallbackEvent( TIMING_repeatCount_t repeatCount, TIMING_time_t period, TIMING_Callback_t Callback, TIMING_event_t * event );
//! Register periodic counter event with first event at given time.
void TIMING_AddRepCounterEventAt( TIMING_time_t firstTimestamp, TIMING_repeatCount_t repeatCount, TIMING_time_t period, TIMING_counter_t volatile * counterPtr, TIMING_event_t * event );
//! Register periodic counter event with first event after given delay.
void TIMING_AddRepCounterEventAfter( TIMING_time_t firstDelay, TIMING_repeatCount_t repeatCount, TIMING_time_t period, TIMING_counter_t volatile * counterPtr, TIMING_event_t * event );
//! Register periodic callback event with first event at given time.
void TIMING_AddRepCallbackEventAt( TIMING_time_t firstTimestamp, TIMING_repeatCount_t repeatCount, TIMING_time_t period, TIMING_Callback_t Callback, TIMING_event_t * event );
//! Register periodic callback event with first event after given delay.
void TIMING_AddRepCallbackEventAfter( TIMING_time_t firstDelay, TIMING_repeatCount_t repeatCount, TIMING_time_t period, TIMING_Callback_t Callback, TIMING_event_t * event );


#endif
// end of file
