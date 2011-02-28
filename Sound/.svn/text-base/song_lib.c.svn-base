/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 song library.
 *
 * The song library contain handling of audio data and feed the audio fifo used 
 * by the sound driver. The functions start, stop and handle system sounds and 
 * music.
 *
 * The song library handles audio content from either SRAM or flash.
 *
 * It is up to the application to allocate memory for the audio fifo, and 
 * provide a pointer to valid audio data. The song library activates the driver 
 * for the audio playback and initiates the fifo handling.
 *
 * The song library repeatedly runs a task that checks for free space in the 
 * fifo and monitors if the currently playing tune should be stopped or 
 * repeated.
 *
 * Note, care should be taken not to have to small fifo for the audio content 
 * and/or to long delay between each callback for fifo stuffing. Each note 
 * consists of 4 bytes (2 integers), and shortest possible tone length is one 
 * timing tick. These two parameters is controlled by following defines:
 * SONG_PERIOD_TIME
 * SONG_LENGTH_OF_TONE
 * 
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
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/Sound/song_lib.c $
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
#include "song_lib.h"
#include <fifo_lib.h>
#include <timing_lib.h>

/*******************
 * Private defines.
 *******************/

#define SONG_PERIOD_TIME 9    //!< The period for repetetive callbacks from timing syst.
#define SONG_LENGTH_OF_TONE 4 //!< The number of bytes making up one tone.


/********************
 * Static variables.
 ********************/

static FIFO_handle_t * pSongFifoHandle; // The audio fifo handler.
static TIMING_event_t songTimingEvent;  // Timing event used for fifo filling.
static uint16_t const * pAudioDataInRam;
static uint16_t const * pAudioDataStartInRam;
static uint16_t CAL_PGM(const * pAudioDataInFlash);
static uint16_t CAL_PGM(const * pAudioDataStartInFlash);
static bool songPlaying = false;        // Keeps track on activity in song library.


/****************************
 * Function implementations.
 ****************************/

/*!
 *  Feeds the audio fifo from RAM, check repeat symbol in soundfiles and 
 *  stop timing events if incoming data show stop symbol. 
*/
void SONG_CallbackControlTask( void )
{
	uint8_t freeFifoSize = 0;
	
	// Check empty space in fifo.
	freeFifoSize = FIFO_GetItemsFree(pSongFifoHandle); 
	
	// Check if possible to load smallest amount of note data and
	// if the next element in array is stop symbol. 
	while ((freeFifoSize >= SONG_LENGTH_OF_TONE) && (*pAudioDataInRam != SOUND_STOP))  {
		// Is next a repeat symbol?
		if ( *pAudioDataInRam == SOUND_REPEAT ) {     
			// Reload pointer.
			pAudioDataInRam = pAudioDataStartInRam;   
			freeFifoSize -= SONG_LENGTH_OF_TONE;
		}
		// Read 4 bytes and move to fifo.
		else {                                      
			// Put duration of note in fifo.
			FIFO_PutUint16 ( pSongFifoHandle, *pAudioDataInRam++ ); 
			// Put Note value of note in fifo.
			FIFO_PutUint16( pSongFifoHandle, *pAudioDataInRam++ );  
			freeFifoSize -= SONG_LENGTH_OF_TONE;
		}
	}
	
	// End/Stop of audiofile?
	if ( *pAudioDataInRam == SOUND_STOP ) {	
		// Check that fifo is empty.
		if ( FIFO_IsEmpty(pSongFifoHandle) ) {	
			// Wait to finish last note.
			if ( !SOUND_IsToneBusy() ) {	
				SOUND_StopAudio();
			}
		}
	}
}


/*!
 *  The songdata are fetched from ram.
 *  Note, any ongoing audio will be interrupted. 
 *
 * \param  pAudioFifoHandle  FIFO to put song data into.
 * \param  pTuneInRam        Song data array.
 */
