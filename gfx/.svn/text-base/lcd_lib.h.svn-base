// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 LCD abstraction library header file
 *
 *      The LCD graphics library provides functions for basic pixel
 *      manipulation, lines, boxes, circles etc. It also provides a parallel
 *      set of functions for operating on SRAM off-screen framebuffers instead
 *      of operating directly on the LCD module.
 *
 *      The LCD screen is divided into eight-pixel high pages, suitable for
 *      standard 5x7 pixel fonts. This feature is utilized by the LCD graphics
 *      library, which provides a set of page-oriented functions for efficient
 *      operation for page-oriented applications, such as the Terminal Font
 *      Library and the Forms Library.
 *
 *      The library is actually divided into two source files and accompanying
 *      header files. The lcd_lib.c and lcd_lib.h files provide the basic pixel
 *      manipulation functions and other graphics functions that can be
 *      optimized for the page-oriented nature of the LCD screen, such as the
 *      page-oriented functions and horizontal and vertical lines.
 *      The gfx_lib.c and gfx_lib.h files provide higher-level graphics
 *      functions that make use of the pixel manipulation functions, such as
 *      circles and arbitrary lines. Such graphics functions cannot utilize
 *      the page-oriented nature of the LCD screen.
 *
 *      If you want to save code space by using only single-pixel manipulating
 *      functions, do not refer to the other optimized functions and they will
 *      not be linked into the final application.
 *
 *      Note that all functions that involve reading (and modifying) memory
 *      contents on the LCD module itself will not work with the serial LCD
 *      interface, as reading is not supported by that interface.
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
 * $Revision: 2395 $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/gfx/lcd_lib.h $
 * $Date: 2007-09-13 13:16:08 -0600 (to, 13 sep 2007) $  \n
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
#ifndef LCD_LIB_H
#define LCD_LIB_H

#include <stdint.h>
#include <cal.h>


/************************************************************
 * Private variables made public since macros are using them
 ************************************************************/

extern uint8_t LCD_topPage; //!< Keeps track of current top page.



/*************************
 * LCD layout definitions
 *************************/

#define LCD_WIDTH  128  //!< LCD width in pixels.
#define LCD_HEIGHT  64  //!< LCD height in pixels.
#define LCD_PAGE_HEIGHT 8  //!< Pixels per page height.
#define LCD_PAGE_COUNT (LCD_HEIGHT / LCD_PAGE_HEIGHT)
#define LCD_BUF_SIZE (LCD_WIDTH * LCD_HEIGHT / 8)  //!< LCD framebuffer size in bytes.



/**********************
 * Function prototypes
 **********************/

void LCD_Init( void );  //!< Initialize LCD controller and clear display.
void LCD_SetTopPage( uint8_t lcdPage, uint8_t subPageOffset );  //!< Select which page should be on top.

