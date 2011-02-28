// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 picture library source file.
 *
 *      The picture library provides functions for copying images or sections of
 *      images back and forth between the LCD module and SRAM or Flash memory.
 *      Pictures are stored in the same page-oriented format as used by the LCD
 *      controller itself, with the first byte being the upper-left column of
 *      eight pixels and the second byte being the next eight-pixel column to
 *      the right. For the same reason, the vertical resolution when positioning
 *      images is eight pixels.
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
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/Picture_lib/picture_lib.c $
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

#include "picture_lib.h"
#include <lcd_lib.h>



void PICTURE_ClearScreen(void)
{
  LCD_SetScreen( 0x00 ); // Fill screen with 0x00.
}


/*!
 * \param  pPictureInRam  Pointer to picture in ram with same size as LCD
 */
void PICTURE_CopyFullscreenRamToLcd (const uint8_t * pPictureInRam)
{
  LCD_WriteFrameBuffer( pPictureInRam );
}


/*!
 * \param  pPictureInFlash  Pointer to picture in flash with same size as LCD
 */
void PICTURE_CopyFullscreenFlashToLcd ( const uint8_t CAL_PGM(* pPictureInFlash))
{
  uint8_t page = 0;
  do{
    LCD_WritePage_F( pPictureInFlash, page, 0, LCD_WIDTH);  // Write full row with page height.
    pPictureInFlash += LCD_WIDTH;
    page++;
  }while (page < LCD_PAGE_HEIGHT);
}


/*!
 * \param  pDestInRam  Pointer to allocated memory in ram where to save the contents of the LCD
 */
void PICTURE_CopyFullscreenLcdToRam ( uint8_t * pDestInRam )
{
  uint8_t sourcePage = 0, sourceX = 0;  // Set first byte address on LCD.
  uint8_t width = LCD_WIDTH;
  uint8_t pagesLeft = LCD_PAGE_HEIGHT;
  uint8_t destWidth = LCD_WIDTH;

  // Copy page-by-page.
  while (pagesLeft-- > 0) {
    // Copy byte-by-byte within page.
    LCD_ReadPage( pDestInRam, sourcePage, sourceX, width );    
    // Move to next page.
    pDestInRam += destWidth;
    sourcePage++;
  }
}


/*!
 *  \param  source        Pointer to source picture.
 *  \param  sourceWidth   The source picture width in pixels .
 *  \param  sourceX       X address where to start to read source file.
 *  \param  sourcePage    Page address where to start to read source file.
 *  \param  dest          Pointer to destination memory.
 *  \param  destWidth     Destination memory buffer width in pixels.
 *  \param  destX         X address where to place first byte in destination file.
 *  \param  destPage      Page address where to place first byte destination file.
 *  \param  width         The destination picture width in pixels.
 *  \param  heightInPages The destination picture height in pages (1 Byte - 1 Page).
 */
void PICTURE_CopyFlashToRam( 
		uint8_t CAL_PGM(const * source), uint16_t sourceWidth, uint16_t sourceX, uint8_t sourcePage,
		uint8_t * dest, uint8_t destWidth, uint8_t destX, uint8_t destPage,
		uint8_t width, uint8_t heightInPages )
{
  // Move data pointers to starting points.
  source += (sourceWidth * sourcePage) + sourceX;
  dest += (destWidth * destPage) + destX;

  // Copy page-by-page.
  uint8_t pagesLeft = heightInPages;
  while (pagesLeft-- > 0) {
    // Copy byte-by-byte within page.
    uint8_t colsLeft = width;
    while (colsLeft-- > 0) {
      *dest++ = CAL_pgm_read_byte(source);
      source++;
    }
    // Move to next page.
    source += sourceWidth - width;
    dest += destWidth - width;
  }
}

/*!
 *  \param  source        Pointer to source file.
 *  \param  sourceWidth   The source picture width in pixels .
 *  \param  sourceX       X address where to start to read source file.
 *  \param  sourcePage    Page address where to start to read source file.
 *  \param  dest          Pointer to destination address.
 *  \param  destWidth     The destination memory area width in pixels.
 *  \param  destX         X address where to place first byte in destination file.
 *  \param  destPage      Page address where to place first byte destination file.
 *  \param  width         The destination picture width in pixels.
 *  \param  heightInPages The destination picture height in pages (1 Byte - 1 Page).
 */
