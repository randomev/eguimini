/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 LCD contrast settings demo application.
 *
 *         This application lets you change the contrast of the LCD screen.
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
 * $Date: 2007-07-11 17:34:22 +0200 (Wed, 11 Jul 2007) $
 * $URL: http://svn.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/trunk/code/IAR/production_demo_rev_A/lcdcontrast.c $
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

#include "forms_lib.h"
#include <lcd_lib.h>
#include <backlight_driver.h>
#include <widgets_lib.h>
#include <dialog_lib.h>
#include <common.h>
#include <memblock_lib.h>
#include <termfont_lib.h>
#include <popup_lib.h>
#include <s6b1713_driver.h>
#include <avr/eeprom.h>

#define BUTTON_COUNT 1


static char const CAL_PGM_DEF(titleString[]) = "Contrast and color";

static char const CAL_PGM_DEF(contrastCaption[]) = 	"Contrast [0:63]";
static char const CAL_PGM_DEF(redCaption[]) = 		"Red     [0:254]";
static char const CAL_PGM_DEF(greenCaption[]) = 	"Green   [0:254]";
static char const CAL_PGM_DEF(blueCaption[]) = 		"Blue    [0:254]";
static char const CAL_PGM_DEF(intensityCaption[]) = "Intens. [0:254]";

static char const CAL_PGM_DEF( buttonOK[] ) = "OK";

static char const CAL_PGM_DEF(* CAL_PGM_DEF(buttonStrings[BUTTON_COUNT])) = {
	buttonOK,
};


static WIDGETS_integer_t CONFIG_contrastSelection = 4;
static WIDGETS_integer_t CONFIG_redSelection = 50;
static WIDGETS_integer_t CONFIG_greenSelection = 50;
static WIDGETS_integer_t CONFIG_blueSelection = 50;
static WIDGETS_integer_t CONFIG_intensitySelection = 254;


void SetLcdRegister(WIDGETS_id_t userId, WIDGETS_integer_t const * data )
{
	uint8_t value = *data;
	S6B1713_SetReferenceVoltage(value);
	eeprom_write_word(8,value);
}

void SetRedRegister(WIDGETS_id_t userId, WIDGETS_integer_t const * data )
{
	uint8_t value = *data;
	BACKLIGHT_SetRed(value);
	eeprom_write_word(10,value);
}

void SetGreenRegister(WIDGETS_id_t userId, WIDGETS_integer_t const * data )
{
	uint8_t value = *data;
	BACKLIGHT_SetGreen(value);
	eeprom_write_word(12,value);
}

void SetBlueRegister(WIDGETS_id_t userId, WIDGETS_integer_t const * data )
{
	uint8_t value = *data;
	BACKLIGHT_SetBlue(value);
	eeprom_write_word(14,value);
}

void SetIntensityRegister(WIDGETS_id_t userId, WIDGETS_integer_t const * data )
{
	uint8_t value = *data;
	BACKLIGHT_Init();
	BACKLIGHT_SetIntensity(value);
	eeprom_write_word(16,value);
}


