/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 terminal emulator demo application.
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
 * $Revision: 2105 $
 * $Date: 2007-06-26 14:42:19 +0200 (ti, 26 jun 2007) $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/trunk/code/IAR/production_demo_rev_A/terminal.c $
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

#include <uart_driver.h>
#include <terminal_lib.h>
#include <memblock_lib.h>
#include <fifo_lib.h>
#include <power_driver.h>
#include <popup_lib.h>
#include <joystick_driver.h>


#define TERMINAL_RXBUFSIZE 255
#define TERMINAL_TXBUFSIZE 16


void Terminal( void )
{
	//POPUP_MsgBox( 16, 2, 6, "Move joystick to\r\nexit terminal.", NULL );

	TERM_terminal_t * term = MEM_ALLOC( TERM_terminal_t );
	FIFO_data_t * rxBuffer = MEM_ALLOC_ARRAY( FIFO_data_t, TERMINAL_RXBUFSIZE );
	FIFO_data_t * txBuffer = MEM_ALLOC_ARRAY( FIFO_data_t, TERMINAL_TXBUFSIZE );
	
	if ((term == NULL) || (rxBuffer == NULL) || (txBuffer == NULL)) {
		POPUP_MsgBox( 10, 2, 6, "Not enough\r\nmemory!", NULL );
	} else {
		FIFO_handle_t rxFifo;
		FIFO_handle_t txFifo;
		
		FIFO_Init( &rxFifo, rxBuffer, TERMINAL_RXBUFSIZE );
		FIFO_Init( &txFifo, txBuffer, TERMINAL_TXBUFSIZE );
		
		UART_Init( &rxFifo, &txFifo );
		LCD_SetScreen( 0x00 );
		TERM_Init( term, 8, 1, true );
		TERM_EnableWriteThrough( term );
		TERM_Redraw( term );
		
		bool exit = false;
		do {
			uint8_t joystickState = JOYSTICK_GetState();
			while ((FIFO_HasData( &rxFifo, FIFO_data_t ) == false) && (joystickState == 0x00)) {
				POWER_EnterIdleSleepMode();
				joystickState = JOYSTICK_GetState();
			}
			
			if (joystickState != 0x00) {
				exit = true;
			}
			else if (FIFO_HasData( &rxFifo, FIFO_data_t ) == true) {
				FIFO_data_t charInput;
				FIFO_GetData( &rxFifo, &charInput );
				TERM_ProcessChar( term, charInput );
			}
		} while (exit == false);
		
		UART_SetRxFifo( NULL );
		UART_SetTxFifo( NULL );
	}
	
	MEM_FREE( term );
	MEM_FREE( rxBuffer );
	MEM_FREE( txBuffer );
}
