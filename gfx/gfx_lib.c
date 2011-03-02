/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  LCD graphics library source file
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
 * $Revision: 2420 $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/gfx/gfx_lib.c $
 * $Date: 2007-09-17 11:57:01 -0600 (ma, 17 sep 2007) $  \n
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

#include "gfx_lib.h"
#include "lcd_lib.h"
#include <stdint.h>
#include "../production_demo_rev_A/flashpics.h"
#include "../production_demo_rev_A/bignumbers.h"

// Init soc data for different SOC-levels.
static uint8_t const CAL_PGM_DEF(* const big_number_pictures[14]) = {
	FLASHPICS_0,
	FLASHPICS_1,
	FLASHPICS_2,
	FLASHPICS_3,
	FLASHPICS_4,
	FLASHPICS_5,
	FLASHPICS_6,
	FLASHPICS_7,
	FLASHPICS_8,
	FLASHPICS_9,
	FLASHPICS_X,
	FLASHPICS_minus,
	FLASHPICS_plus,
	FLASHPICS_pros
};

// last SOC state to memory so that we don't update unnecessary
uint8_t last_soc = 0;
// Init soc data for different SOC-levels.
static uint8_t const CAL_PGM_DEF(* const soc_pictures[102]) = {
	FLASHPICS_battery_0,
	FLASHPICS_battery_0,
	FLASHPICS_battery_1,
	FLASHPICS_battery_1,
	FLASHPICS_battery_2,
	FLASHPICS_battery_2,
	FLASHPICS_battery_3,
	FLASHPICS_battery_3,
	FLASHPICS_battery_4,
	FLASHPICS_battery_4,
	FLASHPICS_battery_5,
	FLASHPICS_battery_5,
	FLASHPICS_battery_6,
	FLASHPICS_battery_6,
	FLASHPICS_battery_7,
	FLASHPICS_battery_7,
	FLASHPICS_battery_8,
	FLASHPICS_battery_8,
	FLASHPICS_battery_9,
	FLASHPICS_battery_9,
	FLASHPICS_battery_10,
	FLASHPICS_battery_10,
	FLASHPICS_battery_11,
	FLASHPICS_battery_11,
	FLASHPICS_battery_12,
	FLASHPICS_battery_12,
	FLASHPICS_battery_13,
	FLASHPICS_battery_13,
	FLASHPICS_battery_14,
	FLASHPICS_battery_14,
	FLASHPICS_battery_15,
	FLASHPICS_battery_15,
	FLASHPICS_battery_16,
	FLASHPICS_battery_16,
	FLASHPICS_battery_17,
	FLASHPICS_battery_17,
	FLASHPICS_battery_18,
	FLASHPICS_battery_18,
	FLASHPICS_battery_19,
	FLASHPICS_battery_19,
	FLASHPICS_battery_20,
	FLASHPICS_battery_20,
	FLASHPICS_battery_21,
	FLASHPICS_battery_21,
	FLASHPICS_battery_22,
	FLASHPICS_battery_22,
	FLASHPICS_battery_23,
	FLASHPICS_battery_23,
	FLASHPICS_battery_24,
	FLASHPICS_battery_24,
	FLASHPICS_battery_25,
	FLASHPICS_battery_25,
	FLASHPICS_battery_26,
	FLASHPICS_battery_26,
	FLASHPICS_battery_27,
	FLASHPICS_battery_27,
	FLASHPICS_battery_28,
	FLASHPICS_battery_28,
	FLASHPICS_battery_29,
	FLASHPICS_battery_29,
	FLASHPICS_battery_30,
	FLASHPICS_battery_30,
	FLASHPICS_battery_31,
	FLASHPICS_battery_31,
	FLASHPICS_battery_32,
	FLASHPICS_battery_32,
	FLASHPICS_battery_33,
	FLASHPICS_battery_33,
	FLASHPICS_battery_34,
	FLASHPICS_battery_34,
	FLASHPICS_battery_35,
	FLASHPICS_battery_35,
	FLASHPICS_battery_36,
	FLASHPICS_battery_36,
	FLASHPICS_battery_37,
	FLASHPICS_battery_37,
	FLASHPICS_battery_38,
	FLASHPICS_battery_38,
	FLASHPICS_battery_39,
	FLASHPICS_battery_39,
	FLASHPICS_battery_40,
	FLASHPICS_battery_40,
	FLASHPICS_battery_41,
	FLASHPICS_battery_41,
	FLASHPICS_battery_42,
	FLASHPICS_battery_42,
	FLASHPICS_battery_43,
	FLASHPICS_battery_43,
	FLASHPICS_battery_44,
	FLASHPICS_battery_44,
	FLASHPICS_battery_45,
	FLASHPICS_battery_45,
	FLASHPICS_battery_46,
	FLASHPICS_battery_46,
	FLASHPICS_battery_47,
	FLASHPICS_battery_47,
	FLASHPICS_battery_48,
	FLASHPICS_battery_48,
	FLASHPICS_battery_49,
	FLASHPICS_battery_49,
	FLASHPICS_battery_50,
	FLASHPICS_battery_50
};
		
