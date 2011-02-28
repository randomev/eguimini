/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 song data file.
 *
 *      This file contains audio data for the song library in DB101. 
 *      The audio data is given as an array of uint16_t. Each note contains of a 
 *      duration and a timer value corresponding to a frequency. Please view
 *      sound_driver.h and sound_driver.c regarding the interpretation of the 
 *      data given here.
 *
 *      The final information in an audio array needs to be SOUND_STOP or 
 *      SOUND_REPEAT. If not included the song library will continue read 
 *      following memory addresses after ended audio data as sound.
 *
 *      The audio format is as follows:
 *        title[] ={
 *                  duration1, tone1, duration2, tone2 etc..
 *                  lastduration, lasttone,
 *                  SOUND_STOP or SOUND_REPEAT
 *                  };
 *
 *      In this file it is used macro's where the duration time of a tone is 
 *      given in seconds (SOUND_timeToTimingTicks( timeSec )) and the tone is 
 *      given in Hz (SOUND_freqToRegValue( freq )) or directly as a note 
 *      (e.g. SOUND_A1 for A440 = 440Hz).
 *      
 *      Note, it is possible to use SOUND_PAUSE instead of a tone to insert 
 *      silence.
 *
 *      One duration unit correspond to one tick in the timing library.
 *
 *      The Title for each tune (or sound) is available. The naming should be
 *      selfexplaining ( i.e. SONG_FurEliseTitle and SONG_FurEliseTune ).
 *
 *      Note, more information and definitions on notes in the sound_driver.h.
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
 * $Revision: 2168 $
 * $Date: 2007-07-10 03:05:47 -0600 (ti, 10 jul 2007) $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/Sound/song_data.h $
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

// Für Elise.
char CAL_PGM_DEF(SONG_furEliseTitle[])       = "Fuer Elise";

const uint16_t CAL_PGM_DEF(SONG_furEliseTune[]) = {
  SOUND_timeToTimingTicks(0.125),SOUND_E2, 
  SOUND_timeToTimingTicks(0.125), SOUND_Dx2,
  SOUND_timeToTimingTicks(0.125),SOUND_E2, 
  SOUND_timeToTimingTicks(0.125),SOUND_Dx2, 
  SOUND_timeToTimingTicks(0.125),SOUND_E2, 
  SOUND_timeToTimingTicks(0.125),SOUND_B1, 
  SOUND_timeToTimingTicks(0.125),SOUND_D2,
  SOUND_timeToTimingTicks(0.125),SOUND_C2, 
  SOUND_timeToTimingTicks(0.25),SOUND_A1, 
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.125),SOUND_C1, 
  SOUND_timeToTimingTicks(0.125),SOUND_E1, 
  SOUND_timeToTimingTicks(0.125),SOUND_A1, 
  SOUND_timeToTimingTicks(0.25),SOUND_B1, 
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE,    
  SOUND_timeToTimingTicks(0.125),SOUND_E1, 
  SOUND_timeToTimingTicks(0.125),SOUND_Gx1,
  SOUND_timeToTimingTicks(0.125),SOUND_B1, 
  SOUND_timeToTimingTicks(0.25),SOUND_C2, 
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE,  
  SOUND_timeToTimingTicks(0.125),SOUND_E1, 
  SOUND_timeToTimingTicks(0.125),SOUND_E2, 
  SOUND_timeToTimingTicks(0.125),SOUND_Dx2,
  SOUND_timeToTimingTicks(0.125),SOUND_E2, 
  SOUND_timeToTimingTicks(0.125),SOUND_Dx2,
  SOUND_timeToTimingTicks(0.125),SOUND_E2, 
  SOUND_timeToTimingTicks(0.125),SOUND_B1,   
  SOUND_timeToTimingTicks(0.125),SOUND_D2, 
  SOUND_timeToTimingTicks(0.125),SOUND_C2, 
  SOUND_timeToTimingTicks(0.25),SOUND_A1, 
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE,  
  SOUND_timeToTimingTicks(0.125),SOUND_C1, 
  SOUND_timeToTimingTicks(0.125),SOUND_E1, 
  SOUND_timeToTimingTicks(0.125),SOUND_A1, 
  SOUND_timeToTimingTicks(0.25),SOUND_B1, 
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE,    
  SOUND_timeToTimingTicks(0.125),SOUND_E1, 
  SOUND_timeToTimingTicks(0.125),SOUND_C2, 
  SOUND_timeToTimingTicks(0.125),SOUND_B1, 
  SOUND_timeToTimingTicks(0.25),SOUND_A1, 
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE,
  SOUND_REPEAT
};

