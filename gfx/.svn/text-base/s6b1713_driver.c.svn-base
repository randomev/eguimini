// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 LCD controller driver source file.
 *
 *      The LCD controller driver provides an interface to the S6B1713
 *      controller chip on the LCD module. The driver takes care of initializing
 *      the communication interface to the controller and provides interface
 *      functions for all commands that the S6B1713 chip provides. Even though
 *      the S6B1713 chip supports both serial and parallel interfaces, the
 *      default implementation on DB101 is to use the memory-mapped interface,
 *      where the Command Register and Data Register of the S6B1713 chip are
 *      mapped into SRAM space of the AVR. This is the interface with the
 *      highest communication speed.
 *
 *      Note that this driver does not provide any graphics functionality at
 *      all. It only provides a one-to-one interface to all S6B1713 commands.
 *      The LCD Graphics Library uses this driver to implement higher-level
 *      graphics functionality, such as pixel control, lines, and circles.
 *
 *      64128G LCD module datasheet:
 *      http://www.displaytech.com.hk/pdf/graphic/64128g%20series-v13.pdf [Accessed 2007-04-20]
 *
 *      S6B1713 LCD controller datasheet:
 *      http://www.crystalfontz.com/products/DS_S6B1713_R42.pdf [Accessed 2007-04-20]
 *
 *      The DB101 hardware supports three means of communicating with the LCD
 *      module: (1) Bit-banged serial, (2) bit-banged parallel, and (3) memory-
 *      mapped access.
 *
 *      With big-banged serial access, the LCD module data lines DB6 and DB7 serve
 *      as SerialData and SerialClock, respectively. Serial communication only
 *      supports write operations.
 *
 *      With bit-banged parallel access, the LCD module is accessed through the full
 *      8-bit microprocessor interface (8080-type by default), using eight data lines,
 *      read and write strobes and Chip Select line. Parallel access supports both
 *      reading and writing data.
 *
 *      With memory-mapped access, we utilize the built-in external memory interface
 *      of the AVR, which interfaces directly with the 8080-type microprocessor
 *      interface of the LCD module. From a software point-of-view the LCD Control
 *      and Data registers are mapped into AVR data memory space and can be accessed
 *      directly as any other data memory location.
 *
 *      There are five function that depend on the type of interface used, namely
 *      S6B1713_Init, S6B1713_WriteData, S6B1713_WriteCommand, S6B1713_ReadData(),
 *      and S6B1713_ReadStatus(). These are implemented as macros that maps the name
 *      to the corresponding function for the specific interface, such as S6B1713M_Init
 *      for the memory-mapped init function, S6B1713S_Init for the serial version and so on.
 *
 *      One of the following three symbols must be defined to select what interface
 *      to build the library for: MEM_MAPPED_LCD, PARALLEL_LCD, or SERIAL_LCD.
 *
 *      The rest of the driver functions use these access functions and are thus not
 *      dependent on interface type.
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
 * $Revision: 2414 $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/gfx/s6b1713_driver.c $
 * $Date: 2007-09-17 01:58:12 -0600 (ma, 17 sep 2007) $  \n
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

#include "s6b1713_driver.h"


/*******************
 * Private defines.
 *******************/

/*
 * LCD controller command codes. Refer to datasheet.
 */

#define S6B1713_CMD_DISPLAY_ON                 0xAF
#define S6B1713_CMD_DISPLAY_OFF                0xAE
#define S6B1713_CMD_INITIAL_DISPLAY_LINE       0x40
#define S6B1713_CMD_REFERENCE_VOLTAGE_MODE     0x81
#define S6B1713_CMD_ADC_NORMAL_DIRECTION       0xA0
#define S6B1713_CMD_ADC_REVERSE_DIRECTION      0xA1
#define S6B1713_CMD_REVERSE_DISPLAY_ON_OFF     0xA6
#define S6B1713_CMD_REVERSE_DISPLAY_ON         0xA7
#define S6B1713_CMD_ENTIRE_DISPLAY_OFF         0xA4
#define S6B1713_CMD_ENTIRE_DISPLAY_ON          0xA5
#define S6B1713_CMD_LCD_BIAS_0                 0xA2
#define S6B1713_CMD_LCD_BIAS_1                 0xA3
#define S6B1713_CMD_RESET                      0xE2
#define S6B1713_CMD_SHL_NORMAL_DIRECTION       0xC0
#define S6B1713_CMD_SHL_REVERSE_DIRECTION      0xC8
#define S6B1713_CMD_POWER_CONTROL              0x28
#define S6B1713_CMD_INTERNAL_RESISTOR_RATIO    0x20
#define S6B1713_CMD_STATIC_INDICATOR_MODE      0xAC



/*
 * LCD control pin and port definitions for the DB101 board.
 */

#define LCD_RESET_PORT_REG PORTC  //!< Port output register where RESET line is connected.
#define LCD_RESET_DDR_REG  DDRC   //!< Port direction register where RESET line is connected.

#if defined(PCB_REV_C)
  #define LCD_RESET_PIN      PC2    //!< Pin number for RESET line.
#elif defined(PCB_REV_E)
  #define LCD_RESET_PIN      PC1    //!< Pin number for RESET line.
#endif



/****************************
 * Function implementations.
 ****************************/


