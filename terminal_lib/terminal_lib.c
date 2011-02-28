/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 terminal / VT100 library source file
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
 * $Revision: 2436 $
 * $Date: 2007-09-17 15:18:48 -0600 (ma, 17 sep 2007) $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/terminal_lib/terminal_lib.c $
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


#include "terminal_lib.h"
#include <termfont_lib.h>
#include <backlight_driver.h>
#include <common.h>


/*******************
 * Private defines.
 *******************/

#define TERMINAL_ESC 27             //!< Define the Escape ASCII value.
#define TERM_MAXPARAMDIGITS 4    //!< Maximum number of characters making up one parameter.
#define TERMINAL_MAXSGRPARAMETERS 4 //!< Maximum number of SGR parameters possible in DB101 


/*********************
 * Private constants.
 *********************/

static TERM_graphicAttributes_t TERM_defaultAttributes = { false, false, false };


/********************************
 * Internal function prototypes.
 ********************************/

/* mthomas: hmm, would be better to let the compiler decide
   about inlining - anyway, can be changed easily thru
   macros in cal.h later */

static inline void TERM_WriteBufferToLCD(TERM_terminal_t * pTerminal); //!< Write all contents to LCD.

static inline void TERM_ScrollTerminalOneLine(TERM_terminal_t * pTerminal);  //!< Scroll the terminal buffer one line.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessOrdinaryChar( TERM_terminal_t * pTerminal, uint8_t dataIn ) );
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessEscSeqChar( TERM_terminal_t * pTerminal, uint8_t dataIn ) );
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessControlCharacter(TERM_terminal_t * pTerminal, uint8_t graphicCharacter) ); //!< Process incoming graphic charcters (BEL, BS, TAB, LF, VT, FF, CR).
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessBackspace(TERM_terminal_t * pTerminal) );       //!< Process backspace (BS) in terminal.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessHorisontalTab(TERM_terminal_t *pTerminal) );    //!< Process horisontal tab (HT)(TAB) in given terminal.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessNewLine(TERM_terminal_t * pTerminal) );         //!< Process new line (NL) in given terminal.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessVerticalTab(TERM_terminal_t * pTerminal) );     //!< Process vertical tab (VT) in given terminal.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessFormFeed(TERM_terminal_t * pTerminal) );        //!< Process form feed (FF) in given terminal.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessCarriageReturn(TERM_terminal_t * pTerminal) );  //!< Process carriage return (CR) in given terminal.

/* Functions for Escape Code handling */
static inline void TERM_ProcessEscapeSequence(TERM_terminal_t * pTerminal, uint8_t data); //!< Checks what escape sequence functionaltiy is ordered.
static inline void TERM_DisplayCharOnLCD( TERM_termbufferdata_t const * data, uint8_t row, uint8_t column ); //!< Updates character on LCD display.
static inline void TERM_RemoveCursor(TERM_terminal_t * pTerminal); //!< Remove cursor and restore character on position.
static inline void TERM_DisplayCursor(TERM_terminal_t * pTerminal);     //!< Write cursor at position given in terminal struct.
static inline uint8_t TERM_ExtractParameter( uint8_t * str, uint16_t * resultStorage ); //!< Transform ASCII values in escape sequence to numbers.

CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessCursorUp(TERM_terminal_t * pTerminal) );         //!< CUU, move cursor n rows up.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessCursorDown(TERM_terminal_t * pTerminal) );       //!< CUD, move cursor n rows down.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessCursorRight(TERM_terminal_t * pTerminal) );      //!< CUF, move cursor right n columns.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessCursorLeft(TERM_terminal_t * pTerminal) );       //!< CUB, move cursor left n columns.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessCursorNextLine(TERM_terminal_t * pTerminal) );   //!< CNL, move cursor to beginning of next n lines. If n is omitted, next line is used.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessCursorPreviousLine(TERM_terminal_t * pTerminal) ); //!< CPL, move cursor to beginning of previous n lines. If n is omitted, previous line is used.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessCursorToColumn(TERM_terminal_t * pTerminal) );   //!< CHA, move cursor to column n.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessCursorToPosition(TERM_terminal_t * pTerminal) ); //!< CUP, move cursor to row n and column m.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessHMode( TERM_terminal_t * pTerminal ) );
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessLMode( TERM_terminal_t * pTerminal ) );
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessTerminalReset( TERM_terminal_t * pTerminal ) );
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessClearPartOfScreen(TERM_terminal_t * pTerminal) );//!< ED, Clears part of the screen. If n is zero (or missing), clear from cursor to end of screen. If n is one, clear from cursor to beginning of the screen. If n is two, clear entire screen and moves cursor to upper left.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ClearFromCursorToEnd(TERM_terminal_t * pTerminal) );    //!< Clear terminal buffer from cursor to end of screen.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ClearFromCursorToStart(TERM_terminal_t * pTerminal) );  //!< Clear terminal buffer from cursor to start of screen.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ClearWholeTerminal(TERM_terminal_t * pTerminal) );      //!< Clear the terminal buffer.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessClearPartOfLine(TERM_terminal_t * pTerminal) );  //!< EL, Erases part of the line. If n is zero (or missing), clear from cursor to the end of the line. If n is one, clear from cursor to beginning of the line. If n is two, clear entire line. Cursor position does not change.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ClearFromCursorToEndOfLine(TERM_terminal_t * pTerminal) ); //!< Clear from cursor to end of line.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ClearFromCursorToStartOfLine(TERM_terminal_t * pTerminal) ); //!< Clear from cursor to start of line.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ClearWholeLine(TERM_terminal_t * pTerminal) );          //!< Clear whole line.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessSelectGraphicRendition(TERM_terminal_t * pTerminal) ); //!< Select Graphic Rendition parameters.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessSgrParameter(TERM_terminal_t * pTerminal, uint16_t parameter) ); //!< Processes a Select Graphics Rendition parameter, given in SGR escape sequence. 
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessSaveCursorPosition(TERM_terminal_t * pTerminal) );      //!< SCP, Saves the cursor position.
CAL_ALWAYS_INLINE_DECLARE( static inline void TERM_ProcessRestoreCursorPosition(TERM_terminal_t * pTerminal) );   //!< RCP, Restore the cursor position.