void LCD_UpdateSOC(uint8_t soc)
{
	// Battery icon location is right of the screen, from top left 96 px X, 0 px Y
	// Battery icon size is 32 x 64 px.

	//PICTURE_CopyFullscreenFlashToLcd( CAL_pgm_read_puint8(&pictures[picIdx]) );
	
	//! Display section of image data stored in FLASH on display.
	/*void PICTURE_CopyFlashToLcd(
		uint8_t const CAL_PGM(* source), uint16_t sourceWidth, uint16_t sourceX, uint8_t sourcePage,
		uint8_t destX, uint8_t destPage,
		uint8_t width, uint8_t heightInPages );	
	*/
	if (soc < 1)
		soc = 0;
	if (soc > 100)
		soc = 100;
		
	PICTURE_CopyFlashToLcd(CAL_pgm_read_puint8(&soc_pictures[soc]), 32, 0, 0, 96, 0, 32, 8);
	
	LCD_UpdateBigNumbers(soc);
	
}

void LCD_UpdateMinVolt(uint16_t volt)
{
	//TERMFONT_DisplayChar( *str, page, column );
	// y, x
	// y = rows
	// x = pixels ?
	char buffer [sizeof(uint16_t)*8+1];
	int charw=6;
	int startpos=60;
	
	TERMFONT_DisplayString("Min Volt:", 7, 0);
	
	// convert to ascii
    itoa(volt,buffer,10);

	TERMFONT_DisplayChar(buffer, 7, startpos );
	/*
	TERMFONT_DisplayChar(buffer[0], 7, startpos );
	TERMFONT_DisplayChar('.', 7, startpos+charw );
	TERMFONT_DisplayChar(buffer[1], 7, startpos+2*charw );
	TERMFONT_DisplayChar(buffer[2], 7, startpos+3*charw );

	TERMFONT_DisplayString("V" , 7, startpos+5*charw );
*/
}

void LCD_UpdateMaxTemp(uint8_t temp)
{
	char buffer [sizeof(uint8_t)*8+1];
    itoa(temp,buffer,10);

	TERMFONT_DisplayString( "Max Temp:", 5, 0 );
	TERMFONT_DisplayString( buffer, 5, 60 );
	TERMFONT_DisplayString( "C", 5, 84 );

}

void LCD_UpdateBigNumbers(uint8_t value)
{
	// 1 - 3 BIG numbers to the middle of the screen
	//
	// 1 number, position 32,0 px from left corner
	// 2 numbers, positions 16,0 px and 48,0 px 
	// 3 numbers, positions 0,0 px, 32,0 px and 64,0 px
	//
	// Each number 32 px x 32 px in size.
	//

	int first=0;
	int second=0;
	int third=0;
	
	if (value < 1)
		value = 0;
	if (value > 254)
		value = 254;
	
	// clear background "just in case" there is some garbage on the screen
	LCD_ClrBox(0,0,96,32);

	if (value < 10)
	{
		// 1 number to the middle of the screen
		PICTURE_CopyFlashToLcd(CAL_pgm_read_puint8(&big_number_pictures[value]), 32, 0, 0, 32, 0, 32, 4);

		// percent mark to this row
		PICTURE_CopyFlashToLcd(FLASHPICS_pros, 32, 0, 0, 64, 0, 32, 4);

	} 
	else if (value > 9 && value < 100) 
	{
		/*
		
		// 2 numbers to the middle of the screen
		first = value / 10;
		// 1st number
		PICTURE_CopyFlashToLcd(CAL_pgm_read_puint8(&big_number_pictures[first]), 32, 0, 0, 16, 0, 32, 4);
		
		second = value - (first*10);
		// 2nd number
		PICTURE_CopyFlashToLcd(CAL_pgm_read_puint8(&big_number_pictures[second]), 32, 0, 0, 48, 0, 32, 4);
		
		*/
		
		// 2 numbers and percent mark to the middle of the screen
		first = value / 10;
		// 1st number
		PICTURE_CopyFlashToLcd(CAL_pgm_read_puint8(&big_number_pictures[first]), 32, 0, 0, 0, 0, 32, 4);
		
		second = value - (first*10);
		// 2nd number
		PICTURE_CopyFlashToLcd(CAL_pgm_read_puint8(&big_number_pictures[second]), 32, 0, 0, 32, 0, 32, 4);
		
		// percent mark to this row
		PICTURE_CopyFlashToLcd(FLASHPICS_pros, 32, 0, 0, 64, 0, 32, 4);
		
		// percent mark to next row
		//PICTURE_CopyFlashToLcd(FLASHPICS_pros, 32, 0, 0, 32, 4, 32, 4);

	}
	else
	{
		// 3 numbers to the screen
		
		first = value / 100;
		second = (value - (first*100)) / 10;
		third = value - (first*100) - (second*10);

		// 1st number
		PICTURE_CopyFlashToLcd(CAL_pgm_read_puint8(&big_number_pictures[first]), 32, 0, 0, 0, 0, 32, 4);
		
		// 2nd number
		PICTURE_CopyFlashToLcd(CAL_pgm_read_puint8(&big_number_pictures[second]), 32, 0, 0, 32, 0, 32, 4);

		// 3rd number
		PICTURE_CopyFlashToLcd(CAL_pgm_read_puint8(&big_number_pictures[third]), 32, 0, 0, 64, 0, 32, 4);
			
	}

}