void LcdContrast( void )
{

	FORMS_form_t * form = MEM_ALLOC( FORMS_form_t );
	WIDGETS_StaticText_t * titleCtrl = MEM_ALLOC( WIDGETS_StaticText_t );
	WIDGETS_Separator_t * topSep = MEM_ALLOC( WIDGETS_Separator_t );
	WIDGETS_SpinCtrl_t * contrastCtrl = MEM_ALLOC( WIDGETS_SpinCtrl_t );
	WIDGETS_SpinCtrl_t * redCtrl = MEM_ALLOC( WIDGETS_SpinCtrl_t );
	WIDGETS_SpinCtrl_t * greenCtrl = MEM_ALLOC( WIDGETS_SpinCtrl_t );
	WIDGETS_SpinCtrl_t * blueCtrl = MEM_ALLOC( WIDGETS_SpinCtrl_t );
	WIDGETS_SpinCtrl_t * intensityCtrl = MEM_ALLOC( WIDGETS_SpinCtrl_t );

	WIDGETS_Separator_t * bottomSep = MEM_ALLOC( WIDGETS_Separator_t );
	WIDGETS_ButtonRow_t * buttonRow = MEM_ALLOC( WIDGETS_ButtonRow_t );

	if ((form == NULL) || (titleCtrl == NULL) || (topSep == NULL) ||
			(contrastCtrl == NULL) || (bottomSep == NULL) ||
			(buttonRow == NULL) || (redCtrl == NULL) || (greenCtrl == NULL) || 
			(blueCtrl == NULL) || (intensityCtrl == NULL)) {
		POPUP_MsgBox( 10, 2, 6, "Not enough\r\nmemory!", NULL );
	} else {
		FORMS_Init( form, true );
		
		WIDGETS_StaticText_Init( titleCtrl, 0, 1, true, false );
		FORMS_SetCaption_F( &titleCtrl->element, titleString );
		FORMS_AddBottomElement( form, &titleCtrl->element );
	
		WIDGETS_Separator_Init( topSep );
		FORMS_AddBottomElement( form, &topSep->element );
		
		WIDGETS_SpinCtrl_Init( contrastCtrl,
				0, // Id doesn't care.
				&CONFIG_contrastSelection,
				WIDGETS_SpinCtrl_DrawDec,
				0, // from 0...
				63, // to last value
				1, // step size = 1
				2 ); // data field is 2 chars.
		FORMS_SetCaption_F( &contrastCtrl->element, contrastCaption );
		WIDGETS_SpinCtrl_SetOnChange( contrastCtrl, SetLcdRegister );
		FORMS_AddBottomElement( form, &contrastCtrl->element );

		WIDGETS_SpinCtrl_Init( redCtrl,
				0, // Id doesn't care.
				&CONFIG_redSelection,
				WIDGETS_SpinCtrl_DrawDec,
				0, // from 0...
				254, // to last value
				1, // step size = 1
				3 ); // data field is 2 chars.
		FORMS_SetCaption_F( &redCtrl->element, redCaption );
		WIDGETS_SpinCtrl_SetOnChange( redCtrl, SetRedRegister );
		FORMS_AddBottomElement( form, &redCtrl->element );

		WIDGETS_SpinCtrl_Init( greenCtrl,
				0, // Id doesn't care.
				&CONFIG_greenSelection,
				WIDGETS_SpinCtrl_DrawDec,
				0, // from 0...
				254, // to last value
				1, // step size = 1
				3 ); // data field is 2 chars.
		FORMS_SetCaption_F( &greenCtrl->element, greenCaption );
		WIDGETS_SpinCtrl_SetOnChange( greenCtrl, SetGreenRegister );
		FORMS_AddBottomElement( form, &greenCtrl->element );

		WIDGETS_SpinCtrl_Init( blueCtrl,
				0, // Id doesn't care.
				&CONFIG_blueSelection,
				WIDGETS_SpinCtrl_DrawDec,
				0, // from 0...
				254, // to last value
				1, // step size = 1
				3 ); // data field is 2 chars.
		FORMS_SetCaption_F( &blueCtrl->element, blueCaption );
		WIDGETS_SpinCtrl_SetOnChange( blueCtrl, SetBlueRegister );
		FORMS_AddBottomElement( form, &blueCtrl->element );

		WIDGETS_SpinCtrl_Init( intensityCtrl,
				0, // Id doesn't care.
				&CONFIG_intensitySelection,
				WIDGETS_SpinCtrl_DrawDec,
				0, // from 0...
				254, // to last value
				1, // step size = 1
				3 ); // data field is 2 chars.
		FORMS_SetCaption_F( &intensityCtrl->element, intensityCaption );
		WIDGETS_SpinCtrl_SetOnChange( intensityCtrl, SetIntensityRegister );
		FORMS_AddBottomElement( form, &intensityCtrl->element );
	
		WIDGETS_Separator_Init( bottomSep );
		FORMS_AddBottomElement( form, &bottomSep->element );
		
		WIDGETS_id_t const buttonIDs[] = { DIALOG_OK };
		WIDGETS_ButtonRow_Init( buttonRow, buttonIDs, 0, BUTTON_COUNT );
		WIDGETS_ButtonRow_SetStringList_FF( buttonRow, buttonStrings );
		WIDGETS_ButtonRow_SetOnClick( buttonRow, DIALOG_OnClickInModal );
		FORMS_AddBottomElement( form, &buttonRow->element );
	
		FORMS_SetFocusedElement( form, &contrastCtrl->element );
		///mt result not used WIDGETS_id_t result = DIALOG_RunModal( form );
		DIALOG_RunModal( form );
		
		// Done
		FORMS_CleanUp();
	}

	MEM_FREE( form );
	MEM_FREE( titleCtrl );
	MEM_FREE( topSep );
	MEM_FREE( contrastCtrl );
	MEM_FREE( bottomSep );
	MEM_FREE( buttonRow );
}