/****************************
 * Function implementations.
 ****************************/



/*!
 * Initializes a terminal buffer (VT100 style) at given address in RAM. 
 * Settings correspond to defaults in standard Hyperterminal/VT100 terminal. 
 *
 * \param  pTerminal      Pointer to terminal to initialize
 * \param  tab            Step size for tab
 * \param  verticaltab    Step size for vertical tabs
 * \param  cursorVisible  True if the cursor should be visible
 */
void TERM_Init( TERM_terminal_t * pTerminal, uint8_t tab, uint8_t verticaltab, bool cursorVisible )
{
	// Prepare line pointers into term buffer.
	for	(uint8_t row = 0; row < TERMFONT_LINES_PER_SCREEN; ++row) {
		pTerminal->termBuffer[row] = pTerminal->termBufferRows[row];
	}
	
	// Clear the data in the terminal. Characters and SGR parameters.
	for(uint8_t row=0 ; row < TERMFONT_LINES_PER_SCREEN ; row++){
		TERM_termbufferdata_t * pItem = pTerminal->termBuffer[row];
		uint8_t columnsLeft = TERMFONT_CHARS_PER_LINE;
		do{
			pItem->character = ' ';
			pItem->attributes = TERM_defaultAttributes;

			--columnsLeft;
			++pItem;
		} while(columnsLeft > 0);
	}
	
	// Clear current SGR parameters (no underline, strikethrough or inverted).
	pTerminal->currentAttributes.singleUnderline = false;
	pTerminal->currentAttributes.strikeThrough = false;
	pTerminal->currentAttributes.imageInverted = false;
	
	// Make sure escape sequence status buffer is cleared.
	for(uint8_t i=0 ; i < (TERM_MAX_ESCAPE_LENGTH) ; i++){
		pTerminal->escapeSequence[i] = 0;
	}
	pTerminal->escapeSequenceIndex = 0;
	pTerminal->escapeSequenceActive = false;
	
	// Default no update of LCD screen.
	pTerminal->directLCDUpdate = false;
	pTerminal->cursorVisible = cursorVisible;
	pTerminal->disableLineWrap = false;
	pTerminal->savedRow = 0;
	pTerminal->savedColumn = 0;
	pTerminal->currentRow = 0;
	pTerminal->currentColumn = 0;
	pTerminal->tabstepsize = tab;
	pTerminal->verticaltabstepsize = verticaltab;
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 * \param  pDataIn    Pointer to null-terminated string
 */
void TERM_ProcessString(TERM_terminal_t * pTerminal, uint8_t * pDataIn)
{
	while (*pDataIn != 0) {
		TERM_ProcessChar( pTerminal, *pDataIn++ );
	}
}

// Only used from TERM_ProcessChar
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessOrdinaryChar( TERM_terminal_t * pTerminal, uint8_t dataIn ) )
{
	// Remove cursor on LCD if write through active.  
	TERM_RemoveCursor(pTerminal);

	// Is it printable?
	if ((dataIn >= ' ') && (dataIn <= '~')) {
		// Place character in buffer with current attributes.
		uint8_t const row = pTerminal->currentRow;
		uint8_t const column = pTerminal->currentColumn;
		TERM_termbufferdata_t * const bufferPtr =
				&(pTerminal->termBuffer[row][column]);
		bufferPtr->character = dataIn;
		bufferPtr->attributes = pTerminal->currentAttributes;
		
		// Update LCD if required.
		if(pTerminal->directLCDUpdate == true){
			TERM_DisplayCharOnLCD( bufferPtr, row, column );
		}
		
		// Advance cursor if not at end of line.
		if (pTerminal->currentColumn < (TERMFONT_CHARS_PER_LINE-1)){
			++(pTerminal->currentColumn);
		} else {
			pTerminal->currentColumn = 0;
			// Advance line if not a bottom of screen.
			if (pTerminal->currentRow < (TERMFONT_LINES_PER_SCREEN-1)){
				++(pTerminal->currentRow);
			} else {
				if (pTerminal->disableLineWrap == false) {
					TERM_ScrollTerminalOneLine(pTerminal);
				} else {
					pTerminal->currentRow = 0;
				}
			}
		}
	}
	// Is it a valid control character, then?
	else if ((dataIn >= 7) && (dataIn <= 13)) {
		TERM_ProcessControlCharacter(pTerminal, dataIn);
	}

	// Display cursor back on screen if required.			
	TERM_DisplayCursor(pTerminal);
}


