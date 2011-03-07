/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 main demo application.
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
 * $Revision: 2347 $
 * $Date: 2007-09-11 00:18:39 -0600 (ti, 11 sep 2007) $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/production_demo_rev_A/main.c $
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

#include <lcd_lib.h>
#include <s6b1713_driver.h>
#include <joystick_driver.h>
#include <fifo_lib.h>
#include <timing_lib.h>
#include <rtc_driver.h>
#include <backlight_driver.h>
#include <dialog_lib.h>
#include <gfx_lib.h>
#include <memblock_lib.h>
#include <picture_lib.h>
#include <popup_lib.h>
#include <power_driver.h>

#include "flashpics.h"
#include "logo.h"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#define SPLASH_SCREEN_SECONDS 3

#define CONFIG_SET_RS232_INTERFACE()  DDRD |= (1 << PD4); PORTD &= ~(1 << PD4);
#define TERMINAL_RXBUFSIZE 255
#define TERMINAL_TXBUFSIZE 16
#define wdt_enable(WDTO_1S)

#define BAUD 57600 

TIMING_event_t joystickCallbackEvent;

volatile uint8_t urx;
volatile uint8_t urx_recv;
int Red=50;
int Green=50;
int Blue=50;
int Contrast=4;
int Intensity=254;

#ifdef __GNUC__

char const CAL_PGM_DEF(welcomeTitle[]) = "       eGUI mini";

char const CAL_PGM_DEF(welcomeText[]) =
	"         by         "
	"      eCars.Now!    "
	"                    "
	"     Open Source    "
	"      Electric      "
	"       Vehicle      "
	"        User        "
	"      Interface     "
	"                    "
	"   For more info    "
	"      visit         "
	" www.ecars-now.org  ";

#else

char const CAL_PGM_DEF(welcomeTitle[]) = "Atmel DB101 Demo";

char const CAL_PGM_DEF(welcomeText[]) =
	"Welcome to the demo "
	"application for the "
	"DB101 display kit.  "
	"Please refer to the "
	"appnotes AVR481 and "
	"AVR482 available at "
	"www.atmel.com/avr   "
	"for more details.   ";
#endif

#define APP_COUNT 4
char const CAL_PGM_DEF(appTitle[]) = "eGUI mini";
char const CAL_PGM_DEF(app0[]) = "DRIVE!";
char const CAL_PGM_DEF(app1[]) = "Adjust Display";
char const CAL_PGM_DEF(app2[]) = "Configure system";
char const CAL_PGM_DEF(app3[]) = "Demo";
char const CAL_PGM_DEF(* apps[APP_COUNT]) = { app0, app1, app2, app3 };
WIDGETS_StaticText_t textWidgets[APP_COUNT];
 

void InitMemory( void );

void SmokeyDemo( void );
void GameOfLife( void );
void Snake( void );
void Clock( void );
void Slideshow( void );
void Walkabout( void );
void ConfigUart( void );
void SoundDemo( void );
void Terminal( void );
void LcdContrast( void );


void UnknownError( void )
{
	POPUP_MsgBox( 14, 1, 10, "Unknown error!", NULL );
	for (;;) {}
}


/*** CUSTOM CODE FOR SCREEN DUMP TO USART1 ***/
static uint8_t lcdPageBuffer[LCD_WIDTH];
char const CAL_PGM_DEF(xpmHeader[]) =
		"/* XPM */\r\n"
		"static char * db101_screen_dump[] = {\r\n"
		"\"128 64 2 1\",\r\n"
		"\"   c #FFFFFF\",\r\n"
		"\".  c #000000\",\r\n";			

void SendChar( char ch )
{
	do { /* nothing */ } while ((UCSR1A & (1 << UDRE1)) == 0);
	UDR1 = ch;
}

