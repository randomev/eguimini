/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 configure UART demo application.
 *
 *         This application lets you change all settings about the UART
 *         on the DB101.
 *
 *         Number of data bits, stop bits, baud rate and interface (logic or RS232)
 *         can be adjusted. The DB101 supports both logic level and RS232 electrical
 *         interfaces.
 *
 *         NOTE: At the moment only 7 or 8 data bits are accepted
 *
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
 * $Revision: 2395 $
 * $Date: 2007-09-13 13:16:08 -0600 (to, 13 sep 2007) $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/production_demo_rev_A/configuart.c $
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

#include "forms_lib.h"
#include <lcd_lib.h>
#include <backlight_driver.h>
#include <widgets_lib.h>
#include <dialog_lib.h>
#include <common.h>
#include <memblock_lib.h>
#include <termfont_lib.h>
#include <popup_lib.h>

#define CONTROLLER_COUNT 3
#define CONTROLLER_WIDTH 10 //eCars-eECU, Curtis, SigmaDrive

#define CONTROLLER_ECARSECU 0
#define CONTROLLER_CURTIS 1
#define CONTROLLER_SIGMADRIVE 2

#define BUTTON_COUNT 2

static char const CAL_PGM_DEF(titleString[]) = "Configure system";

static char const CAL_PGM_DEF(controllerCaption[])      = "Control";

static char const CAL_PGM_DEF(controllereCars[]) 		= "eCars eECU";
static char const CAL_PGM_DEF(controllerCurtis[]) 		= "    Curtis";
static char const CAL_PGM_DEF(controllerSigmaDrive[])  	= "SigmaDrive";

static char const CAL_PGM_DEF(* CAL_PGM_DEF(controllerStrings[CONTROLLER_COUNT])) = {
	controllereCars,
	controllerCurtis,
	controllerSigmaDrive
};

static char const CAL_PGM_DEF(buttonApply[])  = "Apply";
static char const CAL_PGM_DEF(buttonCancel[]) = "Cancel";

static char const CAL_PGM_DEF( * CAL_PGM_DEF(buttonStrings[BUTTON_COUNT])) = {
	buttonApply,
	buttonCancel
};

static WIDGETS_integer_t CONFIG_controllerSelection = 0;


void ConfigSystem( void )
{
	FORMS_form_t * form = MEM_ALLOC( FORMS_form_t );
	WIDGETS_StaticText_t * titleCtrl = MEM_ALLOC( WIDGETS_StaticText_t );
	WIDGETS_Separator_t * topSep = MEM_ALLOC( WIDGETS_Separator_t );
	WIDGETS_SpinCtrl_t * controllerCtrl = MEM_ALLOC( WIDGETS_SpinCtrl_t );
	WIDGETS_Separator_t * bottomSep = MEM_ALLOC( WIDGETS_Separator_t );
	WIDGETS_ButtonRow_t * buttonRow = MEM_ALLOC( WIDGETS_ButtonRow_t );

	if ((form == NULL) || (titleCtrl == NULL) || (topSep == NULL) ||
			(controllerCtrl == NULL) || (bottomSep == NULL) ||
			(buttonRow == NULL)) {
		POPUP_MsgBox( 10, 2, 6, "Not enough\r\nmemory!", NULL );
	} else {
		FORMS_Init( form, true );
		
		WIDGETS_StaticText_Init( titleCtrl, 0, 1, true, false );
		FORMS_SetCaption_F( &titleCtrl->element, titleString );
		FORMS_AddBottomElement( form, &titleCtrl->element );
	
		WIDGETS_Separator_Init( topSep );
		FORMS_AddBottomElement( form, &topSep->element );
		
		WIDGETS_SpinCtrl_Init( controllerCtrl,
				0, // Id doesn't care.
				&CONFIG_controllerSelection,
				WIDGETS_SpinCtrl_DrawString,
				0, // from 0...
				CONTROLLER_COUNT - 1, // to last baud
				1, // step size = 1
				CONTROLLER_WIDTH ); // data field is 6 chars.
		FORMS_SetCaption_F( &controllerCtrl->element, controllerCaption );
		WIDGETS_SpinCtrl_SetStringList_FF( controllerCtrl, controllerStrings );
		FORMS_AddBottomElement( form, &controllerCtrl->element );	
	
		WIDGETS_Separator_Init( bottomSep );
		FORMS_AddBottomElement( form, &bottomSep->element );
		
		WIDGETS_id_t const buttonIDs[] = { DIALOG_OK, DIALOG_CANCEL };
		WIDGETS_ButtonRow_Init( buttonRow, buttonIDs, 0, 2 );
		WIDGETS_ButtonRow_SetStringList_FF( buttonRow, buttonStrings );
		WIDGETS_ButtonRow_SetOnClick( buttonRow, DIALOG_OnClickInModal );
		FORMS_AddBottomElement( form, &buttonRow->element );
	
		FORMS_SetFocusedElement( form, &controllerCtrl->element );
		
		WIDGETS_id_t result = DIALOG_RunModal( form );
		

		if (result == DIALOG_OK) {

		}
		
		FORMS_CleanUp();
	}

	MEM_FREE( form );
	MEM_FREE( titleCtrl );
	MEM_FREE( topSep );
	MEM_FREE( controllerCtrl );
	MEM_FREE( bottomSep );
	MEM_FREE( buttonRow );
}
