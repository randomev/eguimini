/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 Backlight driver source file
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
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/backlight_driver/backlight_driver.c $
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

#include <stdint.h>
#include <stdbool.h>
#include <cal.h>
#include <common.h>

#include "backlight_driver.h"

/********************************
 * Private constants and defines
 ********************************/

/*
 * Constants used to convert ADC readings to voltages when checking
 * whether external power is connected or not. No backlight possible
 * when running on batteries.
 */
//! Minimum external voltage required to support backlight.
#define BACKLIGHT_VOLTAGE_THRESHOLD 0.5
//! ADC internal reference voltage.
#define BACKLIGHT_ADC_REF 2.56
//! External voltage divider factor.
#define BACKLIGHT_ADC_DIVIDE 4
//! Maximum ADC reading, corresponds to reference voltage.
#define BACKLIGHT_ADC_MAX 1024
//! Calculation macro converting threshold voltage to ADC value.
#define BACKLIGHT_ADC_THRESHOLD (BACKLIGHT_ADC_MAX * BACKLIGHT_VOLTAGE_THRESHOLD / BACKLIGHT_ADC_DIVIDE / BACKLIGHT_ADC_REF)


#define OCR_RED OCR3A     //!< Redefinition of Output Compare Register 3 A.
#define OCR_GREEN OCR3B   //!< Redefinition of Output Compare Register 3 B.
#define OCR_BLUE OCR3C    //!< Redefinition of Output Compare Register 3 C.
/* Note that OCR registers are 16 bits. The backlight driver uses only 8 bit,
   thus only lower byte will be affected. */

// SRAM variables to preserve RGB settings outside BACKLIGHT module
uint8_t BACKLIGHT_red = 0, BACKLIGHT_green = 0, BACKLIGHT_blue = 0, 
        BACKLIGHT_intensity = 0xFF;


/***************************
 * Function implementations
 ***************************/

/*!
 * Check if we have external power or is running on batteries
 * 
 * \retval  true if we have external power, false otherwise
 */
static bool BACKLIGHT_HasExternalPower( void )
{
	bool hasExternalPower;
//	uint16_t v_regulator1;
	uint16_t v_regulator0;
//	uint16_t v_external;

	PRR0 &= ~(1 << PRADC); // Turn on ADC.
	ADMUX = (1 << REFS1) | (1 << REFS0); // Internal 2.56V ref.
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable with 57kHz @ 7.38MHz.
	// One dummy conversion.
	ADCSRA |= (1 << ADSC);
	do {} while ((ADCSRA & (1 << ADSC)) != 0x00);
/*
	ADMUX = (1 << REFS1) | (1 << REFS0) | 0x00; // ADC0 = v_regulator1
	DELAY_MS( 10 );
	ADCSRA |= (1 << ADSC);
	do {} while ((ADCSRA & (1 << ADSC)) != 0x00);
	v_regulator1 = ADC;
*/	
	ADMUX = (1 << REFS1) | (1 << REFS0) | 0x01; // ADC1 = v_regulator0
	DELAY_MS( 10 );
	ADCSRA |= (1 << ADSC);
	do {} while ((ADCSRA & (1 << ADSC)) != 0x00);
	v_regulator0 = ADC;
/*	
	ADMUX = (1 << REFS1) | (1 << REFS0) | 0x02; // ADC2 = v_external
	DELAY_MS( 10 );
	ADCSRA |= (1 << ADSC);
	do {} while ((ADCSRA & (1 << ADSC)) != 0x00);
	v_external = ADC;
*/		
	// Check for external power present.
	if (v_regulator0 > BACKLIGHT_ADC_THRESHOLD) {
		hasExternalPower = true;
	} else {
		hasExternalPower = false;
	}
	ADCSRA &= ~(1 << ADEN);
	PRR0 |= (1 << PRADC); // Turn off ADC.
	
	return hasExternalPower;
}

/*!
 * Initialize the Backlight if we have external power (ie, not running on batteries)
 * by setting up the timers for the PWM
 */
void BACKLIGHT_Init( void )
{
	//if (BACKLIGHT_HasExternalPower()) {
	if (true) {
		PRR1 &= ~(1 << PRTIM3); // Turn on timer 3.
	
		// Set PWM pins for RGB backlight to output.
		DDRE |= (1 << PE3) | (1 << PE4) | (1 << PE5);
		// Start with black, in power up initialization.
		OCR_RED = BACKLIGHT_red;
		OCR_GREEN = BACKLIGHT_green;
		OCR_BLUE = BACKLIGHT_blue;
		// Clear OC3x on upcounting, Set OC3x on downcounting
		// PWM, Phase correct, 8-bit. Mode 1 in datasheet
		TCCR3A = (1 << COM3A1) | (0 << COM3A0) | 
				 (1 << COM3B1) | (0 << COM3B0) | 
				 (1 << COM3C1) | (0 << COM3C0) | 
				 (0 << WGM31) | (1 << WGM30);
		TCCR3B = (0 << WGM33) | (0 << WGM32) | 
				 (0 << CS32) | (0 << CS31) | 
				 (1 << CS30);
	}
}