void S6B1713M_Init( void )
{
	// Hard reset the LCD controller chip before doing anything else.
	S6B1713_HardReset();
	
#if defined(PCB_REV_C)
	// Enable external memory interface, no wait states, bus keeper on, 10-bit addresses.
	XMCRB = (1 << XMBK) | (1 << XMM2) | (1 << XMM1) | (0 << XMM0);
	XMCRA = (1 << SRE) | (0 << SRW11) | (0 << SRW10);
#elif defined(PCB_REV_E)
	// Enable external memory interface, no wait states, bus keeper on, 8-bit addresses.
	XMCRB = (1 << XMBK) | (1 << XMM2) | (1 << XMM1) | (1 << XMM0);
	XMCRA = (1 << SRE) | (0 << SRW11) | (0 << SRW10);
	DDRC |= (1 << PC0); // A0 line to LCD.
#endif
}


void S6B1713_HardReset( void )
{
	// Pulse RESET pin low for 100 milliseconds.
	LCD_RESET_PORT_REG &= ~(1 << LCD_RESET_PIN);
	LCD_RESET_DDR_REG |= (1 << LCD_RESET_PIN);
	DELAY_MS( 100 );
	LCD_RESET_PORT_REG |= (1 << LCD_RESET_PIN);
}


void S6B1713_SetDisplayOn( void )
{
	S6B1713_WriteCommand( S6B1713_CMD_DISPLAY_ON );
}


void S6B1713_SetDisplayOff( void )
{
	S6B1713_WriteCommand( S6B1713_CMD_DISPLAY_OFF );
}

/*
 * \param  startLine  Set this line as the first LCD line
 */
void S6B1713_SetInitialDisplayLine( uint8_t startLine )
{
	// Truncate line to 6 bits (0-63) and issue command.
	startLine &= 0x3f;
	S6B1713_WriteCommand( S6B1713_CMD_INITIAL_DISPLAY_LINE | startLine );
}

/*
 * \param  value  Value for reference voltage. Refer to datasheet
 */
void S6B1713_SetReferenceVoltage( uint8_t value )
{
	// Truncate value to 6 bits (0-63) and issue command.
	value &= 0x3f;
	S6B1713_WriteCommand( S6B1713_CMD_REFERENCE_VOLTAGE_MODE );
	S6B1713_WriteCommand( value );
}


void S6B1713_SetADCNormalDirection( void )
{
	S6B1713_WriteCommand( S6B1713_CMD_ADC_NORMAL_DIRECTION );
}


void S6B1713_SetADCReverseDirection( void )
{
	S6B1713_WriteCommand( S6B1713_CMD_ADC_REVERSE_DIRECTION );
}


void S6B1713_SetReverseDisplayOff( void )
{
	S6B1713_WriteCommand( S6B1713_CMD_REVERSE_DISPLAY_ON_OFF );
}


void S6B1713_SetReverseDisplayOn( void )
{
	S6B1713_WriteCommand( S6B1713_CMD_REVERSE_DISPLAY_ON );
}


void S6B1713_SetEntireDisplayOff( void )
{
	S6B1713_WriteCommand( S6B1713_CMD_ENTIRE_DISPLAY_OFF );
}


void S6B1713_SetEntireDisplayOn( void )
{
	S6B1713_WriteCommand( S6B1713_CMD_ENTIRE_DISPLAY_ON );
}


void S6B1713_SetLCDBias0( void )
{
	S6B1713_WriteCommand( S6B1713_CMD_LCD_BIAS_0 );
}


void S6B1713_SetLCDBias1( void )
{
	S6B1713_WriteCommand( S6B1713_CMD_LCD_BIAS_1 );
}


void S6B1713_Reset( void )
{
	S6B1713_WriteCommand( S6B1713_CMD_RESET );
}


void S6B1713_SetSHLNormalDirection( void )
{
	S6B1713_WriteCommand( S6B1713_CMD_SHL_NORMAL_DIRECTION );
}


void S6B1713_SetSHLReverseDirection( void )
{
	S6B1713_WriteCommand( S6B1713_CMD_SHL_REVERSE_DIRECTION );
}

/*
 * \param  bitmask  Set internal voltage regulator. You should use already defined values
 */
void S6B1713_SetPowerControl( uint8_t bitmask )
{
	// Clear all but relevant bits and issue command.
	bitmask &= (S6B1713_VOLTAGE_CONVERTER | S6B1713_VOLTAGE_REGULATOR | S6B1713_VOLTAGE_FOLLOWER);
	S6B1713_WriteCommand( S6B1713_CMD_POWER_CONTROL | bitmask );
}

/*
 * \param  ratio  Set internal resistor ratio to this enum.
 */
void S6B1713_SetInternalResistorRatio( S6B1713_INTERNAL_RESISTOR_RATIO_t ratio )
{
	// Since we are using an enum, the ratio value should always be valid, but we AND with 0x07 to be safe.
	S6B1713_WriteCommand( S6B1713_CMD_INTERNAL_RESISTOR_RATIO | (ratio & 0x07) );
}


/*
 * \param  mode  Enum specifying how the static indicator shall behave.
 */
void S6B1713_SetStaticIndicatorMode( S6B1713_STATIC_INDICATOR_MODE_t mode )
{
	// Since we are using an enum, the mode value should always be valid, but we AND with 0x03 to be safe.
	S6B1713_WriteCommand( S6B1713_CMD_STATIC_INDICATOR_MODE );
	S6B1713_WriteCommand( mode & 0x03 );
}


void S6B1713_PowerSave( void )
{
	S6B1713_SetDisplayOff();
	S6B1713_SetEntireDisplayOn();
}


void S6B1713_PowerSaveOff( void )
{
	S6B1713_SetEntireDisplayOff();
	S6B1713_SetDisplayOn();
}
