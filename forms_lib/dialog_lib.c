// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  Standard dialog box library source file.
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
 * $Revision: 2436 $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/forms_lib/dialog_lib.c $
 * $Date: 2007-09-17 15:18:48 -0600 (ma, 17 sep 2007) $  \n
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

#include "dialog_lib.h"
#include <widgets_lib.h>
#include <fifo_lib.h>
#include <power_driver.h>


/*********************************
 * Private constants and defines.
 *********************************/

#define DIALOG_EVENTFIFOSIZE 8  //!< Depth of dialog box event queue.

#define DIALOG_MAXBUTTONS 4  //!< Number of standard button texts for dialog boxes.

char const CAL_PGM_DEF(DIALOG_txtYes[])    = "YES";     //!< Dialog button "YES" text.
char const CAL_PGM_DEF(DIALOG_txtNo[])     = "NO";      //!< Dialog button "NO" text.
char const CAL_PGM_DEF(DIALOG_txtOK[])     = "OK";      //!< Dialog button "OK" text.
char const CAL_PGM_DEF(DIALOG_txtCancel[]) = "CANCEL";  //!< Dialog button "CANCEL" text.



/*******************************
 * Private types and variables.
 *******************************/

//! Dialog event type. Will be extended as new event types are added.
typedef struct DIALOG_event_struct
{
	JOYSTICK_event_t joystickEvent;  //!< Joystick event placeholder 
} DIALOG_event_t;

//! Pointer to the current form being shown modally.
static FORMS_form_t * DIALOG_currentModalForm = NULL;
//! Set to true when modal dialog box should be exited.
static bool volatile DIALOG_modalClicked;
//! Set the custom ID when modal dialog box should be exited.
static WIDGETS_id_t volatile DIALOG_modalId;
//! FIFO object holding dialog box event queue.
static FIFO_handle_t DIALOG_eventFifo;
//! Data buffer workspace for the event queue.
static FIFO_data_t DIALOG_eventFifoBuf[DIALOG_EVENTFIFOSIZE * sizeof(DIALOG_event_t)];



/****************************
 * Function implementations.
 ****************************/

/*! \brief  Internal function to handle joystick events.
 *
 * When running modal dialog boxes, this function will be registered
 * as the joystick handler callback. It puts the joystick event into to
 * dialog box event queue. The main loop will then process the event
 * outside the interrupt handler domain.
 *
 * \param  event  Pointer to joystick event to be copied into event queue.
 */
static void DIALOG_JoystickHandler( JOYSTICK_event_t const * event )
{
	if (FIFO_HasRoomFor(&DIALOG_eventFifo, JOYSTICK_event_t)) {
		FIFO_PutData( &DIALOG_eventFifo, event );
	}
}


/*!
 * This function should be registered as the OnClick callback for any form
 * element that the user wishes should cause the modal dialog to exit.
 * This function will set the flag that causes the main loop to exit the
 * box, and also stores a custom id for later use.
 *
 * \param  userId  Custom ID for later use. Could be the element's ID.
 */
void DIALOG_OnClickInModal( WIDGETS_id_t userId )
{
	DIALOG_modalClicked = true;
	DIALOG_modalId = userId;
}


/*! \brief  Internal function to prepare a button row element based an a button mask.
 *
 * This function looks at the bits the button bitmask and prepares a ButtonRow
 * widget with all requested buttons. This is an internal function used by the
 * standard dialog box functions in this library.
 * This function does not allocate any memory itself, so several parameters
 * are used to provide SRAM workspaces for this function to operate on. It is
 * the caller's responsibility to provide enough SRAM for these workspaces.
 *
 * \param  buttonMask         Bitmask composed from DIALOG_YES, DIALOG_NO etc.
 * \param  defaultButtonMask  Bitmask selecting one button to be default.
 * \param  buttonCaptions     SRAM array of Flash string pointers, workspace for button captions.
 * \param  buttonIds          SRAM array of IDs, workspace for button IDs.
 * \param  buttonWidget       SRAM workspace for the one ButtonRow widget object.
 *
 * \return  Number of buttons found in the bitmask.
 */