void PICTURE_CopyRamToRam(
		uint8_t const * source, uint16_t sourceWidth, uint16_t sourceX, uint8_t sourcePage,
		uint8_t * dest, uint8_t destWidth, uint8_t destX, uint8_t destPage,
		uint8_t width, uint8_t heightInPages )
{
  // Move data pointers to starting points.
  source += (sourceWidth * sourcePage) + sourceX;
  dest += (destWidth * destPage) + destX;
  
  // Copy page-by-page.
  uint8_t pagesLeft = heightInPages;
  while (pagesLeft-- > 0) {
    // Copy byte-by-byte within page.
    uint8_t colsLeft = width;
    while (colsLeft-- > 0) {
      *dest++ = *source++;
    }
    // Move to next page.
    source += sourceWidth - width;
    dest += destWidth - width;
  }
}

/*!
 *  \param  source        Pointer to source file.
 *  \param  sourceWidth   The source picture width in pixels .
 *  \param  sourceX       X address where to start to read source file.
 *  \param  sourcePage    Page address where to start to read source file.
 *  \param  destX         X address where to place first byte in destination file.
 *  \param  destPage      Page address where to place first byte destination file.
 *  \param  width         The destination picture width in pixels.
 *  \param  heightInPages The destination picture height in pages (1 Byte - 1 Page).
 */
void PICTURE_CopyFlashToLcd(
		uint8_t CAL_PGM(const * source), uint16_t sourceWidth, uint16_t sourceX, uint8_t sourcePage,
		uint8_t destX, uint8_t destPage,
		uint8_t width, uint8_t heightInPages )
{
  // Move data pointer to starting points.
  source += (sourceWidth * sourcePage) + sourceX;
  
  // Copy page-by-page.
  uint8_t pagesLeft = heightInPages;
  while (pagesLeft-- > 0) {
    // Copy byte-by-byte within page.
    LCD_WritePage_F( source, destPage, destX, width );
    
    // Move to next page.
    source += sourceWidth;
    destPage++;
  }
}


/*!
 *  \param  source        Pointer to source file.
 *  \param  sourceWidth   The source picture width in pixels .
 *  \param  sourceX       X address where to start to read source file.
 *  \param  sourcePage    Page address where to start to read source file.
 *  \param  destX         X address where to place first byte in destination file.
 *  \param  destPage      Page address where to place first byte destination file.
 *  \param  width         The destination picture width in pixels.
 *  \param  heightInPages The destination picture height in pages (1 Byte - 1 Page).
 */
void PICTURE_CopyRamToLcd(
		uint8_t const * source, uint16_t sourceWidth, uint16_t sourceX, uint8_t sourcePage,
		uint8_t destX, uint8_t destPage,
		uint8_t width, uint8_t heightInPages )
{
  // Move data pointer to starting points.
  source += (sourceWidth * sourcePage) + sourceX;
  
  // Copy page-by-page.
  uint8_t pagesLeft = heightInPages;
  while (pagesLeft-- > 0) {
    // Copy byte-by-byte within page.
    LCD_WritePage( source, destPage, destX, width );
    
    // Move to next page.
    source += sourceWidth;
    destPage++;
  }
}


/*!
 *  \param  sourceX       X address where to start to read source file.
 *  \param  sourcePage    Page address where to start to read source file.
 *  \param  dest          Pointer to destination address.
 *  \param  destWidth     The destination memory area width in pixels.
 *  \param  destX         X address where to place first byte in destination file.
 *  \param  destPage      Page address where to place first byte destination file.
 *  \param  width         The destination picture width in pixels.
 *  \param  heightInPages The destination picture height in pages (1 Byte - 1 Page).
 */
void PICTURE_CopyLcdToRam(
		uint16_t sourceX, uint8_t sourcePage,
		uint8_t * dest, uint8_t destWidth, uint8_t destX, uint8_t destPage,
		uint8_t width, uint8_t heightInPages )
{
  // Move data pointer to starting points.
  dest += (destWidth * destPage) + destX;
  
  // Copy page-by-page.
  uint8_t pagesLeft = heightInPages;
  while (pagesLeft-- > 0) {
    // Copy byte-by-byte within page.
    LCD_ReadPage( dest, sourcePage, sourceX, width );
    
    // Move to next page.
    dest += destWidth;
    sourcePage++;
  }
}