// Only used from TERM_ProcessChar
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessEscSeqChar( TERM_terminal_t * pTerminal, uint8_t dataIn ) )
{
	// Take care of incoming escape sequence character.
	if (pTerminal->escapeSequenceIndex < TERM_MAX_ESCAPE_LENGTH){
		// Store character.
		pTerminal->escapeSequence[pTerminal->escapeSequenceIndex] = dataIn;
		
		// Advance index if that won't fill us up.
		if (pTerminal->escapeSequenceIndex < (TERM_MAX_ESCAPE_LENGTH - 1)) {
			++(pTerminal->escapeSequenceIndex);
		}

		// If there is a alphabet character, escape sequence is completed.
		if (((dataIn >= 'a') && (dataIn <= 'z')) || ((dataIn >= 'A') && (dataIn <= 'Z'))){
			// Not in an escape sequence anymore.
			pTerminal->escapeSequenceActive = false;

			// Process the sequence.
			TERM_RemoveCursor( pTerminal );
			TERM_ProcessEscapeSequence( pTerminal, dataIn );
			TERM_DisplayCursor( pTerminal );
		}
	}
}


/*!
 * Receive character and process it. Check if character is part of escape 
 * sequence or a control character and handles the character accordingly. 
 * If a complete escape sequence is received this function calls functions for 
 * escape code handling. 
 * If incoming character is not part of an escape sequence the charcter is 
 * placed in the terminal buffer according to cursor placement.
 *
 * \param  pTerminal  Pointer to affected terminal
 * \param  dataIn     Character to process
 */