static FORMS_size_t DIALOG_SetupButtonRow(
		uint8_t buttonMask,
		uint8_t defaultButtonMask,
		char const CAL_PGM(* * buttonCaptions),
		WIDGETS_id_t * buttonIds,
		WIDGETS_ButtonRow_t * buttonWidget )
{
	// For all buttons included in the supplied mask, add caption and
	// id into the buttonCaption and buttonIDs arrays. Also locate the
	// default button's position in the arrays.
	FORMS_size_t buttonCount = 0;
	FORMS_size_t defaultButton = 0;
	if ((buttonMask & DIALOG_YES) != 0x00) {
		if ((defaultButtonMask & DIALOG_YES) != 0x00) {
			defaultButton = buttonCount;
		}
		buttonCaptions[buttonCount] = DIALOG_txtYes;
		buttonIds[buttonCount] = DIALOG_YES;
		++buttonCount;
	}
	if ((buttonMask & DIALOG_NO) != 0x00) {
		if ((defaultButtonMask & DIALOG_NO) != 0x00) {
			defaultButton = buttonCount;
		}
		buttonCaptions[buttonCount] = DIALOG_txtNo;
		buttonIds[buttonCount] = DIALOG_NO;
		++buttonCount;
	}
	if ((buttonMask & DIALOG_OK) != 0x00) {
		if ((defaultButtonMask & DIALOG_OK) != 0x00) {
			defaultButton = buttonCount;
		}
		buttonCaptions[buttonCount] = DIALOG_txtOK;
		buttonIds[buttonCount] = DIALOG_OK;
		++buttonCount;
	}
	if ((buttonMask & DIALOG_CANCEL) != 0x00) {
		if ((defaultButtonMask & DIALOG_CANCEL) != 0x00) {
			defaultButton = buttonCount;
		}
		buttonCaptions[buttonCount] = DIALOG_txtCancel;
		buttonIds[buttonCount] = DIALOG_CANCEL;
		++buttonCount;
	}

	// Set up button row if there are any buttons.
	if (buttonCount > 0) {
		WIDGETS_ButtonRow_Init( buttonWidget, buttonIds, defaultButton, buttonCount );
		WIDGETS_ButtonRow_SetOnClick( buttonWidget, DIALOG_OnClickInModal );
		WIDGETS_ButtonRow_SetStringList_F( buttonWidget, buttonCaptions );
	}
	
	return buttonCount;
}


/*!
 * This function takes care of everyting required to display and operate
 * a dialog box, represented as a prepared form object. If registers its own
 * joystick event handler and handles the dialog box until the DIALOG_OnClickInModal
 * callback is called by an element. E.g. a static text widget having this function
 * registered as its OnClick callback.
 *
 * This function restores the joystick event handler when it exits, but leaves the
 * form graphics untouched in the LCD.
 *
 * \param  form  The form to show and operate as a modal dialog box.
 *
 * \return  The custom ID provided by the element that called the OnClickInModal callback.
 */
WIDGETS_id_t DIALOG_RunModal( FORMS_form_t * form )
{
	// Save old state without being disturbed by interrupts.
	uint8_t savedSREG = SREG;
	CAL_disable_interrupt();
	JOYSTICK_EventHandler_t oldJoystickHandler = JOYSTICK_GetEventHandler();
	
	// Set new state and draw form.
	DIALOG_currentModalForm = form;
	FORMS_Draw( form );
	FIFO_Init( &DIALOG_eventFifo, DIALOG_eventFifoBuf, DIALOG_EVENTFIFOSIZE * sizeof(DIALOG_event_t) );
	JOYSTICK_SetEventHandler( DIALOG_JoystickHandler );
	
	// Enable interrupts and wait for modal form to exit.
	CAL_enable_interrupt();
	DIALOG_modalClicked = false;
	do {
		while (FIFO_HasData( &DIALOG_eventFifo, JOYSTICK_event_t ) == false) {
			POWER_EnterIdleSleepMode();
		}

		JOYSTICK_event_t event;
		FIFO_GetData( &DIALOG_eventFifo, &event );
		FORMS_OnJoystick( DIALOG_currentModalForm, &event );
	} while (DIALOG_modalClicked == false);

	// Wait for all joystick buttons to be released.	
	do {} while (JOYSTICK_GetState() != 0x00);
	
	// Normalize LCD hardware scrolling before releasing control to other parts.
	FORMS_NormalizeLCDScroll( form );
	
	// Restore old state without being disturbed by interrupts.
	CAL_disable_interrupt();
	JOYSTICK_SetEventHandler( oldJoystickHandler );
	SREG = savedSREG;
	
	// Then we can return the userId from one of the form elements.
	return DIALOG_modalId;
}


