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


static char const CAL_PGM_DEF(Clock_Text1[]) = "W/Ah";
static char const CAL_PGM_DEF(Clock_Text2[]) = "Clock Demo";


void DisplayData(void)
{
	
	POPUP_MsgBox( 16, 3, 6,
	              "UP/DOWN = change"
	              "parameter.       "
	              "Sideways = exit  ", NULL );
	LCD_SetScreen( 0x00 );
	
	// Draw some persistent graphics.
//	LCD_SetCircle( LCD_HEIGHT/2, LCD_HEIGHT/2, 31 );
//	TERMFONT_DisplayString_F( Clock_Text1, CLOCK_TEXT1PAGE, (LCD_WIDTH / 2) + CLOCK_TEXT1OFFSET );
//	TERMFONT_DisplayString_F( Clock_Text2, CLOCK_TEXT2PAGE, (LCD_WIDTH / 2) + CLOCK_TEXT1OFFSET );
	
	//LCD_DrawProgressBar(uint8_t Xstart, uint8_t Ystart, uint8_t height, uint8_t lenght, uint8_t maxvalue, uint8_t currvalue, uint8_t peakvalue)
	
	TERMFONT_DisplayString_F( Clock_Text1, 1, 2);

	LCD_DrawProgressBar(5, 40, 20, LCD_WIDTH-5-5, 100, 75, 30);
	
	while ((JOYSTICK_GetState() & (JOYSTICK_LEFT | JOYSTICK_RIGHT)) == 0x00) {
		POWER_EnterIdleSleepMode();
	}

}
