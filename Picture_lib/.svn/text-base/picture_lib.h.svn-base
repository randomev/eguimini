/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 picture library header file.
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
 * $Revision: 2417 $
 * $Date: 2007-09-17 02:32:17 -0600 (ma, 17 sep 2007) $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/Picture_lib/picture_lib.h $
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

#ifndef PICTURE_LIB_H
#define PICTURE_LIB_H

#include <stdint.h>
#include <cal.h>

/***********************
 * Function prototypes.
 ***********************/

/*! \brief Clear the LCD screen from data. 
   Wrapper function for LCD_SetScreen. */
void PICTURE_ClearScreen(void);

/*! \brief Display a picture of same size as the LCD, loaded from RAM. 
 *    Wrapper function, as the functionality is available in LCD library. */
void PICTURE_CopyFullscreenRamToLcd ( const uint8_t * pPictureInRam );

//! \brief Display a picture of same size as the LCD, loaded from FLASH.
// void PICTURE_CopyFullscreenFlashToLcd ( const uint8_t __flash * pPictureInFlash );
void PICTURE_CopyFullscreenFlashToLcd ( const uint8_t CAL_PGM(* pPictureInFlash) );

//! \brief Reads a picture of same size as the LCD to RAM.
void PICTURE_CopyFullscreenLcdToRam ( uint8_t * pDestInRam );


//! Copy section of image data from FLASH to RAM. 
void PICTURE_CopyFlashToRam( 
		uint8_t const CAL_PGM(* source), uint16_t sourceWidth, uint16_t sourceX, uint8_t sourcePage,
		uint8_t * dest, uint8_t destWidth, uint8_t destX, uint8_t destPage,
		uint8_t width, uint8_t heightInPages );


//! Copy section of image data from RAM to RAM. 
void PICTURE_CopyRamToRam(
		uint8_t const * source, uint16_t sourceWidth, uint16_t sourceX, uint8_t sourcePage,
		uint8_t * dest, uint8_t destWidth, uint8_t destX, uint8_t destPage,
		uint8_t width, uint8_t heightInPages );


//! Display section of image data stored in FLASH on display.
void PICTURE_CopyFlashToLcd(
		uint8_t const CAL_PGM(* source), uint16_t sourceWidth, uint16_t sourceX, uint8_t sourcePage,
		uint8_t destX, uint8_t destPage,
		uint8_t width, uint8_t heightInPages );


//! Display section of image data stored in RAM on display.
void PICTURE_CopyRamToLcd(
		uint8_t const * source, uint16_t sourceWidth, uint16_t sourceX, uint8_t sourcePage,
		uint8_t destX, uint8_t destPage,
		uint8_t width, uint8_t heightInPages );


//! Get section of image data from LCD to RAM.
void PICTURE_CopyLcdToRam(
		uint16_t sourceX, uint8_t sourcePage,
		uint8_t * dest, uint8_t destWidth, uint8_t destX, uint8_t destPage,
		uint8_t width, uint8_t heightInPages );

#endif