void LCD_SetScreen( uint8_t pixelMask );  //!< Set all pixels on display to value given by pixelMask, which is an 8-pixel high column.
void LCD_SetPixel( uint8_t x, uint8_t y );  //!< Turn on pixel on display.
void LCD_ClrPixel( uint8_t x, uint8_t y );  //!< Turn off pixel on display.
void LCD_FlipPixel( uint8_t x, uint8_t y );  //!< Flip pixel on display.
uint8_t LCD_GetPixel( uint8_t x, uint8_t y );  //!< Return non-zero if pixel on display is turned on, zero otherwise.
void LCD_SetHLine( uint8_t x1, uint8_t x2, uint8_t y );  //!< Turn on pixels on display in a horizontal line.
void LCD_ClrHLine( uint8_t x1, uint8_t x2, uint8_t y );  //!< Turn off pixels on display in a horizontal line.
void LCD_FlipHLine( uint8_t x1, uint8_t x2, uint8_t y );  //!< Flip pixels on display in a horizontal line.
void LCD_SetVLine( uint8_t x, uint8_t y1, uint8_t y2 );  //!< Turn on pixels on display in a vertical line.
void LCD_ClrVLine( uint8_t x, uint8_t y1, uint8_t y2 );  //!< Turn off pixels on display in a vertical line.
void LCD_FlipVLine( uint8_t x, uint8_t y1, uint8_t y2 );  //!< Flip pixels on display in a vertical line.
void LCD_SetBox( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 );  //!< Turn on pixels on display in a rectangular box.
void LCD_ClrBox( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 );  //!< Turn off pixels on display in a rectangular box.
void LCD_FlipBox( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 );  //!< Flip pixels on display in a rectangular box.
void LCD_WritePage( const uint8_t * data, uint8_t page, uint8_t column, uint8_t width );  //!< Write 8-pixel high block to display.
void LCD_MergePage( const uint8_t * data, uint8_t page, uint8_t column, uint8_t width );  //!< Merge (OR) 8-pixel high block into display.
void LCD_XORPage( const uint8_t * data, uint8_t page, uint8_t column, uint8_t width );  //!< XOR 8-pixel high block into display.
void LCD_SetPage( uint8_t pixelMask, uint8_t page, uint8_t column, uint8_t width ); //!< Set pixels given by pixelMask on display in 8-pixel high block.
void LCD_ClrPage( uint8_t pixelMask, uint8_t page, uint8_t column, uint8_t width ); //!< Set pixels given by pixelMask on display in 8-pixel high block.
void LCD_FlipPage( uint8_t pixelMask, uint8_t page, uint8_t column, uint8_t width );  //!< Clear pixels given by pixelMask on display in 8-pixel high block.
void LCD_ReadPage( uint8_t * data, uint8_t page, uint8_t column, uint8_t width );  //!< Read 8-pixel high block from display.

void LCD_SetBuffer( uint8_t * buffer, uint8_t pixelMask );  //!< Set all pixels in buffer to value given by pixelMask, which is an 8-pixel high column.
void LCD_SetBufferPixel( uint8_t * buffer, uint8_t x, uint8_t y );  //!< Turn on pixel in buffer.
void LCD_ClrBufferPixel( uint8_t * buffer, uint8_t x, uint8_t y );  //!< Turn off pixel in buffer.
void LCD_FlipBufferPixel( uint8_t * buffer, uint8_t x, uint8_t y );  //!< Flip pixel in buffer.
uint8_t LCD_GetBufferPixel( const uint8_t * buffer, uint8_t x, uint8_t y );  //!< Return non-zero if pixel in buffer is turned on, zero otherwise.
void LCD_SetBufferHLine( uint8_t * buffer, uint8_t x1, uint8_t x2, uint8_t y );  //!< Turn on pixels in buffer in a horizontal line.
void LCD_ClrBufferHLine( uint8_t * buffer, uint8_t x1, uint8_t x2, uint8_t y );  //!< Turn off pixels in biffer in a horizontal line.
void LCD_FlipBufferHLine( uint8_t * buffer, uint8_t x1, uint8_t x2, uint8_t y );  //!< Flip pixels in buffer in a horizontal line.
void LCD_SetBufferVLine( uint8_t * buffer, uint8_t x, uint8_t y1, uint8_t y2 );  //!< Turn on pixels in buffer in a vertical line.
void LCD_ClrBufferVLine( uint8_t * buffer, uint8_t x, uint8_t y1, uint8_t y2 );  //!< Turn off pixels in buffer in a vertical line.
void LCD_FlipBufferVLine( uint8_t * buffer, uint8_t x, uint8_t y1, uint8_t y2 );  //!< Flip pixels in buffer in a vertical line.
void LCD_SetBufferBox( uint8_t * buffer, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 );  //!< Turn on pixels in buffer in a rectangular box.
void LCD_ClrBufferBox( uint8_t * buffer, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 );  //!< Turn off pixels in buffer in a rectangular box.
void LCD_FlipBufferBox( uint8_t * buffer, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 );  //!< Flip pixels in buffer in a rectangular box.
void LCD_WriteBufferPage( uint8_t * buffer, const uint8_t * data, uint8_t page, uint8_t column, uint8_t width );  //!<  Write 8-pixel high block to buffer.
void LCD_MergeBufferPage( uint8_t * buffer, const uint8_t * data, uint8_t page, uint8_t column, uint8_t width );  //!< Merge (OR) 8-pixel high block into buffer.
void LCD_XORBufferPage( uint8_t * buffer, const uint8_t * data, uint8_t page, uint8_t column, uint8_t width );  //!< XOR 8-pixel high block into buffer.
void LCD_SetBufferPage( uint8_t * buffer, uint8_t pixelMask, uint8_t page, uint8_t column, uint8_t width ); //!< Set pixels given by pixelMask in buffer in 8-pixel high block.
void LCD_ClrBufferPage( uint8_t * buffer, uint8_t pixelMask, uint8_t page, uint8_t column, uint8_t width ); //!< Clear pixels given by pixelMask in buffer in 8-pixel high block.
void LCD_FlipBufferPage( uint8_t * buffer, uint8_t pixelMask, uint8_t page, uint8_t column, uint8_t width );  //!< Flip pixels given by pixelMask in buffer in 8-pixel high block.
void LCD_ReadBufferPage( const uint8_t * buffer, uint8_t * data, uint8_t page, uint8_t column, uint8_t width );  //!< Read 8-pixel high block from buffer.