void DumpHeader( void )
{
	char const CAL_PGM(* pch) = xpmHeader;
	char ch;

	while ( ( ch = CAL_pgm_read_char( pch ) ) != '\0' ) {
		SendChar( ch );
		pch++;
	}
}
/*
void DumpScreen( void )
{
	PRR1 &= ~(1 << PRUSART1);
	UCSR1B |= (1 << TXEN1);
	UBRR1 = UART_CALC_UBRR_VALUE(115200);
	DDRD |= (1 << PD4); PORTD &= ~(1 << PD4); // Turn on RS232.
	DumpHeader();
	for (uint8_t row = 0; row < LCD_PAGE_COUNT; ++row) {
		LCD_ReadPage( lcdPageBuffer, LCD_ROW2PAGE(row), 0, LCD_WIDTH );
		for (uint8_t subrow = 0; subrow < LCD_PAGE_HEIGHT; ++subrow) {
			SendChar( '"' );
			for (uint8_t column = 0; column < LCD_WIDTH; ++column) {
				if ((lcdPageBuffer[column] & (0x01 << subrow)) != 0x00) {
					SendChar( '.' );
				} else {
					SendChar( ' ' );
				}
			}
			SendChar( '"' );
			SendChar( '\r' );
			SendChar( '\n' );
		}
	}
	SendChar( '}' );
	SendChar( ';' );
	SendChar( '\r' );
	SendChar( '\n' );
}
*/

static int uart_putchar(char c, FILE *stream)
    {

      if (c == '\n')
        uart_putchar('\r', stream);
      while(!(UCSR1A & (1<<UDRE1)));
      UDR1 = c;
      return 0;
    }

SIGNAL(USART1_RX_vect)
{
  uint8_t s;

  s = UCSR1A;
  urx = UDR1;
//  if (bit_is_clear(s, FE1)) {
    urx_recv = 1;
//  }
}


void USART_Init(void)
 {
	
  /* Enable receiver, transmitter and receive interrupt. */
	UCSR1B = (1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1);
//  UCSR1B |= 1<<RXCIE1;  // receive interrupt enabled

//  UCSR1C |= 1<<1;
//  UCSR1C |= 1<<2;
  UBRR1H = 0x00;
  UBRR1L = ((CPU_F/16)/BAUD)-1; // calculates the speed of serial transmission
  
 }

unsigned char ReceiveCharUart1(void) {
        
        // wait for data to be received
        while(!(UCSR1A & (1<<RXC1)));
        // get and return received data from buffer
        return UDR1; 

}

unsigned char ReceiveCharUart1_nonstop(void) {
        
        // wait for data to be received
        if((UCSR1A & (1<<RXC1)))
                // get and return received data from buffer
                return UDR1; 
        else
                // return 0
                return 0;

}

char xtod(char c) {
 if (c>='0' && c<='9') return c-'0';
 if (c>='A' && c<='F') return c-'A'+10;
 if (c>='a' && c<='f') return c-'a'+10;
 return c=0;        // not Hex digit
}
 
int HextoDec(char *hex, int l)
{
    if (*hex==0) return(l);
    return HextoDec(hex+1, l*16+xtod(*hex)); // hex+1?
}
 
int xstrtoi(char *hex)      // hex string to integer
{
    return HextoDec(hex,0);
}

char *substring(size_t start, size_t stop, const char *src, char *dst, size_t size)
{
   int count = stop - start;
   if ( count >= --size )
   {
      count = size;
   }
   sprintf(dst, "%.*s", count, src + start);
   return dst;
}

char *substr_NOT(const char *pstr, int start, int numchars)
{
	char *pnew;
	int i;
	for (i=start;i<start+numchars;i++)
	{
		strcat(pnew, pstr[i]);
	}
	strcat(pnew, '\n');

	return pnew;
}

char *sort_of_works_substr(const char *pstr, int start, int numchars)
{
	char *pnew; // = malloc(numchars+1);
	strncpy(pnew, pstr + start, numchars);
	pnew[numchars] = '\0';
	return pnew;
}

substr(const char *pstr, int start, int numchars, char *pnew)
{
	strncpy(pnew, pstr + start, numchars);
	pnew[numchars] = '\0';
	return pnew;
}
 