/*!
 * Turns of the backlight by turning of timer3.
 * Also clears all RGB intensity values.
 */
void BACKLIGHT_Off(void)
{
    BACKLIGHT_red = 0;
    BACKLIGHT_green = 0;
    BACKLIGHT_blue = 0;
    OCR_RED = 0;
    OCR_GREEN = 0;
    OCR_BLUE = 0;
    PORTE &= ~((1 << PE3) | (1 << PE4) | (1 << PE5));
	PRR1 |= (1 << PRTIM3); // Turn off timer 3.
}

/*!
 * Turns the backlight off by setting the RGB intensity values to zero,
 * but saves the original values so we can start it again with BACKLIGHT_Resume(void)
 */
void BACKLIGHT_Pause(void)
{
    BACKLIGHT_red = OCR_RED;
    BACKLIGHT_green = OCR_GREEN;
    BACKLIGHT_blue = OCR_BLUE;
    OCR_RED = 0;
    OCR_GREEN = 0;
    OCR_BLUE = 0;
}

void BACKLIGHT_Resume(void)
{
    BACKLIGHT_SetIntensity( BACKLIGHT_intensity );
}

/*!
 * \param  red  Amount of red color in backlight.
 */
void BACKLIGHT_SetRed(uint8_t red)
{
    uint8_t max = 0xFF;
    uint16_t mathVar = 0x0000;

    BACKLIGHT_red = red;

    // Adjust original values with intensity if intensity is set.
    if (BACKLIGHT_intensity != 0xFF){
      mathVar = red * BACKLIGHT_intensity;
      red = mathVar / max;
    }
    OCR_RED = red;
}

/*!
 * \param  green  Amount of green color in backlight.
 */
void BACKLIGHT_SetGreen(uint8_t green)
{
    uint8_t max = 0xFF;
    uint16_t mathVar = 0x0000;

    BACKLIGHT_green = green;

    // Adjust original values with intensity if intensity if set.
    if (BACKLIGHT_intensity != 0xFF){
      mathVar = green * BACKLIGHT_intensity;
      green = mathVar / max;
    }
    OCR_GREEN = green;
}

/*!
 * \param  blue  Amount of blue color in backlight.
 */
void BACKLIGHT_SetBlue(uint8_t blue)
{
    uint8_t max = 0xFF;
    uint16_t mathVar = 0x0000;

    BACKLIGHT_blue = blue;

    // Adjust original values with intensity if intensity set .
    if (BACKLIGHT_intensity != 0xFF){
      mathVar = blue * BACKLIGHT_intensity;
      blue = mathVar / max;
    }
    OCR_BLUE = blue;
}

/*!
 * \param  red  Amount of red color in backlight.
 * \param  green  Amount of green color in backlight.
 * \param  blue  Amount of blue color in backlight.
 */
void BACKLIGHT_SetRGB(uint8_t red, uint8_t green , uint8_t blue)
{
    uint8_t max = 0xFF;
    uint16_t mathVar = 0x0000;

    BACKLIGHT_red = red;
    BACKLIGHT_green = green;
    BACKLIGHT_blue = blue;

    // Adjust original values with intensity if intensity is set.
    if (BACKLIGHT_intensity != 0xFF){
      mathVar = red * BACKLIGHT_intensity;
      red = mathVar / max;
      mathVar = green * BACKLIGHT_intensity;
      green = mathVar / max;
      mathVar = blue * BACKLIGHT_intensity;
      blue = mathVar / max;
    }
    OCR_RED = red;
    OCR_GREEN = green;
    OCR_BLUE = blue;
}

/*!
 * Change the intensity of the backlight without changing the color.
 */
void BACKLIGHT_SetIntensity(uint8_t intensity)
{
    uint8_t red, green, blue;
    uint8_t max = 0xFF;
    uint16_t mathVar = 0x0000;
   // Fetch original RGB Levels
    red = BACKLIGHT_red;
    green = BACKLIGHT_green;
    blue = BACKLIGHT_blue;
    // Adjust original values with intensity 
    mathVar = red * intensity;
    red = mathVar / max;
    mathVar = green * intensity;
    green = mathVar / max;
    mathVar = blue * intensity;
    blue = mathVar / max;
    // Set new updated Levels
    BACKLIGHT_intensity = intensity;
    OCR_RED = red;
    OCR_GREEN = green;
    OCR_BLUE = blue;
}
