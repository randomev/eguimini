// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 LCD graphics library header file
 *
 *         This file contains higher-level graphics functions that cannot
 *         directly utilize the page-oriented nature of the LCD screen, and
 *         use the pixel manipulation functions of the lcd_lib.* files instead.
 *         This is an extension to the lcd_lib.* files.
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
 * $Revision: 2416 $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/gfx/gfx_lib.h $
 * $Date: 2007-09-17 02:23:27 -0600 (ma, 17 sep 2007) $  \n
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

#ifndef GFX_LIB_H
#define GFX_LIB_H

#include <stdint.h>



/***********************
 * Function prototypes.
 ***********************/

//! Draw a line between two points using pixel-manipulation
void LCD_SetLine(uint8_t Xstart, uint8_t Ystart, uint8_t Xend, uint8_t Yend);
//! Clear a line between two points using pixel-manipulation
void LCD_ClrLine(uint8_t Xstart, uint8_t Ystart, uint8_t Xend, uint8_t Yend);
//! Draw a circle, specified by center and radius
void LCD_SetCircle(uint8_t Xcenter, uint8_t Ycenter, uint8_t Radius);
//! Clear a circle, specified by center and radius
void LCD_ClrCircle(uint8_t Xcenter, uint8_t Ycenter, uint8_t Radius);
//! Draw a progress bar
void LCD_DrawProgressBar(uint8_t Xstart, uint8_t Ystart, uint8_t height, uint8_t lenght, uint8_t maxvalue, uint8_t currvalue, uint8_t peakvalue);

//! Draw a battery icon
void LCD_UpdateSOC(uint8_t soc);

//! Max temperature
void LCD_UpdateMaxTemp(uint8_t temp);

//! Min voltage
void LCD_UpdateMinVolt(uint16_t volt);

//! Draw big numbers to the center of the screen
void LCD_UpdateBigNumbers(uint8_t value);

#endif
