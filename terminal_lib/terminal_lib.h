/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 terminal / VT100 library header file
 *
 *      The terminal emulator library provides functions for displaying
 *      characters and parsing control codes and sequences in a full-screen
 *      21x8 character terminal. It supports an extensive subset of the VT100
 *      terminal control codes and escape sequences, including cursor movement,
 *      erasing of selected areas and character attributes such as underline and
 *      inverted colors. Character background color control is implemented so
 *      that it controls the entire LCD backlight color instead.
 *
 *      The library supports several operational terminals at the same time, as
 *      long as only one at a time is active on-screen. Incoming data is passed
 *      to a terminal using the TERMINAL_ProcessChar function, which enables the
 *      use of any communication interface as a data source. The application
 *      itself can even use the terminal as a convenient output device for
 *      providing information to the user.
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
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/terminal_lib/terminal_lib.h $
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


#ifndef TERMINAL_LIB_H
#define TERMINAL_LIB_H

#include <termfont_lib.h>
#include <stdint.h>
#include <stdbool.h>

/************************
 * Terminal definitions. 
 ************************/

#define TERM_MAX_ESCAPE_LENGTH 12 //!< Maximum escape sequence in DB101: ESC[nn;nn;nn;nnm. ESC and '[' not counted.

/*
 * Some convenience defines of the terminal escape sequencies. 
 *
 * Use following defines to get the escapesequence executed in the active 
 * terminal.
 *
 * Example of use:     
 * TERM_ProcessString( &pYourTerminalBuffer, TERM_MOVE_CURSOR_LEFT );
 */

#define TERM_ESC "\x1b"                 //!< The escape (ESC) character.
#define TERM_CSI "\x1b["                //!< The control sequence introducer CSI. ("escape sequence start")
#define TERM_MOVE_CURSOR_UP "\x1b[1A"   //!< Move cursor in terminal up one step.
#define TERM_MOVE_CURSOR_DOWN "\x1b[1B" //!< Move cursor in terminal down one step.
#define TERM_MOVE_CURSOR_RIGHT "\x1b[1C"//!< Move cursor in terminal right one step.
#define TERM_MOVE_CURSOR_LEFT "\x1b[1D" //!< Move cursor in terminal left one step.
#define TERM_CLEAR_TO_END "\x1b[J"      //!< Clear terminal from cursor to end of screen. Command ED
#define TERM_CLEAR_TO_START "\x1b[1J"   //!< Clear terminal from cursor to start of screen. Command ED
#define TERM_CLEAR_ALL "\x1b[2J"        //!< Clear whole terminal. Command ED
#define TERM_UNDER_LINE "\x1b[4m"       //!< Set the underline SGR parameter.
#define TERM_UNDER_LINE_OFF "\x1b[24m"  //!< Disable the underline SGR parameter.
#define TERM_STRIKE_THROUGH "\x1b[9m"   //!< Set the strikethrough SGR parameter.
#define TERM_STRIKE_THROUGH_OFF "\x1b[29m" //!< Disable the strikethrough SGR parameter.
#define TERM_INVERTED "\x1b[7m"         //!< Set the inverted/image:negative SGR parameter.
#define TERM_INVERTED_OFF "\x1b[27m"    //!< Disable the inverted/image:negative SGR parameter.


/***********
 * Structs.
 ***********/

//! Defines the graphic paramters for each character.
typedef struct TERM_attributes_struct{
  uint8_t singleUnderline: 1;   //!< singleUnderline = true will give underlined character.
  uint8_t strikeThrough: 1;     //!< strikThrough = true will give line through character.
  uint8_t imageInverted: 1;     //!< imageInverted = true will give negative image.
}TERM_graphicAttributes_t;

//! Defines the type used for graphical attributes on terminal character.
typedef struct TERM_termbufferdata_struct{
  uint8_t character;                             //!< The ASCII number for a single character.
  TERM_graphicAttributes_t attributes;  //!< The graphical attributes for a single character.
}TERM_termbufferdata_t;

//! Defines the terminal buffer.
typedef struct TERM_terminal_struct{
//  TERM_termbufferdata_t termBuffer[TERMFONT_LINES_PER_SCREEN][TERMFONT_CHARS_PER_LINE]; //!< All characters.
  TERM_termbufferdata_t termBufferRows[TERMFONT_LINES_PER_SCREEN][TERMFONT_CHARS_PER_LINE]; //!< All characters.
  TERM_termbufferdata_t * termBuffer[TERMFONT_LINES_PER_SCREEN]; //!< All characters.
  TERM_graphicAttributes_t currentAttributes;     //!< Last given set of graphical attributes.
  uint8_t escapeSequence[TERM_MAX_ESCAPE_LENGTH]; //!< Currently ongoing escape sequence state.
  int8_t escapeSequenceIndex;           //!< Keep track of current position in escape sequence.
  uint8_t escapeSequenceActive: 1;      //!< Flag to signal ongoing escape sequence.
  uint8_t directLCDUpdate: 1;           //!< Flag that all incoming characters should be displayed on LCD.
  uint8_t cursorVisible: 1;             //!< Flag that indicates if cursor is visible.
  uint8_t disableLineWrap: 1;           //!< Flag that indicates that line wrapping is disabled.
  uint8_t savedRow;                     //!< Save row address when "save cursor position" is executed.
  uint8_t savedColumn;                  //!< Save column address when "save cursor position" is executed.
  uint8_t currentRow;                   //!< Keep track of current row address.
  uint8_t currentColumn;                //!< Keep track of current column address.
  uint8_t tabstepsize;                  //!< The given tab step size.
  uint8_t verticaltabstepsize;          //!< The given vertical step size (not commonly used in terminals).
}TERM_terminal_t;


/***********************
 * Function prototypes.
 ***********************/
void TERM_Init( TERM_terminal_t * pTerminal, uint8_t tab, uint8_t verticaltab, bool cursorVisible ); //!< Initializes a terminal buffer (VT100 style) at given address in RAM. User need to define tab stepsize.
void TERM_ProcessChar(TERM_terminal_t * pTerminal, uint8_t dataIn);         //!< Receive character and place in buffer according to cursor placement. Checks if incoming data is part of escape code.
void TERM_ProcessString(TERM_terminal_t * pTerminal, uint8_t * pDataIn);    //!< Receive string and place in buffer according to cursor placement.
void TERM_EnableWriteThrough(TERM_terminal_t * pTerminal);                  //!< Enable direct update on LCD from next character.
void TERM_DisableWriteThrough(TERM_terminal_t * pTerminal);                 //!< Disable direct update on LCD from next character.
void TERM_Redraw(TERM_terminal_t * pTerminal);                              //!< Redraw LCD from buffer.
void TERM_EnableVisibleCursor(TERM_terminal_t * pTerminal);                 //!< Enable visible cursor.
void TERM_DisableVisibleCursor(TERM_terminal_t * pTerminal);                //!< Disable visible cursor.
uint8_t TERM_GetCursorRow(TERM_terminal_t * pTerminal);                     //!< Get cursor row placement. NOTE, not standard terminal protocol.
uint8_t TERM_GetCursorColumn(TERM_terminal_t * pTerminal);                  //!< Get cursor column placement. NOTE, not standard terminal protocol.

#endif
