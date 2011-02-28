/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 sample pictures source file.
 *
 *       This file holds some picture data for demonstration purpose in the
 *       DB101 software.
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
 * $Revision: 2172 $
 * $Date: 2007-07-11 09:34:22 -0600 (on, 11 jul 2007) $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/production_demo_rev_A/flashpics.h $
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
#ifndef FLASHPICS_H
#define FLASHPICS_H

#include <lcd_lib.h>
#include <stdint.h>


#define FLASHPICS_HUGE_WIDTH 460
#define FLASHPICS_HUGE_HEIGHT 288

extern uint8_t const CAL_PGM(FLASHPICS_eCarsLogo[]);
extern uint8_t const CAL_PGM(FLASHPICS_excellenceThroughTechnology[]);
extern uint8_t const CAL_PGM(FLASHPICS_chip[]);
extern uint8_t const CAL_PGM(FLASHPICS_avrLogo[]);
extern uint8_t const CAL_PGM(FLASHPICS_cat[]);
extern uint8_t const CAL_PGM(FLASHPICS_face1[]);
extern uint8_t const CAL_PGM(FLASHPICS_face2[]);
extern uint8_t const CAL_PGM(FLASHPICS_db101pcb[FLASHPICS_HUGE_WIDTH * (FLASHPICS_HUGE_HEIGHT / LCD_PAGE_HEIGHT)]);

extern uint8_t const CAL_PGM(FLASHPICS_ECARS_DEMO_0[]);
extern uint8_t const CAL_PGM(FLASHPICS_ECARS_DEMO_1[]);
extern uint8_t const CAL_PGM(FLASHPICS_ECARS_DEMO_2[]);
extern uint8_t const CAL_PGM(FLASHPICS_ECARS_DEMO_3[]);
extern uint8_t const CAL_PGM(FLASHPICS_ECARS_DEMO_4[]);
extern uint8_t const CAL_PGM(FLASHPICS_ECARS_DEMO_5[]);
extern uint8_t const CAL_PGM(FLASHPICS_ECARS_DEMO_6[]);
extern uint8_t const CAL_PGM(FLASHPICS_ECARS_DEMO_7[]);
extern uint8_t const CAL_PGM(FLASHPICS_ECARS_DEMO_8[]);
extern uint8_t const CAL_PGM(FLASHPICS_ECARS_DEMO_9[]);

extern uint8_t const CAL_PGM(FLASHPICS_battery_0[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_1[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_2[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_3[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_4[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_5[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_6[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_7[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_8[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_9[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_10[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_11[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_12[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_13[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_14[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_15[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_16[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_17[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_18[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_19[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_20[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_21[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_22[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_23[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_24[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_25[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_26[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_27[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_28[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_29[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_30[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_31[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_32[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_33[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_34[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_35[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_36[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_37[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_38[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_39[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_40[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_41[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_42[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_43[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_44[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_45[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_46[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_47[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_48[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_49[]);
extern uint8_t const CAL_PGM(FLASHPICS_battery_50[]);

#endif
