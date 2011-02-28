/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 terminal font handling library header file
 *
 *      The terminal font library provides functions for printing individual
 *      chars and strings from SRAM and Flash memory to the LCD or an off-screen
 *      framebuffer. The library is page-oriented and uses the widely accepted
 *      5x7 pixel terminal font. This library is the basic library used by all
 *      other libraries to display plain text.
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
 * $Revision: 2432 $
 * $Date: 2007-09-17 14:23:04 -0600 (ma, 17 sep 2007) $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/termfont_lib/termfont_lib.h $
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

#ifndef TERMFONT_LIB_H
#define TERMFONT_LIB_H

#include <lcd_lib.h>
#include <stdint.h>


/*****************************
 * Terminal font definitions. 
 *****************************/

#define TERMFONT_CHAR_WIDTH 6   //!< Pixel width of terminal font.
#define TERMFONT_CHAR_HEIGHT 8  //!< Pixel height of terminal font.
#define TERMFONT_CHARS_PER_LINE (LCD_WIDTH/TERMFONT_CHAR_WIDTH)  //!< Number of characters on one LCD line.
#define TERMFONT_LINES_PER_SCREEN (LCD_HEIGHT/TERMFONT_CHAR_HEIGHT)   //!< Number of lines with characters on LCD.

typedef uint8_t TERMFONT_stringSize_t;


/***********************
 * Function prototypes.
 ***********************/

// Terminal font operations direct LCD access
void TERMFONT_DisplayChar(char character, uint8_t page, uint8_t column);  //!< One character to overwrite content on LCD.
void TERMFONT_DisplayString(char const * pTextString, uint8_t page, uint8_t column);  //!< Textstring to overwrite content on LCD.
void TERMFONT_DisplayString_F(char const CAL_PGM(* pTextString), uint8_t page, uint8_t column);  //!< Flash textstring to overwrite content on LCD.
void TERMFONT_MergeChar(char character, uint8_t page, uint8_t column);  //!< One character to merge with content on LCD.
void TERMFONT_MergeString(char const * pTextString, uint8_t page, uint8_t column);  //!< Textstring to merge with content on LCD.
void TERMFONT_MergeString_F(char const CAL_PGM(* pTextString), uint8_t page, uint8_t column);  //!< Flash textstring to merge with content on LCD.
void TERMFONT_XORChar(char character, uint8_t page, uint8_t column);  //!< One character to XOR with content on LCD.
void TERMFONT_XORString(char const * pTextString, uint8_t page, uint8_t column);  //!< Textstring to XOR with content on LCD.
void TERMFONT_XORString_F(char const CAL_PGM(* pTextString), uint8_t page, uint8_t column);  //!< Flash textstring to XOR with content on LCD.
void TERMFONT_SetCursor(uint8_t page, uint8_t column);  //!< Display/set cursor on given address.
void TERMFONT_ClearCursor(uint8_t page, uint8_t column);  //!< Clear cursor on given address.
void TERMFONT_FlipCursor(uint8_t page, uint8_t column);  //!< Flip cursor on given address, e.g. blinking cursor.

// Terminal font operations on LCD buffer
void TERMFONT_DisplayBufferChar( uint8_t * buffer, char character, uint8_t page, uint8_t column);  //!< One character to overwrite buffer content.
void TERMFONT_DisplayBufferString( uint8_t * buffer, char const * pTextString, uint8_t page, uint8_t column);  //!< Textstring to overwrite buffer content.
void TERMFONT_DisplayBufferString_F( uint8_t * buffer, char const CAL_PGM(* pTextString), uint8_t page, uint8_t column);  //!< Flash textstring to overwrite buffer content.
void TERMFONT_DisplayPageBufferChar( uint8_t * pPageBuffer, char character ); //!< One character to page buffer with user defined lenght.
void TERMFONT_DisplayPageBufferString( uint8_t * pPageBuffer, char const * pTextString ); //!< Textstring to page buffer with user defined lenght.
void TERMFONT_DisplayPageBufferString_F( uint8_t * pPageBuffer, char const CAL_PGM(* pTextString) ); //!< Flash textstring to page buffer with user defined lenght.
void TERMFONT_MergeBufferChar( uint8_t * buffer, char character, uint8_t page, uint8_t column);  //!< One character to merge with buffer content.
void TERMFONT_MergeBufferString( uint8_t * buffer, char const * pTextString, uint8_t page, uint8_t column);  //!< Textstring to merge with buffer content.
void TERMFONT_MergeBufferString_F( uint8_t * buffer, char const CAL_PGM(* pTextString), uint8_t page, uint8_t column);  //!< Flash textstring to merge with buffer content.
void TERMFONT_XORBufferChar( uint8_t * buffer, char character, uint8_t page, uint8_t column);  //!< One character to XOR with buffer content.
void TERMFONT_XORBufferString( uint8_t * buffer, char const * pTextString, uint8_t page, uint8_t column);  //!< Textstring to XOR with buffer content.
void TERMFONT_XORBufferString_F( uint8_t * buffer, char const CAL_PGM(* pTextString), uint8_t page, uint8_t column);  //!< Flash textstring to XOR with buffer content.
void TERMFONT_SetBufferCursor( uint8_t * buffer, uint8_t page, uint8_t column);  //!< Set cursor on given address in buffer.
void TERMFONT_ClearBufferCursor( uint8_t * buffer, uint8_t page, uint8_t column);  //!< Clear cursor on given address in buffer.
void TERMFONT_FlipBufferCursor( uint8_t * buffer, uint8_t page, uint8_t column);  //!< Flip cursor on given address in buffer, e.g. blinking cursor.

// Terminal font helper functions.
uint8_t TERMFONT_GetStringWidth( char const * pTextString );
uint8_t TERMFONT_GetStringWidth_F( char const CAL_PGM(* pTextString) );


#endif
