/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 snake game demo application.
 *
 *         This application runs the legendary snake game. You control a snake
 *         moving around a level and should try to pick up as much food as
 *         possible without running into yourself or the wall.
 *
 *         Start the game by pressing enter and then control the snake with the 
 *         joystick. If you try to do a u-turn your your direction will not change
 *         (preventing you from eating yourself up by mistake). The game will run
 *         until you die or you finish the level by eating all food. There are
 *         four levels and they get more and more challenging.
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
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/production_demo_rev_A/snake.c $
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

#include <stdint.h>
#include <cal.h>
#include <common.h>

#include <lcd_lib.h>
#include <joystick_driver.h>
#include <memblock_lib.h>
#include <backlight_driver.h>
#include <popup_lib.h>


#define SNAKE_INITIAL_LENGTH 10
#define SNAKE_FOOD_VALUE 10
#define SNAKE_FOOD_PER_LEVEL 10
#define SNAKE_MAX_LENGTH (SNAKE_INITIAL_LENGTH + (SNAKE_FOOD_VALUE * SNAKE_FOOD_PER_LEVEL))
#define SNAKE_SPEEDUP_DELTA 10
#define SNAKE_INITIAL_DELAY 200
#define SNAKE_DELAY_US_COUNT 350
#define SNAKE_START_X (LCD_WIDTH / 2)
#define SNAKE_START_Y ((LCD_HEIGHT / 2) - 10)
#define SNAKE_EAT_DELAY 200


static void Snake_DrawFrame( void )
{
	LCD_SetHLine( 0, LCD_WIDTH - 1, 0 );
	LCD_SetHLine( 0, LCD_WIDTH - 1, LCD_HEIGHT - 1 );
	LCD_SetVLine( 0, 0, LCD_HEIGHT -1 );
	LCD_SetVLine( LCD_WIDTH - 1, 0, LCD_HEIGHT -1 );
}


static void Snake_DrawLevel( uint8_t level )
{
	Snake_DrawFrame();
	
	switch (level) {
		case 0 : break;
		
		case 1 :
			LCD_SetHLine( 20, 127-20, 64/2 );
		break;
			
		case 2 :
			LCD_SetVLine( 20, 10, 63-10 );
			LCD_SetVLine( 127-20, 10, 63-10 );
		break;
			
		case 3 :
			LCD_SetHLine( 20, 127-20, 10 );
			LCD_SetHLine( 20, 127-20, 63-10 );
		break;
			
		case 4 :
			LCD_SetHLine( 20+5, 127-20-5, 10 );
			LCD_SetHLine( 20+5, 127-20-5, 63-10 );
			LCD_SetVLine( 20, 10+5, 63-10-5 );
			LCD_SetVLine( 127-20, 10+5, 63-10-5 );
		break;
			
		default: break;
	}	
}



static uint8_t Snake_foodX;
static uint8_t Snake_foodY;


void MakeFood( uint8_t seed )
{
	static uint8_t f1, f2, f3, f4;
	static uint8_t x, y;

	uint8_t success = 0;
	do {	
		f1 += 1;
		f2 += 3;
		f3 += 5;
		f4 += 7;
	
		x += f1 * f3;
		y += f2 * f4;
		
		f1 += seed;
		f2 += seed;
		f3 += seed;
		f4 += seed;
		
		uint8_t new_x = (x % (LCD_WIDTH - 2)) + 1;
		uint8_t new_y = (y % (LCD_HEIGHT - 2)) + 1;
		
		if (LCD_GetPixel( new_x, new_y ) == 0) {
			success = 1;
			LCD_SetPixel( new_x, new_y );
			Snake_foodX = new_x;
			Snake_foodY = new_y;
		}
	} while (!success);
}


