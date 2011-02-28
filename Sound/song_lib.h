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
 * $Revision: 2418 $
 * $Date: 2007-09-17 02:40:19 -0600 (ma, 17 sep 2007) $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/Sound/song_lib.h $
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


#ifndef SONG_LIB_H
#define SONG_LIB_H

#include <stdint.h>
#include <cal.h>
#include "sound_driver.h"



/***********************
 * Function prototypes.
 ***********************/

//! This task is called periodically to control song playback. 
void SONG_CallbackControlTask( void );

//! This function initiates the sound driver and start song playback. 
void SONG_StartTune( FIFO_handle_t * pAudioFifoHandle, uint16_t const * pTuneInRam );

//! This task is called periodically to control song playback. 
void SONG_CallbackControlTask_F( void );

//! This function initiates the sound driver and start song playback. 
void SONG_StartTune_F( FIFO_handle_t * pAudioFifoHandle, uint16_t CAL_PGM(const * pTuneInFlash) );

//! This function stop any ongoing playback and flushes sound-fifo.
void SONG_StopTune( void );

//! This function returns true if there is ongoing audio playing.
bool SONG_busy( void );

#endif