/*!
 * This function shows and operates a message box.
 *
 * \param  title              Pointer to Flash title string.
 * \param  text               Pointer to Flash message text string, could be multi-line.
 * \param  buttonMask         Bitmask composed from DIALOG_YES, DIALOG_NO etc.
 * \param  defaultButtonMask  Bitmask selecting one button to be default.
 * \param  useScrollbar       True if box should show a scrollbar.
 *
 * \return  The button row ID from the button clicked, e.g. DIALOG_YES.
 */
WIDGETS_id_t DIALOG_MessageBox_F(
		char const CAL_PGM(* title),
		char const CAL_PGM(* text),
		uint8_t buttonMask,
		uint8_t defaultButtonMask,
		bool useScrollbar )
{
	// Allocate stack space for GUI objects.
	FORMS_form_t boxForm;
	WIDGETS_StaticText_t titleWidget;
	WIDGETS_Separator_t firstSeparator;
	WIDGETS_StaticText_t textWidget;
	WIDGETS_Separator_t secondSeparator;
	WIDGETS_ButtonRow_t buttonWidget;
	char const CAL_PGM(* buttonCaptions[DIALOG_MAXBUTTONS]);
	WIDGETS_id_t buttonIds[DIALOG_MAXBUTTONS];
	
	// Set up objects, except button row.
	FORMS_Init( &boxForm, useScrollbar );
	WIDGETS_StaticText_Init( &titleWidget, 0, 1, true, false );
	FORMS_SetCaption_F( &(titleWidget.element), title );
	WIDGETS_Separator_Init( &firstSeparator );
	WIDGETS_StaticText_Init( &textWidget, 0, 1, true, false );
	FORMS_SetCaption_F( &(textWidget.element), text );
	WIDGETS_Separator_Init( &secondSeparator );

	// Set up button row.
	FORMS_size_t buttonCount = DIALOG_SetupButtonRow( buttonMask, defaultButtonMask, buttonCaptions, buttonIds, &buttonWidget );
	if (buttonCount == 0) { return 0; } // Catch unlikely error.

	// Add elements to form.
	FORMS_AddTopElement( &boxForm, &(textWidget.element) );
	FORMS_AddTopElement( &boxForm, &(firstSeparator.element) );
	FORMS_AddTopElement( &boxForm, &(titleWidget.element) );
	FORMS_AddBottomElement( &boxForm, &(secondSeparator.element) );
	FORMS_AddBottomElement( &boxForm, &(buttonWidget.element) );
	
	// Run modal form.
	return DIALOG_RunModal( &boxForm );
}


/*!
 * This function shows and operates a single choice box.
 *
 * \param  title              Pointer to Flash title string.
 * \param  selectionCount     Number of selections in list.
 * \param  defaultSelection   Index of default selection.
 * \param  selections         SRAM array of Flash string pointer containing selection texts.
 * \param  useScrollbar       True if box should show a scrollbar.
 * \param  workspace          SRAM array of uninitialized static text widgets, workspace.
 *
 * \return  The index of the choice selected.
 */