/* parse an integer from a string */
uint16_t get_num(char * s, char ** next, int8_t * err)
{
  char * e;
  uint16_t n;

  n = strtoul(s, &e, 0);
  if (e == s) {
    //printf("error: get_num(): can't scan arg: \"%s\"\n", s);
    *next = s;
    if (err)
      *err = 1;
    return 0xffff;
  }

  while (*e == '=')
    e++;

  *next = e;

  if (err)
    *err = 0;
  
  return n;
}
/*
 * decode and execute simple commands
 */
static char do_cmdbuf[20];
int la=0;

static __inline__ void do_cmd(char * s)
{
  static char * cmd = do_cmdbuf;
  uint8_t       index;
  char        * args, * p;
  static char * can_addr;
  int8_t rc;
  int16_t value=0;
  char * raw_byte;
  short * raw_short;

  if (s[0] == 0)
    return;

  /* parse the command line, seperating the command from arguments */
  cmd[0] = 0;
  index = 0;
  while ((index < sizeof(do_cmdbuf)) && s[index] && (s[index] != '=')) {
    cmd[index] = s[index];
    index++;
  }
  if (index < sizeof(do_cmdbuf)) {
    cmd[index] = 0;
    args = &s[index];
    while (*args && (*args == '='))
      args++;
    if (*args == 0)
      args = NULL;
  }
  else {
    cmd[sizeof(do_cmdbuf)-1] = 0;
    args = NULL;
  }

  if (cmd[0] == 0) {
    return;
  }

  // 11bit CAN frame ?
  if (strcmp(cmd[0],'t') == 0) {
	//char *pnew = malloc(4);
	
	char *pnew = MEM_ALLOC(4);

	can_addr = substr(cmd, 1, 3, pnew);

//	printf("pnew: %s\n", pnew);
//	printf("CMD: %s\n", cmd);
//	printf("CAN ADDR: %s\n", can_addr);
	
	MEM_FREE(pnew);
	
	//can_addr = substring(1, 4, cmd, can_addr, sizeof can_addr);

	//can_addr = sort_of_works_substr(cmd, 1, 3);
	
	//free(pnew);


	// Layout of normal driving screen:
	// 
	//  |----------------|---------|
	//  | BIG NUMBERS    | BATTERY |
	//  |--------|-------|  ICON   |
	//  |3.45 V  | 31 C  |         |
	//  |--------------------------|
	// 
	// All this information is extracted right here from single CAN-frame with address 630h
	//
	// Summary values of interest @ CAN ID 630h
	//
	// Byte Type 			Desc 								Units per lsb
	// 0	unsigned char	Pack State of Charge				0.5%
	// 1	unsigned char	Pack State of Function (not in use)	n/a
	// 2	unsigned char	Pack State of Health				0.5%
	// 3	unsigned char	Max Pack Temperature				1 deg C
	// 4-5	short			Min Pack Voltage					1mV
	// 6-7	short			Max Pack Voltage					1mV
	 
	if (strcmp(can_addr, "630") == 0) {
		wdt_reset();
		// SOC is byte 0 in 0.5% per LSB
		// CAN-message is formatted as

		// t63080011223344556677		
		// ---------------------
		//          111111111122
		// 123456789012345678901
		
		// 
		//printf("630!\n");
		//char *pnew2 = malloc(3);
		//raw_byte = substring(5, 7, cmd, raw_byte, sizeof raw_byte);
		*pnew = MEM_ALLOC(3);

		// SOC, byte 0
		raw_byte = substr(cmd, 5, 2,pnew);
		MEM_FREE(pnew);
		value = xstrtoi(raw_byte);
		value = value/2;	// 0.5 % per LSB
		LCD_UpdateSOC(value);


		// Max Pack temp, byte 3
		raw_byte = substr(cmd, 11, 2,pnew);
		MEM_FREE(pnew);
		value = xstrtoi(raw_byte);
		LCD_UpdateMaxTemp(value);
		
		char *spnew = MEM_ALLOC(6);

		// Min Pack Voltage, byte 4-5
		raw_short = substr(cmd, 13, 4,spnew);
		MEM_FREE(spnew);
		value = xstrtoi(raw_short);
		LCD_UpdateMinVolt(value);
		
		// Small status line for each frame received. Since ID 630 should
		// be transmitted once per second, there should be small but visible
		// blinking of few pixels in one of the corners of the display. 		
		if (la == 0)
		{
				LCD_ClrLine(1,63,2,63);
				la = 1;
		} else {
				LCD_SetLine(1,63,2,63);
				la = 0;
		}
		
	} // Summary values end	

	
	// Screen contrast and color
	if (strcmp(can_addr, "7DD") == 0) {
		// CAN-message is formatted as

		// t88050011223344		
		// ---------------
		//           11111
		// 012345678901234

		// byte 0 contrast
		// byte 1 red
		// byte 2 green
		// byte 3 blue
		// byte 4 intensity

		// byte 0 contrast
		*pnew = MEM_ALLOC(3);
		raw_byte = substr(cmd, 5, 2,pnew);
		MEM_FREE(pnew);
		value = xstrtoi(raw_byte);		
		eeprom_write_word(8, value);

		// byte 1 red
		*pnew = MEM_ALLOC(3);
		raw_byte = substr(cmd, 7, 2,pnew);
		MEM_FREE(pnew);
		value = xstrtoi(raw_byte);		
		eeprom_write_word(10, value);

		// byte 2 green
		*pnew = MEM_ALLOC(3);
		raw_byte = substr(cmd, 9, 2,pnew);
		MEM_FREE(pnew);
		value = xstrtoi(raw_byte);		
		eeprom_write_word(12, value);

		// byte 3 blue
		*pnew = MEM_ALLOC(3);
		raw_byte = substr(cmd, 11, 2,pnew);
		MEM_FREE(pnew);
		value = xstrtoi(raw_byte);		
		eeprom_write_word(14, value);

		// byte 3 blue
		*pnew = MEM_ALLOC(3);
		raw_byte = substr(cmd, 13, 2,pnew);
		MEM_FREE(pnew);
		value = xstrtoi(raw_byte);		
		eeprom_write_word(16, value);
		
		Contrast = eeprom_read_word((uint16_t*)8);
		Red = eeprom_read_word((uint16_t*)10);
		Green = eeprom_read_word((uint16_t*)12);
		Blue = eeprom_read_word((uint16_t*)14);
		Intensity = eeprom_read_word((uint16_t*)16);

		BACKLIGHT_SetRGB( Red, Green, Blue );
		BACKLIGHT_SetIntensity(Intensity);

		TERMFONT_DisplayString(".Display adjusted.", 7, 0);
		
		DELAY_MS(500);
		
		LCD_ClrBox(0,0,128,64);

	} // Summary values end	

  }

  return;
}

