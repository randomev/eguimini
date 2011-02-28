/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 LCD abstraction library source file
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
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/gfx/lcd_lib.c $
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

#include "lcd_lib.h"
#include <s6b1713_driver.h>
#include <common.h>



/********************
 * Private variables
 ********************/
uint8_t LCD_topPage = 0; // Not static, since it is used by macros in other modules.



/***************************
 * Function implementations
 ***************************/

void LCD_Init( void )
{
	// Initialize interface and perform hard reset.
	S6B1713_Init();
	
	// Step-by-step start of the voltage regulator circuitry.
    S6B1713_SetPowerControl( S6B1713_VOLTAGE_CONVERTER );
	DELAY_MS( 2 ); // Wait for Vout to stabilize, i.e. delay for more than 1ms.
    S6B1713_SetPowerControl( S6B1713_VOLTAGE_CONVERTER | S6B1713_VOLTAGE_REGULATOR );
	DELAY_MS( 2 ); // Wait for Vout to stabilize, i.e. delay for more than 1ms.
    S6B1713_SetPowerControl( S6B1713_VOLTAGE_CONVERTER | S6B1713_VOLTAGE_REGULATOR | S6B1713_VOLTAGE_FOLLOWER );
    S6B1713_SetInternalResistorRatio( S6B1713_INTERNAL_RESISTOR_RATIO_5p29 );
    S6B1713_SetReferenceVoltage( 4 ); // Contrast: Between 0 and 63.
	DELAY_MS( 2 ); // Wait for Vout to stabilize, i.e. delay for more than 1ms.

	// LCD panel configuration.
    S6B1713_SetLCDBias1(); // Bias ratio 1/9 for 1/65 duty cycle.
    S6B1713_SetADCNormalDirection();
    S6B1713_SetSHLNormalDirection();
    LCD_SetTopPage( 0, 0 );
	
	// Clear screen and enable display.
	LCD_SetScreen( 0x00 );
    S6B1713_SetDisplayOn();
	LCD_SetScreen( 0x55 );
	LCD_SetScreen( 0x00 );
}


void LCD_SetTopPage( uint8_t lcdPage, uint8_t subPageOffset )
{
	// Call LCD hardware operation.
	S6B1713_SetInitialDisplayLine( ((lcdPage % LCD_PAGE_COUNT) * LCD_PAGE_HEIGHT) + subPageOffset );
	
	// Update quick ref.
	LCD_topPage = lcdPage;
}


void LCD_SetScreen( uint8_t pixelMask )
{
	// Set pixel mask, page by page.
	for (uint8_t page = 0; page < 8; ++page) {
		// Set current address to start of current 8-pixel high page.
		S6B1713_SetPageAddress( page );
		S6B1713_SetColumnAddress( 0 );

		// Write LCD_WIDTH bytes (LCD_WIDTHx8 pixels).
		uint8_t width = LCD_WIDTH;
		do {
			S6B1713_WriteData( pixelMask );
		} while (--width > 0);
	}
}


void LCD_SetPixel( uint8_t x, uint8_t y )
{
	uint8_t readback;

    // Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// Set starting address.
    S6B1713_SetPageAddress( y >> 3);
    S6B1713_SetColumnAddress( x );

    // A dummy read is required after column address setup
    volatile uint8_t dummy = S6B1713_ReadData(); 

	// Read 8-pixel high column, modify correct pixel and write back.
    readback = S6B1713_ReadData(); 
    S6B1713_WriteData( readback | (1 << (y & 0x07)) );
}


void LCD_ClrPixel( uint8_t x, uint8_t y )
{
	uint8_t readback;

    // Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// Set starting address.
    S6B1713_SetPageAddress( y >> 3);
    S6B1713_SetColumnAddress( x );

    // A dummy read is required after column address change, before reading.
    volatile uint8_t dummy = S6B1713_ReadData(); 

	// Read 8-pixel high column, modify correct pixel and write back.
    readback = S6B1713_ReadData(); 
    S6B1713_WriteData( readback & ~(1 << (y & 0x07)) );
}


void LCD_FlipPixel( uint8_t x, uint8_t y )
{
	uint8_t readback;

    // Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// Set starting address.
    S6B1713_SetPageAddress( y >> 3);
    S6B1713_SetColumnAddress( x );

    // A dummy read is required after column address change, before reading.
    volatile uint8_t dummy = S6B1713_ReadData(); 

	// Read 8-pixel high column, modify correct pixel and write back.
    readback = S6B1713_ReadData(); 
    S6B1713_WriteData( readback ^ (1 << (y & 0x07)) );
}


uint8_t LCD_GetPixel( uint8_t x, uint8_t y )
{
	uint8_t readback;

	// Set starting address.
    S6B1713_SetPageAddress( y >> 3);
    S6B1713_SetColumnAddress( x );

    // A dummy read is required after column address change, before reading.
    volatile uint8_t dummy = S6B1713_ReadData();

	// Read 8-pixel high column and mask out correct pixel.
    readback = S6B1713_ReadData() & (1 << (y & 0x07));
	
	return readback;
}


void LCD_SetHLine( uint8_t x1, uint8_t x2, uint8_t y )
{
	// Swap x1 and x2 if x1 is larger.
	if (x1 > x2) {
		uint8_t temp = x1;
		x1 = x2;
		x2 = temp;
	}

    // Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// Set starting address.
    S6B1713_SetPageAddress( y >> 3);
    S6B1713_SetColumnAddress( x1 );

	// Read 8-pixel high columns, modify correct pixel and write back.
	uint8_t width = x2 - x1 + 1;
	uint8_t pixelMask = 1 << (y & 0x07);
	do {
	    // A dummy read is required after column address change, before reading.
	    volatile uint8_t dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, set correct pixel and write back.
	    uint8_t readback = S6B1713_ReadData(); 
    	S6B1713_WriteData( readback | pixelMask );
	} while (--width > 0);
}


void LCD_ClrHLine( uint8_t x1, uint8_t x2, uint8_t y )
{
	// Swap x1 and x2 if x1 is larger.
	if (x1 > x2) {
		uint8_t temp = x1;
		x1 = x2;
		x2 = temp;
	}

    // Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// Set starting address.
    S6B1713_SetPageAddress( y >> 3);
    S6B1713_SetColumnAddress( x1 );

	// Read 8-pixel high columns, modify correct pixel and write back.
	uint8_t width = x2 - x1 + 1;
	uint8_t pixelMask = ~(1 << (y & 0x07));
	do {
	    // A dummy read is required after column address change, before reading.
	    volatile uint8_t dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, clear correct pixel and write back.
	    uint8_t readback = S6B1713_ReadData(); 
    	S6B1713_WriteData( readback & pixelMask );
	} while (--width > 0);
}