void TERM_ProcessChar(TERM_terminal_t * pTerminal, uint8_t dataIn)
{
	// Is this a start of an escape sequence?
	if(dataIn == TERMINAL_ESC){
		pTerminal->escapeSequenceActive = true; // Escape sequence ongoing
		pTerminal->escapeSequenceIndex = -1;    // Set to -1 to signal start of escape sequence.
	} else {
		// Check that an escape sequence is not already active.
		if(pTerminal->escapeSequenceActive == false) {
			TERM_ProcessOrdinaryChar( pTerminal, dataIn );
		} else {
			// Have we only received an ESC, and are waiting for the '[' or other control codes?
			if (pTerminal->escapeSequenceIndex == -1) {
				switch (dataIn) {
					// Start of Control Sequence?
					case '[' :
						pTerminal->escapeSequenceIndex = 0; // Set index to 0, to indicte ready for received sequence payload.
					break;
					
					case 'c' :
						TERM_ProcessTerminalReset( pTerminal );
					break;
					
					default :
						pTerminal->escapeSequenceActive = false; // Did not get '[' after ESC, so we cancel the sequence.
					break;
				}
			} else{
				TERM_ProcessEscSeqChar( pTerminal, dataIn );
			}     // End, Start of escape sequence('[') check.
		}
	}
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 * \param  data       Escape sequence to process
 */
static void TERM_ProcessEscapeSequence( TERM_terminal_t * pTerminal, uint8_t data )
{
	switch (data) {
		case 'A': TERM_ProcessCursorUp(pTerminal); break;
		case 'B': TERM_ProcessCursorDown(pTerminal); break;
		case 'C': TERM_ProcessCursorRight(pTerminal); break;
		case 'D': TERM_ProcessCursorLeft(pTerminal); break;
		case 'E': TERM_ProcessCursorNextLine(pTerminal); break;
		case 'F': TERM_ProcessCursorPreviousLine(pTerminal); break;
		case 'G': TERM_ProcessCursorToColumn(pTerminal); break;
		case 'H': TERM_ProcessCursorToPosition(pTerminal); break;
		case 'J': TERM_ProcessClearPartOfScreen(pTerminal); break;
		case 'K': TERM_ProcessClearPartOfLine(pTerminal); break;
		case 'f': TERM_ProcessCursorToPosition(pTerminal); break;
		case 'h': TERM_ProcessHMode(pTerminal); break;
		case 'l': TERM_ProcessLMode(pTerminal); break;
		case 'm': TERM_ProcessSelectGraphicRendition(pTerminal); break;
		case 's': TERM_ProcessSaveCursorPosition(pTerminal); break;
		case 'u': TERM_ProcessRestoreCursorPosition(pTerminal); break;

  		default:
			// Escape sequencies not implemented. Ignore and go back to standard operation.
			// NOTE, the character received and interpreted as end character for an 
			// escape sequence end, will be lost.
    	break;
	}
}


/*!
 *  Uses the parameters given for each character to update chacter + underline, strikethough and inverted
 *
 * \param  data    Pointer to TERM_termbufferdata_t, which contains the character and attributes
 * \param  row     Which row (page) to print on
 * \param  column  Which column to print on
 */
static void TERM_DisplayCharOnLCD( TERM_termbufferdata_t const * data, uint8_t row, uint8_t column )
{
	uint8_t lcdPage = LCD_ROW2PAGE(row);
	
	// Display character first.
	TERMFONT_DisplayChar(data->character, lcdPage, (column*TERMFONT_CHAR_WIDTH));

	uint8_t ypos = lcdPage * LCD_PAGE_HEIGHT;
	uint8_t startColumn = column * TERMFONT_CHAR_WIDTH; // Adjust column to pixel y-address.
	uint8_t endColumn = startColumn + TERMFONT_CHAR_WIDTH - 1;

	if (data->attributes.singleUnderline == true) {
		uint8_t yoffset = TERMFONT_CHAR_HEIGHT - 1;
		LCD_FlipHLine( startColumn, endColumn, ypos + yoffset );
	}

	if (data->attributes.strikeThrough == true) {
		uint8_t yoffset = TERMFONT_CHAR_HEIGHT / 2;
		LCD_SetHLine( startColumn, endColumn, ypos + yoffset );
	}

	if (data->attributes.imageInverted == true){
		LCD_FlipPage( 0xff, lcdPage, startColumn, TERMFONT_CHAR_WIDTH);
	}
}


/*!
 * The scrolling is implemented by copying all lines one step up, 
 * and last line is cleared.
 *
 * \param  pTerminal  Pointer to the affected terminal
 */
static void TERM_ScrollTerminalOneLine(TERM_terminal_t * pTerminal)
{
	// Cycle rows.
	TERM_termbufferdata_t * * currentLine = pTerminal->termBuffer;
	TERM_termbufferdata_t * const firstLine = currentLine[0];
	uint8_t rowsLeft = TERMFONT_LINES_PER_SCREEN - 1;
	do {
		*currentLine = *(currentLine + 1);
		++currentLine;
	} while (--rowsLeft);
	*currentLine = firstLine;

	// Clear bottom line.
	TERM_termbufferdata_t * currentChar = firstLine;
	uint8_t columnsLeft = TERMFONT_CHARS_PER_LINE;
	do {
		currentChar->character = ' ';
		currentChar->attributes = TERM_defaultAttributes;
		++currentChar;
	} while (--columnsLeft);
	
	// Place cursor at start of last row.
	pTerminal->currentColumn = 0;
	pTerminal->currentRow = (TERMFONT_LINES_PER_SCREEN-1);
	
	// If LCD should be updated, clear line and draw cursor if required.
	if(pTerminal->directLCDUpdate == true){
		// Let hardware scroll one line for us.
		LCD_SetTopPage( (LCD_GetTopPage() + 1) % LCD_PAGE_COUNT, 0 );
		
		// Clear bottom line.
		LCD_ClrPage( 0xff,
					LCD_ROW2PAGE(TERMFONT_LINES_PER_SCREEN - 1),
					0,
					TERMFONT_CHARS_PER_LINE * TERMFONT_CHAR_WIDTH );
	}
}


/*!
 *
 * \param  pTerminal         Pointer to affected terminal
 * \param  graphicCharacter  Character to process
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessControlCharacter(TERM_terminal_t * pTerminal, uint8_t graphicCharacter) )
{
	switch (graphicCharacter) {
		case 8:  TERM_ProcessBackspace(pTerminal); break;
		case 9:  TERM_ProcessHorisontalTab(pTerminal); break;
		case 10: TERM_ProcessNewLine(pTerminal); break;
		case 11: TERM_ProcessVerticalTab(pTerminal); break;
		case 12: TERM_ProcessFormFeed(pTerminal); break;
		case 13: TERM_ProcessCarriageReturn(pTerminal); break;

		default:
			// Unsupported control characters will be ignored.
		break;
	}
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessBackspace(TERM_terminal_t * pTerminal) )
{
	// Step back once if room for it.
	if (pTerminal->currentColumn > 0) {
		--pTerminal->currentColumn;
	}
}

/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessHorisontalTab(TERM_terminal_t *pTerminal) )
{
  uint8_t column = pTerminal->currentColumn;
  if ((column + pTerminal->tabstepsize) > (TERMFONT_CHARS_PER_LINE-1)){
    column = (TERMFONT_CHARS_PER_LINE-1); // Hitting/exceeding right margin, place cursor to margin.
  }
  else{
    column += pTerminal->tabstepsize;     // Move cursor with tab-steps.
  }
  pTerminal->currentColumn = column;
}

/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessNewLine(TERM_terminal_t * pTerminal) )
{
	uint8_t row = pTerminal->currentRow;
	if (row == (TERMFONT_LINES_PER_SCREEN-1) ){
		if (pTerminal->disableLineWrap == false) {
			TERM_ScrollTerminalOneLine(pTerminal); // If cursor at last line, scroll terminal.
		} else {
			row = 0;
		}
	} else{
		row++;                                 // Go to next row.
	}
	pTerminal->currentRow = row;
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessVerticalTab(TERM_terminal_t * pTerminal) )
{
  uint8_t row = pTerminal->currentRow;
  if ((row + pTerminal->verticaltabstepsize) > (TERMFONT_LINES_PER_SCREEN-1)){
    row = (TERMFONT_LINES_PER_SCREEN-1);    // If cursor hits/passes bottom row, place cursor on last row.
  }
  else{
    row += pTerminal->verticaltabstepsize ; // Add vertical tab stepsize.
  }
  pTerminal->currentRow = row;
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessFormFeed(TERM_terminal_t * pTerminal) )
{
	TERM_ClearWholeTerminal( pTerminal );
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessCarriageReturn(TERM_terminal_t * pTerminal) )
{
	pTerminal->currentColumn = 0;     // Place cursor at beginning of line.
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
static void TERM_WriteBufferToLCD(TERM_terminal_t * pTerminal)
{
	for (uint8_t row = 0; row < TERMFONT_LINES_PER_SCREEN; ++row) {
		TERM_termbufferdata_t const * currentChar = pTerminal->termBuffer[row];
		uint8_t column = 0;
		do {
			TERM_DisplayCharOnLCD( currentChar, row, column );
			++currentChar;
			++column;
		} while (column < TERMFONT_CHARS_PER_LINE);
    }
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
void TERM_EnableWriteThrough(TERM_terminal_t * pTerminal)
{
	if (pTerminal->directLCDUpdate == false) {
		pTerminal->directLCDUpdate = true;
		TERM_DisplayCursor( pTerminal );
	}
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
void TERM_DisableWriteThrough(TERM_terminal_t * pTerminal)
{
	if (pTerminal->directLCDUpdate == true) {
		pTerminal->directLCDUpdate = false;
		TERM_RemoveCursor( pTerminal );
	}
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
void TERM_Redraw(TERM_terminal_t * pTerminal)
{
	if (pTerminal->directLCDUpdate == true) {
		TERM_RemoveCursor( pTerminal );
		TERM_WriteBufferToLCD( pTerminal );
		TERM_DisplayCursor( pTerminal );
	}
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
void TERM_EnableVisibleCursor(TERM_terminal_t * pTerminal)
{
	if (pTerminal->cursorVisible == false) {
		pTerminal->cursorVisible = true;
		TERM_DisplayCursor( pTerminal );
	}
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
void TERM_DisableVisibleCursor(TERM_terminal_t * pTerminal)
{
	if (pTerminal->cursorVisible == true) {
		pTerminal->cursorVisible = false;
		TERM_RemoveCursor( pTerminal );
	}
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
uint8_t TERM_GetCursorRow(TERM_terminal_t * pTerminal)
{
  return pTerminal->currentRow;
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
uint8_t TERM_GetCursorColumn(TERM_terminal_t * pTerminal)
{
  return pTerminal->currentColumn; 
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
static void TERM_RemoveCursor(TERM_terminal_t * pTerminal)
{
	if ((pTerminal->cursorVisible == true) && (pTerminal->directLCDUpdate == true)) {
		TERMFONT_FlipCursor( LCD_ROW2PAGE(pTerminal->currentRow),
				pTerminal->currentColumn * TERMFONT_CHAR_WIDTH );
	}
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
static void TERM_DisplayCursor(TERM_terminal_t * pTerminal)
{
	if ((pTerminal->cursorVisible == true) && (pTerminal->directLCDUpdate == true)) {
		TERMFONT_FlipCursor( LCD_ROW2PAGE(pTerminal->currentRow),
				pTerminal->currentColumn * TERMFONT_CHAR_WIDTH );
	}
}


/*!
 * \param  str            Pointer to escape sequence
 * \param  resultStorage  Pointer to allocated storage for the result string
 *
 * \return  Number of digits (characters) in resultStorage
 */
static uint8_t TERM_ExtractParameter( uint8_t * str, uint16_t * resultStorage )
{
	uint16_t result = 0;
	uint8_t digitCount = 0;

	// Extract as long as digits are found and we stay within maximum digit count.
	while ((*str >= '0') && (*str <= '9') && (digitCount < TERM_MAXPARAMDIGITS)) {
		result *= 10;              // Move previous digit one decimal position to the left.
		result += *str - '0';     // Convert ASCII to number and add to first position(rightmost).
		++str;
		++digitCount;
	}

	// Check that we actually found a number.
	if (digitCount > 0) {
		*resultStorage = result;
	}

	return digitCount;
}



/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessCursorUp(TERM_terminal_t * pTerminal) )
{
	// Extract distance from sequence start.
	uint16_t distance;
	uint8_t digitCount = TERM_ExtractParameter( pTerminal->escapeSequence, &distance );
	if (digitCount == 0){    // If there is no parameter given, default to 1.
		distance = 1;
	}
  
	// Don't move off edge of screen.
	if (distance > pTerminal->currentRow) {
		distance = pTerminal->currentRow;
	}
	
	// Move cursor.
	pTerminal->currentRow -= distance;
}



