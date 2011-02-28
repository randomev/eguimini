// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  Real time clock driver.
 *
 *      The real-time clock driver provides an interface to the asynchronous
 *      timer running off an external 32.768 kHz watch crystal. The driver keeps
 *      track of seconds, minutes, hours, and days. Also, one second is divided
 *      into a number of ticks that corresponds to the rate at which the
 *      asynchronous timer interrupt is called. The default setting is to
 *      configure the timer to overflow every 1/128th second, which is a
 *      suitable tick rate for the polling handler of the Joystick Driver.
 *
 *      In order to make use of the tick for other purposes than just keeping
 *      track of time, the user can register a tick handler, which is a function
 *      that is called after every tick. In the DB101 firmware, the main
 *      processing routine of the Timing Library is registered as the tick
 *      handler of the real-time clock driver. The timing library then takes
 *      care of called the polling routine of the joystick driver.
 *
 *      It is also possible to register a day handler with the driver. The day
 *      handler is called every time the day counter is incremented, and can be
 *      used to implement higher-level calendar functionality.
 *
 *      When migrating between devices, the user might need to reconfigure the RTC
 *      intialization. The timer prescaler settings might need to be changed in
 *      order to achieve the expected tick rate. The settings can be found inside
 *      RTC_Init().
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
 * $Revision: 2406 $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/rtc_driver/rtc_driver.h $
 * $Date: 2007-09-17 01:07:49 -0600 (ma, 17 sep 2007) $  \n
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

#ifndef RTC_DRIVER_H
#define RTC_DRIVER_H

#include <stdint.h>



/*************************
 * Constants and defines.
 *************************/

#define RTC_HOURS_PER_DAY 24
#define RTC_MINUTES_PER_HOUR 60
#define RTC_SECONDS_PER_MINUTE 60
#define RTC_TICKS_PER_SECOND 128  //!< Change this if RTC prescaling changes.



/**********************
 * Types and typedefs.
 **********************/

typedef uint8_t RTC_tick_t; //!< Datatype for counting ticks.
typedef uint16_t RTC_day_t; //!< Datatype for counting days.
typedef void (* RTC_TickHandler_t)( void );  //!< Data type for callback for tick handling.
typedef void (* RTC_DayHandler_t)( void );  //!< Data type for callback for day handling.



/***********************
 * Function prototypes.
 ***********************/

void RTC_Init( void ); //!< Initialize real time clock library.
void RTC_SetTickHandler( RTC_TickHandler_t TickHandler ); //!< Register a tick handler callback.
RTC_TickHandler_t RTC_GetTickHandler( void ); //!< Get current tick handler.
void RTC_RemoveTickHandler( void ); //!< Unregister tick handler callback if present.
void RTC_SetDayHandler( RTC_DayHandler_t DayHandler ); //!< Register a day handler callback.
void RTC_RemoveDayHandler( void ); //!< Unregister day handler callback if present.

//! Set hours, minutes and seconds without touching days and weeks.
void RTC_SetTimeOfDay( uint8_t hours, uint8_t minutes, uint8_t seconds );
//! Copy hours, minutes and seconds into variables pointed to.
void RTC_GetTimeOfDay( uint8_t * hours, uint8_t * minutes, uint8_t * seconds );
//! Set day number without touching other time values.
void RTC_SetDay( RTC_day_t day );
//! Get day number.
RTC_day_t RTC_GetDay( void );
//! Set hour number without touching other time values.
static void RTC_SetHour( uint8_t hour );
//! Set minute number without touching other time values.
static void RTC_SetMinute( uint8_t minute );
//! Set second number and reset tick count, without touching other time values.
void RTC_SetSecond( uint8_t second );
//! Get hour number.
static uint8_t RTC_GetHour( void );
//! Get minute number.
static uint8_t RTC_GetMinute( void );
//! Get second number.
static uint8_t RTC_GetSecond( void );



/*******************************
 * Private types and variables.
 *******************************/

/*
 * In order to stay compatible with future versions
 * of this library, DO NOT ACCESS THESE VARIABLES DIRECTLY.
 * Use the provided access functions instead.
 */

extern volatile uint8_t    RTC_days;     //!< Holds day count.
extern volatile uint8_t    RTC_hours;    //!< Holds hour of day.
extern volatile uint8_t    RTC_minutes;  //!< Holds minute of hour.
extern volatile uint8_t    RTC_seconds;  //!< Holds second of minute.
extern volatile RTC_tick_t RTC_ticks;    //!< Holds tick count. Tick granularity depends on prescaler setting.



/*
 * Short access functions with static linkage.
 * These functions are defined in this header file
 * in order to help to compiler optimize and inline the
 * code, since the functions are short one-liners only.
 */

static inline void RTC_SetHour( uint8_t hour )
{
	RTC_hours = hour;
}


static inline void RTC_SetMinute( uint8_t minute )
{
	RTC_minutes = minute;
}


static inline uint8_t RTC_GetHour( void )
{
	return RTC_hours;
}


static inline uint8_t RTC_GetMinute( void )
{
	return RTC_minutes;
}


static inline uint8_t RTC_GetSecond( void )
{
	return RTC_seconds;
}



#endif
