/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 terminal font handling library source file
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
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/termfont_lib/termfont_lib.c $
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

#include "termfont_lib.h"
#include <lcd_lib.h>


/*******************
 * Private defines.
 *******************/

#define TERMFONT_FONT_DATA_WIDTH 5 //!< The pixel width of ASCI character in table.

/*! 
 * Standard ascii terminal 5x7 font
 * Fill a 6x8 pixel box. 
 * Defines ascii characters 0x20-0x7F (32-127)
 * Note, needs insertion of the empty row between characters.
*/
const uint8_t CAL_PGM_DEF(TERMFONT_fontdata[]) = {
    0x00, 0x00, 0x00, 0x00, 0x00, // (space)    #32
    0x00, 0x00, 0x5F, 0x00, 0x00, // !
    0x00, 0x07, 0x00, 0x07, 0x00, // "
    0x14, 0x7F, 0x14, 0x7F, 0x14, // #
    0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
    0x23, 0x13, 0x08, 0x64, 0x62, // %
    0x36, 0x49, 0x55, 0x22, 0x50, // &
    0x00, 0x05, 0x03, 0x00, 0x00, // '
    0x00, 0x1C, 0x22, 0x41, 0x00, // (          #40
    0x00, 0x41, 0x22, 0x1C, 0x00, // )
    0x08, 0x2A, 0x1C, 0x2A, 0x08, // *
    0x08, 0x08, 0x3E, 0x08, 0x08, // +
    0x00, 0x50, 0x30, 0x00, 0x00, // ,
    0x08, 0x08, 0x08, 0x08, 0x08, // -
    0x00, 0x60, 0x60, 0x00, 0x00, // .
    0x20, 0x10, 0x08, 0x04, 0x02, // /
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x42, 0x61, 0x51, 0x49, 0x46, // 2          #50
    0x21, 0x41, 0x45, 0x4B, 0x31, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
    0x01, 0x71, 0x09, 0x05, 0x03, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x06, 0x49, 0x49, 0x29, 0x1E, // 9
    0x00, 0x36, 0x36, 0x00, 0x00, // :
    0x00, 0x56, 0x36, 0x00, 0x00, // ;
    0x00, 0x08, 0x14, 0x22, 0x41, // <          #60
    0x14, 0x14, 0x14, 0x14, 0x14, // =
    0x41, 0x22, 0x14, 0x08, 0x00, // >
    0x02, 0x01, 0x51, 0x09, 0x06, // ?
    0x32, 0x49, 0x79, 0x41, 0x3E, // @
    0x7E, 0x11, 0x11, 0x11, 0x7E, // A
    0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x7F, 0x41, 0x41, 0x22, 0x1C, // D
    0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x7F, 0x09, 0x09, 0x01, 0x01, // F          #70
    0x3E, 0x41, 0x41, 0x51, 0x32, // G
    0x7F, 0x08, 0x08, 0x08, 0x7F, // H
    0x00, 0x41, 0x7F, 0x41, 0x00, // I
    0x20, 0x40, 0x41, 0x3F, 0x01, // J
    0x7F, 0x08, 0x14, 0x22, 0x41, // K
    0x7F, 0x40, 0x40, 0x40, 0x40, // L
    0x7F, 0x02, 0x04, 0x02, 0x7F, // M
    0x7F, 0x04, 0x08, 0x10, 0x7F, // N
    0x3E, 0x41, 0x41, 0x41, 0x3E, // O
    0x7F, 0x09, 0x09, 0x09, 0x06, // P          #80
    0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
    0x7F, 0x09, 0x19, 0x29, 0x46, // R
    0x46, 0x49, 0x49, 0x49, 0x31, // S
    0x01, 0x01, 0x7F, 0x01, 0x01, // T
    0x3F, 0x40, 0x40, 0x40, 0x3F, // U
    0x1F, 0x20, 0x40, 0x20, 0x1F, // V
    0x7F, 0x20, 0x18, 0x20, 0x7F, // W
    0x63, 0x14, 0x08, 0x14, 0x63, // X
    0x03, 0x04, 0x78, 0x04, 0x03, // Y
    0x61, 0x51, 0x49, 0x45, 0x43, // Z          #90
    0x00, 0x00, 0x7F, 0x41, 0x41, // [
    0x02, 0x04, 0x08, 0x10, 0x20, // "\"
    0x41, 0x41, 0x7F, 0x00, 0x00, // ]
    0x04, 0x02, 0x01, 0x02, 0x04, // ^
    0x40, 0x40, 0x40, 0x40, 0x40, // _
    0x00, 0x01, 0x02, 0x04, 0x00, // `
    0x20, 0x54, 0x54, 0x54, 0x78, // a
    0x7F, 0x48, 0x44, 0x44, 0x38, // b
    0x38, 0x44, 0x44, 0x44, 0x20, // c
    0x38, 0x44, 0x44, 0x48, 0x7F, // d         #100
    0x38, 0x54, 0x54, 0x54, 0x18, // e
    0x08, 0x7E, 0x09, 0x01, 0x02, // f
    0x08, 0x14, 0x54, 0x54, 0x3C, // g
    0x7F, 0x08, 0x04, 0x04, 0x78, // h
    0x00, 0x44, 0x7D, 0x40, 0x00, // i
    0x20, 0x40, 0x44, 0x3D, 0x00, // j
    0x00, 0x7F, 0x10, 0x28, 0x44, // k
    0x00, 0x41, 0x7F, 0x40, 0x00, // l
    0x7C, 0x04, 0x18, 0x04, 0x78, // m
    0x7C, 0x08, 0x04, 0x04, 0x78, // n         #110
    0x38, 0x44, 0x44, 0x44, 0x38, // o
    0x7C, 0x14, 0x14, 0x14, 0x08, // p
    0x08, 0x14, 0x14, 0x18, 0x7C, // q
    0x7C, 0x08, 0x04, 0x04, 0x08, // r
    0x48, 0x54, 0x54, 0x54, 0x20, // s
    0x04, 0x3F, 0x44, 0x40, 0x20, // t
    0x3C, 0x40, 0x40, 0x20, 0x7C, // u
    0x1C, 0x20, 0x40, 0x20, 0x1C, // v
    0x3C, 0x40, 0x30, 0x40, 0x3C, // w
    0x44, 0x28, 0x10, 0x28, 0x44, // x         #120
    0x0C, 0x50, 0x50, 0x50, 0x3C, // y
    0x44, 0x64, 0x54, 0x4C, 0x44, // z
    0x00, 0x08, 0x36, 0x41, 0x00, // {
    0x00, 0x00, 0x7F, 0x00, 0x00, // |
    0x00, 0x41, 0x36, 0x08, 0x00, // }
    0x02, 0x01, 0x02, 0x04, 0x02, // ~
    0x78, 0x44, 0x42, 0x44, 0x78  // DEL
};