// Turkey march.
char CAL_PGM_DEF(SONG_turkeyMarchTitle[])       = "Turkey march"; // By Mozart.

const uint16_t CAL_PGM_DEF(SONG_turkeyMarchTune[]) = {
  SOUND_timeToTimingTicks(0.125),SOUND_Fx1,
  SOUND_timeToTimingTicks(0.125),SOUND_E1,
  SOUND_timeToTimingTicks(0.125),SOUND_Dx1,
  SOUND_timeToTimingTicks(0.125),SOUND_E1,  
  SOUND_timeToTimingTicks(0.375),SOUND_G1,
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE, 

  SOUND_timeToTimingTicks(0.125),SOUND_A1,
  SOUND_timeToTimingTicks(0.125),SOUND_G1,
  SOUND_timeToTimingTicks(0.125),SOUND_Fx1,
  SOUND_timeToTimingTicks(0.125),SOUND_G1,
  SOUND_timeToTimingTicks(0.375),SOUND_B1,
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE, 

  SOUND_timeToTimingTicks(0.125),SOUND_C2,
  SOUND_timeToTimingTicks(0.125),SOUND_B1,
  SOUND_timeToTimingTicks(0.125),SOUND_Ax1,
  SOUND_timeToTimingTicks(0.125),SOUND_B1,
  SOUND_timeToTimingTicks(0.125),SOUND_Fx2,
  SOUND_timeToTimingTicks(0.125),SOUND_E2,
  SOUND_timeToTimingTicks(0.125),SOUND_Dx2,
  SOUND_timeToTimingTicks(0.125),SOUND_E2, 
  SOUND_timeToTimingTicks(0.125),SOUND_Fx2,
  SOUND_timeToTimingTicks(0.125),SOUND_E2,
  SOUND_timeToTimingTicks(0.125),SOUND_Dx2,
  SOUND_timeToTimingTicks(0.125),SOUND_E2,  
  SOUND_timeToTimingTicks(0.375),SOUND_G2,
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE, 

  SOUND_timeToTimingTicks(0.125),SOUND_E2,
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.125),SOUND_G2,
  SOUND_timeToTimingTicks(0.04),SOUND_PAUSE, 

  SOUND_timeToTimingTicks(0.04),SOUND_D2,
  SOUND_timeToTimingTicks(0.04),SOUND_E2,
  SOUND_timeToTimingTicks(0.25),SOUND_Fx2, 
  SOUND_timeToTimingTicks(0.125),SOUND_E2,
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.125),SOUND_D2,
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.125),SOUND_E2,
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE, 
  
  SOUND_timeToTimingTicks(0.04),SOUND_D2,
  SOUND_timeToTimingTicks(0.04),SOUND_E2,
  SOUND_timeToTimingTicks(0.25),SOUND_Fx2,
  SOUND_timeToTimingTicks(0.125),SOUND_E2,
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.125),SOUND_D2,
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.125),SOUND_E2,
  SOUND_timeToTimingTicks(0.04),SOUND_PAUSE, 
  
  SOUND_timeToTimingTicks(0.04),SOUND_D2,
  SOUND_timeToTimingTicks(0.04),SOUND_E2,
  SOUND_timeToTimingTicks(0.25),SOUND_Fx2, 
  SOUND_timeToTimingTicks(0.125),SOUND_E2,
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.125),SOUND_D2,
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.125),SOUND_Cx2,
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.375),SOUND_B1, 
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE, 

  SOUND_REPEAT
};

// Minuet.
char CAL_PGM_DEF(SONG_minuetTitle[])       = "Minuet";

