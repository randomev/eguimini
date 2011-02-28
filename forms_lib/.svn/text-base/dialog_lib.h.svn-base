// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  Standard dialog box library header file.
 *
 *      The dialog box library provides functions for displaying and operating
 *      commonly used dialog boxes, such as message boxes and checklist boxes.
 *      Most functions accept a title for the dialog, a list of strings for the
 *      dialog box contents, flags for the buttons choices to show (OK, CANCEL
 *      etc), and pointers to pre-allocated memory.
 *
 *      The dialog box library does not allocate any memory itself, so the user
 *      must provide free memory for the library as a workspace for the library
 *      functions.
 *
 *      The library uses the Widgets Library and the Forms Library to display
 *      the dialog boxes, and the Joystick Driver through the FIFO Queue Library
 *      to get user input.
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
 * $Revision: 2415 $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/forms_lib/dialog_lib.h $
 * $Date: 2007-09-17 02:14:42 -0600 (ma, 17 sep 2007) $  \n
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

#ifndef DIALOG_LIB_H
#define DIALOG_LIB_H

#include <cal.h>
#include <widgets_lib.h>


/*************************
 * Constants and defines.
 *************************/

#define DIALOG_YES    (1<<0)  //!< Bitmask for enabling the YES button in dialogs.
#define DIALOG_NO     (1<<1)  //!< Bitmask for enabling the NO button in dialogs.
#define DIALOG_OK     (1<<2)  //!< Bitmask for enabling the OK button in dialogs.
#define DIALOG_CANCEL (1<<3)  //!< Bitmask for enabling the CANCEL button in dialogs.



/***********************
 * Function prototypes.
 ***********************/

//! Callback used for ending modal dialog boxes.
void DIALOG_OnClickInModal( WIDGETS_id_t userId );
//! Show dialog, handle all joystick events, end when OnClickInModal is called.
WIDGETS_id_t DIALOG_RunModal( FORMS_form_t * form );

//! Run modal form with message and button row.
WIDGETS_id_t DIALOG_MessageBox_F(
		char const CAL_PGM(* title),
		char const CAL_PGM(* text),
		uint8_t buttonMask,
		uint8_t defaultButtonMask,
		bool useScrollbar );
//! Run modal form with list of choices. Click on choice will exit.
WIDGETS_id_t DIALOG_SingleChoiceBox_F(
		char const CAL_PGM(* title),
		FORMS_size_t selectionCount,
		FORMS_size_t defaultSelection,
		char const CAL_PGM(* * selections),
		bool useScrollbar,
		WIDGETS_StaticText_t * workspace );
//! Run modal form with list of mutual exclusive selecitions and button row.
WIDGETS_id_t DIALOG_RadioListBox_F(
		char const CAL_PGM(* title),
		FORMS_size_t selectionCount,
		FORMS_size_t defaultSelection,
		char const CAL_PGM(* * selections),
		WIDGETS_id_t * userIds,
		WIDGETS_id_t * resultId,
		WIDGETS_SelectCtrl_t * workspace,
		WIDGETS_integer_t * data,
		uint8_t buttonMask,
		uint8_t defaultButtonMask );
//! Run modal form with list of check boxes and button row.
WIDGETS_id_t DIALOG_CheckListBox_F(
		char const CAL_PGM(* title),
		FORMS_size_t selectionCount,
		char const CAL_PGM(* * selections),
		WIDGETS_SelectCtrl_t * workspace,
		WIDGETS_integer_t * data,
		uint8_t buttonMask,
		uint8_t defaultButtonMask,
		bool useScrollbar );


#endif
// end of file