/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessCursorDown(TERM_terminal_t * pTerminal) )
{
	// Extract distance from sequence start.
	uint16_t distance;
	uint8_t digitCount = TERM_ExtractParameter( pTerminal->escapeSequence, &distance );
	if (digitCount == 0){    // If there is no parameter given, default to 1.
		distance = 1;
	}
  
	// Don't move off edge of screen.
	if (distance > (TERMFONT_LINES_PER_SCREEN - 1 - pTerminal->currentRow)) {
		distance = TERMFONT_LINES_PER_SCREEN - 1 - pTerminal->currentRow;
	}
	
	// Move cursor.
	pTerminal->currentRow += distance;
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessCursorRight(TERM_terminal_t * pTerminal) )
{
	// Extract distance from sequence start.
	uint16_t distance;
	uint8_t digitCount = TERM_ExtractParameter( pTerminal->escapeSequence, &distance );
	if (digitCount == 0){    // If there is no parameter given, default to 1.
		distance = 1;
	}
  
	// Don't move off edge of screen.
	if (distance > (TERMFONT_CHARS_PER_LINE - 1 - pTerminal->currentColumn)) {
		distance = TERMFONT_CHARS_PER_LINE - 1 - pTerminal->currentColumn;
	}
	
	// Move cursor.
	pTerminal->currentColumn += distance;
}