/*
 * \param  Xstart  X-coordinate for the start of the line
 * \param  Ystart  Y-coordinate for the start of the line
 * \param  Xend    X-coordinate for the end of the line
 * \param  Yend    X-coordinate for the end of the line
 */
void LCD_DrawProgressBar(uint8_t Xstart, uint8_t Ystart, uint8_t height, uint8_t lenght, uint8_t maxvalue, uint8_t currvalue, uint8_t peakvalue)
{
	uint8_t i;
	uint8_t val2px;
	
	// draw box around corners of progress bar
	LCD_SetLine(Xstart,Ystart,Xstart+lenght,Ystart);
	LCD_SetLine(Xstart+lenght,Ystart,Xstart+lenght,Ystart+height);
	LCD_SetLine(Xstart,Ystart+height,Xstart+lenght,Ystart+height);
	LCD_SetLine(Xstart,Ystart,Xstart,Ystart+height);
	
	// calculate value in pixels
	val2px = lenght/maxvalue*currvalue;
	
	// clear box from previous values
	for (i=1;i<lenght;i++)
	{
		LCD_ClrLine(Xstart+i,Ystart+1,Xstart+i,Ystart+height-1);		
	}
	
	// fill box up to value
	for (i=1;i<val2px;i++)
	{
		LCD_SetLine(Xstart+i,Ystart,Xstart+i,Ystart+height);		
	}
}

/*
 * \param  Xstart  X-coordinate for the start of the line
 * \param  Ystart  Y-coordinate for the start of the line
 * \param  Xend    X-coordinate for the end of the line
 * \param  Yend    X-coordinate for the end of the line
 */
void LCD_SetLine(uint8_t Xstart, uint8_t Ystart, uint8_t Xend, uint8_t Yend)
{
    uint8_t x, y, i;
    int8_t e, xinc, yinc;
    int8_t dx, dy;

    if (Xstart > Xend){
        dx = Xstart;        /* swap x1,y1  with x2,y2 */
        Xstart = Xend;
        Xend = dx;
        dy = Ystart;
        Ystart = Yend;
        Yend = dy;
    }
	
    dx = Xend - Xstart;
    dy = Yend - Ystart;

    x = Xstart;
    y = Ystart;

    if (dx < 0){
        xinc = -1;
        dx = -dx;
    } else {
	    xinc = 1;
	}
    if (dy < 0){
        yinc = -1;
        dy = -dy;
    } else {
        yinc = 1;
	}

    /****************************************/

    if (dx > dy){
        e = dy - dx;
        for (i = 0; i <= dx; i++)
            {
            /* write out the data */
            LCD_SetPixel(x,y);
            if (e >= 0){
                e -= dx;
                y += yinc;
            } 
            e += dy;
            x += xinc;
        }
    } else {
        e = dx - dy;
        for (i = 0; i <= dy; i++){
            LCD_SetPixel(x,y);
    
            if (e >= 0){
                e -= dy;
                x += xinc;
            }
            e += dx;
            y += yinc;
        }
    }
}