const uint16_t CAL_PGM_DEF(SONG_minuetTune[]) = 
{
  SOUND_timeToTimingTicks(0.25),SOUND_D2,
  SOUND_timeToTimingTicks(0.125),SOUND_G1,
  SOUND_timeToTimingTicks(0.125),SOUND_A1,
  SOUND_timeToTimingTicks(0.125),SOUND_B1,
  SOUND_timeToTimingTicks(0.125),SOUND_C2, 
  SOUND_timeToTimingTicks(0.25),SOUND_D2,
  SOUND_timeToTimingTicks(0.25),SOUND_G1,
  SOUND_timeToTimingTicks(0.25),SOUND_G1,
  SOUND_timeToTimingTicks(0.25),SOUND_E2,
  SOUND_timeToTimingTicks(0.125),SOUND_C2, 
  SOUND_timeToTimingTicks(0.125),SOUND_D2,
  SOUND_timeToTimingTicks(0.125),SOUND_E2,
  SOUND_timeToTimingTicks(0.125),SOUND_Fx2,
  SOUND_timeToTimingTicks(0.25),SOUND_G2,
  SOUND_timeToTimingTicks(0.25),SOUND_G1, 
  SOUND_timeToTimingTicks(0.25),SOUND_G1,
  SOUND_timeToTimingTicks(0.25),SOUND_C2,
  SOUND_timeToTimingTicks(0.125),SOUND_D2,
  SOUND_timeToTimingTicks(0.125),SOUND_C2,
  SOUND_timeToTimingTicks(0.125),SOUND_B1, 
  SOUND_timeToTimingTicks(0.125),SOUND_A1,
  SOUND_timeToTimingTicks(0.25),SOUND_B1,
  SOUND_timeToTimingTicks(0.125),SOUND_C2,
  SOUND_timeToTimingTicks(0.125),SOUND_B1,
  SOUND_timeToTimingTicks(0.125),SOUND_A1, 
  SOUND_timeToTimingTicks(0.125),SOUND_G1,
  SOUND_timeToTimingTicks(0.25),SOUND_Fx1,
  SOUND_timeToTimingTicks(0.125),SOUND_G1,
  SOUND_timeToTimingTicks(0.125),SOUND_A1,
  SOUND_timeToTimingTicks(0.125),SOUND_B1, 
  SOUND_timeToTimingTicks(0.125),SOUND_G1,
  SOUND_timeToTimingTicks(0.25),SOUND_B1,
  SOUND_timeToTimingTicks(0.25),SOUND_A1,
  SOUND_timeToTimingTicks(0.032),SOUND_PAUSE, 
  SOUND_REPEAT
};

// Solveigs sang, by Grieg.
/* The year of 2007 is the 100 year anniversary for Edvard Grieg. 
   DB101 is released in the year of 2007, and includes a Grieg song. 
   A bit sad song, but very beautiful. */
char CAL_PGM_DEF(SONG_solveigsSangTitle[])      =   "Solveigs sang";

