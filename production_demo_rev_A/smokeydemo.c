/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 smokey demo application.
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
 * $Revision: 2347 $
 * $Date: 2007-09-11 00:18:39 -0600 (ti, 11 sep 2007) $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/production_demo_rev_A/smokeydemo.c $
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

#include <lcd_lib.h>
#include <joystick_driver.h>
#include <memblock_lib.h>
#include <backlight_driver.h>
#include <termfont_lib.h>
#include <stdint.h>
#include <popup_lib.h>


#define SMOKEYDEMO_BOXCOUNT 10
#define SMOKEYDEMO_TEXTPAGE1 1
#define SMOKEYDEMO_TEXTPAGE2 6
#define SMOKEYDEMO_TEXTDELAY 12
#define SMOKEYDEMO_DELAYMS 5


char const CAL_PGM_DEF(SmokeyDemo_DemoText1[]) = "-- ATMEL AVR  DB101 ";
char const CAL_PGM_DEF(SmokeyDemo_DemoText2[]) = "Smokey Graphics Demo";

void SmokeyDemo( void )
{
	POPUP_MsgBox( 14, 1, 6,
	              "ENTER to exit.", NULL );
	LCD_SetScreen( 0x00 );

	// Allocate some workspace.
	uint8_t * frameBuffer = MEM_ALLOC_ARRAY( uint8_t, LCD_WIDTH * LCD_HEIGHT / 8 );
	
	if (frameBuffer == NULL) {
		POPUP_MsgBox( 10, 2, 6, "Not enough\r\nmemory!", NULL );
	} else {
		// Start coord, deltas and old-coord buffers for all boxes.
		int16_t x[SMOKEYDEMO_BOXCOUNT] = { 10, 90, 30, 120, 45, 80, 35, 5, 110, 50 };
		int16_t y[SMOKEYDEMO_BOXCOUNT] = { 25, 56, 42, 50, 20, 40, 10, 55, 60, 35 };
		int16_t dx[SMOKEYDEMO_BOXCOUNT] = { 1, -2, -1, 2, 1, -1, 1, 2, -1, 1 };
		int16_t dy[SMOKEYDEMO_BOXCOUNT] = { -1, 1, 2, -1, -2, 1, 2, 1, -2, 1 };
		int16_t oldx[SMOKEYDEMO_BOXCOUNT];
		int16_t oldy[SMOKEYDEMO_BOXCOUNT];
		
		// Prepare buffer with demo texts.
		LCD_SetBuffer( frameBuffer, 0x00 );
		TERMFONT_XORBufferString_F( frameBuffer, SmokeyDemo_DemoText1, SMOKEYDEMO_TEXTPAGE1, 0 );
		TERMFONT_XORBufferString_F( frameBuffer, SmokeyDemo_DemoText2, SMOKEYDEMO_TEXTPAGE2, 0 );		
		LCD_WriteFrameBuffer( frameBuffer );
	
		uint8_t scroll_countdown = SMOKEYDEMO_TEXTDELAY;
		do {
			// Draw boxes using XOR operations and dump everything to screen.
			for (uint8_t i = 0; i < SMOKEYDEMO_BOXCOUNT; i += 2) {
				LCD_FlipBufferBox( frameBuffer, x[i+0], y[i+0], x[i+1], y[i+1] );
			}
			LCD_WriteFrameBuffer( frameBuffer );

			// Play around with backlight.
			BACKLIGHT_SetRGB( x[3]*2, x[5]*2, y[2]*4 );
			
			// Store old box coords and move corners, bouncing off walls if required.
			for (uint8_t i = 0; i < SMOKEYDEMO_BOXCOUNT; ++i) {
				oldx[i] = x[i];
				x[i] += dx[i];
				if ((x[i] < 0) || (x[i] >= LCD_WIDTH)) {
					dx[i] = -dx[i];
					x[i] += dx[i];
				}
				oldy[i] = y[i];
				y[i] += dy[i];
				if ((y[i] < 0) || (y[i] >= LCD_HEIGHT)) {
					dy[i] = -dy[i];
					y[i] += dy[i];
				}
			}

			// Just to make things look better.
			DELAY_MS( SMOKEYDEMO_DELAYMS );
			
			// Remove all boxes again, by XORing them away.
			for (uint8_t i = 0; i < SMOKEYDEMO_BOXCOUNT; i += 2) {
				LCD_FlipBufferBox( frameBuffer, oldx[i+0], oldy[i+0], oldx[i+1], oldy[i+1] );
			}
			
			
			// Scroll text lines if countdown is zero, decrement if not.
			if (scroll_countdown == 0) {
				// Store last byte of page.
				uint8_t * ptr1 = frameBuffer + (LCD_WIDTH * SMOKEYDEMO_TEXTPAGE1) + (LCD_WIDTH - 1);
				uint8_t temp1 = *ptr1;
				// Copy every byte but the first.
				for (uint8_t i = 0; i < 127; ++i) {
					*ptr1 = *(ptr1 - 1);
					--ptr1;
				}
				// Copy last byte.
				*ptr1 = temp1;
	
				// Store first byte of page.
				uint8_t * ptr2 = frameBuffer + (LCD_WIDTH * SMOKEYDEMO_TEXTPAGE2) + 0;
				uint8_t temp2 = *ptr2;
				// Copy every byte but the last.
				for (uint8_t i = 0; i < 127; ++i) {
					*ptr2 = *(ptr2 + 1);
					++ptr2;
				}
				// Copy last byte.
				*ptr2 = temp2;
				
				// Reset scroll countdown.
				scroll_countdown = SMOKEYDEMO_TEXTDELAY;
			} else {
				// Decrement scroll countdown.
				--scroll_countdown;
			}
		} while (!JOYSTICK_IsEnterPressed());
		
		// Clean up.
		MEM_FREE( frameBuffer );
		BACKLIGHT_SetRGB( 0, 0, 0 );
	}
}
