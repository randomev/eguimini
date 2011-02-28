/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 sound demo application.
 *
 *         This application lets you listen to some differents song.
 *         It will first greet you with a checklist where you select
 *         which songs you want to here and then it plays them for you.
 *        
 *         You can skip to the next song by moving the joystick in any
 *         direction. When it have played all songs it will show you the 
 *         selection form again. If you don't want to listen anymore, 
 *         choose cancel.
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
 * $Revision: 2353 $
 * $Date: 2007-09-11 07:29:06 -0600 (ti, 11 sep 2007) $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/production_demo_rev_A/sounddemo.c $
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

#include "sound_driver.h"
#include "song_lib.h"
#include "song_data.h"    // Raw song data. "Music" files.
#include <backlight_driver.h>
#include <timing_lib.h>
#include <rtc_driver.h>
#include <joystick_driver.h>
#include <lcd_lib.h>
#include <fifo_lib.h>
#include <termfont_lib.h>
#include <rtc_driver.h>
#include <popup_lib.h>
#include <widgets_lib.h>
#include <dialog_lib.h>
#include <memblock_lib.h>

#include <stdint.h>
#include <common.h>



#define SONG_COUNT 12
char const CAL_PGM_DEF(SONG_formTitle[]) = "Select tunes";

char const CAL_PGM_DEF(* SONG_titles[SONG_COUNT]) = {
	SONG_furEliseTitle,
	SONG_turkeyMarchTitle,
	SONG_minuetTitle,
	SONG_solveigsSangTitle,
	SONG_siren1Title,
	SONG_siren2Title,
	SONG_whistleTitle,
	SONG_toneScaleTitle,
	SONG_positiveBeepTitle,
	SONG_negativeBeepTitle,
	SONG_disasterBeepTitle,
	SONG_positiveActionTitle
};

uint16_t const CAL_PGM_DEF(* SONG_data[SONG_COUNT]) = {
	SONG_furEliseTune,
	SONG_turkeyMarchTune,
	SONG_minuetTune,
	SONG_solveigsSangTune,
	SONG_siren1Tune,
	SONG_siren2Tune,
	SONG_whistleTune,
	SONG_toneScaleTune,
	SONG_positiveBeepTune,
	SONG_negativeBeepTune,
	SONG_disasterBeepTune,
	SONG_positiveActionTune
};

#define SONG_FIFO_SIZE 40 //!< The size of the Audio fifo. Corresponds to 10 notes, since one note is 4 bytes.


WIDGETS_id_t SONG_DoSelectionForm( WIDGETS_integer_t * songSelections )
{
	WIDGETS_id_t result = DIALOG_CANCEL;
	
	WIDGETS_SelectCtrl_t * checkWidgets = MEM_ALLOC_ARRAY( WIDGETS_SelectCtrl_t, SONG_COUNT );
	if (checkWidgets == NULL) {
		POPUP_MsgBox( 10, 2, 6, "Not enough\r\nmemory!", NULL );
	} else {
		result = DIALOG_CheckListBox_F( SONG_formTitle,
		                                SONG_COUNT,
		                                SONG_titles,
		                                checkWidgets,
		                                songSelections,
		                                DIALOG_OK | DIALOG_CANCEL,
		                                DIALOG_OK,
		                                true );
	}
	
	MEM_FREE( checkWidgets );
	return result;
}


void SONG_PlayTune( uint8_t idx, FIFO_handle_t * fifo )
{
	// Start song.
	///mt SONG_StartTune_F( fifo, SONG_data[idx] );
	SONG_StartTune_F( fifo, CAL_pgm_read_puint16(&SONG_data[idx]) );
/*	
	// Prepare popup message template.
	char message[16*4+1] =
			"Now playing:    "
			"\"               "
			"Move joystick   "
			"for next tune...";
	
	// Embed song title into message.
#if 0
	char const __flash * titleStr = SONG_titles[idx];
	char * messagePtr = message + 17;
	uint8_t charsLeft = 14;
	while ((*titleStr != 0) && (charsLeft > 0)) {
		*messagePtr++ = *titleStr++;
		--charsLeft;
	}
#else
	char const CAL_PGM(* titleStr) = CAL_pgm_read_pchar( &SONG_titles[idx] );
	char * messagePtr = message + 17;
	char ch;
	uint8_t charsLeft = 14;
	while ( ((ch = CAL_pgm_read_char(titleStr)) != 0) && (charsLeft > 0)) {
		*messagePtr++ = ch;
		titleStr++;
		--charsLeft;
	}
#endif

	*messagePtr++ = '"';
	while (charsLeft > 0) {
		*messagePtr++ = ' ';
		--charsLeft;
	}

	// Show popup until user moves joystick.	
	POPUP_MsgBox( 16, 4, 6, message, NULL );
	// Stop song.
	*/
	DELAY_MS(500);
	SONG_StopTune();
}

void SoundDemo( void )
{
	FIFO_data_t * fifoBuf = MEM_ALLOC_ARRAY( FIFO_data_t, SONG_FIFO_SIZE );
	WIDGETS_integer_t * songSelections = MEM_ALLOC_ARRAY( WIDGETS_integer_t, SONG_COUNT );
	
	if ((songSelections == NULL) || (fifoBuf == NULL)) {
		POPUP_MsgBox( 10, 2, 6, "Not enough\r\nmemory!", NULL );
	} else {
		// Prepare FIFO for song data.
		FIFO_handle_t fifo;
		FIFO_Init( &fifo, fifoBuf, SONG_FIFO_SIZE );
		
		// Prepare check box initial values.
		for (uint8_t i = 0; i < SONG_COUNT; ++i) {
			songSelections[i] = WIDGETS_SELECTCTRL_DESELECTED;
		}
		
		// Run selection dialog until canceled.
		WIDGETS_id_t result;
		do {
			// Run selection box.
			result = SONG_DoSelectionForm( songSelections );
			
			// Process selections if not canceled.
			if (result != DIALOG_CANCEL) {
				// Play selected songs.
				for (uint8_t i = 0; i < SONG_COUNT; ++i) {
					if (songSelections[i] == WIDGETS_SELECTCTRL_SELECTED) {
						SONG_PlayTune( i, &fifo );
					}
				}
			}
		} while (result != DIALOG_CANCEL);
	}
	
	MEM_FREE( fifoBuf );
	MEM_FREE( songSelections );
}


void PlaySound( int song )
{
	FIFO_data_t * fifoBuf = MEM_ALLOC_ARRAY( FIFO_data_t, SONG_FIFO_SIZE );
	WIDGETS_integer_t * songSelections = MEM_ALLOC_ARRAY( WIDGETS_integer_t, SONG_COUNT );
	
	// Prepare FIFO for song data.
	FIFO_handle_t fifo;
	FIFO_Init( &fifo, fifoBuf, SONG_FIFO_SIZE );
	
	SONG_PlayTune( song, &fifo );
	
	MEM_FREE( fifoBuf );
	MEM_FREE( songSelections );
}