WIDGETS_id_t DIALOG_SingleChoiceBox_F(
		char const CAL_PGM(* title),
		FORMS_size_t selectionCount,
		FORMS_size_t defaultSelection,
		char const CAL_PGM(* * selections),
		bool useScrollbar,
		WIDGETS_StaticText_t * workspace )
{
	// We need some objects.
	FORMS_form_t boxForm;
	WIDGETS_StaticText_t titleWidget;
	WIDGETS_Separator_t separator;
	
	// Set up objects, except static texts.
	FORMS_Init( &boxForm, useScrollbar );
	WIDGETS_StaticText_Init( &titleWidget, 0, 1, true, false );
	FORMS_SetCaption_F( &(titleWidget.element), title );
	WIDGETS_Separator_Init( &separator );
	
	// Add elements to form, except static texts.
	FORMS_AddTopElement( &boxForm, &(titleWidget.element) );
	FORMS_AddBottomElement( &boxForm, &(separator.element) );
	
	// Set up and add static texts for choices.
	for (FORMS_size_t i = 0; i < selectionCount; ++i) {
		WIDGETS_StaticText_Init( workspace + i, i, 1, true, false );
		///mt FORMS_SetCaption_F( &(workspace[i].element), selections[i] );
		FORMS_SetCaption_F( &(workspace[i].element), CAL_pgm_read_pchar(&selections[i]) );
		WIDGETS_StaticText_SetOnClick( workspace + i, DIALOG_OnClickInModal );
		FORMS_AddBottomElement( &boxForm, &(workspace[i].element) );
	}
	
	// Set focus on default selection.
	FORMS_SetFocusedElement( &boxForm, &(workspace[defaultSelection].element) );
	
	// Run modal form.
	return DIALOG_RunModal( &boxForm );
}


/*!
 * This function shows and operates a radio list box.
 *
 * \param  title              Pointer to Flash title string.
 * \param  selectionCount     Number of selections in list.
 * \param  defaultSelection   Index of default selection.
 * \param  selections         SRAM array of Flash string pointer containing selection texts.
 * \param  userIds            SRAM array of IDs to assign to list elements.
 * \param  resultId           Pointer to variable in which to store the ID of the choice.
 * \param  workspace          SRAM array of uninitialized select ctrl widgets, workspace.
 * \param  data               SRAM array of uninitialized data used hold choice states (SELECTED/DESELECTED).
 * \param  buttonMask         Bitmask composed from DIALOG_YES, DIALOG_NO etc.
 * \param  defaultButtonMask  Bitmask selecting one button to be default.
 *
 * \return  The button row ID from the button clicked, e.g. DIALOG_YES.
 */
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
		uint8_t defaultButtonMask )
{
	// Allocate stack space for GUI objects.
	FORMS_form_t boxForm;
	WIDGETS_StaticText_t titleWidget;
	WIDGETS_Separator_t firstSeparator;
	WIDGETS_Separator_t secondSeparator;
	WIDGETS_ButtonRow_t buttonWidget;
	char const CAL_PGM(* buttonCaptions[DIALOG_MAXBUTTONS]);
	WIDGETS_id_t buttonIds[DIALOG_MAXBUTTONS];
	WIDGETS_ExclusionGroup_t radioGroup;
	
	// Set up objects, except radio ctrls and button row
	FORMS_Init( &boxForm, false );
	WIDGETS_StaticText_Init( &titleWidget, 0, 1, true, false );
	FORMS_SetCaption_F( &(titleWidget.element), title );
	WIDGETS_Separator_Init( &firstSeparator );
	WIDGETS_Separator_Init( &secondSeparator );
	
	// Set up button row.
	FORMS_size_t buttonCount = DIALOG_SetupButtonRow( buttonMask, defaultButtonMask, buttonCaptions, buttonIds, &buttonWidget );
	if (buttonCount == 0) { return 0; } // Catch unlikely error.
	
	// Add elements to form, except bottom separator and button row.
	FORMS_AddTopElement( &boxForm, &(titleWidget.element) );
	FORMS_AddBottomElement( &boxForm, &(firstSeparator.element) );
	
	// Set up and add radio ctrls for choices.
	WIDGETS_ExclusionGroup_Init( &radioGroup, workspace + defaultSelection );
	for (FORMS_size_t i = 0; i < selectionCount; ++i) {
		// Set correct data value.
		if (i == defaultSelection) {
			data[i] = WIDGETS_SELECTCTRL_SELECTED;
		} else {
			data[i] = WIDGETS_SELECTCTRL_DESELECTED;
		}

		// Init and add control.
		WIDGETS_SelectCtrl_InitRadio( workspace + i, userIds[i], data + i, &radioGroup );
		///mt FORMS_SetCaption_F( &(workspace[i].element), selections[i] );
		FORMS_SetCaption_F( &(workspace[i].element), CAL_pgm_read_pchar(&selections[i]) );
		FORMS_AddBottomElement( &boxForm, &(workspace[i].element) );
	}

	// Add bottom elements.	
	FORMS_AddBottomElement( &boxForm, &(secondSeparator.element) );
	FORMS_AddBottomElement( &boxForm, &(buttonWidget.element) );

	// Run modal form.
	WIDGETS_id_t buttonResult = DIALOG_RunModal( &boxForm );
	*resultId = WIDGETS_ExclusionGroup_GetId( &radioGroup );
	return buttonResult;
}