void SONG_StartTune( FIFO_handle_t * pAudioFifoHandle, uint16_t const * pTuneInRam )
{
	if ( songPlaying == true ) {
		SONG_StopTune();
	}
	
	// Give the fifo handler to song library.
	pSongFifoHandle = pAudioFifoHandle; 
	// Set flag that audio are playing.
	songPlaying = true;                 
	// Save start address of audio in SRAM.
	pAudioDataStartInRam = pTuneInRam;  
	// Save start address to working pointer.
	pAudioDataInRam = pTuneInRam;       
	
	/* Insert song event in timing events.
	   Repeat until manually removed, repeat every SONG_PERIOD_TIME, 
	   callback "SONG_CallbackControlTask", 
	   Timing event "songTimingEvent". */
	TIMING_AddRepCallbackEvent( TIMING_INFINITE_REPEAT, SONG_PERIOD_TIME, SONG_CallbackControlTask, &songTimingEvent );
	
	// Start up timers and fifo read out control.
	SOUND_StartAudio( pSongFifoHandle ); 
}


/*!
 *  Feeds the audio fifo from flash, check repeat symbol in soundfiles 
 *  and stop timing events if incoming data show stop symbol.
 */
void SONG_CallbackControlTask_F( void )
{
	uint8_t freeFifoSize = 0;
	
	// Check empty space in fifo.
	freeFifoSize = FIFO_GetItemsFree( pSongFifoHandle );  
	
	// Check if possible to load smallest amount of note data and
	// if the next element in array is stop symbol. 
	while ((freeFifoSize >= SONG_LENGTH_OF_TONE) && (CAL_pgm_read_word(pAudioDataInFlash) != SOUND_STOP)) {
		// Is next a repeat character?
		if ( CAL_pgm_read_word(pAudioDataInFlash) == SOUND_REPEAT ) {	
			// Reload pointer.
			pAudioDataInFlash = pAudioDataStartInFlash; 
			freeFifoSize -= SONG_LENGTH_OF_TONE;
		}
		// Read 4 bytes and move to fifo.
		else {
			// Put duration of note in fifo.
			FIFO_PutUint16 ( pSongFifoHandle, CAL_pgm_read_word(pAudioDataInFlash++) ); 
			// Put Note value of note in fifo.
			FIFO_PutUint16( pSongFifoHandle, CAL_pgm_read_word(pAudioDataInFlash++) );  
			freeFifoSize -= SONG_LENGTH_OF_TONE;
		}
	}

	// End/Stop of audiofile?
	if ( CAL_pgm_read_word(pAudioDataInFlash) == SOUND_STOP ) {	
		// Check that fifo is empty.
		if ( FIFO_IsEmpty(pSongFifoHandle) ) {	
			// Wait to finish last note.
			if ( !SOUND_IsToneBusy() ) {	
				SOUND_StopAudio();
			}
		}
	}
}


/*!
 *  The songdata are fetched from flash. 
 *  Note, any ongoing audio will be interrupted. 
 *
 * \param  pAudioFifoHandle  FIFO to put song data into.
 * \param  pTuneInFlash      Song data array.
 */
void SONG_StartTune_F( FIFO_handle_t * pAudioFifoHandle, uint16_t CAL_PGM(const * pTuneInFlash) )
{
	if (songPlaying == true) {
		SONG_StopTune();
	}

	// Give the fifo handler to song library.
	pSongFifoHandle = pAudioFifoHandle;     
	// Signal that audio are playing.
	songPlaying = true;                     
	// Save start address of audio in flash.
	pAudioDataStartInFlash = pTuneInFlash;  
	// Save start address to working pointer.
	pAudioDataInFlash = pTuneInFlash;       
	
	/* Insert song event in timing events.
	   Repeat until manually removed, repeat every SONG_PERIOD_TIME, 
	   callback "SONG_CallbackControlTask_F", 
	   Timing event "songTimingEvent". */
	TIMING_AddRepCallbackEvent( TIMING_INFINITE_REPEAT, SONG_PERIOD_TIME, SONG_CallbackControlTask_F, &songTimingEvent );
	
	// Start up timers and fifo read out control.
	SOUND_StartAudio( pAudioFifoHandle );   
}

void SONG_StopTune( void )
{
	// Call sound driver and stop timer1.
	SOUND_StopAudio();                      
	// Manually remove repetitive timing event.
	TIMING_RemoveEvent( &songTimingEvent ); 
	// Clear the audio fifo
	FIFO_Flush( pSongFifoHandle );          
	// Set flag -> not active.
	songPlaying = false;                    
}

bool SONG_busy( void )
{
	return songPlaying;
}
