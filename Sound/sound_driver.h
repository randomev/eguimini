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
 * $Revision: 2409 $
 * $Date: 2007-09-17 01:27:28 -0600 (ma, 17 sep 2007) $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/Sound/sound_driver.h $
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


#ifndef SOUND_DRIVER_H
#define SOUND_DRIVER_H

#include <stdint.h>
#include <fifo_lib.h>
#include <rtc_driver.h>

/*****************************************************************************
*
*   Tone definition. Each tone are set up with a value which will give the 
*   right frequency when applied to a 16-bits timer with PWM. These values are based
*   on a CLKcpu running @ 7372800 Hz.
*
*   First find the frequency for all tones. 
*
*   Formula:    ToneHz = Bf * 2^(ToneX/12)
*   
*   ToneX: the actual tone, e.g. C0 = 3
*   Bf: Basefrequency = 220Hz ("A")
*   (Note, the basefrequency used here is the lowest note defined in DB101. 
*   Standard musical pitch is often refered to 440Hz ("A" one octave higher))
*       
*   E.g: For tone C0 this would be:     C0 = 220 * 2^(3/12)
*                                       C0 = 261,6256...
*
*   Now we must find which value to put in a 16-bits timer with PWM to achieve 
*   this frequency
*
*   Formula:    Timer value = 7372800 Hz / ToneHz / 2
*
*   E.g: For tone C0 this would be:     Timer value = 7372800 / 261,6256... / 2
*                                       Timer value = 14090
*
*   Set up a 16-bits timer to run at Phase/Freq-correct PWM, top value = ICR1,
*   set OC1A when upcounting, clear when downcounting.
*   
*****************************************************************************/

/*! \brief Macro for tone duration time to timer tick conversion.
 *        May be used to transform time in seconds to corresponding number of 
 *        timer ticks for a tone to be played in sound driver.
 *        Parameter timeSec is given in seconds and allow decimals (e.g. 0.02s).
 *        Note that the calculation rounds the tick count down to the nearest integer.
 */
#define SOUND_timeToTimingTicks(timeSec) ((timeSec) * RTC_TICKS_PER_SECOND)

/*! \brief Macro for frequency to ICR register TOP value conversion.
 *        May be used to transform any frequency to be used with sound driver
 *        and song library functions.
 */
#define SOUND_freqToRegValue( freq ) ((CPU_F / freq )/2) 