/*!
 * This function shows and operates a check list box.
 *
 * \param  title              Pointer to Flash title string.
 * \param  selectionCount     Number of selections in list.
 * \param  selections         SRAM array of Flash string pointer containing selection texts.
 * \param  workspace          SRAM array of uninitialized select ctrl widgets, workspace.
 * \param  data               SRAM array of data used to initialize and hold choice states (SELECTED/DESELECTED).
 * \param  buttonMask         Bitmask composed from DIALOG_YES, DIALOG_NO etc.
 * \param  defaultButtonMask  Bitmask selecting one button to be default.
 * \param  useScrollbar       True if box should show a scrollbar.
 *
 * \return  The button row ID from the button clicked, e.g. DIALOG_YES.
 */
WIDGETS_id_t DIALOG_CheckListBox_F(
		char const CAL_PGM(* title),
		FORMS_size_t selectionCount,
		char const CAL_PGM(* * selections),
		WIDGETS_SelectCtrl_t * workspace,
		WIDGETS_integer_t * data,
		uint8_t buttonMask,
		uint8_t defaultButtonMask,
		bool useScrollbar )
{
	// Allocate stack space for GUI objects.
	FORMS_form_t boxForm;
	WIDGETS_StaticText_t titleWidget;
	WIDGETS_Separator_t firstSeparator;
	WIDGETS_Separator_t secondSeparator;
	WIDGETS_ButtonRow_t buttonWidget;
	char const CAL_PGM(* buttonCaptions[DIALOG_MAXBUTTONS]);
	WIDGETS_id_t buttonIds[DIALOG_MAXBUTTONS];
	
	// Set up objects, except check ctrls and button row
	FORMS_Init( &boxForm, useScrollbar );
	WIDGETS_StaticText_Init( &titleWidget, 0, 1, true, false );
	FORMS_SetCaption_F( &(titleWidget.element), title );
	WIDGETS_Separator_Init( &firstSeparator );
	WIDGETS_Separator_Init( &secondSeparator );
	
	// Set up button row.
	FORMS_size_t buttonCount = DIALOG_SetupButtonRow( buttonMask, defaultButtonMask, buttonCaptions, buttonIds, &buttonWidget );
	if (buttonCount == 0) { return 0; } // Catch unlikely error.

	// Add elements to form, except bottom separator and button row.
	FORMS_AddTopElement( &boxForm, &(titleWidget.element) );
	FORMS_AddBottomElement( &boxForm, &(firstSeparator.element) );
	
	// Set up and add select ctrls for choices.
	for (FORMS_size_t i = 0; i < selectionCount; ++i) {
		WIDGETS_SelectCtrl_InitCheck( workspace + i, 0, data + i ); // No use for the userId, therefore 0.
		///mtFORMS_SetCaption_F( &(workspace[i].element), selections[i] );
		FORMS_SetCaption_F( &(workspace[i].element), CAL_pgm_read_pchar(&(selections[i])) );
		FORMS_AddBottomElement( &boxForm, &(workspace[i].element) );
	}

	// Add bottom elements.	
	FORMS_AddBottomElement( &boxForm, &(secondSeparator.element) );
	FORMS_AddBottomElement( &boxForm, &(buttonWidget.element) );

	// Run modal form.
	WIDGETS_id_t buttonResult = DIALOG_RunModal( &boxForm );
	return buttonResult;
}


// end of file