const uint16_t CAL_PGM_DEF(SONG_solveigsSangTune[]) = {
  SOUND_timeToTimingTicks(1.0),SOUND_D1,
  SOUND_timeToTimingTicks(1.0),SOUND_G1,
  SOUND_timeToTimingTicks(0.5),SOUND_A1,
  SOUND_timeToTimingTicks(0.5),SOUND_Ax1,
  SOUND_timeToTimingTicks(1.0),SOUND_C2,
  SOUND_timeToTimingTicks(0.5),SOUND_D2,
  SOUND_timeToTimingTicks(0.5),SOUND_Dx2,
  SOUND_timeToTimingTicks(0.04),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.5),SOUND_Dx2,
  SOUND_timeToTimingTicks(0.5),SOUND_D2,
  SOUND_timeToTimingTicks(0.04),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.5),SOUND_D2,
  SOUND_timeToTimingTicks(0.5),SOUND_Ax1,
  SOUND_timeToTimingTicks(1.0),SOUND_G1,
  SOUND_timeToTimingTicks(0.04),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.5),SOUND_G1,
  SOUND_timeToTimingTicks(0.5),SOUND_Ax1,
  SOUND_timeToTimingTicks(0.04),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.5),SOUND_Ax1,
  SOUND_timeToTimingTicks(0.5),SOUND_A1,
  SOUND_timeToTimingTicks(0.04),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.5),SOUND_A1,
  SOUND_timeToTimingTicks(0.5),SOUND_Fx1,
  SOUND_timeToTimingTicks(0.04),SOUND_PAUSE,   
  SOUND_timeToTimingTicks(0.125),SOUND_Fx1,
  SOUND_timeToTimingTicks(4.0),SOUND_D1,

  SOUND_timeToTimingTicks(1.0),SOUND_PAUSE,
  SOUND_timeToTimingTicks(1.0),SOUND_D1,
  SOUND_timeToTimingTicks(1.0),SOUND_G1,
  SOUND_timeToTimingTicks(0.5),SOUND_A1,
  SOUND_timeToTimingTicks(0.5),SOUND_Ax1,
  SOUND_timeToTimingTicks(1.0),SOUND_C2,
  SOUND_timeToTimingTicks(0.5),SOUND_D2,
  SOUND_timeToTimingTicks(0.5),SOUND_Dx2,
  SOUND_timeToTimingTicks(0.04),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.5),SOUND_Dx2,
  SOUND_timeToTimingTicks(0.5),SOUND_D2,
  SOUND_timeToTimingTicks(0.04),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.5),SOUND_D2,
  SOUND_timeToTimingTicks(0.5),SOUND_Ax1,
  SOUND_timeToTimingTicks(1.0),SOUND_G1,
  SOUND_timeToTimingTicks(0.04),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.5),SOUND_G1,
  SOUND_timeToTimingTicks(0.5),SOUND_Ax1,
  SOUND_timeToTimingTicks(0.04),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.5),SOUND_Ax1,
  SOUND_timeToTimingTicks(0.5),SOUND_A1,
  SOUND_timeToTimingTicks(0.04),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.5),SOUND_A1,
  SOUND_timeToTimingTicks(0.5),SOUND_G1,
  SOUND_timeToTimingTicks(4.0),SOUND_D2,

  SOUND_timeToTimingTicks(1.0),SOUND_PAUSE, 
  SOUND_REPEAT
};

// Standard sirene sound(note using standard tonescale).
char CAL_PGM_DEF(SONG_siren1Title[])      =   "Siren 1";

const uint16_t CAL_PGM_DEF(SONG_siren1Tune[]) = {
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1250),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1259),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1269),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1279),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1289),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1298),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1309),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1319),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1330),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1341),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1351),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1362),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1373),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1385),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1397),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1409),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1421),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1433),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1445),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1458),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1470),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1483),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1497),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1511),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1525),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1539),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1553),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1567),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1582),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1597),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1613),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1629),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1645),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1661),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1678),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1678),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1661),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1645),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1629),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1613),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1597),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1582),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1567),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1553),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1539),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1525),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1511),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1497),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1483),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1470),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1458),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1445),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1433),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1421),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1409),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1397),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1385),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1373),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1362),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1351),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1341),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1330),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1319),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1309),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1298),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1289),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1279),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1269),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1259),
  SOUND_timeToTimingTicks(0.063),SOUND_freqToRegValue(1250),
  SOUND_REPEAT
};

// Standard two tone sirene.
char CAL_PGM_DEF(SONG_siren2Title[])      =   "Siren 2";

const uint16_t CAL_PGM_DEF(SONG_siren2Tune[]) = {
  SOUND_timeToTimingTicks(0.5),SOUND_G2, SOUND_timeToTimingTicks(0.5),SOUND_C2,  
  SOUND_REPEAT
};


// Whistle!
char CAL_PGM_DEF(SONG_whistleTitle[])      =   "Whistle";