#define SOUND_A   SOUND_freqToRegValue(220 )   //!< Tone, for music and sound creation.
#define SOUND_Ax  SOUND_freqToRegValue(233 )   //!< Tone, for music and sound creation.
#define SOUND_B   SOUND_freqToRegValue(247 )   //!< Tone, for music and sound creation.
#define SOUND_C0  SOUND_freqToRegValue(262 )   //!< Tone, for music and sound creation.
#define SOUND_Cx0 SOUND_freqToRegValue(277 )   //!< Tone, for music and sound creation.
#define SOUND_D0  SOUND_freqToRegValue(294 )   //!< Tone, for music and sound creation.
#define SOUND_Dx0 SOUND_freqToRegValue(311 )   //!< Tone, for music and sound creation.
#define SOUND_E0  SOUND_freqToRegValue(330 )   //!< Tone, for music and sound creation.
#define SOUND_F0  SOUND_freqToRegValue(349 )   //!< Tone, for music and sound creation.
#define SOUND_Fx0 SOUND_freqToRegValue(370 )   //!< Tone, for music and sound creation.
#define SOUND_G0  SOUND_freqToRegValue(392 )   //!< Tone, for music and sound creation.
#define SOUND_Gx0 SOUND_freqToRegValue(415 )   //!< Tone, for music and sound creation.
#define SOUND_A0  SOUND_freqToRegValue(440 )   //!< Tone, for music and sound creation.
#define SOUND_Ax0 SOUND_freqToRegValue(466 )   //!< Tone, for music and sound creation.
#define SOUND_B0  SOUND_freqToRegValue(494 )   //!< Tone, for music and sound creation.
#define SOUND_C1  SOUND_freqToRegValue(523 )   //!< Tone, for music and sound creation.
#define SOUND_Cx1 SOUND_freqToRegValue(554 )   //!< Tone, for music and sound creation.
#define SOUND_D1  SOUND_freqToRegValue(587 )   //!< Tone, for music and sound creation.
#define SOUND_Dx1 SOUND_freqToRegValue(622 )   //!< Tone, for music and sound creation.
#define SOUND_E1  SOUND_freqToRegValue(659 )   //!< Tone, for music and sound creation.
#define SOUND_F1  SOUND_freqToRegValue(698 )   //!< Tone, for music and sound creation.
#define SOUND_Fx1 SOUND_freqToRegValue(740 )   //!< Tone, for music and sound creation.
#define SOUND_G1  SOUND_freqToRegValue(784 )   //!< Tone, for music and sound creation.
#define SOUND_Gx1 SOUND_freqToRegValue(831 )   //!< Tone, for music and sound creation.
#define SOUND_A1  SOUND_freqToRegValue(880 )   //!< Tone, for music and sound creation.
#define SOUND_Ax1 SOUND_freqToRegValue(933 )   //!< Tone, for music and sound creation.
#define SOUND_B1  SOUND_freqToRegValue(988 )   //!< Tone, for music and sound creation.
#define SOUND_C2  SOUND_freqToRegValue(1046)   //!< Tone, for music and sound creation.
#define SOUND_Cx2 SOUND_freqToRegValue(1109)   //!< Tone, for music and sound creation.
#define SOUND_D2  SOUND_freqToRegValue(1175)   //!< Tone, for music and sound creation.
#define SOUND_Dx2 SOUND_freqToRegValue(1245)   //!< Tone, for music and sound creation.
#define SOUND_E2  SOUND_freqToRegValue(1318)   //!< Tone, for music and sound creation.
#define SOUND_F2  SOUND_freqToRegValue(1397)   //!< Tone, for music and sound creation.
#define SOUND_Fx2 SOUND_freqToRegValue(1480)   //!< Tone, for music and sound creation.
#define SOUND_G2  SOUND_freqToRegValue(1568)   //!< Tone, for music and sound creation.
#define SOUND_Gx2 SOUND_freqToRegValue(1661)   //!< Tone, for music and sound creation.
#define SOUND_A2  SOUND_freqToRegValue(1760)   //!< Tone, for music and sound creation.
#define SOUND_Ax2 SOUND_freqToRegValue(1865)   //!< Tone, for music and sound creation.
#define SOUND_B2  SOUND_freqToRegValue(1976)   //!< Tone, for music and sound creation.
#define SOUND_C3  SOUND_freqToRegValue(2093)   //!< Tone, for music and sound creation.
#define SOUND_Cx3 SOUND_freqToRegValue(2218)   //!< Tone, for music and sound creation.
#define SOUND_D3  SOUND_freqToRegValue(2350)   //!< Tone, for music and sound creation.
#define SOUND_Dx3 SOUND_freqToRegValue(2489)   //!< Tone, for music and sound creation.
#define SOUND_E3  SOUND_freqToRegValue(2637)   //!< Tone, for music and sound creation.
#define SOUND_F3  SOUND_freqToRegValue(2795)   //!< Tone, for music and sound creation.
#define SOUND_Fx3 SOUND_freqToRegValue(2961)   //!< Tone, for music and sound creation.
#define SOUND_G3  SOUND_freqToRegValue(3135)   //!< Tone, for music and sound creation.
#define SOUND_Gx3 SOUND_freqToRegValue(3321)   //!< Tone, for music and sound creation.
#define SOUND_A3  SOUND_freqToRegValue(3521)   //!< Tone, for music and sound creation.
#define SOUND_Ax3 SOUND_freqToRegValue(3731)   //!< Tone, for music and sound creation.
#define SOUND_B3  SOUND_freqToRegValue(3951)   //!< Tone, for music and sound creation.
#define SOUND_C4  SOUND_freqToRegValue(4184)   //!< Tone, for music and sound creation.

#define SOUND_PAUSE   1       //!< Pause. To be used as a note.

#define SOUND_STOP    0       //!< Stop/finished this song/tune.
#define SOUND_REPEAT  0xFFFF  //!< Repeat this song/tune.


/***********************
 * Function prototypes.
 ***********************/


//! Set up and start timer1 to be used for sound generation.
void SOUND_StartAudio( FIFO_handle_t * pAudioFifoHandle );

//! Stop timer1 for sound generation.
void SOUND_StopAudio( void );

//! This function returns "true" if there is a tone currently playing.
bool SOUND_IsToneBusy( void ); 


#endif