void LCD_WriteFrameBuffer( const uint8_t * buffer );  //!< Write buffer to display.
void LCD_ReadFrameBuffer( uint8_t * buffer );  //!< Read display into buffer.

void LCD_WritePage_F( const uint8_t CAL_PGM(* data), uint8_t page, uint8_t column, uint8_t width );  //!<  Write 8-pixel high block to display (from Flash).
void LCD_MergePage_F( const uint8_t CAL_PGM(* data), uint8_t page, uint8_t column, uint8_t width );  //!< Merge (OR) 8-pixel high block into display (from Flash).
void LCD_XORPage_F(  const uint8_t CAL_PGM(* data), uint8_t page, uint8_t column, uint8_t width );  //!< XOR 8-pixel high block into display (from Flash).

void LCD_WriteBufferPage_F( uint8_t * buffer, const uint8_t CAL_PGM(* data), uint8_t page, uint8_t column, uint8_t width );  //!<  Write 8-pixel high block to buffer (from Flash).
void LCD_MergeBufferPage_F( uint8_t * buffer, const uint8_t CAL_PGM(* data), uint8_t page, uint8_t column, uint8_t width );  //!< Merge (OR) 8-pixel high block into buffer (from flash).
void LCD_XORBufferPage_F( uint8_t * buffer, const uint8_t CAL_PGM(* data), uint8_t page, uint8_t column, uint8_t width );  //!< XOR 8-pixel high block into buffer (from Flash).



/*********************
 * Convenience macros
 *********************/

#define LCD_GetTopPage() (LCD_topPage)

// Optimize conversion macros below with AND instead of MOD if LCD_PAGE_COUNT is power of 2.
#if ((LCD_PAGE_COUNT - 1) & (LCD_PAGE_COUNT)) == 0
	#define LCD_ROW2PAGE(row)  (((row) + LCD_GetTopPage()) & (LCD_PAGE_COUNT - 1))
	#define LCD_PAGE2ROW(page) (((page) + LCD_PAGE_COUNT - LCD_GetTopPage()) & (LCD_PAGE_COUNT - 1))
#else
	#define LCD_ROW2PAGE(row)  (((row) + LCD_GetTopPage()) % LCD_PAGE_COUNT)
	#define LCD_PAGE2ROW(page) (((page) + LCD_PAGE_COUNT - LCD_GetTopPage()) % LCD_PAGE_COUNT)
#endif

#endif
