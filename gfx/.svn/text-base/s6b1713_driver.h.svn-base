// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 LCD controller driver header file
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
 * $Revision: 2408 $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/gfx/s6b1713_driver.h $
 * $Date: 2007-09-17 01:14:14 -0600 (ma, 17 sep 2007) $  \n
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

#ifndef S6B1713_DRIVER_H
#define S6B1713_DRIVER_H

#include <stdint.h>
#include <cal.h>


/*********************
 * Defines and enums.
 *********************/

#define PCB_REV_E
//#define PCB_REV_C

/*
 * LCD interface configuration selection.
 */

#define MEM_MAPPED_LCD  //!< Uncomment to compile for memory-mapped interface.
//#define PARALLEL_LCD  //!< Uncomment to compile for bit-banged parallel interface.
//#define SERIAL_LCD    //!< Uncomment to compile for bit-banged serial interace.

/*
 * LCD status byte bit masks. Use with return values from S6B1713_ReadStatus.
 */

#define S6B1713_STATUS_BUSY                    0x80  //!< Mask out BUSY bit from status byte. All instructions ignored while BUSY=1.
#define S6B1713_STATUS_ADC                     0x40  //!< Mask out ADC bit from status byte. ADC=0 means reverse segment line (column) order.
#define S6B1713_STATUS_ONOFF                   0x20  //!< Mask out ONOFF bit from status byte. ONOFF=1 means display is off.
#define S6B1713_STATUS_RESET                   0x10  //!< Mask out RESET bit from status byte. RESET=1 means display is being reset.


/*
 * LCD power control bit masks. Used by S6B1713_SetPowerControl.
 */

#define S6B1713_VOLTAGE_CONVERTER              0x04  //!< Set this bit to enabled internal voltage converter.
#define S6B1713_VOLTAGE_REGULATOR              0x02  //!< Set this bit to enabled internal voltage regulator (needs converter).
#define S6B1713_VOLTAGE_FOLLOWER               0x01  //!< Set this bit to enabled internal voltage follower (needs regulator).



/*
 * LCD control value enumerators.
 */

/*! \brief  Available values for S6B1713_SetInternalResistorRatio().
 *
 *  Use one of these enumerated values to decide the ratio "1+(Rb/Ra)"
 *  between the internal resistors Ra and Rb used by the LCD internal
 *  voltage regulator. The value S6B1713_INTERNAL_RESISTOR_RATIO_1p90
 *  would select the ratio "1+(Rb/Ra)" to be 1.90 and so on. This ratio
 *  is typically st during initialization and never changed afterwards.
 *  Please refer to the S6B1713 datasheet for more details.
 */
typedef enum S6B1713_INTERNAL_RESISTOR_RATIO_enum
{
	S6B1713_INTERNAL_RESISTOR_RATIO_1p90 = 0x00,
	S6B1713_INTERNAL_RESISTOR_RATIO_2p19 = 0x01,
	S6B1713_INTERNAL_RESISTOR_RATIO_2p55 = 0x02,
	S6B1713_INTERNAL_RESISTOR_RATIO_3p02 = 0x03,
	S6B1713_INTERNAL_RESISTOR_RATIO_3p61 = 0x04,
	S6B1713_INTERNAL_RESISTOR_RATIO_4p35 = 0x05,
	S6B1713_INTERNAL_RESISTOR_RATIO_5p29 = 0x06,
	S6B1713_INTERNAL_RESISTOR_RATIO_6p48 = 0x07
} S6B1713_INTERNAL_RESISTOR_RATIO_t;


/*! \brief  Available values for S6B1713_SetStaticIndicatorMode().
 *
 *  Use one of these enumerated values to select the blink mode
 *  for the static indicator output. This functionality is not available
 *  on the 64128G module, but included in this driver for completeness.
 *  If the function is not used, it will not get linked into the final
 *  application code anyway.
 *  Please refer to the S6B1713 datasheet for more details.
 */
typedef enum S6B1713_STATIC_INDICATOR_MODE_enum
{
	S6B1713_STATIC_INDICATOR_MODE_OFF            = 0x00,
	S6B1713_STATIC_INDICATOR_MODE_ONE_SEC_BLINK  = 0x01,
	S6B1713_STATIC_INDICATOR_MODE_HALF_SEC_BLINK = 0x02,
	S6B1713_STATIC_INDICATOR_MODE_ON             = 0x03
} S6B1713_STATIC_INDICATOR_MODE_t;



/*
 * Macros that map generic LCD interface functions to
 * parallel, serial og memory-mapped functions.
 */

