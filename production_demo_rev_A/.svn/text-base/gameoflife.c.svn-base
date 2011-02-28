/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 Game of Life demo application.
 *
 *         This application runs Game of Life until enter is pressed.
 *
 *         You can read about it at http://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
 *         But in short it is "a cellular automaton devised by the British mathematician 
 *         John Horton Conway in 1970. It is the best-known example of a cellular automaton."
 *
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
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/production_demo_rev_A/gameoflife.c $
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


#include <lcd_lib.h>
#include <stdint.h>
#include <joystick_driver.h>
#include <memblock_lib.h>
#include <popup_lib.h>


static void GameOfLife_Evolve( uint8_t * from_buffer, uint8_t * to_buffer )
{
	for (uint8_t y = 0; y < 64; ++y) {
		for (uint8_t x = 0; x < 128; ++x) {
			uint8_t neighbor_count = 0;
			if (LCD_GetBufferPixel( from_buffer, x+1, y   ) != 0) { ++neighbor_count; }
			if (LCD_GetBufferPixel( from_buffer, x+1, y-1 ) != 0) { ++neighbor_count; }
			if (LCD_GetBufferPixel( from_buffer, x  , y-1 ) != 0) { ++neighbor_count; }
			if (LCD_GetBufferPixel( from_buffer, x-1, y-1 ) != 0) { ++neighbor_count; }
			if (LCD_GetBufferPixel( from_buffer, x-1, y   ) != 0) { ++neighbor_count; }
			if (LCD_GetBufferPixel( from_buffer, x-1, y+1 ) != 0) { ++neighbor_count; }
			if (LCD_GetBufferPixel( from_buffer, x  , y+1 ) != 0) { ++neighbor_count; }
			if (LCD_GetBufferPixel( from_buffer, x+1, y+1 ) != 0) { ++neighbor_count; }

			if (LCD_GetBufferPixel( from_buffer, x, y ) != 0) {
				if ((neighbor_count != 2) && (neighbor_count != 3)) {
					LCD_ClrBufferPixel( to_buffer, x, y ); // loneliness or overcrowded
				} else {
					LCD_SetBufferPixel( to_buffer, x, y ); // survive
				}
			} else {
				if (neighbor_count == 3) {
					LCD_SetBufferPixel( to_buffer, x, y );
				} else {
					LCD_ClrBufferPixel( to_buffer, x, y );
				}
			}
		}
	}		
}


// Gospel glider gun
static uint8_t const CAL_PGM_DEF(GameOfLife_gospelGliderGun[]) = {
	 20,  10, // First pair is X/Y offset for rest of coords.
	  6,   9,
	  6,  10,
	  7,   9,
	  7,  10,
	 15,   9,
	 16,   9,
	 14,  10,
	 16,  10,
	 14,  11,
	 15,  11,
	 22,  11,
	 23,  11,
	 22,  12,
	 24,  12,
	 22,  13,
	 29,   7,
	 30,   7,
	 28,   8,
	 30,   8,
	 28,   9,
	 29,   9,
	 40,   7,
	 41,   7,
	 40,   8,
	 41,   8,
	 30,  19,
	 31,  19,
	 32,  19,
	 30,  20,
	 31,  21,
	 41,  14,
	 42,  14,
	 41,  15,
	 43,  15,
	 41,  16,
	255  // 255 indicates end of list.
};


static void GameOfLife_DrawInitial( uint8_t * frameBuffer, uint8_t const CAL_PGM(* seedXY) )
{
#if 0
	uint8_t const xOffs = seedXY[0];
	uint8_t const yOffs = seedXY[1];
	seedXY += 2;

	while (*seedXY != 255) {
		uint8_t x = seedXY[0] + xOffs;
		uint8_t y = seedXY[1] + yOffs;
		seedXY += 2;
		LCD_SetBufferPixel( frameBuffer, x, y );
	}
#else
	uint8_t const xOffs = CAL_pgm_read_byte(&seedXY[0]);
	uint8_t const yOffs = CAL_pgm_read_byte(&seedXY[1]);
	seedXY += 2;

	while ( CAL_pgm_read_byte(seedXY) != 255) {
		uint8_t x = CAL_pgm_read_byte(&seedXY[0]) + xOffs;
		uint8_t y = CAL_pgm_read_byte(&seedXY[1]) + yOffs;
		seedXY += 2;
		LCD_SetBufferPixel( frameBuffer, x, y );
	}
#endif
}


void GameOfLife( void )
{
	POPUP_MsgBox( 14, 2, 6,
	              "Press and hold"
                  "ENTER to exit.", NULL );
	LCD_SetScreen( 0x00 );
	
	// Allocate some workspace.
	uint8_t * frameBuffer1 = MEM_ALLOC_ARRAY( uint8_t, LCD_WIDTH * LCD_HEIGHT / 8 );
	uint8_t * frameBuffer2 = MEM_ALLOC_ARRAY( uint8_t, LCD_WIDTH * LCD_HEIGHT / 8 );
	
	if ((frameBuffer1 == NULL) || (frameBuffer2 == NULL)) {
		POPUP_MsgBox( 10, 2, 6, "Not enough\r\nmemory!", NULL );
	} else {
		// Prepare initial seeds.
		LCD_SetBuffer( frameBuffer1, 0x00 );
		LCD_SetBuffer( frameBuffer2, 0x00 );
		GameOfLife_DrawInitial( frameBuffer1, GameOfLife_gospelGliderGun );
		
		// Evolve until ENTER pressed.
		do {
			LCD_WriteFrameBuffer( frameBuffer1 );
			GameOfLife_Evolve( frameBuffer1, frameBuffer2 );
			LCD_WriteFrameBuffer( frameBuffer2 );
			GameOfLife_Evolve( frameBuffer2, frameBuffer1 );
		} while (!JOYSTICK_IsEnterPressed());
		
		// Clean up.
		MEM_FREE( frameBuffer1 );
		MEM_FREE( frameBuffer2 );
	}
}