/*
 * accept characters and build a command line, when return is pressed,
 * pass the command to 'do_cmd()' 
 */
#define CMD_BUF_LEN 128
static char recv_input_cmdbuf[CMD_BUF_LEN];

void recv_input(uint8_t ch)
{
  static uint8_t idx=0;

  if ((ch == '\r')||(ch == '\n')) {
    //uart_putc('\n');
    recv_input_cmdbuf[idx] = 0;
    do_cmd(recv_input_cmdbuf);
    //printf_P(s_prompt);
    idx = 0;
  }
//  else if ((ch == '\b') && idx) {
//    printf_P(s_bksp);
//    if (idx > 0)
//      idx--;
//    recv_input_cmdbuf[idx] = 0;
//  }
  else {
    //uart_putc(ch); // if we want echo, uncomment this
    recv_input_cmdbuf[idx++] = ch;
    if (idx == CMD_BUF_LEN) {
      idx = 0;
      recv_input_cmdbuf[idx] = 0;
      //printf_P(s_invalid);
      //printf_P(s_prompt);
    }
  }
}
	
unsigned char USART_Receive( void ) 
{
	/* Wait for empty transmit buffer */ 
//	while ( !( UCSR1A & (1<<UDRE1)) ); 

	/* Put data into buffer, sends the data */ 
//	UDR1 = 'a'; 

	/* Wait for data to be received */ 
	while ( !(UCSR1A & (1<<RXC1)) ); 

	/* Get and return received data from buffer */ 
	return UDR1;
}


static int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