/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessCursorLeft(TERM_terminal_t * pTerminal) )
{
	// Extract distance from sequence start.
	uint16_t distance;
	uint8_t digitCount = TERM_ExtractParameter( pTerminal->escapeSequence, &distance );
	if (digitCount == 0){    // If there is no parameter given, default to 1.
		distance = 1;
	}
  
	// Don't move off edge of screen.
	if (distance > pTerminal->currentColumn) {
		distance = pTerminal->currentColumn;
	}
	
	// Move cursor.
	pTerminal->currentColumn -= distance;
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessCursorNextLine(TERM_terminal_t * pTerminal) )
{  
	// Similar to "move down", but we also move to start of column.
	TERM_ProcessCursorDown( pTerminal );
	pTerminal->currentColumn = 0;
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessCursorPreviousLine(TERM_terminal_t * pTerminal) )
{
	// Similar to "move up", but we also move to start of column.
	TERM_ProcessCursorUp( pTerminal );
	pTerminal->currentColumn = 0;
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessCursorToColumn(TERM_terminal_t * pTerminal) )
{
	// Extract column parameter from start of sequence.
	uint16_t newColumn;
	uint8_t digitCount = TERM_ExtractParameter( pTerminal->escapeSequence, &newColumn );
	if (digitCount != 0) {
		// Don't move off edge.
		if (newColumn < 1) {
			newColumn = 1;
		} else if (newColumn > TERMFONT_CHARS_PER_LINE) {
			newColumn = TERMFONT_CHARS_PER_LINE;
		}
		
		// Move cursor.
		pTerminal->currentColumn = newColumn - 1; // Internal index starts on 0, parameter start on 1.
	}
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessCursorToPosition(TERM_terminal_t * pTerminal) )
{  
	uint8_t digitCount;

	// Extract new row value from start of sequence.
	uint16_t newRow;
	digitCount = TERM_ExtractParameter( pTerminal->escapeSequence, &newRow );
	if (digitCount == 0) {
		newRow = 1;
	}

	// Extract new column value from position after ';' after row parameter.
	uint16_t newColumn;
	digitCount = TERM_ExtractParameter( pTerminal->escapeSequence + digitCount + 1, &newColumn);
	if (digitCount == 0) {
		newColumn = 1;
	}
	
	// Don't move off edges of creen.
  	if (newRow < 1) {
		newRow = 1;
	} else if (newRow > TERMFONT_LINES_PER_SCREEN) {
		newRow = TERMFONT_LINES_PER_SCREEN;
	}
	
	if (newColumn < 1) {
		newColumn = 1;
	} else if (newColumn > TERMFONT_CHARS_PER_LINE) {
		newColumn = TERMFONT_CHARS_PER_LINE;
	}
	
	// Apply new cursor position.
    pTerminal->currentRow = newRow - 1;
    pTerminal->currentColumn = newColumn - 1;
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessHMode( TERM_terminal_t * pTerminal ) )
{
	// Extract the mode parameter.
	uint16_t parameter;
	uint8_t digitCount = TERM_ExtractParameter( pTerminal->escapeSequence, &parameter );
	if (digitCount == 0){
		parameter = 0;  // No parameter given in the escape sequence, use '0'.
	}
	
	// Act on parameter.
	switch (parameter) {
		case 7 : pTerminal->disableLineWrap = false; break;
		
		default:
			// Ignore unsupported.
		break;
	}
}

/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessLMode( TERM_terminal_t * pTerminal ) )
{
	// Extract the mode parameter.
	uint16_t parameter;
	uint8_t digitCount = TERM_ExtractParameter( pTerminal->escapeSequence, &parameter );
	if (digitCount == 0){
		parameter = 0;  // No parameter given in the escape sequence, use '0'.
	}
	
	// Act on parameter.
	switch (parameter) {
		case 7 : pTerminal->disableLineWrap = true; break;
		
		default:
			// Ignore unsupported.
		break;
	}
}