/****************************
 * Function implementations.
 ****************************/


/*!
 * \param  character  Character to print
 * \param  page  On which page to print the character
 * \param  column  On which column to start print the character
 */
void TERMFONT_DisplayChar(char character, uint8_t page, uint8_t column)
{
    const uint8_t CAL_PGM(*pCharacterStart);
    uint8_t fontIndex;
    // Check that the ASCII character is printable, and set to font table ref.
    if ((character >= ' ') && (character <= '~')){
        fontIndex = character - ' ';
        pCharacterStart = TERMFONT_fontdata + (fontIndex*TERMFONT_FONT_DATA_WIDTH); // Initialize pointer to characterfont.
        LCD_ClrPage( 0xFF, page, column, 1 );  // Clear first empty line, character divider.
        column++;                              // Set address to where fontdata should start.
        // Send character and address to character to display.
        LCD_WritePage_F(pCharacterStart, page, column, TERMFONT_FONT_DATA_WIDTH);
    }
}

/*! 
 * Display a string of characters. 
 * Note, if string longer than screen, the line will wrap and overwrite itself.
 *
 * \param  pTextString  Pointer to null-terminated string
 * \param  page  On which page to print the string
 * \param  column  On which column to start print the string
 */
void TERMFONT_DisplayString(char const * pTextString, uint8_t page, uint8_t column){
  
    if (pTextString == NULL) { return; }
    
    while (*pTextString != 0x00)
    {
        TERMFONT_DisplayChar(*pTextString++, page, column);
	column += TERMFONT_CHAR_WIDTH;
    }
}

/*!
 * Display a string of characters from flash
 * Note, if string longer than screen, the line will wrap and overwrite itself.
 *
 * \param  pTextString  Pointer to null-terminated string in flash
 * \param  page  On which page to print the string
 * \param  column  On which column to start print the string
 */