CAL_MAIN()
{
	wdt_enable(WDTO_4S);

	LCD_UpdateSOC(1);

	stdout = &mystdout;

	DDRD |= (1 << PD4); PORTD &= ~(1 << PD4); // Turn on RS232.

	USART_Init();

	uint8_t ch;

	LCD_UpdateSOC(2);
        
	// Initialize everything.
	InitMemory();

	LCD_UpdateSOC(3);

	LCD_Init();

	LCD_UpdateSOC(4);

	TIMING_Init();

	LCD_UpdateSOC(5);

	RTC_Init();

	LCD_UpdateSOC(6);

	RTC_SetTickHandler( TIMING_TickHandler );

	LCD_UpdateSOC(7);

	JOYSTICK_Init( 100, 50 );

	//DDRD |= (1 << PD4); PORTD &= ~(1 << PD4); // Turn on RS232.

	//USART_Init();

//	CAL_enable_interrupt();

	TIMING_AddRepCallbackEvent( TIMING_INFINITE_REPEAT, 1, JOYSTICK_PollingHandler, &joystickCallbackEvent );

	// happy startup sound
	LCD_UpdateSOC(8);

	// BATTERY ICON SOC-LEVEL TEST
	int soc = 0;

	// interrupt enable
	SREG |= 1<<7;

	LCD_UpdateSOC(9);

	// Init CAN-adapter
	// few returns to wake up the device
	/*
	printf("\r\r");
	
	// version query
	printf("V\r");
	
	// auto poll/send = ON
	printf("X1\r");
	
	printf("S6\r");	// (S)peed 500 kBit
	
	// Open the CAN channel
	printf("O\r");	// (O)pen the CAN-bus
	*/

	LCD_UpdateSOC(10);

	PlaySound(11);

	// Display splash screen, wait for joystick.

	//1 POWER_EnterIdleSleepMode();

	//PICTURE_CopyFullscreenFlashToLcd( FLASHPICS_excellenceThroughTechnology );
	//PICTURE_CopyFullscreenFlashToLcd( FLASHPICS_eCarsLogo );
//	PICTURE_CopyFullscreenFlashToLcd( FLASHPICS_PalonenLABS_128x64px );

	// init backlight
	BACKLIGHT_Init();
	Contrast = eeprom_read_word((uint16_t*)8);
	Red = eeprom_read_word((uint16_t*)10);
	Green = eeprom_read_word((uint16_t*)12);
	Blue = eeprom_read_word((uint16_t*)14);
	Intensity = eeprom_read_word((uint16_t*)16);

	BACKLIGHT_SetRGB( Red, Green, Blue );
	BACKLIGHT_SetIntensity(Intensity);
/*
	TIMING_event_t * splashScreenEvent = MEM_ALLOC( TIMING_event_t );
	if (splashScreenEvent == NULL) { UnknownError(); }
	TIMING_counter_t volatile splashCounter = 0;
	TIMING_AddCounterEventAfter( RTC_TICKS_PER_SECOND * SPLASH_SCREEN_SECONDS,
			&splashCounter, splashScreenEvent );
	bool exit = false;
	do {
		POWER_EnterIdleSleepMode();
		if (JOYSTICK_GetState() != 0x00) { exit = true; };
		if (splashCounter != 0) { exit = true; }
	} while (exit == false);


	TIMING_RemoveEvent( splashScreenEvent );
	MEM_FREE( splashScreenEvent );

	DELAY_MS(500);
*/
	LCD_ClrBox(0,0,128,64);

//	exit = false;	
	/*
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
			//TERM_ProcessChar( term, charInput );
		}
	} while (exit == false);
	*/

//	MEM_FREE( rxBuffer );
//	MEM_FREE( txBuffer );
//	LcdContrast();

//if 	(JOYSTICK_GetState() & (JOYSTICK_ENTER) == 0x00) {
//	LcdContrast();
//}

	while (1)
 	{
		if (urx_recv) 
		{	
			cli();
            urx_recv = 0;
            ch = urx;
            sei();
            /* build a command line and execute commands when complete */
            recv_input(ch);
		}
	}

	CAL_MAIN_LAST();
}