/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessTerminalReset( TERM_terminal_t * pTerminal ) )
{
	bool const wasOnScreen = pTerminal->directLCDUpdate;

	TERM_Init(pTerminal, pTerminal->tabstepsize, pTerminal->tabstepsize, pTerminal->cursorVisible);
	if (wasOnScreen) {
		BACKLIGHT_SetRGB(0x00, 0x00, 0x00);
		TERM_EnableWriteThrough( pTerminal );
		TERM_Redraw( pTerminal );
	}
}



/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessClearPartOfScreen(TERM_terminal_t * pTerminal) )
{
	// Extract the escape sequence parameter.
	uint16_t parameter;
	uint8_t digitCount = TERM_ExtractParameter( pTerminal->escapeSequence, &parameter);
	if (digitCount == 0){
		parameter = 0;  // No parameter given in the escape sequence, use '0'.
	}

	// Branch to correct handler.	
	switch (parameter) {
		case 0: TERM_ClearFromCursorToEnd(pTerminal); break;
		case 1: TERM_ClearFromCursorToStart(pTerminal); break;
		case 2: TERM_ClearWholeTerminal(pTerminal); break;
		
		default:  // Error, only 0 -> 2 allowed in ED excape sequence.
			// Ignore unsupported.
		break;
	}
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ClearFromCursorToEnd(TERM_terminal_t * pTerminal) )
{
  uint8_t rowStop = pTerminal->currentRow;
  // Clear from currect row and column.
  for (uint8_t row = rowStop ; row < TERMFONT_LINES_PER_SCREEN ; row++){
    TERM_termbufferdata_t * pItem = pTerminal->termBuffer[row];
    uint8_t columnsLeft = 0;
    if (row == rowStop){  // Clear from column address on the row where the cursor is.
      columnsLeft = pTerminal->currentColumn;
      pItem = pItem + columnsLeft;
    }
    else{                 // Clear from column address 0 on other rows.
      columnsLeft = 0;
    }
    do{                   // Clearing is done in this do-while.
      pItem->character = ' ';
      pItem->attributes = TERM_defaultAttributes;

      ++columnsLeft;
      ++pItem;
    } while(columnsLeft < TERMFONT_CHARS_PER_LINE );
  }
  // If LCD should be updated, redraw buffer.
  if(pTerminal->directLCDUpdate == true){
    TERM_WriteBufferToLCD(pTerminal);
  }
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ClearFromCursorToStart(TERM_terminal_t * pTerminal) )
{
  uint8_t rowStop = pTerminal->currentRow;
  // Clear from first row and column.
  for (uint8_t row = 0 ; row <= rowStop ; row++){
    TERM_termbufferdata_t * pItem = pTerminal->termBuffer[row];
    uint8_t columnsLeft = 0;
    if (row == rowStop){  // Clear from column address on the row where the cursor is.
      columnsLeft = pTerminal->currentColumn + 1;
    }
    else{                 // Clear from last column address on other rows.
      columnsLeft = TERMFONT_CHARS_PER_LINE;
    }
    do{                   // Clearing is done in this do-while.
      pItem->character = ' ';
      pItem->attributes = TERM_defaultAttributes;

      --columnsLeft;
      ++pItem;
    } while(columnsLeft > 0);
  }
  // If LCD should be updated, redraw buffer.
  if(pTerminal->directLCDUpdate == true)
  {
    TERM_WriteBufferToLCD(pTerminal);
  }
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ClearWholeTerminal(TERM_terminal_t * pTerminal) )
{
  // Clear from address first row and first column.
  for(uint8_t row=0 ; row < TERMFONT_LINES_PER_SCREEN ; row++){
    TERM_termbufferdata_t * pItem = pTerminal->termBuffer[row];
    uint8_t columnsLeft = TERMFONT_CHARS_PER_LINE;
    do{
      pItem->character = ' ';
      pItem->attributes = TERM_defaultAttributes;

      --columnsLeft;
      ++pItem;
    } while(columnsLeft > 0);
  }

  // If LCD should be updated, clear screen.
  if(pTerminal->directLCDUpdate == true){
    TERM_WriteBufferToLCD(pTerminal);
  }
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessClearPartOfLine(TERM_terminal_t * pTerminal) )
{
	// Extract the escape sequence parameter.
	uint16_t parameter;
	uint8_t digitCount = TERM_ExtractParameter( pTerminal->escapeSequence, &parameter);
	if (digitCount == 0){
		parameter = 0;  // No parameter given in the escape sequence, use '0'.
	}

	// Branch to correct handler.
	switch (parameter) {
		case 0: TERM_ClearFromCursorToEndOfLine(pTerminal); break;
		case 1: TERM_ClearFromCursorToStartOfLine(pTerminal); break;
		case 2: TERM_ClearWholeLine(pTerminal); break;
		
		default:
			// Ignore unsupported.
		break;
	}
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ClearFromCursorToEndOfLine(TERM_terminal_t * pTerminal) )
{
  uint8_t row = pTerminal->currentRow;
  TERM_termbufferdata_t * pItem = pTerminal->termBuffer[row];
  uint8_t columnsLeft = pTerminal->currentColumn;
  pItem = pItem + columnsLeft;
  do{   // The clearing of data is done in this do-while.
    pItem->character = ' ';
    pItem->attributes = TERM_defaultAttributes;

    ++columnsLeft;
    ++pItem;
  } while(columnsLeft < TERMFONT_CHARS_PER_LINE );
  // If LCD should be updated, redraw buffer.
  if(pTerminal->directLCDUpdate == true){
    TERM_WriteBufferToLCD(pTerminal);
  }
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ClearFromCursorToStartOfLine(TERM_terminal_t * pTerminal) )
{
  uint8_t row = pTerminal->currentRow;
  TERM_termbufferdata_t * pItem = pTerminal->termBuffer[row];
  uint8_t columnsLeft = pTerminal->currentColumn + 1;
  do{   // The clearing of data is done in this do-while.
    pItem->character = ' ';
    pItem->attributes = TERM_defaultAttributes;

    --columnsLeft;
    ++pItem;
  } while(columnsLeft > 0);
  // If LCD should be updated, redraw buffer.
  if(pTerminal->directLCDUpdate == true)
  {
    TERM_WriteBufferToLCD(pTerminal);
  }
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ClearWholeLine(TERM_terminal_t * pTerminal) )
{
  uint8_t row = pTerminal->currentRow;
  TERM_termbufferdata_t * pItem = pTerminal->termBuffer[row];
  uint8_t columnsLeft = TERMFONT_CHARS_PER_LINE;
  do{   // The clearing of data is done in this do-while.
    pItem->character = ' ';
    pItem->attributes = TERM_defaultAttributes;

    --columnsLeft;
    ++pItem;
  } while(columnsLeft > 0);
  // If LCD should be updated, clear screen.
  if(pTerminal->directLCDUpdate == true){
    TERM_WriteBufferToLCD(pTerminal);
  }
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessSelectGraphicRendition(TERM_terminal_t * pTerminal) )
{
	uint8_t digitCount;
	uint8_t startIndex = 0;
	do{
		uint16_t parameter;
		digitCount = TERM_ExtractParameter( pTerminal->escapeSequence + startIndex, &parameter);
		startIndex += digitCount + 1; // Calculate next index start. +1 to skip ';' separator.
		if (digitCount > 0) {
			TERM_ProcessSgrParameter( pTerminal, parameter );
		}
	} while (digitCount != 0); // Extract parameters until the escape sequence is emptied.
}



