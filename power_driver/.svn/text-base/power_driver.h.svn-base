// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 power management driver header file
 *
 *      The power management driver provides an interface to the power
 *      management and sleep system of the AVR microcontroller. The driver hides
 *      the details of preparing for and entering sleep modes.
 *
 *      When the deeper sleep modes are used, for instance Power Save sleep
 *      mode, some interrupts are still in operation. However, the user might
 *      not want to wake up and resume the main application every time the
 *      real-time clock ticks. The clock driver should process the tick and then
 *      enter Power Save sleep mode again. In order to allow the user to resume
 *      the main application on selected conditions only, the driver provides a
 *      callback function, named POWER_WakeupCallback, which tells the power
 *      management driver to resume the main application instead of re-entering
 *      sleep mode. The DB101 firmware uses this feature to resume the main
 *      application upon joystick movement and not after every clock tick.
 *
 *      Note that the driver initializes the AVR microcontroller with all
 *      peripherals in Power Reduction Mode. This means that the user must
 *      manually disable Power Reduction Mode for individual peripherals when
 *      required. Typically, the driver for the peripheral should take care of
 *      this for you.
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
 * $Revision: 2404 $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/power_driver/power_driver.h $
 * $Date: 2007-09-17 00:44:48 -0600 (ma, 17 sep 2007) $  \n
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
#ifndef POWER_DRIVER_H
#define POWER_DRIVER_H

/***********************
 * Function prototypes.
 ***********************/

void POWER_WakeupCallback( void ); //!< Request a full wakeup
void POWER_EnterIdleSleepMode( void ); //!< Enter idle mode. Whole application will wake up on interrupts
void POWER_EnterPowerSaveMode( void ); //!< Enter PowerSave mode
void POWER_Init( void ); //!< Initialise the power-driver


#endif
