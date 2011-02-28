/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 popup library source file.
 *
 *      The popup box library provides functions to display a popup message over
 *      any other graphics, wait for the user to move the joystick and then
 *      exit. Optionally, the popup library can redraw the form that was
 *      previously on screen. The library takes care of centering the box on
 *      screen and wrapping the text over several lines.
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
 * $Date: 2007-09-17 15:18:48 -0600 (ma, 17 sep 2007) $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/gfx/popup_lib.c $
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
#include "popup_lib.h"
#include <lcd_lib.h>
#include <termfont_lib.h>
#include <joystick_driver.h>
#include <power_driver.h>



/***************************
 * Function implementations
 ***************************/

/*!
 * Pops up a message-box with specified text. Takes care of text-wrapping.
 * The box will be centered on the display, and you can specify margin between box and text.
 * Quits on joystick action and redraws "background" form if specified
 *
 * \param  widthInChars   How wide in chars you want the text in the box to be
 * \param  heightInPages  How high you want the box to be in pages
 * \param  margin         Number of pixels between the text and the sides of the box
 * \param  str            Pointer to a null-terminated string. Supports LF and CR.
 * \param  parentForm     If specified, this form will be redrawn before the function returns
 */
void POPUP_MsgBox( uint8_t widthInChars, uint8_t heightInPages, uint8_t margin, char const * str, FORMS_form_t * parentForm )
{
	// Reset any hardware LCD scrolling first.
	if (parentForm != NULL) {
		FORMS_NormalizeLCDScroll( parentForm );
	}
	
	// Calculate some coordinates, with text centered in screen.
	uint8_t const widthInPixels = widthInChars * TERMFONT_CHAR_WIDTH;
	uint8_t const startColumn = (LCD_WIDTH / 2) - (widthInPixels / 2);
	uint8_t const startX = startColumn - margin;
	uint8_t const endColumn = (LCD_WIDTH / 2) + (widthInPixels / 2);
	uint8_t const endX = endColumn + margin;
	uint8_t const startPage = (LCD_HEIGHT / LCD_PAGE_HEIGHT / 2) - 1 - ((heightInPages - 1) / 2);
	uint8_t const startY = (startPage * LCD_PAGE_HEIGHT) - margin;
	uint8_t const endPage = startPage + heightInPages;
	uint8_t const endY = (endPage * LCD_PAGE_HEIGHT) + margin;
	
	// Draw popup frame.
	LCD_SetHLine( startX, endX, startY );
	LCD_SetHLine( startX, endX, endY );
	LCD_SetHLine( startX + 1, endX + 1, endY + 1 );
	LCD_SetVLine( startX, startY, endY );
	LCD_SetVLine( endX, startY, endY );
	LCD_SetVLine( endX + 1, startY + 1, endY + 1 );
	LCD_ClrBox( startX + 1, startY + 1, endX - 1, endY - 1 );
	
	// Draw text with wrapping on line ends.
	uint8_t column = startColumn;
	uint8_t page = startPage;
	while ((*str != 0) && (page < endPage)) {
		if (*str == '\r') {
			column = startColumn;
		}
		else if (*str == '\n') {
			++page;
		}
		else {
			if (column >= endColumn) {
				++page;
				column = startColumn;
			}
			TERMFONT_DisplayChar( *str, page, column );
			column += TERMFONT_CHAR_WIDTH;
		}
		++str;
	}
	
	// Wait until joystick is released (could still be held down after select a menu item).
	while (JOYSTICK_GetState() != 0x00) { POWER_EnterIdleSleepMode(); }
	// Wait until joystick is pressed (user wants to exit popup box).
	while (JOYSTICK_GetState() == 0x00) { POWER_EnterIdleSleepMode(); }
	// Wait until joystick is released again (in order not to give unwanted joystick events to the calling application).
	while (JOYSTICK_GetState() != 0x00) { POWER_EnterIdleSleepMode(); }

	// Redraw parent form if requested.
	if (parentForm != NULL) {
		FORMS_Draw( parentForm );
	}
}


// end of file