void TERMFONT_DisplayString_F(char const CAL_PGM(* pTextString), uint8_t page, uint8_t column)
{
	char ch;

    if (pTextString == NULL) { return; }
    
    while ( (ch=CAL_pgm_read_char(pTextString)) != 0x00)
    {
        TERMFONT_DisplayChar(ch, page, column);
		pTextString++;
		column += TERMFONT_CHAR_WIDTH;
    }
}


/*!
 * \param  character  Character to print
 * \param  page  On which page to print the character
 * \param  column  On which column to start print the character
 */
void TERMFONT_MergeChar(char character, uint8_t page, uint8_t column)
{
    const uint8_t CAL_PGM(*pCharacterStart);
    uint8_t fontIndex;
    // Check that the ASCII character is printable, and set to font table ref.
    if ((character >= ' ') && (character <= '~')){
        fontIndex = character - ' ';
        pCharacterStart = TERMFONT_fontdata + (fontIndex*TERMFONT_FONT_DATA_WIDTH); // Initialize pointer to characterfont.
        column = column + 1;  // Set pixel position address to where fontdata shall start.
        LCD_MergePage_F(pCharacterStart, page, column, TERMFONT_FONT_DATA_WIDTH); // Send address to character to display.
    }
}

/*! 
 * Merge a string of characters. 
 * Note, if string longer than screen, the line will wrap and overwrite itself.
 *
 * \param  pTextString  Pointer to null-terminated string
 * \param  page  On which page to print the string
 * \param  column  On which column to start print the string
 */
void TERMFONT_MergeString(char const * pTextString, uint8_t page, uint8_t column)
{
	if (pTextString == NULL) { return; }
    while (*pTextString != 0x00)
    {
        TERMFONT_MergeChar(*pTextString++, page, column);
		column += TERMFONT_CHAR_WIDTH;
    }
}

/*!
 * Merge a string of characters from flash
 * Note, if string longer than screen, the line will wrap and overwrite itself.
 *
 * \param  pTextString  Pointer to null-terminated string in flash
 * \param  page  On which page to print the string
 * \param  column  On which column to start print the string
 */
void TERMFONT_MergeString_F(char const CAL_PGM(* pTextString), uint8_t page, uint8_t column)
{
	char ch;
	if (pTextString == NULL) { return; }

    while ( (ch=CAL_pgm_read_char(pTextString)) != 0x00)
    {
        TERMFONT_MergeChar(ch, page, column);
		pTextString++;
		column += TERMFONT_CHAR_WIDTH;
    }
}

/*!
 * \param  character  Character to print
 * \param  page  On which page to print the character
 * \param  column  On which column to start print the character
 */
void TERMFONT_XORChar(char character, uint8_t page, uint8_t column)
{
    const uint8_t CAL_PGM(*pCharacterStart);
    uint8_t fontIndex;
    // Check that the ASCII character is printable, and set to font table ref.
    if ((character >= ' ') && (character <= '~')){
        fontIndex = character - ' ';
        pCharacterStart = TERMFONT_fontdata + (fontIndex*TERMFONT_FONT_DATA_WIDTH); // Initialize pointer to characterfont.
        column = column + 1;  // Set pixel position address to where fontdata shall start.
        LCD_XORPage_F(pCharacterStart, page, column, TERMFONT_FONT_DATA_WIDTH); // Send address to character to display.
    }
}

/*! 
 * XOR a string of characters. 
 * Note, if string longer than screen, the line will wrap and overwrite itself.
 *
 * \param  pTextString  Pointer to null-terminated string
 * \param  page  On which page to print the string
 * \param  column  On which column to start print the string
 */
void TERMFONT_XORString_F(char const CAL_PGM(* pTextString), uint8_t page, uint8_t column)
{
	char ch;
	if (pTextString == NULL) { return; }

    while ( (ch=CAL_pgm_read_char(pTextString)) != 0x00)
    {
        TERMFONT_XORChar(ch, page, column);
		pTextString++;
		column += TERMFONT_CHAR_WIDTH;
    }
}

/*!
 * XOR a string of characters from flash
 * Note, if string longer than screen, the line will wrap and overwrite itself.
 *
 * \param  pTextString  Pointer to null-terminated string in flash
 * \param  page  On which page to print the string
 * \param  column  On which column to start print the string
 */