bool Snake_PlayLevel( uint8_t * xPos, uint8_t * yPos )
{
	// Green background initially.
	BACKLIGHT_SetRGB( 0, 255, 0 );

	// Start in middle, slightly up.
	xPos[0] = SNAKE_START_X;
	yPos[0] = SNAKE_START_Y;
	LCD_SetPixel( xPos[0], yPos[0] );

	// Wait for enter.
	uint8_t food_seed = 0;
	do { ++food_seed; } while (!JOYSTICK_IsEnterPressed());
	MakeFood( food_seed );

	// Main snake loop.
	uint8_t left_to_grow = SNAKE_INITIAL_LENGTH;
	uint8_t left_to_shrink = 0;
	uint16_t snake_head = 0;
	uint16_t snake_tail = 0;
	uint8_t direction = JOYSTICK_LEFT;
	uint16_t snake_delay = SNAKE_INITIAL_DELAY;
	uint8_t food_left = SNAKE_FOOD_PER_LEVEL;

	bool alive = true;
	do {
		// Increase food seed every iteration, to seed the random number some.
		++food_seed;
		
		// Check buttons and change direction if not U-turn.
		if (JOYSTICK_IsLeftPressed()  && (direction != JOYSTICK_RIGHT)) { direction = JOYSTICK_LEFT; }
		if (JOYSTICK_IsUpPressed()    && (direction != JOYSTICK_DOWN))  { direction = JOYSTICK_UP; }
		if (JOYSTICK_IsRightPressed() && (direction != JOYSTICK_LEFT))  { direction = JOYSTICK_RIGHT; }
		if (JOYSTICK_IsDownPressed()  && (direction != JOYSTICK_UP))    { direction = JOYSTICK_DOWN; }
		
		// Draw next head if not shrinking.
		if (left_to_shrink == 0) {
			// Next pos in circular coord buffers.
			uint16_t new_head = snake_head + 1;
			if (new_head >= SNAKE_MAX_LENGTH) { new_head = 0; }
			if (new_head == snake_tail) { ++left_to_shrink; };
			
			// Find next head coords.
			uint16_t new_x = 0;
			uint16_t new_y = 0;
			switch (direction) {
				case JOYSTICK_RIGHT :
					new_x = xPos[snake_head] + 1;
					new_y = yPos[snake_head];
				break;

				case JOYSTICK_UP :
					new_x = xPos[snake_head];
					new_y = yPos[snake_head] - 1;
				break;

				case JOYSTICK_LEFT :
					new_x = xPos[snake_head] - 1;
					new_y = yPos[snake_head];
				break;

				case JOYSTICK_DOWN :
					new_x = xPos[snake_head];
					new_y = yPos[snake_head] + 1;
				break;
					
				default: break;
			}
			
			// Check for collision.
			if (LCD_GetPixel( new_x, new_y ) != 0) {
				// Collided with food?
				if ((new_x == Snake_foodX) && (new_y == Snake_foodY)) {
					// Blink blue when eating food.
					BACKLIGHT_SetRGB( 0, 0, 255 );
					DELAY_MS( SNAKE_EAT_DELAY );
					BACKLIGHT_SetRGB( 0, 255, 0 );

					// Make more food?
					if (--food_left) {
						MakeFood( food_seed );
					}
					
					// Update snake state variables.
					left_to_grow += SNAKE_FOOD_VALUE;
					snake_head = new_head;
					xPos[snake_head] = new_x;
					yPos[snake_head] = new_y;
					
					// Increase speed.
					if (snake_delay > 0) {
						snake_delay -= SNAKE_SPEEDUP_DELTA;
					}
				// No? Then we collided with wall or obstacle.
				} else {
					alive = false;
					BACKLIGHT_SetRGB( 255, 0, 0 );
					DELAY_S( 1 );
				}
			// No collision, then draw new head.
			} else {
				snake_head = new_head;
				xPos[snake_head] = new_x;
				yPos[snake_head] = new_y;
				LCD_SetPixel( new_x, new_y );
			}
		} else {
			--left_to_shrink;
		}
		
		// Erase tail if now growing.
		if (left_to_grow == 0) {
			LCD_ClrPixel( xPos[snake_tail], yPos[snake_tail] );
			++snake_tail;
			if (snake_tail >= SNAKE_MAX_LENGTH) { snake_tail = 0; }
		} else {
			--left_to_grow;
		}
		
		// Delay.
		for (uint16_t i = 0; i < snake_delay; ++i) {
			DELAY_US( SNAKE_DELAY_US_COUNT );
		}
	} while (alive && food_left);
	
	// If all food eaten, and still alive, blink white a few times.
	if (alive) {
		BACKLIGHT_SetRGB( 255, 255, 255 );
		DELAY_MS( 100 );
		BACKLIGHT_SetRGB( 0, 0, 0 );
		DELAY_MS( 100 );
		BACKLIGHT_SetRGB( 255, 255, 255 );
		DELAY_MS( 100 );
		BACKLIGHT_SetRGB( 0, 0, 0 );
		DELAY_MS( 100 );
		BACKLIGHT_SetRGB( 255, 255, 255 );
		DELAY_MS( 100 );
		BACKLIGHT_SetRGB( 0, 0, 0 );
		DELAY_MS( 100 );
		BACKLIGHT_SetRGB( 255, 255, 255 );
	}
	
	return alive;
}



void Snake( void )
{
	
	
	uint8_t * xPos = MEM_ALLOC_ARRAY( uint8_t, SNAKE_MAX_LENGTH );
	uint8_t * yPos = MEM_ALLOC_ARRAY( uint8_t, SNAKE_MAX_LENGTH );
	
	if ((xPos == NULL) || (yPos == NULL)) {
		POPUP_MsgBox( 10, 2, 6, "Not enough\r\nmemory!", NULL );
	} else {
		uint8_t level = 0;
		bool died = false;
		do {
			LCD_SetScreen( 0x00 );
			Snake_DrawLevel( level );
			if (Snake_PlayLevel( xPos, yPos ) == true) {
				++level;
			} else {
				died = true;
			}
		} while (died == false);
		
		MEM_FREE( xPos );
		MEM_FREE( yPos );
		BACKLIGHT_SetRGB( 0, 0, 0 );
	}
}
