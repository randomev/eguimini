#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdbool.h>

#include <cal.h>

#define SUCCESS	0
#define FAILURE	-1

typedef int8_t err_t;	//!< err_t is used as return value for functions that return failure/success (0 is success, -1 is unspecific failure, positive numbers are defined failure).

#define CPU_F 7372800

#define DELAY_CLK(_clk) CAL_delay_cycles( (_clk) );
#define DELAY_US(_us)   DELAY_CLK( (CPU_F / 1000000) * (_us) );
#define DELAY_MS(_ms)   DELAY_CLK( (CPU_F / 1000) * (_ms) );
#define DELAY_S(_s)     DELAY_CLK( CPU_F * (_s) );


#define CREATE_BYTE(b7,b6,b5,b4,b3,b2,b1,b0) \
		(((b7) << 7) |  \
		 ((b6) << 6) |  \
		 ((b5) << 5) |  \
		 ((b4) << 4) |  \
		 ((b3) << 3) |  \
		 ((b2) << 2) |  \
		 ((b1) << 1) |  \
		 ((b0) << 0))


#define BEGIN_CRITICAL_SECTION \
	{  \
		uint8_t savedSREG = SREG;  \
		CAL_disable_interrupt();

#define END_CRITICAL_SECTION \
		SREG = savedSREG;  \
	}

/*****************************************************************************
 SPI UART global settings - used by uart_spi_master_lib
*****************************************************************************/
#define SBAUD (CPU_F/2)			//!< Define baudrate for UART SPI
#define UART0_SPI		   		//!< Define which UART_SPI to use (be aware that there is no check that this UART actually exists!)
#define USPI_XCK_PORT (PORTE)  //!< Definition of the UART SPI port out register.
#define USPI_XCK_DDR  (DDRE)	//!< Definition of the UART SPI data direction register.
#define USPI_XCK_PIN  (PORTE2) //!< Definition of the UART SPI clock pin.
#define USPI_CS_PORT  (PORTG)  //!< Definition of the UART SPI port out register.
#define USPI_CS_DDR   (DDRG)	//!< Definition of the UART SPI data direction register.
#define USPI_CS_PIN   (PORTG5) //!< Definition of the UART SPI chip select/slave select pin.

#endif