void TERMFONT_XORString(char const * pTextString, uint8_t page, uint8_t column)
{
	if (pTextString == NULL) { return; }

    while (*pTextString != 0x00)
    {
        TERMFONT_XORChar(*pTextString++, page, column);
		column += TERMFONT_CHAR_WIDTH;
    }
}

/*!
 * \param  page  On which page to set the cursor
 * \param  column  On which column to set the cursor
 */
void TERMFONT_SetCursor(uint8_t page, uint8_t column)
{
    LCD_SetPage( 0xFF, page, column, TERMFONT_CHAR_WIDTH );
}

/*!
 * \param  page  On which page to clear the cursor
 * \param  column  On which column to clear the cursor
 */
void TERMFONT_ClearCursor(uint8_t page, uint8_t column)
{
    LCD_ClrPage( 0xFF, page, column, TERMFONT_CHAR_WIDTH );
}

/*!
 * \param  page  On which page to flip the cursor
 * \param  column  On which column to flip the cursor
 */
void TERMFONT_FlipCursor(uint8_t page, uint8_t column)
{
    LCD_FlipPage( 0xFF, page, column, TERMFONT_CHAR_WIDTH );
}

/*!
 * \param  buffer  Pointer to buffer in which to print
 * \param  character  Character to print
 * \param  page  On which page to print the character
 * \param  column  On which column to start print the character
 */
void TERMFONT_DisplayBufferChar( uint8_t * buffer, char character, uint8_t page, uint8_t column)
{
    const uint8_t CAL_PGM(*pCharacterStart);
    uint8_t fontIndex;
    // Check that the ASCII character is printable, and set to font table ref.
    if ((character >= ' ') || (character <= '~')){
      fontIndex = character - ' ';
      pCharacterStart = TERMFONT_fontdata + (fontIndex*TERMFONT_FONT_DATA_WIDTH); // Initialize pointer to characterfont.
      LCD_ClrBufferPage( buffer, 0xFF, page, column, 1); // Clear first empty line, character divider.
      column++;
      LCD_WriteBufferPage_F( buffer, pCharacterStart, page, column, TERMFONT_FONT_DATA_WIDTH); // Send address to character to display.
    }
}

/*! 
 * Print a string of characters to buffer
 * Note, if string longer than screen, the line will wrap and overwrite itself.
 *
 * \param  buffer  Pointer to buffer in which to print
 * \param  pTextString  Pointer to null-terminated string
 * \param  page  On which page to print the string
 * \param  column  On which column to start print the string
 */
void TERMFONT_DisplayBufferString( uint8_t * buffer, char const * pTextString, uint8_t page, uint8_t column)
{
	if (pTextString == NULL) { return; }
    while (*pTextString != 0x00)
    {
        TERMFONT_DisplayBufferChar( buffer, *pTextString++, page, column);
		column += TERMFONT_CHAR_WIDTH;
    }
}

/*! 
 * Print a string of characters to buffer from flash
 * Note, if string longer than screen, the line will wrap and overwrite itself.
 *
 * \param  buffer  Pointer to buffer in which to print
 * \param  pTextString  Pointer to null-terminated string in flash
 * \param  page  On which page to print the string
 * \param  column  On which column to start print the string
 */
void TERMFONT_DisplayBufferString_F( uint8_t * buffer, char const CAL_PGM(*pTextString), uint8_t page, uint8_t column)
{
	char ch;
	if (pTextString == NULL) { return; }

    while ( (ch=CAL_pgm_read_char(pTextString)) != 0x00)
    {
        TERMFONT_DisplayBufferChar( buffer, ch , page, column);
		pTextString++;
		column += TERMFONT_CHAR_WIDTH;
    }
}

/*!
 * \param  pPageBuffer  Pointer to a page in a buffer
 * \param  character  Character to print on that page
 */
void TERMFONT_DisplayPageBufferChar( uint8_t * pPageBuffer, char character )
{
    const uint8_t CAL_PGM(*pCharacterStart);
    uint8_t fontIndex;
    // Check that the ASCII character is printable, and set to font table ref.
    if ((character >= ' ') && (character <= '~')){
      fontIndex = character - ' ';
      pCharacterStart = TERMFONT_fontdata + (fontIndex*TERMFONT_FONT_DATA_WIDTH); // Initialize pointer to characterfont.
      LCD_ClrBufferPage( pPageBuffer, 0xFF, 0, 0, 1); // Clear first empty line, character divider.
      LCD_WriteBufferPage_F( pPageBuffer, pCharacterStart, 0, 1, TERMFONT_FONT_DATA_WIDTH); // Send address to character to display.
    }
}

