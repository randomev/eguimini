/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 clock demo application.
 *
 *         Prints both an analog and digital clock on the display.
 *
 *         Joystick up changes second, down changes minute and enter
 *         changes hour. Pressing sideways quits the program.
 *
 *         Display is updated every fourth second.
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
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/production_demo_rev_A/clock.c $
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

#include <math.h>
#include <stdint.h>

#include <cal.h>
#include <common.h>

#include <lcd_lib.h>
#include <gfx_lib.h>
#include <joystick_driver.h>
#include <termfont_lib.h>
#include <rtc_driver.h>
#include <timing_lib.h>
#include <power_driver.h>
#include <popup_lib.h>


#define CLOCK_DIGITSPAGE 4
#define CLOCK_DIGITSOFFSET TERMFONT_CHAR_WIDTH
#define CLOCK_TEXT1PAGE 1
#define CLOCK_TEXT1OFFSET 0
#define CLOCK_TEXT2PAGE 2
#define CLOCK_TEXT2OFFSET 0


static char const CAL_PGM_DEF(Clock_Text1[]) = "DB101";
static char const CAL_PGM_DEF(Clock_Text2[]) = "Clock Demo";



static void Clock_ComputeDial( float angle, float iRadius, float oRadius, uint8_t * ixp, uint8_t * iyp, uint8_t * oxp, uint8_t * oyp )
{
	float radians = angle * 3.141592654 / 180;
	float ix = (LCD_HEIGHT / 2)  + sin( radians ) * iRadius;
	float iy = (LCD_HEIGHT / 2) - cos( radians ) * iRadius;
	float ox = (LCD_HEIGHT / 2)  + sin( radians ) * oRadius;
	float oy = (LCD_HEIGHT / 2) - cos( radians ) * oRadius;
	*ixp = ix;
	*iyp = iy;
	*oxp = ox;
	*oyp = oy;
}


static void Clock_UpdateAnalogSecond( uint8_t second )
{
	static uint8_t oldix = 0;
	static uint8_t oldiy = 0;
	static uint8_t oldox = 0;
	static uint8_t oldoy = 0;
	
	uint8_t newix;
	uint8_t newiy;
	uint8_t newox;
	uint8_t newoy;

	Clock_ComputeDial( second * 360 / 60, 0, 25, &newix, &newiy, &newox, &newoy );

	LCD_ClrLine( oldix, oldiy, oldox, oldoy );
	LCD_SetLine( newix, newiy, newox, newoy );

	oldix = newix;
	oldiy = newiy;
	oldox = newox;
	oldoy = newoy;
}	


static void Clock_UpdateAnalogMinute( float minute, float second )
{
	static uint8_t oldix = 0;
	static uint8_t oldiy = 0;
	static uint8_t oldox = 0;
	static uint8_t oldoy = 0;
	
	uint8_t newix;
	uint8_t newiy;
	uint8_t newox;
	uint8_t newoy;

	Clock_ComputeDial( (minute * 360 / 60) + (second * (360 / 60) / 60), 0, 20, &newix, &newiy, &newox, &newoy );

	LCD_ClrLine( oldix, oldiy, oldox, oldoy );
	LCD_SetLine( newix, newiy, newox, newoy );

	oldix = newix;
	oldiy = newiy;
	oldox = newox;
	oldoy = newoy;
}


static void Clock_UpdateAnalogHour( float hour, float minute )
{
	static uint8_t oldix = 0;
	static uint8_t oldiy = 0;
	static uint8_t oldox = 0;
	static uint8_t oldoy = 0;
	
	uint8_t newix;
	uint8_t newiy;
	uint8_t newox;
	uint8_t newoy;

	Clock_ComputeDial( (hour * 360 / 12) + (minute * (360 / 12) / 60), 0, 15, &newix, &newiy, &newox, &newoy );

	LCD_ClrLine( oldix, oldiy, oldox, oldoy );
	LCD_SetLine( newix, newiy, newox, newoy );

	oldix = newix;
	oldiy = newiy;
	oldox = newox;
	oldoy = newoy;
}


static void Clock_ProcessAdjust( void )
{
	// Read time.
	uint8_t hour, minute, second;
	RTC_GetTimeOfDay( &hour, &minute, &second );
	
	// Handle adjustment.
	if (JOYSTICK_IsUpPressed()) {
		if (second < (RTC_SECONDS_PER_MINUTE - 1)) {
			++second;
		} else {
			second = 0;
		}
	}
	if (JOYSTICK_IsDownPressed()) {
		if (minute < (RTC_MINUTES_PER_HOUR - 1)) {
			++minute;
		} else {
			minute = 0;
		}
	}
	if (JOYSTICK_IsEnterPressed()) {
		if (hour < (RTC_HOURS_PER_DAY - 1)) {
			++hour;
		} else {
			hour = 0;
		}
	}
	
	// Write back time.
	RTC_SetTimeOfDay( hour, minute, second );
}


static void Clock_UpdateDigital( uint8_t hour, uint8_t minute, uint8_t second )
{
	char digital[] = "00:00:00";
	
	digital[0] = '0' + (hour / 10);
	digital[1] = '0' + (hour % 10);
	digital[3] = '0' + (minute / 10);
	digital[4] = '0' + (minute % 10);
	digital[6] = '0' + (second / 10);
	digital[7] = '0' + (second % 10);
	
	TERMFONT_DisplayString( digital, CLOCK_DIGITSPAGE, (LCD_WIDTH / 2) + CLOCK_DIGITSOFFSET );
}


static void Clock_Update( void )
{
	// Read time.
	uint8_t hour, minute, second;
	RTC_GetTimeOfDay( &hour, &minute, &second );

	// Update analog dials.
	Clock_UpdateAnalogSecond( second );
	Clock_UpdateAnalogMinute( minute, second );
	Clock_UpdateAnalogHour( hour, minute );
	
	// Update digital part.
	Clock_UpdateDigital( hour, minute, second );
		
	// Process joystick.
	Clock_ProcessAdjust();
}

void Clock( void )
{
	POPUP_MsgBox( 16, 3, 6,
	              "UP/ENTER/DOWN to"
	              "adjust time.    "
	              "Sideways to exit", NULL );
	LCD_SetScreen( 0x00 );
	
	// Draw some persistent graphics.
	LCD_SetCircle( LCD_HEIGHT/2, LCD_HEIGHT/2, 31 );
	TERMFONT_DisplayString_F( Clock_Text1, CLOCK_TEXT1PAGE, (LCD_WIDTH / 2) + CLOCK_TEXT1OFFSET );
	TERMFONT_DisplayString_F( Clock_Text2, CLOCK_TEXT2PAGE, (LCD_WIDTH / 2) + CLOCK_TEXT1OFFSET );
	
	// Install event handler to fire every fourth second, in order to allow for rapid adjustments.
	TIMING_event_t secondEvent;
	TIMING_AddRepCallbackEvent( TIMING_INFINITE_REPEAT, RTC_TICKS_PER_SECOND/4, Clock_Update, &secondEvent );
	while ((JOYSTICK_GetState() & (JOYSTICK_LEFT | JOYSTICK_RIGHT)) == 0x00) {
		POWER_EnterIdleSleepMode();
	}
	TIMING_RemoveEvent( &secondEvent );
}