const uint16_t CAL_PGM_DEF(SONG_whistleTune[]) = 
{
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2499), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2574), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2631), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2707), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2778), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2858), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2944), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3029), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3124), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3228), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3333), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3448), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3572), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3705), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3872), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3998),
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(4165), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(4378), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(4545), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(4775), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(5002), 
  SOUND_timeToTimingTicks(0.016),SOUND_PAUSE, 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2499), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2564),
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2631), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2707), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2778), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2858), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2944), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3029), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3124), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3225),
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3333), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3448), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3572), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3705), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3872), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3998), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3998), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3848),
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3705), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3572), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3448), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3333), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3228), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3124), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(3029), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2942),
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2858), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2778), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2707), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2631), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2574), 
  SOUND_timeToTimingTicks(0.016),SOUND_freqToRegValue(2499),
  SOUND_STOP
};

// The full tonescale implemented in DB101.
char CAL_PGM_DEF(SONG_toneScaleTitle[])       = "Scale";

const uint16_t CAL_PGM_DEF(SONG_toneScaleTune[]) = {
  SOUND_timeToTimingTicks(0.063),SOUND_A,
  SOUND_timeToTimingTicks(0.063),SOUND_Ax,
  SOUND_timeToTimingTicks(0.063),SOUND_B,
  SOUND_timeToTimingTicks(0.063),SOUND_C0,
  SOUND_timeToTimingTicks(0.063),SOUND_Cx0,  
  SOUND_timeToTimingTicks(0.063),SOUND_D0,
  SOUND_timeToTimingTicks(0.063),SOUND_Dx0,
  SOUND_timeToTimingTicks(0.063),SOUND_E0,
  SOUND_timeToTimingTicks(0.063),SOUND_F0,
  SOUND_timeToTimingTicks(0.063),SOUND_Fx0,  
  SOUND_timeToTimingTicks(0.063),SOUND_G0,
  SOUND_timeToTimingTicks(0.063),SOUND_Gx0,
  SOUND_timeToTimingTicks(0.063),SOUND_A0,
  SOUND_timeToTimingTicks(0.063),SOUND_Ax0,
  SOUND_timeToTimingTicks(0.063),SOUND_B0,  
  SOUND_timeToTimingTicks(0.063),SOUND_C1,
  SOUND_timeToTimingTicks(0.063),SOUND_Cx1,
  SOUND_timeToTimingTicks(0.063),SOUND_D1,
  SOUND_timeToTimingTicks(0.063),SOUND_Dx1,
  SOUND_timeToTimingTicks(0.063),SOUND_E1,  
  SOUND_timeToTimingTicks(0.063),SOUND_F1,
  SOUND_timeToTimingTicks(0.063),SOUND_Fx1,
  SOUND_timeToTimingTicks(0.063),SOUND_G1,
  SOUND_timeToTimingTicks(0.063),SOUND_Gx1,
  SOUND_timeToTimingTicks(0.063),SOUND_A1,  
  SOUND_timeToTimingTicks(0.063),SOUND_Ax1,
  SOUND_timeToTimingTicks(0.063),SOUND_B1,
  SOUND_timeToTimingTicks(0.063),SOUND_C2,
  SOUND_timeToTimingTicks(0.063),SOUND_Cx2,
  SOUND_timeToTimingTicks(0.063),SOUND_D2,  
  SOUND_timeToTimingTicks(0.063),SOUND_Dx2,
  SOUND_timeToTimingTicks(0.063),SOUND_E2,
  SOUND_timeToTimingTicks(0.063),SOUND_F2,
  SOUND_timeToTimingTicks(0.063),SOUND_Fx2,
  SOUND_timeToTimingTicks(0.063),SOUND_G2,  
  SOUND_timeToTimingTicks(0.063),SOUND_Gx2,
  SOUND_timeToTimingTicks(0.063),SOUND_A2,
  SOUND_timeToTimingTicks(0.063),SOUND_Ax2,
  SOUND_timeToTimingTicks(0.063),SOUND_B2,
  SOUND_timeToTimingTicks(0.063),SOUND_C3,  
  SOUND_timeToTimingTicks(0.063),SOUND_Cx3,
  SOUND_timeToTimingTicks(0.063),SOUND_D3,
  SOUND_timeToTimingTicks(0.063),SOUND_Dx3,
  SOUND_timeToTimingTicks(0.063),SOUND_E3,
  SOUND_timeToTimingTicks(0.063),SOUND_F3,
  SOUND_timeToTimingTicks(0.063),SOUND_Fx3,
  SOUND_timeToTimingTicks(0.063),SOUND_G3,
  SOUND_timeToTimingTicks(0.063),SOUND_Gx3,
  SOUND_timeToTimingTicks(0.063),SOUND_A3,
  SOUND_timeToTimingTicks(0.063),SOUND_Ax3,
  SOUND_timeToTimingTicks(0.063),SOUND_B3,  
  SOUND_timeToTimingTicks(0.25) ,SOUND_C4, 
  SOUND_timeToTimingTicks(0.063),SOUND_B3,
  SOUND_timeToTimingTicks(0.063),SOUND_Ax3,
  SOUND_timeToTimingTicks(0.063),SOUND_A3,
  SOUND_timeToTimingTicks(0.063),SOUND_Gx3,
  SOUND_timeToTimingTicks(0.063),SOUND_G3,  
  SOUND_timeToTimingTicks(0.063),SOUND_Fx3,
  SOUND_timeToTimingTicks(0.063),SOUND_F3,
  SOUND_timeToTimingTicks(0.063),SOUND_E3,
  SOUND_timeToTimingTicks(0.063),SOUND_Dx3,
  SOUND_timeToTimingTicks(0.063),SOUND_D3,  
  SOUND_timeToTimingTicks(0.063),SOUND_Cx3,
  SOUND_timeToTimingTicks(0.063),SOUND_C3,
  SOUND_timeToTimingTicks(0.063),SOUND_B2,
  SOUND_timeToTimingTicks(0.063),SOUND_Ax2,
  SOUND_timeToTimingTicks(0.063),SOUND_A2,  
  SOUND_timeToTimingTicks(0.063),SOUND_Gx2,
  SOUND_timeToTimingTicks(0.063),SOUND_G2,
  SOUND_timeToTimingTicks(0.063),SOUND_Fx2,
  SOUND_timeToTimingTicks(0.063),SOUND_F2,
  SOUND_timeToTimingTicks(0.063),SOUND_E2,
  SOUND_timeToTimingTicks(0.063),SOUND_Dx2,
  SOUND_timeToTimingTicks(0.063),SOUND_D2,
  SOUND_timeToTimingTicks(0.063),SOUND_Cx2,
  SOUND_timeToTimingTicks(0.063),SOUND_C2,
  SOUND_timeToTimingTicks(0.063),SOUND_B1,
  SOUND_timeToTimingTicks(0.063),SOUND_Ax1,
  SOUND_timeToTimingTicks(0.063),SOUND_A1,
  SOUND_timeToTimingTicks(0.063),SOUND_Gx1,
  SOUND_timeToTimingTicks(0.063),SOUND_G1,
  SOUND_timeToTimingTicks(0.063),SOUND_Fx1,
  SOUND_timeToTimingTicks(0.063),SOUND_F1,
  SOUND_timeToTimingTicks(0.063),SOUND_E1,
  SOUND_timeToTimingTicks(0.063),SOUND_Dx1,
  SOUND_timeToTimingTicks(0.063),SOUND_D1,
  SOUND_timeToTimingTicks(0.063),SOUND_Cx1,
  SOUND_timeToTimingTicks(0.063),SOUND_C1,
  SOUND_timeToTimingTicks(0.063),SOUND_B0,
  SOUND_timeToTimingTicks(0.063),SOUND_Ax0,
  SOUND_timeToTimingTicks(0.063),SOUND_A0,
  SOUND_timeToTimingTicks(0.063),SOUND_Gx0,
  SOUND_timeToTimingTicks(0.063),SOUND_G0,
  SOUND_timeToTimingTicks(0.063),SOUND_Fx0,
  SOUND_timeToTimingTicks(0.063),SOUND_F0,
  SOUND_timeToTimingTicks(0.063),SOUND_E0,
  SOUND_timeToTimingTicks(0.063),SOUND_Dx0,
  SOUND_timeToTimingTicks(0.063),SOUND_D0,
  SOUND_timeToTimingTicks(0.063),SOUND_Cx0,
  SOUND_timeToTimingTicks(0.063),SOUND_C0,
  SOUND_timeToTimingTicks(0.063),SOUND_B,
  SOUND_timeToTimingTicks(0.063),SOUND_Ax,  
  SOUND_timeToTimingTicks(0.063),SOUND_A,  
  SOUND_STOP
};