/*! 
 * Print a string of characters to a specific page in a buffer
 * Note, if string longer than screen, the line will wrap and overwrite itself.
 *
 * \param  pPageBuffer  Pointer to a page in a buffer
 * \param  pTextString  Pointer to null-terminated string
 */
void TERMFONT_DisplayPageBufferString( uint8_t * pPageBuffer, char const * pTextString )
{
	if (pTextString == NULL) { return; }
    while (*pTextString != 0x00){
        TERMFONT_DisplayPageBufferChar( pPageBuffer, *pTextString++ );
        pPageBuffer += TERMFONT_CHAR_WIDTH;
    }
}

/*! 
 * Print a string of characters from flash to a specific page in a buffer
 * Note, if string longer than screen, the line will wrap and overwrite itself.
 *
 * \param  pPageBuffer  Pointer to a page in a buffer
 * \param  pTextString  Pointer to null-terminated string in flash
 */
void TERMFONT_DisplayPageBufferString_F( uint8_t * pPageBuffer, char const CAL_PGM(* pTextString) )
{
	char ch;
	if (pTextString == NULL) { return; }
    while ( (ch=CAL_pgm_read_char(pTextString)) != 0x00){
        TERMFONT_DisplayPageBufferChar( pPageBuffer, ch );
		pTextString++;
        pPageBuffer += TERMFONT_CHAR_WIDTH;
    }
}

/*!
 * \param  buffer  Pointer to buffer in which to merge the character
 * \param  character  Character to print
 * \param  page  On which page to print the character
 * \param  column  On which column to start print the character
 */
void TERMFONT_MergeBufferChar( uint8_t * buffer, char character, uint8_t page, uint8_t column)
{
    const uint8_t CAL_PGM(*pCharacterStart);
    uint8_t fontIndex;
    // Check that the ASCII character is printable, and set to font table ref.
    if ((character >= ' ') && (character <= '~')){
        fontIndex = character - ' ';
        pCharacterStart = TERMFONT_fontdata + (fontIndex*TERMFONT_FONT_DATA_WIDTH); // Initialize pointer to characterfont.
        column = column + 1;  // Set pixel position address to where fontdata shall start.
        LCD_MergeBufferPage_F(buffer, pCharacterStart, page, column, TERMFONT_FONT_DATA_WIDTH); // Send address to character to display.
    }
}

/*! 
 * Merge a string of characters to buffer
 * Note, if string longer than screen, the line will wrap and overwrite itself.
 *
 * \param  buffer  Pointer to buffer in which to merge the string
 * \param  pTextString  Pointer to null-terminated string
 * \param  page  On which page to print the string
 * \param  column  On which column to start print the string
 */
void TERMFONT_MergeBufferString( uint8_t * buffer, char const * pTextString, uint8_t page, uint8_t column)
{
	if (pTextString == NULL) { return; }
    while (*pTextString != 0x00)
    {
      TERMFONT_MergeBufferChar( buffer, *pTextString++, page, column);
      column += TERMFONT_CHAR_WIDTH;
    }
}

/*! 
 * Merge a string of characters from flash to buffer
 * Note, if string longer than screen, the line will wrap and overwrite itself.
 *
 * \param  buffer  Pointer to buffer in which to merge the string
 * \param  pTextString  Pointer to null-terminated string in flash
 * \param  page  On which page to print the string
 * \param  column  On which column to start print the string
 */
void TERMFONT_MergeBufferString_F( uint8_t * buffer, char const CAL_PGM(* pTextString), uint8_t page, uint8_t column)
{
	char ch;
	if (pTextString == NULL) { return; }
    while ( (ch=CAL_pgm_read_char(pTextString)) != 0x00)
    {
      TERMFONT_MergeBufferChar( buffer, ch, page, column);
	  pTextString++;
      column += TERMFONT_CHAR_WIDTH;
    }
}

/*!
 * \param  buffer  Pointer to buffer in which to XOR the character
 * \param  character  Character to print
 * \param  page  On which page to print the character
 * \param  column  On which column to start print the character
 */
