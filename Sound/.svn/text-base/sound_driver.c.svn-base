/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 sound driver.
 *
 * The sound driver provides the DB101 with basic functions to control the piezo
 * element. 
 *
 * DB101 uses the timer1 to drive the piezo element with a square wave. The 
 * driver is activated from other parts of the firmware with a function call to 
 * the SOUND_StartAudio function with a pointer to the fifo containing audio 
 * data to be played. The sound driver reads the fifo one tone at the time, set 
 * up the timer1 and insert a timing event to trigger when the tone ends.
 *
 * The tone is defined by two integers. First integer is the number of timing 
 * ticks the tone should be played, and the second integer is value to be loaded
 * into ICR register as TOP value for the PWM operation. 
 *
 * The timer 1 is operated in phase and frequency correct operation. Changing 
 * value, thus changing frequency, will give a smooth transition to next tone. 
 * Playing the same tone directly after each other will be experienced as one 
 * long tone.
 *
 * It is possible to take control over the sound driver with new audio content. 
 * To be sure not to interrupt currently ongoing tone, it is possible to check 
 * activity first with SOUND_IsToneBusy.
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
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/Sound/sound_driver.c $
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

#include "sound_driver.h"
#include <timing_lib.h>


/*****************************
 * Private defines and types.
 *****************************/

#define SOUND_CHECK_FIFO_DELAY  10  //!< The number of timing ticks before next time to check fifo.

static TIMING_event_t audioTimingEvent;
static FIFO_handle_t * pSoundfifoHandle;
static bool toneActive = false;           //!< Keep track of tone activity.


/********************************
 * Internal function prototypes.
 ********************************/

//! This task sets the note to be played.
static void SOUND_PlayAudioTask( void );


/****************************
 * Function implementations.
 ****************************/

/*! 
 *  Set up timing event for sound fifo check. 
 *
 * \param  pAudioFifoHandle  FIFO to read song data from.
 */
void SOUND_StartAudio( FIFO_handle_t * pAudioFifoHandle )
{
	// Enable timer 1.
	PRR0 &= ~(1 << PRTIM1); 
	// Set the piezo driving pin as output.
	DDRB |= (1<<PB7);       
	// Clear OC1C when upcounting, set when downcounting
	// Phase/Freq-correct PWM, top value = ICR1
	// Mode 8 in ATmega1281 datasheet.
	// No settings to be done in TCCR1C.
	TCCR1A = (1<<COM1C1);   
	TCCR1B = (1<<WGM13);    
	// Start Timer1, no prescaler.
	TCCR1B |= (1 << CS10);  
	
	pSoundfifoHandle = pAudioFifoHandle;
	
	// Set callback to check if there have been any data in fifo.
	TIMING_AddCallbackEventAfter( SOUND_CHECK_FIFO_DELAY, SOUND_PlayAudioTask, &audioTimingEvent );
}


/*!
 * The function is called from the timing event system with a fixed
 * time intervall. It Checks duration of a tone, and loads new tones 
 * when previous tone expire.
 * This task is initiated by the SOUND_StartAudio function, and is 
 * called from the timing system. 
 */
static void SOUND_PlayAudioTask( void )
{
	uint8_t storedSreg = SREG;
	CAL_disable_interrupt();
	
	uint16_t tone;

	// Length of tone defined as number of timing lib ticks.
	TIMING_time_t durationTicks = 0;  
	
	// Check if there are a complete note in fifo.
	if ( FIFO_HasData(pSoundfifoHandle, uint32_t) == true ) {	
		// Get length of tone.
		durationTicks = FIFO_GetUint16 ( pSoundfifoHandle ); 
		// Get frequency of tone.
		tone = FIFO_GetUint16( pSoundfifoHandle );  
		
		// If pause, stop timer1.
		if ( tone == SOUND_PAUSE ) {	
			toneActive = false;
			// Stop Timer1.
			TCCR1B &= ~(1 << CS10);
		} else {	
			toneActive = true;
			// Start Timer1.
			TCCR1B |= (1 << CS10);                  
		}
		
		// Following 2 lines may be uncommented if a strike effect is wanted when setting a new tone.
		// TCNT1H = 0;                  
		// TCNT1L = 0;
		
		// Adjust value to give square vawe.
		uint16_t tempToneH = tone / 2;  
		uint8_t tempToneL = tempToneH;
		// High byte -> Low byte.
		tempToneH >>= 8;                
		// Set output compare to half of TOP value.
		// Half value produces a square vawe out on PWM.
		OCR1CH = tempToneH;             
		OCR1CL = tempToneL;             
		
		// Read the TOP value.
		tempToneH = tone;               
		// High byte -> Low byte.
		tempToneH >>= 8;                
		// Load ICR1H, high byte (TOP value).
		ICR1H = tempToneH;              
		// Load ICR1L, low byte (TOP value).
		ICR1L = tone;                   
		
		// Add a callback timing event to this funtion after tone duration is ended.
		TIMING_AddCallbackEventAfter( durationTicks, SOUND_PlayAudioTask, &audioTimingEvent );
	}
	
	// No available data in fifo, go silent.
	else {	
		toneActive = false;
		// Stop Timer1.
		TCCR1B &= ~(1 << CS10);         
		
		// Set up timing event to fire and check the fifo after given delay.
		TIMING_AddCallbackEventAfter( SOUND_CHECK_FIFO_DELAY, SOUND_PlayAudioTask, &audioTimingEvent );
	}
	
	SREG = storedSreg;
}


/*!
 *  Remove timing event for sound fifo check. 
 */
void SOUND_StopAudio( void )
{
	toneActive = false;
	// Stop Timer1.
	TCCR1B &= ~(1 << CS10);   
	// Clear TOP value to make sure no note is played at next startup.
	ICR1H = 0;                
	ICR1L = 0;
	// Remove the "audio fifo check" event (active or not).
	TIMING_RemoveEvent( &audioTimingEvent );  
	// Release the piezo driving pin.
	DDRB &= ~(1<<PB7);                        
	// Disable timer 1.
	PRR0 |= (1 << PRTIM1);    
}


/*!
 * \return True if any tone is currently playing, false if not.
 */
bool SOUND_IsToneBusy( void )
{
	return toneActive;
}