/*!
 * \param  pTerminal  Pointer to affected terminal
 * \param  parameter  16bit Select Graphics Rendition escape sequence
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessSgrParameter(TERM_terminal_t * pTerminal, uint16_t parameter) )
{
	switch( parameter ) {
		case 0: // Reset, all attributes off.
			pTerminal->currentAttributes = TERM_defaultAttributes;
			BACKLIGHT_SetRGB(0x00, 0x00, 0x00);  //Set backlight OFF.
		break;
		
		case 4:  pTerminal->currentAttributes.singleUnderline = true; break;
		case 7:  pTerminal->currentAttributes.imageInverted = true; break;
		case 9:  pTerminal->currentAttributes.strikeThrough = true; break;
		case 24: pTerminal->currentAttributes.singleUnderline = false; break;
		case 27: pTerminal->currentAttributes.imageInverted = false; break;
		case 29: pTerminal->currentAttributes.strikeThrough = false; break;
		case 40: BACKLIGHT_SetRGB( 0x00, 0x00, 0x00 ); break;
		case 41: BACKLIGHT_SetRGB( 0xFF, 0x00, 0x00 ); break;
		case 42: BACKLIGHT_SetRGB( 0x00, 0xFF, 0x00 ); break;
		case 43: BACKLIGHT_SetRGB( 0xFF, 0x55, 0x00 ); break;
		case 44: BACKLIGHT_SetRGB( 0x00, 0x00, 0xFF ); break;
		case 45: BACKLIGHT_SetRGB( 0xFF, 0x00, 0x80 ); break;
		case 46: BACKLIGHT_SetRGB( 0x00, 0x80, 0x80 ); break;
		case 47: BACKLIGHT_SetRGB( 0xFF, 0x80, 0x80 ); break;
		case 49: BACKLIGHT_SetRGB( 0x00, 0x00, 0x00 ); break;
		
		default:
			// Ingore unsupported.
		break;
	}
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessSaveCursorPosition(TERM_terminal_t * pTerminal) )
{
	pTerminal->savedRow = pTerminal->currentRow;
	pTerminal->savedColumn = pTerminal->currentColumn;
}


/*!
 * \param  pTerminal  Pointer to affected terminal
 */
CAL_ALWAYS_INLINE_DEFINE( static inline void TERM_ProcessRestoreCursorPosition(TERM_terminal_t * pTerminal) )
{
	pTerminal->currentRow = pTerminal->savedRow;
	pTerminal->currentColumn = pTerminal->savedColumn;
}