#ifdef MEM_MAPPED_LCD
  #define S6B1713_Init()               S6B1713M_Init()
  #define S6B1713_WriteData(_value)    S6B1713M_WriteData(_value)
  #define S6B1713_WriteCommand(_value) S6B1713M_WriteCommand(_value)
  #define S6B1713_ReadData()           S6B1713M_ReadData()
  #define S6B1713_ReadStatus()         S6B1713M_ReadStatus()
#endif

#ifdef PARALLEL_LCD
  #error "Parallel LCD interface not implemented yet."
  #define S6B1713_Init()               S6B1713P_Init()
  #define S6B1713_WriteData(_value)    S6B1713P_WriteData(_value)
  #define S6B1713_WriteCommand(_value) S6B1713P_WriteCommand(_value)
  #define S6B1713_ReadData()           S6B1713P_ReadData()
  #define S6B1713_ReadStatus()         S6B1713P_ReadStatus()
#endif

#ifdef SERIAL_LCD
  #error "Serial LCD interface not implemented yet."
  #define S6B1713_Init()               S6B1713S_Init()
  #define S6B1713_WriteData(_value)    S6B1713S_WriteData(_value)
  #define S6B1713_WriteCommand(_value) S6B1713S_WriteCommand(_value)
  // Serial LCD interface does not allow for data readback, therefore we return 0x00 always.
  #define S6B1713_ReadData()           (0x00)
  #define S6B1713_ReadStatus()         (0x00)
#endif


/*
 * Macros for memory-mapped LCD module access.
 *
 * These functions are implemented as macros since they are used very often and
 * we want to remove the function call overhead.
 *
 * The registers are accessed through memory addresses 0x8200 and 0x8300.
 * The choice of these addresses are motivated by the need to stay in
 * upper half of memory space (recommended in datasheet), while using the
 * first two addresses that makes the address line A8 go low and high respectively.
 * Address line A8 is connected to the single address line of the LCD module. Setting
 * it high selects the data register, low selects the control register.
 * For PCB REV E, A8 must be bit-banged, regardless.
 */

#if defined(PCB_REV_C)
  //! Macro function for writing a byte to memory-mapped LCD Data register at address 0x8100.
  #define S6B1713M_WriteData(value)     { *((volatile uint8_t *) 0x8100) = value; }
  //! Macro function for writing a byte to memory-mapped LCD Control register at address 0x8000.
  #define S6B1713M_WriteCommand(value)  { *((volatile uint8_t *) 0x8000) = value; }
  //! Macro function for reading a byte from memory-mapped LCD Data register at address 0x8100.
  #define S6B1713M_ReadData()           ( *((volatile uint8_t *) 0x8100))
  //! Macro function for reading a byte from memory-mapped LCD Control/Status register at address 0x8000.
  #define S6B1713M_ReadStatus()         ( *((volatile uint8_t *) 0x8000))
#elif defined(PCB_REV_E)
  //! Macro function for writing a byte to memory-mapped LCD Data register at address 0x8100.
  #define S6B1713M_WriteData(value)     { PORTC |= (1 << PC0); *((volatile uint8_t *) 0x8100) = value; }
  //! Macro function for writing a byte to memory-mapped LCD Control register at address 0x8000.
  #define S6B1713M_WriteCommand(value)  { PORTC &= ~(1 << PC0); *((volatile uint8_t *) 0x8000) = value; }
  //! Macro function for reading a byte from memory-mapped LCD Data register at address 0x8100.
  #define S6B1713M_ReadData()           ( PORTC |= (1 << PC0), *((volatile uint8_t *) 0x8100))
  //! Macro function for reading a byte from memory-mapped LCD Control/Status register at address 0x8000.
  #define S6B1713M_ReadStatus()         ( PORTC &= ~(1 << PC0), *((volatile uint8_t *) 0x8000))
#endif



/***********************
 * Function prototypes.
 ***********************/

void S6B1713M_Init( void );  //!< Initialize memory-mapped interface. The LCD controller itself is not initialized.

void S6B1713_HardReset( void );  //!< Pulse the LCD controller RESET pin low in order to reset the module.