// Small good beep sound.
char CAL_PGM_DEF(SONG_positiveBeepTitle[]) = "Beep 1";

const uint16_t CAL_PGM_DEF(SONG_positiveBeepTune[]) = { 
  SOUND_timeToTimingTicks(0.03),SOUND_E2,
  SOUND_timeToTimingTicks(0.02),SOUND_PAUSE, 
  SOUND_STOP
};

// Small negative beep sound.
char CAL_PGM_DEF(SONG_negativeBeepTitle[]) = "Beep 2";

const uint16_t CAL_PGM_DEF(SONG_negativeBeepTune[]) = { 
  SOUND_timeToTimingTicks(0.25),SOUND_Ax,
  SOUND_timeToTimingTicks(0.245),SOUND_PAUSE, 
  SOUND_STOP
};

// Small mallfunction beep sound.
char CAL_PGM_DEF(SONG_disasterBeepTitle[]) = "Malfunction";

const uint16_t CAL_PGM_DEF(SONG_disasterBeepTune[]) = {
  SOUND_timeToTimingTicks(0.016),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_Fx0,
  SOUND_timeToTimingTicks(0.016),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_Fx0,
  SOUND_timeToTimingTicks(0.016),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_Fx0,
  SOUND_timeToTimingTicks(0.016),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_Fx0, 
  SOUND_timeToTimingTicks(0.016),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_Fx0,
  SOUND_timeToTimingTicks(0.016),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_Fx0,
  SOUND_timeToTimingTicks(0.016),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_Fx0,
  SOUND_timeToTimingTicks(0.016),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_Fx0, 
  SOUND_timeToTimingTicks(0.016),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_Fx0,
  SOUND_timeToTimingTicks(0.016),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_Fx0,
  SOUND_timeToTimingTicks(0.016),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_Fx0,
  SOUND_timeToTimingTicks(0.016),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_Fx0, 
  SOUND_timeToTimingTicks(0.016),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_Fx0,
  SOUND_timeToTimingTicks(0.016),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_Fx0,
  SOUND_timeToTimingTicks(0.016),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_Fx0,
  SOUND_timeToTimingTicks(0.016),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_Fx0, 
  SOUND_STOP };

