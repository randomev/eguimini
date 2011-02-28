/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 Backlight driver header file
 *
 *      The backlight driver provides an interface to control the RGB backlight
 *      for the LCD module on DB101. The driver hides the control of the PWM
 *      outputs connected to the backlight LEDs and provides functions to set
 *      individual red, green, and blue levels in addition to a common intensity
 *      value that controls the overall intensity of the backlight. Setting the
 *      intensity to zero turn off the backlight regardless of the settings for
 *      the individual colors.
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
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/backlight_driver/backlight_driver.h $
 * $Date: 2007-09-17 14:23:04 -0600 (ma, 17 sep 2007) $  \n
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

#ifndef BACKLIGHT_DRIVER_H
#define BACKLIGHT_DRIVER_H

#include <stdint.h>


/**********************
 * Function prototypes
 **********************/

void BACKLIGHT_Init(void);  //!< Activate the pins controlling the RGB LED's, and initialize PWM on timer.
void BACKLIGHT_Off(void);  //!< Switch backlight off, and clear all intensity values.
void BACKLIGHT_Pause(void);  //!< Swith backlight off, and saves the RGB intensity values.
void BACKLIGHT_Resume(void);  //!< Switch backlight on, with previous used RGB values.
void BACKLIGHT_SetRed(uint8_t red);  //!< Set the intensity level for red LED.
void BACKLIGHT_SetGreen(uint8_t green);  //!< Set the intensity level for green LED.
void BACKLIGHT_SetBlue(uint8_t blue);  //!< Set the intensity level for blue LED.
void BACKLIGHT_SetRGB(uint8_t red, uint8_t green , uint8_t blue);  //!< Set the intensity level for all RGB LED's.
void BACKLIGHT_SetIntensity(uint8_t intensity);  //!< Dim all RGB LED's with value given to the function.



#endif