void TERMFONT_XORBufferChar( uint8_t * buffer, char character, uint8_t page, uint8_t column)
{
    const uint8_t CAL_PGM(*pCharacterStart);
    uint8_t fontIndex;
    // Check that the ASCII character is printable, and set to font table ref.
    if ((character >= ' ') || (character <= '~')){
        fontIndex = character - ' ';
        pCharacterStart = TERMFONT_fontdata + (fontIndex*TERMFONT_FONT_DATA_WIDTH); // Initialize pointer to characterfont.
        column = column + 1;  // Set pixel position address to where fontdata shall start.
        LCD_XORBufferPage_F( buffer, pCharacterStart, page, column, TERMFONT_FONT_DATA_WIDTH); // Send address to character to display.
    }
}

/*! 
 * XOR a string of characters to buffer
 * Note, if string longer than screen, the line will wrap and overwrite itself.
 *
 * \param  buffer  Pointer to buffer in which to XOR the string
 * \param  pTextString  Pointer to null-terminated string
 * \param  page  On which page to print the string
 * \param  column  On which column to start print the string
 */
void TERMFONT_XORBufferString( uint8_t * buffer, char const * pTextString, uint8_t page, uint8_t column)
{
	if (pTextString == NULL) { return; }
    while (*pTextString != 0x00)
    {
        TERMFONT_XORBufferChar( buffer, *pTextString++, page, column);
		column += TERMFONT_CHAR_WIDTH;
    }
}

/*! 
 * XOR a string of characters from flash to buffer
 * Note, if string longer than screen, the line will wrap and overwrite itself.
 *
 * \param  buffer  Pointer to buffer in which to XOR the string
 * \param  pTextString  Pointer to null-terminated string in flash
 * \param  page  On which page to print the string
 * \param  column  On which column to start print the string
 */
void TERMFONT_XORBufferString_F( uint8_t * buffer, char const CAL_PGM(* pTextString), uint8_t page, uint8_t column)
{
	char ch;
	if (pTextString == NULL) { return; }
    while ( (ch = CAL_pgm_read_byte(pTextString)) != 0x00)
    {
        TERMFONT_XORBufferChar( buffer, ch, page, column);
		pTextString++;
		column += TERMFONT_CHAR_WIDTH;
    }
}

/*!
 * \param  buffer  Pointer to buffer we want to set the cursor in
 * \param  page  On which page to set the cursor
 * \param  column  On which column to set the cursor
 */
void TERMFONT_SetBufferCursor( uint8_t * buffer, uint8_t page, uint8_t column)
{
    // Define the 6 bytes describing the cursor
    uint8_t cursor[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    uint8_t *pcursor = &cursor[0];
    LCD_WriteBufferPage(buffer, pcursor, page, column, TERMFONT_CHAR_WIDTH);
}

/*!
 * \param  buffer  Pointer to buffer we want to clear the cursor in
 * \param  page  On which page to clear the cursor
 * \param  column  On which column to clear the cursor
 */
void TERMFONT_ClearBufferCursor( uint8_t * buffer, uint8_t page, uint8_t column)
{
    // Define the 6 bytes describing the cursor
    uint8_t cursor[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t *pcursor = &cursor[0];
    LCD_WriteBufferPage(buffer, pcursor, page, column, TERMFONT_CHAR_WIDTH);
}

/*!
 * \param  buffer  Pointer to buffer we want to flip the cursor in
 * \param  page  On which page to flip the cursor
 * \param  column  On which column to flip the cursor
 */
void TERMFONT_FlipBufferCursor( uint8_t * buffer, uint8_t page, uint8_t column)
{
    LCD_FlipBufferPage(buffer, 0xFF, page, column, TERMFONT_CHAR_WIDTH);
}


/*!
 * \param  pTextString  Pointer to null-terminated string
 *
 * \return  Width of the string in pixels
 */
uint8_t TERMFONT_GetStringWidth( char const * pTextString )
{
	if (pTextString == NULL) { return 0; }
	uint8_t width = 0;
    while (*pTextString++ != 0x00)
    {
		width += TERMFONT_CHAR_WIDTH;
    }
	return width;
}

/*!
 * \param  pTextString  Pointer to null-terminated string in flash
 *
 * \return  Width of the string in pixels
 */
uint8_t TERMFONT_GetStringWidth_F( char const CAL_PGM(* pTextString) )
{
	if (pTextString == NULL) { return 0; }

	uint8_t width = 0;
    while ( CAL_pgm_read_char(pTextString++) != 0x00)
    {
		width += TERMFONT_CHAR_WIDTH;
    }
	return width;
}