// Small positive tune.
char CAL_PGM_DEF(SONG_positiveActionTitle[]) = "Success";

const uint16_t CAL_PGM_DEF(SONG_positiveActionTune[]) = { 
  SOUND_timeToTimingTicks(0.094),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_PAUSE,
  SOUND_timeToTimingTicks(0.094),SOUND_E1,
  SOUND_timeToTimingTicks(0.016),SOUND_PAUSE,
  SOUND_timeToTimingTicks(0.094),SOUND_G1, 
  SOUND_timeToTimingTicks(0.016),SOUND_PAUSE,
  SOUND_timeToTimingTicks(0.25),SOUND_C2,
  SOUND_timeToTimingTicks(0.125),SOUND_PAUSE, 
  SOUND_STOP
}; 

/*
const uint16_t CAL_PGM_DEF(SONG_positiveActionTune[]) = { 
  SOUND_timeToTimingTicks(0.094),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_PAUSE,
  SOUND_timeToTimingTicks(0.094),SOUND_E1,
  SOUND_timeToTimingTicks(0.016),SOUND_PAUSE,
  SOUND_timeToTimingTicks(0.094),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_PAUSE,
  SOUND_timeToTimingTicks(0.094),SOUND_E1,
  SOUND_timeToTimingTicks(0.016),SOUND_PAUSE,
  SOUND_timeToTimingTicks(0.094),SOUND_C0,
  SOUND_timeToTimingTicks(0.016),SOUND_PAUSE,
  SOUND_timeToTimingTicks(0.194),SOUND_E1,
  SOUND_STOP
};
*/