/*
 * \param  Xstart  X-coordinate for the start of the line
 * \param  Ystart  Y-coordinate for the start of the line
 * \param  Xend    X-coordinate for the end of the line
 * \param  Yend    X-coordinate for the end of the line
 */
void LCD_ClrLine(uint8_t Xstart, uint8_t Ystart, uint8_t Xend, uint8_t Yend)
{
    uint8_t x, y, i;
    int8_t e, xinc, yinc;
    int8_t dx, dy;

    if (Xstart > Xend){
        dx = Xstart;        /* swap x1,y1  with x2,y2 */
        Xstart = Xend;
        Xend = dx;
        dy = Ystart;
        Ystart = Yend;
        Yend = dy;
    }

    dx = Xend - Xstart;
    dy = Yend - Ystart;

    x = Xstart;
    y = Ystart;

    if (dx < 0){
        xinc = -1;
        dx = -dx;
    } else {
	    xinc = 1;
	}
    if (dy < 0){
        yinc = -1;
        dy = -dy;
    } else {
        yinc = 1;
	}

    /****************************************/

    if (dx > dy){
        e = dy - dx;
        for (i = 0; i <= dx; i++)
            {
            /* write out the data */
            LCD_ClrPixel(x,y);
            if (e >= 0){
                e -= dx;
                y += yinc;
            } 
            e += dy;
            x += xinc;
        }
    } else {
        e = dx - dy;
        for (i = 0; i <= dy; i++){
            LCD_ClrPixel(x,y);
    
            if (e >= 0){
                e -= dy;
                x += xinc;
            } 
            e += dx;
            y += yinc;
        }
    }
}


/*
 * \param  Xcenter  X-coordinate for the center of the circle
 * \param  Ycenter  Y-coordinate for the center of the circle
 * \param  Radius   Radius of the circle in pixels
 */
void LCD_SetCircle(uint8_t Xcenter, uint8_t Ycenter, uint8_t Radius)
{
    int16_t tswitch;
    uint8_t X, Y;
    uint8_t dYX;

    dYX = Ycenter - Xcenter;
    X = 0;
    Y = Radius;
    tswitch = 3 - 2 * Radius;
    while (X <= Y) {
        LCD_SetPixel(Xcenter + X, Ycenter + Y);     
        LCD_SetPixel(Xcenter + X, Ycenter - Y);
        LCD_SetPixel(Xcenter - X, Ycenter + Y);     
        LCD_SetPixel(Xcenter - X, Ycenter - Y);
        LCD_SetPixel(Ycenter + Y - dYX, Ycenter + X); 
        LCD_SetPixel(Ycenter + Y - dYX, Ycenter - X);
        LCD_SetPixel(Ycenter - Y - dYX, Ycenter + X); 
        LCD_SetPixel(Ycenter - Y - dYX, Ycenter - X);

        if (tswitch < 0) {
            tswitch += (4 * X + 6);
        }
        else {
            tswitch += (4 * (X - Y) + 10);
            Y--;
        }
        X++;
    }
}


/*
 * \param  Xcenter  X-coordinate for the center of the circle
 * \param  Ycenter  Y-coordinate for the center of the circle
 * \param  Radius   Radius of the circle in pixels
 */
void LCD_ClrCircle(uint8_t Xcenter, uint8_t Ycenter, uint8_t Radius)
{
    int16_t tswitch;
    uint8_t X, Y;
    uint8_t dYX;

    dYX = Ycenter - Xcenter;
    X = 0;
     Y = Radius;
    tswitch = 3 - 2 * Radius;
    while (X <= Y) {
        LCD_ClrPixel(Xcenter + X, Ycenter + Y);     
        LCD_ClrPixel(Xcenter + X, Ycenter - Y);
        LCD_ClrPixel(Xcenter - X, Ycenter + Y);     
        LCD_ClrPixel(Xcenter - X, Ycenter - Y);
        LCD_ClrPixel(Ycenter + Y - dYX, Ycenter + X); 
        LCD_ClrPixel(Ycenter + Y - dYX, Ycenter - X);
        LCD_ClrPixel(Ycenter - Y - dYX, Ycenter + X); 
        LCD_ClrPixel(Ycenter - Y - dYX, Ycenter - X);

        if (tswitch < 0) {
            tswitch += (4 * X + 6);
        }
        else {
            tswitch += (4 * (X - Y) + 10);
            Y--;
        }
        X++;
    }
}