void S6B1713_SetDisplayOn( void );  //!< Enable display.
void S6B1713_SetDisplayOff( void );  //!< Disable display, essentially blanking it.
void S6B1713_SetInitialDisplayLine( uint8_t startLine );  //!< Select which line to display as the first LCD line, essentially rolling the display.
void S6B1713_SetReferenceVoltage( uint8_t value );  //!< Select reference voltage to the internal regulator. Refer to datasheet.
static void S6B1713_SetPageAddress( uint8_t address );  //!< Select memory page 0-7. On page is eight pixels high starting from top.
static void S6B1713_SetColumnAddress( uint8_t address );  //!< Select in-page column address. One dummy data read is required before reading actual data.
void S6B1713_SetADCNormalDirection( void );  //!< Select normal segment line (column) order. Applies to written data, not display itself.
void S6B1713_SetADCReverseDirection( void );  //!< Select reverse segment line (column) order. Applies to written data, not display itself.
void S6B1713_SetReverseDisplayOff( void );  //!< Select ordinary display of all pixels, instead of all-pixels-reversed.
void S6B1713_SetReverseDisplayOn( void );  //!< Select reverse display of all pixels.
void S6B1713_SetEntireDisplayOff( void );  //!< Select ordinary display of all pixels, instead of all-pixels-on.
void S6B1713_SetEntireDisplayOn( void );  //!< Set all pixels on, regardless of memory contents. Memory is left untouched. Overrides reverse display.
void S6B1713_SetLCDBias0( void );  //!< Select LCD bias ratio 1/7 (given 1/65 duty ratio). Refer to datasheet.
void S6B1713_SetLCDBias1( void );  //!< Select LCD bias ratio 1/9 (given 1/65 duty ratio). Refer to datasheet.
static void S6B1713_SetReadModifyWriteOn( void );  //!< Disable automatic address increment after data read operations (only write will auto increment).
static void S6B1713_SetReadModifyWriteOff( void );  //!< Enable automatic address increment after both data read and write operations.
void S6B1713_Reset( void );  //!< Software reset of the LCD controller.
void S6B1713_SetSHLNormalDirection( void );  //!< Select normal common line (row) order. Applies to display itself, not written data.
void S6B1713_SetSHLReverseDirection( void );  //!< Select reverse common line (row) order. Applies to display itself, not written data.
void S6B1713_SetPowerControl( uint8_t bitmask );  //!< Controls internal voltage regulator circuitry, Use bit mask defines.
void S6B1713_SetInternalResistorRatio( S6B1713_INTERNAL_RESISTOR_RATIO_t ratio );  //!< Set voltage regulator resistor ratio (5.29 for 64128G module).
void S6B1713_SetStaticIndicatorMode( S6B1713_STATIC_INDICATOR_MODE_t mode );  //!< Set static indicator blink mode. Not available for 64128G module.
void S6B1713_PowerSave( void );  //!< Enter power save mode.
void S6B1713_PowerSaveOff( void );  //!< Leave power save mode.



/******************************************
 * Inline functions and defines they need.
 ******************************************/

/*
 * Definitions required for the inline functions below.
 */

#define S6B1713_CMD_PAGE_ADDRESS               0xB0
#define S6B1713_CMD_COLUMN_ADDRESS_HIGH        0x10
#define S6B1713_CMD_COLUMN_ADDRESS_LOW         0x00
#define S6B1713_CMD_READ_MODIFY_WRITE_ON       0xE0
#define S6B1713_CMD_READ_MODIFY_WRITE_OFF      0xEE



/*
 * Often-used functions inlined for speed optimization.
 */

CAL_ALWAYS_INLINE( static inline void S6B1713_SetPageAddress( uint8_t address ) )
{
	// Truncate address to 4 bits (0-7) and issue command.
	address &= 0x0f;
	S6B1713_WriteCommand( S6B1713_CMD_PAGE_ADDRESS | address );
}


CAL_ALWAYS_INLINE( static inline void S6B1713_SetColumnAddress( uint8_t address ) )
{
	// Truncate address to 7 bits (0-127) and issue command.
	address &= 0x7f;
	S6B1713_WriteCommand( S6B1713_CMD_COLUMN_ADDRESS_HIGH | ((address & 0xF0) >> 4) );
	S6B1713_WriteCommand( S6B1713_CMD_COLUMN_ADDRESS_LOW | (address & 0x0F) );
}


CAL_ALWAYS_INLINE( static inline void S6B1713_SetReadModifyWriteOn( void ) )
{
	S6B1713_WriteCommand( S6B1713_CMD_READ_MODIFY_WRITE_ON );
}


CAL_ALWAYS_INLINE( static inline void S6B1713_SetReadModifyWriteOff( void ) )
{
	S6B1713_WriteCommand( S6B1713_CMD_READ_MODIFY_WRITE_OFF );
}


#endif