void LCD_FlipHLine( uint8_t x1, uint8_t x2, uint8_t y )
{
	// Swap x1 and x2 if x1 is larger.
	if (x1 > x2) {
		uint8_t temp = x1;
		x1 = x2;
		x2 = temp;
	}

    // Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// Set starting address.
    S6B1713_SetPageAddress( y >> 3);
    S6B1713_SetColumnAddress( x1 );

	// Read 8-pixel high columns, modify correct pixel and write back.
	uint8_t width = x2 - x1 + 1;
	uint8_t pixelMask = 1 << (y & 0x07);
	do {
	    // A dummy read is required after column address change, before reading.
	    volatile uint8_t dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, flip correct pixel and write back.
	    uint8_t readback = S6B1713_ReadData(); 
    	S6B1713_WriteData( readback ^ pixelMask );
	} while (--width > 0);
}


void LCD_SetVLine( uint8_t x, uint8_t y1, uint8_t y2 )
{
	// Swap y1 and y2 if y1 is larger.
	if (y1 > y2) {
		uint8_t temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	// Prepare some.
	uint8_t y1Page = y1 >> 3;
	uint8_t y2Page = y2 >> 3;
	
	uint8_t y1Bit = y1 & 0x07;
	uint8_t y2Bit = y2 & 0x07;
	
	uint8_t y1PixelMask = 0xff << y1Bit;
	uint8_t y2PixelMask = 0xff >> (7 - y2Bit);
	
	// Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// These variables are used often below.
	volatile uint8_t dummy;
	uint8_t readback;

	// If y1 and y2 are in within same 8-pixel column, we must combine bitmasks.
	if (y1Page == y2Page) {
		uint8_t pixelMask = y1PixelMask & y2PixelMask;

		// Set starting address.
    	S6B1713_SetPageAddress( y1Page );
	    S6B1713_SetColumnAddress( x );

	    // A dummy read is required after column address change, before reading.
		dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, set correct pixels and write back.
		readback = S6B1713_ReadData(); 
		S6B1713_WriteData( readback | pixelMask );
	}
	// Else, if y1 and y2 are in different pages, update all affected columns.
	else {
		int8_t fullColumns = y2Page - y1Page - 1; // Number of full 8-pixel columns.
			
		// --- Update pixels in y1's page. ---
		
		// Set starting address.
		S6B1713_SetPageAddress( y1Page );
		S6B1713_SetColumnAddress( x );

	    // A dummy read is required after column address change, before reading.
		dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, set correct pixels and write back.
		readback = S6B1713_ReadData(); 
		S6B1713_WriteData( readback | y1PixelMask );
		
		// --- Update 8-pixel columns in between y1 and y2 pages. ---
		
		uint8_t page = y1Page + 1;
		while (fullColumns-- > 0) {
			// Set starting address.
    		S6B1713_SetPageAddress( page++ );
		    S6B1713_SetColumnAddress( x );

			// Set all pixels.
			S6B1713_WriteData( 0xff );
		}
		
		// --- Update pixels in y2's page. ---
		
		// Set starting address.
    	S6B1713_SetPageAddress( y2Page );
	    S6B1713_SetColumnAddress( x );

	    // A dummy read is required after column address change, before reading.
	    dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, set correct pixels and write back.
	    readback = S6B1713_ReadData(); 
    	S6B1713_WriteData( readback | y2PixelMask );
	}
}


void LCD_ClrVLine( uint8_t x, uint8_t y1, uint8_t y2 )
{
	// Swap y1 and y2 if y1 is larger.
	if (y1 > y2) {
		uint8_t temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	// Prepare some.
	uint8_t y1Page = y1 >> 3;
	uint8_t y2Page = y2 >> 3;
	
	uint8_t y1Bit = y1 & 0x07;
	uint8_t y2Bit = y2 & 0x07;
	
	uint8_t y1PixelMask = ~(0xff << y1Bit);
	uint8_t y2PixelMask = ~(0xff >> (7 - y2Bit));
	
	// Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// These variables are used often below.
	volatile uint8_t dummy;
	uint8_t readback;

	// If y1 and y2 are in within same 8-pixel column, we must combine bitmasks.
	if (y1Page == y2Page) {
		uint8_t pixelMask = y1PixelMask | y2PixelMask;

		// Set starting address.
    	S6B1713_SetPageAddress( y1Page );
	    S6B1713_SetColumnAddress( x );

	    // A dummy read is required after column address change, before reading.
		dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, clear correct pixels and write back.
		readback = S6B1713_ReadData(); 
		S6B1713_WriteData( readback & pixelMask );
	}
	// Else, if y1 and y2 are in different pages, update all affected columns.
	else {
		int8_t fullColumns = y2Page - y1Page - 1; // Number of full 8-pixel columns.
			
		// --- Update pixels in y1's page. ---
		
		// Set starting address.
		S6B1713_SetPageAddress( y1Page );
		S6B1713_SetColumnAddress( x );

	    // A dummy read is required after column address change, before reading.
		dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, clear correct pixels and write back.
		readback = S6B1713_ReadData(); 
		S6B1713_WriteData( readback & y1PixelMask );
		
		// --- Update 8-pixel columns in between y1 and y2 pages. ---
		
		uint8_t page = y1Page + 1;
		while (fullColumns-- > 0) {
			// Set starting address.
    		S6B1713_SetPageAddress( page++ );
		    S6B1713_SetColumnAddress( x );

			// Clear all pixels.
			S6B1713_WriteData( 0x00 );
		}
		
		// --- Update pixels in y2's page. ---
		
		// Set starting address.
    	S6B1713_SetPageAddress( y2Page );
	    S6B1713_SetColumnAddress( x );

	    // A dummy read is required after column address change, before reading.
	    dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, clear correct pixels and write back.
	    readback = S6B1713_ReadData(); 
    	S6B1713_WriteData( readback & y2PixelMask );
	}
}


void LCD_FlipVLine( uint8_t x, uint8_t y1, uint8_t y2 )
{
	// Swap y1 and y2 if y1 is larger.
	if (y1 > y2) {
		uint8_t temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	// Prepare some.
	uint8_t y1Page = y1 >> 3;
	uint8_t y2Page = y2 >> 3;
	
	uint8_t y1Bit = y1 & 0x07;
	uint8_t y2Bit = y2 & 0x07;
	
	uint8_t y1PixelMask = 0xff << y1Bit;
	uint8_t y2PixelMask = 0xff >> (7 - y2Bit);
	
	// Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// These variables are used often below.
	volatile uint8_t dummy;
	uint8_t readback;

	// If y1 and y2 are in within same 8-pixel column, we must combine bitmasks.
	if (y1Page == y2Page) {
		uint8_t pixelMask = y1PixelMask & y2PixelMask;

		// Set starting address.
    	S6B1713_SetPageAddress( y1Page );
	    S6B1713_SetColumnAddress( x );

	    // A dummy read is required after column address change, before reading.
		dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, flip correct pixels and write back.
		readback = S6B1713_ReadData(); 
		S6B1713_WriteData( readback ^ pixelMask );
	}
	// Else, if y1 and y2 are in different pages, update all affected columns.
	else {
		int8_t fullColumns = y2Page - y1Page - 1; // Number of full 8-pixel columns.
			
		// --- Update pixels in y1's page. ---
		
		// Set starting address.
		S6B1713_SetPageAddress( y1Page );
		S6B1713_SetColumnAddress( x );

	    // A dummy read is required after column address change, before reading.
		dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, flip correct pixels and write back.
		readback = S6B1713_ReadData(); 
		S6B1713_WriteData( readback ^ y1PixelMask );
		
		// --- Update 8-pixel columns in between y1 and y2 pages. ---
		
		uint8_t page = y1Page + 1;
		while (fullColumns-- > 0) {
			// Set starting address.
    		S6B1713_SetPageAddress( page++ );
		    S6B1713_SetColumnAddress( x );

		    // A dummy read is required after column address change, before reading.
		    dummy = S6B1713_ReadData(); 

			// Flip all pixels.
			readback = S6B1713_ReadData(); 
			S6B1713_WriteData( readback ^ 0xff );
		}
		
		// --- Update pixels in y2's page. ---
		
		// Set starting address.
    	S6B1713_SetPageAddress( y2Page );
	    S6B1713_SetColumnAddress( x );

	    // A dummy read is required after column address change, before reading.
	    dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, flip correct pixels and write back.
	    readback = S6B1713_ReadData(); 
    	S6B1713_WriteData( readback ^ y2PixelMask );
	}
}


void LCD_SetBox( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 )
{
	// Swap x1 and x2 if x1 is larger.
	if (x1 > x2) {
		uint8_t temp = x1;
		x1 = x2;
		x2 = temp;
	}

	// Swap y1 and y2 if y1 is larger.
	if (y1 > y2) {
		uint8_t temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	// Prepare some.
	uint8_t y1Page = y1 >> 3;
	uint8_t y2Page = y2 >> 3;
	
	uint8_t y1Bit = y1 & 0x07;
	uint8_t y2Bit = y2 & 0x07;
	
	uint8_t y1PixelMask = 0xff << y1Bit;
	uint8_t y2PixelMask = 0xff >> (7 - y2Bit);
	
	uint8_t width = x2 - x1 + 1;
	
	// Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// These variables are used often below.
	volatile uint8_t dummy;
	uint8_t readback;
	uint8_t columnsLeft;

	// If y1 and y2 are in within same 8-pixel column, we must combine bitmasks.
	if (y1Page == y2Page) {
		uint8_t pixelMask = y1PixelMask & y2PixelMask;

		// Set starting address.
    	S6B1713_SetPageAddress( y1Page );
	    S6B1713_SetColumnAddress( x1 );

		// Update whole width.
		columnsLeft = width;
		do {
		    // A dummy read is required after column address change, before reading.
			dummy = S6B1713_ReadData(); 

			// Read 8-pixel column, set correct pixels and write back.
			readback = S6B1713_ReadData(); 
			S6B1713_WriteData( readback | pixelMask );
		} while (--columnsLeft > 0);
	}
	// Else, if y1 and y2 are in different pages, update all affected columns.
	else {
		int8_t fullColumns = y2Page - y1Page - 1; // Number of full 8-pixel columns.
			
		// --- Update pixels in y1's page. ---
		
		// Set starting address.
		S6B1713_SetPageAddress( y1Page );
		S6B1713_SetColumnAddress( x1 );

		// Update whole width.
		columnsLeft = width;
		do {
		    // A dummy read is required after column address change, before reading.
			dummy = S6B1713_ReadData(); 

			// Read 8-pixel column, set correct pixels and write back.
			readback = S6B1713_ReadData(); 
			S6B1713_WriteData( readback | y1PixelMask );
		} while (--columnsLeft > 0);
		
		// --- Update 8-pixel columns in between y1 and y2 pages. ---
		
		uint8_t page = y1Page + 1;
		while (fullColumns-- > 0) {
			// Set starting address.
    		S6B1713_SetPageAddress( page++ );
		    S6B1713_SetColumnAddress( x1 );

			// Set all pixels.
			columnsLeft = width;
			do {
				S6B1713_WriteData( 0xff );
			} while (--columnsLeft > 0);
		}
		
		// --- Update pixels in y2's page. ---
		
		// Set starting address.
    	S6B1713_SetPageAddress( y2Page );
	    S6B1713_SetColumnAddress( x1 );

		// Update whole width.
		columnsLeft = width;
		do {
		    // A dummy read is required after column address change, before reading.
			dummy = S6B1713_ReadData(); 

			// Read 8-pixel column, set correct pixels and write back.
		    readback = S6B1713_ReadData(); 
    		S6B1713_WriteData( readback | y2PixelMask );
		} while (--columnsLeft > 0);
	}
}


void LCD_ClrBox( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 )
{
	// Swap x1 and x2 if x1 is larger.
	if (x1 > x2) {
		uint8_t temp = x1;
		x1 = x2;
		x2 = temp;
	}

	// Swap y1 and y2 if y1 is larger.
	if (y1 > y2) {
		uint8_t temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	// Prepare some.
	uint8_t y1Page = y1 >> 3;
	uint8_t y2Page = y2 >> 3;
	
	uint8_t y1Bit = y1 & 0x07;
	uint8_t y2Bit = y2 & 0x07;
	
	uint8_t y1PixelMask = ~(0xff << y1Bit);
	uint8_t y2PixelMask = ~(0xff >> (7 - y2Bit));
	
	uint8_t width = x2 - x1 + 1;
	
	// Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// These variables are used often below.
	volatile uint8_t dummy;
	uint8_t readback;
	uint8_t columnsLeft;

	// If y1 and y2 are in within same 8-pixel column, we must combine bitmasks.
	if (y1Page == y2Page) {
		uint8_t pixelMask = y1PixelMask | y2PixelMask;

		// Set starting address.
    	S6B1713_SetPageAddress( y1Page );
	    S6B1713_SetColumnAddress( x1 );

		// Update whole width.
		columnsLeft = width;
		do {
		    // A dummy read is required after column address change, before reading.
			dummy = S6B1713_ReadData(); 

			// Read 8-pixel column, clear correct pixels and write back.
			readback = S6B1713_ReadData(); 
			S6B1713_WriteData( readback & pixelMask );
		} while (--columnsLeft > 0);
	}
	// Else, if y1 and y2 are in different pages, update all affected columns.
	else {
		int8_t fullColumns = y2Page - y1Page - 1; // Number of full 8-pixel columns.
			
		// --- Update pixels in y1's page. ---
		
		// Set starting address.
		S6B1713_SetPageAddress( y1Page );
		S6B1713_SetColumnAddress( x1 );

		// Update whole width.
		columnsLeft = width;
		do {
		    // A dummy read is required after column address change, before reading.
			dummy = S6B1713_ReadData(); 

			// Read 8-pixel column, clear correct pixels and write back.
			readback = S6B1713_ReadData(); 
			S6B1713_WriteData( readback & y1PixelMask );
		} while (--columnsLeft > 0);
		
		// --- Update 8-pixel columns in between y1 and y2 pages. ---
		
		uint8_t page = y1Page + 1;
		while (fullColumns-- > 0) {
			// Set starting address.
    		S6B1713_SetPageAddress( page++ );
		    S6B1713_SetColumnAddress( x1 );

			// Clear all pixels.
			columnsLeft = width;
			do {
				S6B1713_WriteData( 0x00 );
			} while (--columnsLeft > 0);
		}
		
		// --- Update pixels in y2's page. ---
		
		// Set starting address.
    	S6B1713_SetPageAddress( y2Page );
	    S6B1713_SetColumnAddress( x1 );

		// Update whole width.
		columnsLeft = width;
		do {
		    // A dummy read is required after column address change, before reading.
			dummy = S6B1713_ReadData(); 

			// Read 8-pixel column, clear correct pixels and write back.
		    readback = S6B1713_ReadData(); 
    		S6B1713_WriteData( readback & y2PixelMask );
		} while (--columnsLeft > 0);
	}
}


void LCD_FlipBox( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 )
{
	// Swap x1 and x2 if x1 is larger.
	if (x1 > x2) {
		uint8_t temp = x1;
		x1 = x2;
		x2 = temp;
	}

	// Swap y1 and y2 if y1 is larger.
	if (y1 > y2) {
		uint8_t temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	// Prepare some.
	uint8_t y1Page = y1 >> 3;
	uint8_t y2Page = y2 >> 3;
	
	uint8_t y1Bit = y1 & 0x07;
	uint8_t y2Bit = y2 & 0x07;
	
	uint8_t y1PixelMask = 0xff << y1Bit;
	uint8_t y2PixelMask = 0xff >> (7 - y2Bit);
	
	uint8_t width = x2 - x1 + 1;
	
	// Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// These variables are used often below.
	volatile uint8_t dummy;
	uint8_t readback;
	uint8_t columnsLeft;

	// If y1 and y2 are in within same 8-pixel column, we must combine bitmasks.
	if (y1Page == y2Page) {
		uint8_t pixelMask = y1PixelMask & y2PixelMask;

		// Set starting address.
    	S6B1713_SetPageAddress( y1Page );
	    S6B1713_SetColumnAddress( x1 );

		// Update whole width.
		columnsLeft = width;
		do {
		    // A dummy read is required after column address change, before reading.
			dummy = S6B1713_ReadData(); 

			// Read 8-pixel column, flip correct pixels and write back.
			readback = S6B1713_ReadData(); 
			S6B1713_WriteData( readback ^ pixelMask );
		} while (--columnsLeft > 0);
	}
	// Else, if y1 and y2 are in different pages, update all affected columns.
	else {
		int8_t fullColumns = y2Page - y1Page - 1; // Number of full 8-pixel columns.
			
		// --- Update pixels in y1's page. ---
		
		// Set starting address.
		S6B1713_SetPageAddress( y1Page );
		S6B1713_SetColumnAddress( x1 );

		// Update whole width.
		columnsLeft = width;
		do {
		    // A dummy read is required after column address change, before reading.
			dummy = S6B1713_ReadData(); 

			// Read 8-pixel column, flip correct pixels and write back.
			readback = S6B1713_ReadData(); 
			S6B1713_WriteData( readback ^ y1PixelMask );
		} while (--columnsLeft > 0);
		
		// --- Update 8-pixel columns in between y1 and y2 pages. ---
		
		uint8_t page = y1Page + 1;
		while (fullColumns-- > 0) {
			// Set starting address.
    		S6B1713_SetPageAddress( page++ );
		    S6B1713_SetColumnAddress( x1 );

			// Flip all pixels.
			columnsLeft = width;
			do {
				// A dummy read is required after column address change, before reading.
				dummy = S6B1713_ReadData(); 
	
				// Read 8-pixel column, flip all pixels and write back.
				readback = S6B1713_ReadData(); 
				S6B1713_WriteData( readback ^ 0xff );
			} while (--columnsLeft > 0);
		}
		
		// --- Update pixels in y2's page. ---
		
		// Set starting address.
    	S6B1713_SetPageAddress( y2Page );
	    S6B1713_SetColumnAddress( x1 );

		// Update whole width.
		columnsLeft = width;
		do {
		    // A dummy read is required after column address change, before reading.
			dummy = S6B1713_ReadData(); 

			// Read 8-pixel column, flip correct pixel and write back.
		    readback = S6B1713_ReadData(); 
    		S6B1713_WriteData( readback ^ y2PixelMask );
		} while (--columnsLeft > 0);
	}
}


void LCD_WritePage( const uint8_t * data, uint8_t page, uint8_t column, uint8_t width )
{
	// Set current address to start of current 8-pixel high page.
	S6B1713_SetPageAddress( page );
	S6B1713_SetColumnAddress( column );

	// Write "size" bytes (size*8 pixels).
	do {
		S6B1713_WriteData( *data++ );
	} while (--width > 0);
}


void LCD_MergePage( const uint8_t * data, uint8_t page, uint8_t column, uint8_t width )
{
    // Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// Set current address to start of current 8-pixel high page.
	S6B1713_SetPageAddress( page );
	S6B1713_SetColumnAddress( column );

	// Merge (bitwise OR) "size" bytes (size*8 pixels) from data into page.
	do {
	    // A dummy read is required after column address change, before reading.
	    volatile uint8_t dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, XOR with data column and write back.
		uint8_t readback = S6B1713_ReadData();
		S6B1713_WriteData( readback | *data++ );
	} while (--width > 0);
}


void LCD_XORPage( const uint8_t * data, uint8_t page, uint8_t column, uint8_t width )
{
    // Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// Set current address to start of current 8-pixel high page.
	S6B1713_SetPageAddress( page );
	S6B1713_SetColumnAddress( column );

	// XOR "size" bytes (size*8 pixels) from data into page.
	do {
	    // A dummy read is required after column address change, before reading.
	    volatile uint8_t dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, XOR with data column and write back.
		uint8_t readback = S6B1713_ReadData();
		S6B1713_WriteData( readback ^ *data++ );
	} while (--width > 0);
}


void LCD_SetPage( uint8_t pixelMask, uint8_t page, uint8_t column, uint8_t width )
{
    // Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// Set current address to start of current 8-pixel high page.
	S6B1713_SetPageAddress( page );
	S6B1713_SetColumnAddress( column );

	// XOR pixel mask into page.
	do {
	    // A dummy read is required after column address change, before reading.
	    volatile uint8_t dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, set pixels and write back.
		uint8_t readback = S6B1713_ReadData();
		S6B1713_WriteData( readback | pixelMask );
	} while (--width > 0);
}


void LCD_ClrPage( uint8_t pixelMask, uint8_t page, uint8_t column, uint8_t width )
{
    // Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// Set current address to start of current 8-pixel high page.
	S6B1713_SetPageAddress( page );
	S6B1713_SetColumnAddress( column );

	// XOR pixel mask into page.
	uint8_t inversePixelMask = ~pixelMask;
	do {
	    // A dummy read is required after column address change, before reading.
	    volatile uint8_t dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, clear pixels and write back.
		uint8_t readback = S6B1713_ReadData();
		S6B1713_WriteData( readback & inversePixelMask );
	} while (--width > 0);
}


void LCD_FlipPage( uint8_t pixelMask, uint8_t page, uint8_t column, uint8_t width )
{
    // Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// Set current address to start of current 8-pixel high page.
	S6B1713_SetPageAddress( page );
	S6B1713_SetColumnAddress( column );

	// XOR pixel mask into page.
	do {
	    // A dummy read is required after column address change, before reading.
	    volatile uint8_t dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, XOR with pixel mask and write back.
		uint8_t readback = S6B1713_ReadData();
		S6B1713_WriteData( readback ^ pixelMask );
	} while (--width > 0);
}


void LCD_ReadPage( uint8_t * data, uint8_t page, uint8_t column, uint8_t width )
{
    // Make sure LCD address auto-increments after each data read.
    S6B1713_SetReadModifyWriteOff();

	// Set current address to start of current 8-pixel high page.
	S6B1713_SetPageAddress( page );
	S6B1713_SetColumnAddress( column );

    // A dummy read is required after column address change, before reading.
    volatile uint8_t dummy = S6B1713_ReadData(); 

	// Read "size" bytes (size*8 pixels).
	do {
		*data++ = S6B1713_ReadData();
	} while (--width > 0);
}


void LCD_SetBuffer( uint8_t * buffer, uint8_t pixelMask )
{
	uint16_t size = LCD_WIDTH * (LCD_HEIGHT/8);
	do {
		*buffer++ = pixelMask;
	} while (--size > 0);
}

	
void LCD_SetBufferPixel( uint8_t * buffer, uint8_t x, uint8_t y )
{
	uint16_t bufferIdx = ((y >> 3) * LCD_WIDTH) + x;
	uint8_t pixelMask = 1 << (y & 0x07);
	
	buffer[bufferIdx] |= pixelMask;
}


void LCD_ClrBufferPixel( uint8_t * buffer, uint8_t x, uint8_t y )
{
	uint16_t bufferIdx = ((y >> 3) * LCD_WIDTH) + x;
	uint8_t inversePixelMask = ~(1 << (y & 0x07));

	buffer[bufferIdx] &= inversePixelMask;
}


void LCD_FlipBufferPixel( uint8_t * buffer, uint8_t x, uint8_t y )
{
	uint16_t bufferIdx = ((y >> 3) * LCD_WIDTH) + x;
	uint8_t pixelMask = 1 << (y & 0x07);

	buffer[bufferIdx] ^= pixelMask;
}


uint8_t LCD_GetBufferPixel( const uint8_t * buffer, uint8_t x, uint8_t y )
{
	uint16_t bufferIdx = ((y >> 3) * LCD_WIDTH) + x;
	uint8_t pixelMask = 1 << (y & 0x07);

	uint8_t maskedPixel = (buffer[bufferIdx] & pixelMask);
	return maskedPixel;
}


void LCD_SetBufferHLine( uint8_t * buffer, uint8_t x1, uint8_t x2, uint8_t y )
{
	// Swap x1 and x2 if x1 is larger.
	if (x1 > x2) {
		uint8_t temp = x1;
		x1 = x2;
		x2 = temp;
	}

	// Prepare some.
	uint16_t bufferIdx = ((y >> 3) * LCD_WIDTH) + x1;
	uint8_t pixelMask = 1 << (y & 0x07);

	// Read 8-pixel high columns, set correct pixel and write back.
	buffer += bufferIdx;
	uint8_t width = x2 - x1 + 1;
	do {
		*buffer++ |= pixelMask;
	} while (--width > 0);
}


void LCD_ClrBufferHLine( uint8_t * buffer, uint8_t x1, uint8_t x2, uint8_t y )
{
	// Swap x1 and x2 if x1 is larger.
	if (x1 > x2) {
		uint8_t temp = x1;
		x1 = x2;
		x2 = temp;
	}

	// Prepare some.
	uint16_t bufferIdx = ((y >> 3) * LCD_WIDTH) + x1;
	uint8_t pixelMask = ~(1 << (y & 0x07));

	// Read 8-pixel high columns, clear correct pixel and write back.
	buffer += bufferIdx;
	uint8_t width = x2 - x1 + 1;
	do {
		*buffer++ &= pixelMask;
	} while (--width > 0);
}


void LCD_FlipBufferHLine( uint8_t * buffer, uint8_t x1, uint8_t x2, uint8_t y )
{
	// Swap x1 and x2 if x1 is larger.
	if (x1 > x2) {
		uint8_t temp = x1;
		x1 = x2;
		x2 = temp;
	}

	// Prepare some.
	uint16_t bufferIdx = ((y >> 3) * LCD_WIDTH) + x1;
	uint8_t pixelMask = 1 << (y & 0x07);

	// Read 8-pixel high columns, flip correct pixel and write back.
	buffer += bufferIdx;
	uint8_t width = x2 - x1 + 1;
	do {
		*buffer++ ^= pixelMask;
	} while (--width > 0);
}


void LCD_SetBufferVLine( uint8_t * buffer, uint8_t x, uint8_t y1, uint8_t y2 )
{
	// Swap y1 and y2 if y1 is larger.
	if (y1 > y2) {
		uint8_t temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	// Prepare some.
	uint8_t y1Page = y1 >> 3;
	uint8_t y2Page = y2 >> 3;
	
	uint8_t y1Bit = y1 & 0x07;
	uint8_t y2Bit = y2 & 0x07;
	
	uint8_t y1PixelMask = 0xff << y1Bit;
	uint8_t y2PixelMask = 0xff >> (7 - y2Bit);
	
	// If y1 and y2 are in within same 8-pixel column, we must combine bitmasks.
	if (y1Page == y2Page) {
		uint8_t pixelMask = y1PixelMask & y2PixelMask;

		// Read 8-pixel column, set correct pixels and write back.
		uint16_t bufferIdx = (y1Page * LCD_WIDTH) + x;
		buffer[bufferIdx] |= pixelMask;
	}
	// Else, if y1 and y2 are in different pages, update all affected columns.
	else {
		int8_t fullColumns = y2Page - y1Page - 1; // Number of full 8-pixel columns.
		uint16_t bufferIdx = (y1Page * LCD_WIDTH) + x;
		buffer += bufferIdx;
			
		// --- Update pixels in y1's page. ---
		
		// Read 8-pixel column, set correct pixels and write back.
		*buffer |= y1PixelMask;
		
		// --- Update 8-pixel columns in between y1 and y2 pages. ---
		
		while (fullColumns-- > 0) {
			// Set all pixels.
			buffer += LCD_WIDTH;
			*buffer = 0xff;
		}
		
		// --- Update pixels in y2's page. ---
		
		// Read 8-pixel column, set correct pixels and write back.
		buffer += LCD_WIDTH;
		*buffer |= y2PixelMask;
	}
}


void LCD_ClrBufferVLine( uint8_t * buffer, uint8_t x, uint8_t y1, uint8_t y2 )
{
	// Swap y1 and y2 if y1 is larger.
	if (y1 > y2) {
		uint8_t temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	// Prepare some.
	uint8_t y1Page = y1 >> 3;
	uint8_t y2Page = y2 >> 3;
	
	uint8_t y1Bit = y1 & 0x07;
	uint8_t y2Bit = y2 & 0x07;
	
	uint8_t y1PixelMask = ~(0xff << y1Bit);
	uint8_t y2PixelMask = ~(0xff >> (7 - y2Bit));
	
	// If y1 and y2 are in within same 8-pixel column, we must combine bitmasks.
	if (y1Page == y2Page) {
		uint8_t pixelMask = y1PixelMask | y2PixelMask;

		// Read 8-pixel column, clear correct pixels and write back.
		uint16_t bufferIdx = (y1Page * LCD_WIDTH) + x;
		buffer[bufferIdx] &= pixelMask;
	}
	// Else, if y1 and y2 are in different pages, update all affected columns.
	else {
		int8_t fullColumns = y2Page - y1Page - 1; // Number of full 8-pixel columns.
		uint16_t bufferIdx = (y1Page * LCD_WIDTH) + x;
		buffer += bufferIdx;
			
		// --- Update pixels in y1's page. ---
		
		// Read 8-pixel column, clear correct pixels and write back.
		*buffer &= y1PixelMask;
		
		// --- Update 8-pixel columns in between y1 and y2 pages. ---
		
		while (fullColumns-- > 0) {
			// Set all pixels.
			buffer += LCD_WIDTH;
			*buffer = 0x00;
		}
		
		// --- Update pixels in y2's page. ---
		
		// Read 8-pixel column, clear correct pixels and write back.
		buffer += LCD_WIDTH;
		*buffer &= y2PixelMask;
	}
}


void LCD_FlipBufferVLine( uint8_t * buffer, uint8_t x, uint8_t y1, uint8_t y2 )
{
	// Swap y1 and y2 if y1 is larger.
	if (y1 > y2) {
		uint8_t temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	// Prepare some.
	uint8_t y1Page = y1 >> 3;
	uint8_t y2Page = y2 >> 3;
	
	uint8_t y1Bit = y1 & 0x07;
	uint8_t y2Bit = y2 & 0x07;
	
	uint8_t y1PixelMask = 0xff << y1Bit;
	uint8_t y2PixelMask = 0xff >> (7 - y2Bit);
	
	// If y1 and y2 are in within same 8-pixel column, we must combine bitmasks.
	if (y1Page == y2Page) {
		uint8_t pixelMask = y1PixelMask & y2PixelMask;

		// Read 8-pixel column, flip correct pixels and write back.
		uint16_t bufferIdx = (y1Page * LCD_WIDTH) + x;
		buffer[bufferIdx] ^= pixelMask;
	}
	// Else, if y1 and y2 are in different pages, update all affected columns.
	else {
		int8_t fullColumns = y2Page - y1Page - 1; // Number of full 8-pixel columns.
		uint16_t bufferIdx = (y1Page * LCD_WIDTH) + x;
		buffer += bufferIdx;
			
		// --- Update pixels in y1's page. ---
		
		// Read 8-pixel column, clip correct pixels and write back.
		*buffer ^= y1PixelMask;
		
		// --- Update 8-pixel columns in between y1 and y2 pages. ---
		
		while (fullColumns-- > 0) {
			// Flip all pixels.
			buffer += LCD_WIDTH;
			*buffer ^= 0xff;
		}
		
		// --- Update pixels in y2's page. ---
		
		// Read 8-pixel column, flip correct pixels and write back.
		buffer += LCD_WIDTH;
		*buffer ^= y2PixelMask;
	}
}


void LCD_SetBufferBox( uint8_t * buffer, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 )
{
	// Swap x1 and x2 if x1 is larger.
	if (x1 > x2) {
		uint8_t temp = x1;
		x1 = x2;
		x2 = temp;
	}

	// Swap y1 and y2 if y1 is larger.
	if (y1 > y2) {
		uint8_t temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	// Prepare some.
	uint8_t y1Page = y1 >> 3;
	uint8_t y2Page = y2 >> 3;
	
	uint8_t y1Bit = y1 & 0x07;
	uint8_t y2Bit = y2 & 0x07;
	
	uint8_t y1PixelMask = 0xff << y1Bit;
	uint8_t y2PixelMask = 0xff >> (7 - y2Bit);
	
	uint8_t width = x2 - x1 + 1;
	
	// This variable is used often below.
	uint8_t columnsLeft;

	// If y1 and y2 are in within same 8-pixel column, we must combine bitmasks.
	if (y1Page == y2Page) {
		uint16_t bufferIdx = (y1Page * LCD_WIDTH) + x1;
		uint8_t pixelMask = y1PixelMask & y2PixelMask;

		// Update whole width.
		columnsLeft = width;
		buffer += bufferIdx;
		do {
			// Read 8-pixel column, set correct pixels and write back.
			*buffer++ |= pixelMask;
		} while (--columnsLeft > 0);
	}
	// Else, if y1 and y2 are in different pages, update all affected columns.
	else {
		int8_t fullColumns = y2Page - y1Page - 1; // Number of full 8-pixel columns.
			
		// --- Update pixels in y1's page. ---

		uint16_t bufferIdx = (y1Page * LCD_WIDTH) + x1;		

		// Update whole width.
		columnsLeft = width;
		buffer += bufferIdx;
		do {
			// Read 8-pixel column, set correct pixels and write back.
			*buffer++ |= y1PixelMask;
		} while (--columnsLeft > 0);
		
		// --- Update 8-pixel columns in between y1 and y2 pages. ---
		
		while (fullColumns-- > 0) {
			// Set all pixels.
			buffer += LCD_WIDTH - width;
			columnsLeft = width;
			do {
				*buffer++ = 0xff;
			} while (--columnsLeft > 0);
		}
		
		// --- Update pixels in y2's page. ---
		
		// Update whole width.
		columnsLeft = width;
		buffer += LCD_WIDTH - width;
		do {
			// Read 8-pixel column, set correct pixels and write back.
			*buffer++ |= y2PixelMask;
		} while (--columnsLeft > 0);
	}
}


void LCD_ClrBufferBox( uint8_t * buffer, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 )
{
	// Swap x1 and x2 if x1 is larger.
	if (x1 > x2) {
		uint8_t temp = x1;
		x1 = x2;
		x2 = temp;
	}

	// Swap y1 and y2 if y1 is larger.
	if (y1 > y2) {
		uint8_t temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	// Prepare some.
	uint8_t y1Page = y1 >> 3;
	uint8_t y2Page = y2 >> 3;
	
	uint8_t y1Bit = y1 & 0x07;
	uint8_t y2Bit = y2 & 0x07;
	
	uint8_t y1PixelMask = ~(0xff << y1Bit);
	uint8_t y2PixelMask = ~(0xff >> (7 - y2Bit));
	
	uint8_t width = x2 - x1 + 1;
	
	// This variable is used often below.
	uint8_t columnsLeft;

	// If y1 and y2 are in within same 8-pixel column, we must combine bitmasks.
	if (y1Page == y2Page) {
		uint16_t bufferIdx = (y1Page * LCD_WIDTH) + x1;
		uint8_t pixelMask = y1PixelMask | y2PixelMask;

		// Update whole width.
		columnsLeft = width;
		buffer += bufferIdx;
		do {
			// Read 8-pixel column, clear correct pixels and write back.
			*buffer++ &= pixelMask;
		} while (--columnsLeft > 0);
	}
	// Else, if y1 and y2 are in different pages, update all affected columns.
	else {
		int8_t fullColumns = y2Page - y1Page - 1; // Number of full 8-pixel columns.
			
		// --- Update pixels in y1's page. ---

		uint16_t bufferIdx = (y1Page * LCD_WIDTH) + x1;		

		// Update whole width.
		columnsLeft = width;
		buffer += bufferIdx;
		do {
			// Read 8-pixel column, clear correct pixels and write back.
			*buffer++ &= y1PixelMask;
		} while (--columnsLeft > 0);
		
		// --- Update 8-pixel columns in between y1 and y2 pages. ---
		
		while (fullColumns-- > 0) {
			// Clear all pixels.
			buffer += LCD_WIDTH - width;
			columnsLeft = width;
			do {
				*buffer++ = 0x00;
			} while (--columnsLeft > 0);
		}
		
		// --- Update pixels in y2's page. ---
		
		// Update whole width.
		columnsLeft = width;
		buffer += LCD_WIDTH - width;
		do {
			// Read 8-pixel column, clear correct pixels and write back.
			*buffer++ &= y2PixelMask;
		} while (--columnsLeft > 0);
	}
}


void LCD_FlipBufferBox( uint8_t * buffer, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2 )
{
	// Swap x1 and x2 if x1 is larger.
	if (x1 > x2) {
		uint8_t temp = x1;
		x1 = x2;
		x2 = temp;
	}

	// Swap y1 and y2 if y1 is larger.
	if (y1 > y2) {
		uint8_t temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	// Prepare some.
	uint8_t y1Page = y1 >> 3;
	uint8_t y2Page = y2 >> 3;
	
	uint8_t y1Bit = y1 & 0x07;
	uint8_t y2Bit = y2 & 0x07;
	
	uint8_t y1PixelMask = 0xff << y1Bit;
	uint8_t y2PixelMask = 0xff >> (7 - y2Bit);
	
	uint8_t width = x2 - x1 + 1;
	
	// This variable is used often below.
	uint8_t columnsLeft;

	// If y1 and y2 are in within same 8-pixel column, we must combine bitmasks.
	if (y1Page == y2Page) {
		uint16_t bufferIdx = (y1Page * LCD_WIDTH) + x1;
		uint8_t pixelMask = y1PixelMask & y2PixelMask;

		// Update whole width.
		columnsLeft = width;
		buffer += bufferIdx;
		do {
			// Read 8-pixel column, flip correct pixels and write back.
			*buffer++ ^= pixelMask;
		} while (--columnsLeft > 0);
	}
	// Else, if y1 and y2 are in different pages, update all affected columns.
	else {
		int8_t fullColumns = y2Page - y1Page - 1; // Number of full 8-pixel columns.
			
		// --- Update pixels in y1's page. ---

		uint16_t bufferIdx = (y1Page * LCD_WIDTH) + x1;		

		// Update whole width.
		columnsLeft = width;
		buffer += bufferIdx;
		do {
			// Read 8-pixel column, flip correct pixels and write back.
			*buffer++ ^= y1PixelMask;
		} while (--columnsLeft > 0);
		
		// --- Update 8-pixel columns in between y1 and y2 pages. ---
		
		while (fullColumns-- > 0) {
			// Flip all pixels.
			buffer += LCD_WIDTH - width;
			columnsLeft = width;
			do {
				*buffer++ ^= 0xff;
			} while (--columnsLeft > 0);
		}
		
		// --- Update pixels in y2's page. ---
		
		// Update whole width.
		columnsLeft = width;
		buffer += LCD_WIDTH - width;
		do {
			// Read 8-pixel column, flip correct pixels and write back.
			*buffer++ ^= y2PixelMask;
		} while (--columnsLeft > 0);
	}
}


void LCD_WriteBufferPage( uint8_t * buffer, const uint8_t * data, uint8_t page, uint8_t column, uint8_t width )
{
	// Move buffer pointer to correct location.
	uint16_t bufferIdx = (page * LCD_WIDTH) + column;
	buffer += bufferIdx;

	// Write "size" bytes (size*8 pixels).
	do {
		*buffer++ = *data++;
	} while (--width > 0);
}


void LCD_MergeBufferPage( uint8_t * buffer, const uint8_t * data, uint8_t page, uint8_t column, uint8_t width )
{
	// Move buffer pointer to correct location.
	uint16_t bufferIdx = (page * LCD_WIDTH) + column;
	buffer += bufferIdx;

	// Merge (bitwise OR) "size" bytes (size*8 pixels) from data into buffer page.
	do {
		*buffer++ |= *data++;
	} while (--width > 0);
}


void LCD_XORBufferPage( uint8_t * buffer, const uint8_t * data, uint8_t page, uint8_t column, uint8_t width )
{
	// Move buffer pointer to correct location.
	uint16_t bufferIdx = (page * LCD_WIDTH) + column;
	buffer += bufferIdx;

	// XOR "size" bytes (size*8 pixels) from data into buffer page.
	do {
		*buffer++ ^= *data++;
	} while (--width > 0);
}


void LCD_SetBufferPage( uint8_t * buffer, uint8_t pixelMask, uint8_t page, uint8_t column, uint8_t width )
{
	// Move buffer pointer to correct location.
	uint16_t bufferIdx = (page * LCD_WIDTH) + column;
	buffer += bufferIdx;

	// Set pixels in buffer page.
	do {
		*buffer++ |= pixelMask;
	} while (--width > 0);
}


void LCD_ClrBufferPage( uint8_t * buffer, uint8_t pixelMask, uint8_t page, uint8_t column, uint8_t width )
{
	// Move buffer pointer to correct location.
	uint16_t bufferIdx = (page * LCD_WIDTH) + column;
	buffer += bufferIdx;

	// XOR pixel mask into buffer page.
	uint8_t inversePixelMask = ~pixelMask;
	do {
		*buffer++ &= inversePixelMask;
	} while (--width > 0);
}


void LCD_FlipBufferPage( uint8_t * buffer, uint8_t pixelMask, uint8_t page, uint8_t column, uint8_t width )
{
	// Move buffer pointer to correct location.
	uint16_t bufferIdx = (page * LCD_WIDTH) + column;
	buffer += bufferIdx;

	// XOR pixel mask into buffer page.
	do {
		*buffer++ ^= pixelMask;
	} while (--width > 0);
}


void LCD_ReadBufferPage( const uint8_t * buffer, uint8_t * data, uint8_t page, uint8_t column, uint8_t width )
{
	// Move buffer pointer to correct location.
	uint16_t bufferIdx = (page * LCD_WIDTH) + column;
	buffer += bufferIdx;

	// Read "size" bytes (size*8 pixels).
	do {
		*data++ = *buffer++;
	} while (--width > 0);
}


void LCD_WriteFrameBuffer( const uint8_t * buffer )
{
	// Copy data from SRAM to LCD module, page by page.
	for (uint8_t page = 0; page < 8; ++page) {
		// Set current address to start of current 8-pixel high page.
		S6B1713_SetPageAddress( page );
		S6B1713_SetColumnAddress( 0 );

		// Write LCD_WIDTH bytes (LCD_WIDTHx8 pixels).
		uint8_t width = LCD_WIDTH;
		do {
			S6B1713_WriteData( *buffer++ );
		} while (--width > 0);
	}
}


void LCD_ReadFrameBuffer( uint8_t * buffer )
{
    // Make sure LCD address auto-increments after each data read.
    S6B1713_SetReadModifyWriteOff();

	// Copy data from LCD module to SRAM, page by page.
	for (uint8_t page = 0; page < 8; ++page) {
		// Set current address to start of current 8-pixel high page.
		S6B1713_SetPageAddress( page );
		S6B1713_SetColumnAddress( 0 );

		// A dummy read is required after column address change, before reading.
	    volatile uint8_t dummy = S6B1713_ReadData();

		// Read LCD_WIDTH bytes (LCD_WIDTHx8 pixels).
		uint8_t width = LCD_WIDTH;
		do {
			*buffer++ = S6B1713_ReadData();
		} while (--width > 0);
	}
}


void LCD_WritePage_F( const uint8_t CAL_PGM(* data), uint8_t page, uint8_t column, uint8_t width )
{
	// Set current address to start of current 8-pixel high page.
	S6B1713_SetPageAddress( page );
	S6B1713_SetColumnAddress( column );

	// Write "size" bytes (size*8 pixels).
	do {
		// S6B1713_WriteData( *data++ );
		S6B1713_WriteData( CAL_pgm_read_byte( data ) );
		data++;
	} while (--width > 0);
}


void LCD_MergePage_F( const uint8_t CAL_PGM(* data), uint8_t page, uint8_t column, uint8_t width )
{
    // Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

	// Set current address to start of current 8-pixel high page.
	S6B1713_SetPageAddress( page );
	S6B1713_SetColumnAddress( column );

	// Merge (bitwise OR) "size" bytes (size*8 pixels) from data into page.
	do {
		// A dummy read is required after column address change, before reading.
		volatile uint8_t dummy = S6B1713_ReadData(); 

		// Read 8-pixel column, XOR with data column and write back.
		uint8_t readback = S6B1713_ReadData();
		S6B1713_WriteData( readback | CAL_pgm_read_byte( data ) );
		data++;
	} while (--width > 0);
}


void LCD_XORPage_F( const uint8_t CAL_PGM(* data), uint8_t page, uint8_t column, uint8_t width )
{
    // Make sure data read does not auto-increment address, so that we can read-modify-write.
    S6B1713_SetReadModifyWriteOn();

    // Set current address to start of current 8-pixel high page.
    S6B1713_SetPageAddress( page );
    S6B1713_SetColumnAddress( column );

    // XOR "size" bytes (size*8 pixels) from data into page.
    do {
        // A dummy read is required after column address change, before reading.
        volatile uint8_t dummy = S6B1713_ReadData(); 

        // Read 8-pixel column, XOR with data column and write back.
        uint8_t readback = S6B1713_ReadData();
        S6B1713_WriteData( readback ^ CAL_pgm_read_byte(data) );
        data++;
    } while (--width > 0);
}


void LCD_WriteBufferPage_F( uint8_t * buffer, const uint8_t CAL_PGM(* data), uint8_t page, uint8_t column, uint8_t width )
{
    // Move buffer pointer to correct location.
    uint16_t bufferIdx = (page * LCD_WIDTH) + column;
    buffer += bufferIdx;

    // Write "size" bytes (size*8 pixels).
    do {
        *buffer++ = CAL_pgm_read_byte(data);
        data++;
    } while (--width > 0);

}


void LCD_MergeBufferPage_F( uint8_t * buffer, const uint8_t CAL_PGM(* data), uint8_t page, uint8_t column, uint8_t width )
{
    // Move buffer pointer to correct location.
    uint16_t bufferIdx = (page * LCD_WIDTH) + column;
    buffer += bufferIdx;

    // Merge (bitwise OR) "size" bytes (size*8 pixels) from data into buffer page.
    do {
        *buffer++ |= CAL_pgm_read_byte(data);
        data++;
    } while (--width > 0);
}


void LCD_XORBufferPage_F( uint8_t * buffer, const uint8_t CAL_PGM(* data), uint8_t page, uint8_t column, uint8_t width )
{
    // Move buffer pointer to correct location.
    uint16_t bufferIdx = (page * LCD_WIDTH) + column;
    buffer += bufferIdx;

    // XOR "size" bytes (size*8 pixels) from data into buffer page.
    do {
        *buffer++ ^= CAL_pgm_read_byte(data);
        data++;
    } while (--width > 0);
}


// end of file
