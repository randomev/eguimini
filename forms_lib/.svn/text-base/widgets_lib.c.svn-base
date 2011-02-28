// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  GUI widgets library source file.
 *
 *      The widgets library provides functions to build complex graphical user
 *      interface controls, called widgets, using the basic form element
 *      structures from the Forms Library. The widgets in turn can be used to
 *      build complex interactive and appealing forms for your application.
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
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/forms_lib/widgets_lib.c $
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
#include <common.h>

#include "widgets_lib.h"
#include <lcd_lib.h>
#include <termfont_lib.h>
#include <rtc_driver.h>


/**********************************
 * Private configuration constants
 **********************************/

//! Repeated action tick delay common for all widgets (press-and-hold action).
#define WIDGETS_REPDELAY (RTC_TICKS_PER_SECOND / 16)
//! Number of spin steps before increasing step size.
#define WIDGETS_SPIN_CTRL_ACCELERATION_DELAY 8
//! Step size multiplication factor when accelerating.
#define WIDGETS_SPIN_CTRL_ACCELERATION_FACTOR 2
//! Width in pixels of select control icons.
#define WIDGETS_SELECTCTRL_ICONWIDTH  8
//! Left and right margin width in pixels for separators.
#define WIDGETS_SEPARATOR_SPACING 1
//! Separator line vertical positing within a 8-pixel page.
#define WIDGETS_SEPARATOR_POSITION 3



/*******************************
 * Internal function prototypes
 *******************************/

// -------------
// Spin Controls
// -------------

//! OnJoystick event handler for spin control objects.
static bool WIDGETS_SpinCtrl_OnJoystick ( FORMS_element_t * element, JOYSTICK_event_t const * event, FORMS_elementVisibility_t const * visibility );
//! OnKeyboard event handler for spin control objects.
static bool WIDGETS_SpinCtrl_OnKeyboard ( FORMS_element_t * element, char ch, FORMS_elementVisibility_t const * visibility );
//! OnGetFocus event handler for spin control objects.
static void WIDGETS_SpinCtrl_OnGetFocus ( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility );
//! OnLoseFocus event handler for spin control objects.
static void WIDGETS_SpinCtrl_OnLoseFocus( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility );
//! Draw/redraw request handler for spin control objects.
static void WIDGETS_SpinCtrl_DrawPage   ( FORMS_element_t const * element, FORMS_size_t internalPage, uint8_t lcdPage );

//! Internal function to draw spin arrows icon.
static void WIDGETS_SpinCtrl_SetArrows( WIDGETS_SpinCtrl_t const * widget, uint8_t lcdPage );
//! Intenrnal function to clear spin arrows icon.
static void WIDGETS_SpinCtrl_ClrArrows( WIDGETS_SpinCtrl_t const * widget, uint8_t lcdPage );
//! Internal function handling graphics and callbacks after status change.
static void WIDGETS_SpinCtrl_UpdateAfterChange( WIDGETS_SpinCtrl_t const * widget,
		FORMS_elementVisibility_t const * visibility );
//! Internal function to handle step size acceleration.
static void WIDGETS_SpinCtrl_Accelerate( WIDGETS_SpinCtrl_t * widget );
//! Internal function to spin up one stepsize.
static void WIDGETS_SpinCtrl_StepUp( WIDGETS_SpinCtrl_t * widget );
//! Internal function used as callback for repeated step-up action.
static void WIDGETS_SpinCtrl_StepUpCallback( void );
//! Internal function to spin down one stepsize.
static void WIDGETS_SpinCtrl_StepDown( WIDGETS_SpinCtrl_t * widget );
//! Internal function used as callback for repeated step-down action.
static void WIDGETS_SpinCtrl_StepDownCallback( void );


// -------------
// Text Controls
// -------------

//! OnJoystick event handler for text control objects.
static bool WIDGETS_TextCtrl_OnJoystick ( FORMS_element_t * element, JOYSTICK_event_t const * event, FORMS_elementVisibility_t const * visibility );
//! OnKeyboard event handler for text control objects.
static bool WIDGETS_TextCtrl_OnKeyboard ( FORMS_element_t * element, char ch, FORMS_elementVisibility_t const * visibility );
//! OnGetFocus event handler for text control objects.
static void WIDGETS_TextCtrl_OnGetFocus ( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility );
//! OnLoseFocus event handler for text control objects.
static void WIDGETS_TextCtrl_OnLoseFocus( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility );
//! Draw/redraw request handler for text control objects.
static void WIDGETS_TextCtrl_DrawPage   ( FORMS_element_t const * element, FORMS_size_t internalPage, uint8_t lcdPage );

//! Internal function to draw scroll arrows when data is focused.
static void WIDGETS_TextCtrl_DrawActiveArrows( WIDGETS_TextCtrl_t const * widget, uint8_t lcdPage );
//! Internal function to draw scroll arrows when data is not focused.
static void WIDGETS_TextCtrl_DrawPassiveArrows( WIDGETS_TextCtrl_t const * widget, uint8_t lcdPage );
//! Internal function to do necesasry operations and calls to draw the entire control.
static void WIDGETS_TextCtrl_UpdateGfx( WIDGETS_TextCtrl_t const * widget, uint8_t lcdPage );
//! Internal function used as callback for repeated step-up action.
static void WIDGETS_TextCtrl_StepUpCallback( void );
//! Internal function used as callback for repeated step-down action.
static void WIDGETS_TextCtrl_StepDownCallback( void );
//! Internal function to move cursor left, scroll if necessary.
static void WIDGETS_TextCtrl_MoveCursorLeft( WIDGETS_TextCtrl_t * widget, FORMS_elementVisibility_t const * visibility );
//! Internal function to move cursor right, scroll if necessary.
static void WIDGETS_TextCtrl_MoveCursorRight( WIDGETS_TextCtrl_t * widget, FORMS_elementVisibility_t const * visibility );
//! Internal function used as callback for repeated move-left action.
static void WIDGETS_TextCtrl_MoveLeftCallback( void );
//! Internal function used as callback for repeated move-right action.
static void WIDGETS_TextCtrl_MoveRightCallback( void );


// -----------
// Button Rows
// -----------

//! OnJoystick event handler for button row objects.
static bool WIDGETS_ButtonRow_OnJoystick ( FORMS_element_t * element, JOYSTICK_event_t const * event, FORMS_elementVisibility_t const * visibility );
//! OnKeyboard event handler for button row objects.
static bool WIDGETS_ButtonRow_OnKeyboard ( FORMS_element_t * element, char ch, FORMS_elementVisibility_t const * visibility );
//! OnGetFocus event handler for button row objects.
static void WIDGETS_ButtonRow_OnGetFocus ( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility );
//! OnLoseFocus event handler for button row objects.
static void WIDGETS_ButtonRow_OnLoseFocus( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility );
//! Draw/redraw request handler for button row objects.
static void WIDGETS_ButtonRow_DrawPage   ( FORMS_element_t const * element, FORMS_size_t internalPage, uint8_t lcdPage );

//! Internal function actually doing graphics updates for button row objects.
static void WIDGETS_ButtonRow_UpdateGfx( WIDGETS_ButtonRow_t const * widget, uint8_t lcdPage, bool drawText, bool drawBrackets );


// ------------
// Static Texts
// ------------

//! OnJoystick event handler for static text objects.
static bool WIDGETS_StaticText_OnJoystick ( FORMS_element_t * element, JOYSTICK_event_t const * event, FORMS_elementVisibility_t const * visibility );
//! OnKeyboard event handler for static text objects.
static bool WIDGETS_StaticText_OnKeyboard ( FORMS_element_t * element, char ch, FORMS_elementVisibility_t const * visibility );
//! OnGetFocus event handler for static text objects.
static void WIDGETS_StaticText_OnGetFocus ( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility );
//! OnLoseFocus event handler for static text objects.
static void WIDGETS_StaticText_OnLoseFocus( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility );
//! Draw/redraw request handler for static text objects.
static void WIDGETS_StaticText_DrawPage   ( FORMS_element_t const * element, FORMS_size_t internalPage, uint8_t lcdPage );


// ----------
// Separators
// ----------

//! OnJoystick event handler for separator objects.
static bool WIDGETS_Separator_OnJoystick ( FORMS_element_t * element, JOYSTICK_event_t const * event, FORMS_elementVisibility_t const * visibility );
//! OnKeyboard event handler for separator objects.
static bool WIDGETS_Separator_OnKeyboard ( FORMS_element_t * element, char ch, FORMS_elementVisibility_t const * visibility );
//! OnGetFocus event handler for separator objects.
static void WIDGETS_Separator_OnGetFocus ( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility );
//! OnLoseFocus event handler for separator objects.
static void WIDGETS_Separator_OnLoseFocus( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility );
//! Draw/redraw request handler for separator objects.
static void WIDGETS_Separator_DrawPage   ( FORMS_element_t const * element, FORMS_size_t internalPage, uint8_t lcdPage );


// ---------------
// Select Controls
// ---------------

//! OnJoystick event handler for select control objects.
static bool WIDGETS_SelectCtrl_OnJoystick ( FORMS_element_t * element, JOYSTICK_event_t const * event, FORMS_elementVisibility_t const * visibility );
//! OnKeyboard event handler for select control objects.
static bool WIDGETS_SelectCtrl_OnKeyboard ( FORMS_element_t * element, char ch, FORMS_elementVisibility_t const * visibility );
//! OnGetFocus event handler for select control objects.
static void WIDGETS_SelectCtrl_OnGetFocus ( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility );
//! OnLoseFocus event handler for select control objects.
static void WIDGETS_SelectCtrl_OnLoseFocus( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility );
//! Draw/redraw request handler for select control objects.
static void WIDGETS_SelectCtrl_DrawPage   ( FORMS_element_t const * element, FORMS_size_t internalPage, uint8_t lcdPage );

//! Internal function to draw the select icon.
static void WIDGETS_SelectCtrl_DrawIcon( WIDGETS_SelectCtrl_t const * widget, uint8_t lcdPage );
//! Internal function handling graphics and callbacks after status change.
static void WIDGETS_SelectCtrl_UpdateAfterChange( WIDGETS_SelectCtrl_t const * widget,
		FORMS_elementVisibility_t const * visibility );
//! Internal function handling all necessities for selecting the control.
static void WIDGETS_SelectCtrl_Select( WIDGETS_SelectCtrl_t * widget, FORMS_elementVisibility_t const * visibility );
//! Internal function handling all necessities for deselecting the control.
static void WIDGETS_SelectCtrl_Deselect( WIDGETS_SelectCtrl_t * widget, FORMS_elementVisibility_t const * visibility );



/*************************
 * Internal constant data
 *************************/

// -------------
// Spin Controls
// -------------

//! Up/down arrow icon for spin controls.
static uint8_t const CAL_PGM_DEF(WIDGETS_SpinCtrl_GFX_upDownArrow[TERMFONT_CHAR_WIDTH] ) = {
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,1,0,1,0,0 ),
	CREATE_BYTE( 0,0,1,1,0,1,1,0 ),
	CREATE_BYTE( 0,1,1,1,0,1,1,1 ),
	CREATE_BYTE( 0,0,1,1,0,1,1,0 ),
	CREATE_BYTE( 0,0,0,1,0,1,0,0 )
};

//! Event and request handlers required to operate spin control objects.
static FORMS_elementTraits_t const CAL_PGM_DEF(WIDGETS_SpinCtrl_Traits) = {
	WIDGETS_SpinCtrl_OnJoystick,
	WIDGETS_SpinCtrl_OnKeyboard,
	WIDGETS_SpinCtrl_OnGetFocus,
	WIDGETS_SpinCtrl_OnLoseFocus,
	WIDGETS_SpinCtrl_DrawPage
};


// -------------
// Text Controls
// -------------

//! Left open arrow icon for text control, indicating more-text-this-way.
static uint8_t const CAL_PGM_DEF(WIDGETS_TextCtrl_GFX_activeOpenLeftArrow[TERMFONT_CHAR_WIDTH]) = {
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,1,0,0,0 ),
	CREATE_BYTE( 0,0,0,1,1,1,0,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 )
};

//! Right open arrow icon for text control, indicating more-text-this-way.
static uint8_t const CAL_PGM_DEF(WIDGETS_TextCtrl_GFX_activeOpenRightArrow[TERMFONT_CHAR_WIDTH]) = {
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,0,1,1,1,0,0 ),
	CREATE_BYTE( 0,0,0,0,1,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 )
};

//! Left closed arrow icon for text control, indicating no-more-text-this-way.
static uint8_t const CAL_PGM_DEF(WIDGETS_TextCtrl_GFX_activeClosedLeftArrow[TERMFONT_CHAR_WIDTH]) = {
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,0,0,1,0,0,0 ),
	CREATE_BYTE( 0,0,0,1,1,1,0,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 )
};

//! Right closed arrow icon for text control, indicating no-more-text-this-way.
static uint8_t const CAL_PGM_DEF(WIDGETS_TextCtrl_GFX_activeClosedRightArrow[TERMFONT_CHAR_WIDTH]) = {
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,0,1,1,1,0,0 ),
	CREATE_BYTE( 0,0,0,0,1,0,0,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 )
};

//! Left passive open arrow icon for text control, used when text is not focused.
static uint8_t const CAL_PGM_DEF(WIDGETS_TextCtrl_GFX_passiveOpenLeftArrow[TERMFONT_CHAR_WIDTH]) = {
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,1,0,0,0 ),
	CREATE_BYTE( 0,0,0,1,0,1,0,0 ),
	CREATE_BYTE( 0,0,1,0,0,0,1,0 )
};

//! Right passive open arrow icon for text control, used when text is not focused.
static uint8_t const CAL_PGM_DEF(WIDGETS_TextCtrl_GFX_passiveOpenRightArrow[TERMFONT_CHAR_WIDTH]) = {
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,1,0,0,0,1,0 ),
	CREATE_BYTE( 0,0,0,1,0,1,0,0 ),
	CREATE_BYTE( 0,0,0,0,1,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 )
};

//! Left passive closed arrow icon for text control, used when text is not focused.
static uint8_t const CAL_PGM_DEF(WIDGETS_TextCtrl_GFX_passiveClosedLeftArrow[TERMFONT_CHAR_WIDTH]) = {
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,0,0,1,0,0,0 ),
	CREATE_BYTE( 0,0,0,1,0,1,0,0 ),
	CREATE_BYTE( 0,0,1,0,0,0,1,0 )
};

//! Right passive closed arrow icon for text control, used when text is not focused.
static uint8_t const CAL_PGM_DEF(WIDGETS_TextCtrl_GFX_passiveClosedRightArrow[TERMFONT_CHAR_WIDTH]) = {
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,1,0,0,0,1,0 ),
	CREATE_BYTE( 0,0,0,1,0,1,0,0 ),
	CREATE_BYTE( 0,0,0,0,1,0,0,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 )
};

//! Event and request handlers required to operate text control objects.
static FORMS_elementTraits_t const CAL_PGM_DEF(WIDGETS_TextCtrl_Traits) = {
	WIDGETS_TextCtrl_OnJoystick,
	WIDGETS_TextCtrl_OnKeyboard,
	WIDGETS_TextCtrl_OnGetFocus,
	WIDGETS_TextCtrl_OnLoseFocus,
	WIDGETS_TextCtrl_DrawPage
};


// -----------
// Button Rows
// -----------

//! Button start bracket icon for button row widgets.
static uint8_t const CAL_PGM_DEF(WIDGETS_ButtonRow_GFX_startBracket[TERMFONT_CHAR_WIDTH]) = {
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,1,1,1,0,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 )
};

//! Button end bracket icon for button row widgets.
static uint8_t const CAL_PGM_DEF(WIDGETS_ButtonRow_GFX_endBracket[TERMFONT_CHAR_WIDTH]) = {
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,0,1,1,1,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 )
};

//! Event and request handlers required to operate button row objects.
static FORMS_elementTraits_t const CAL_PGM_DEF(WIDGETS_ButtonRow_Traits) = {
	WIDGETS_ButtonRow_OnJoystick,
	WIDGETS_ButtonRow_OnKeyboard,
	WIDGETS_ButtonRow_OnGetFocus,
	WIDGETS_ButtonRow_OnLoseFocus,
	WIDGETS_ButtonRow_DrawPage
};


// ------------
// Static Texts
// ------------

//! Event and request handlers required to operate static text objects.
static FORMS_elementTraits_t const CAL_PGM_DEF(WIDGETS_StaticText_Traits) = {
	WIDGETS_StaticText_OnJoystick,
	WIDGETS_StaticText_OnKeyboard,
	WIDGETS_StaticText_OnGetFocus,
	WIDGETS_StaticText_OnLoseFocus,
	WIDGETS_StaticText_DrawPage
};


// ----------
// Separators
// ----------

//! Event and request handlers required to operate separator objects.
static FORMS_elementTraits_t const CAL_PGM_DEF(WIDGETS_Separator_Traits) = {
	WIDGETS_Separator_OnJoystick,
	WIDGETS_Separator_OnKeyboard,
	WIDGETS_Separator_OnGetFocus,
	WIDGETS_Separator_OnLoseFocus,
	WIDGETS_Separator_DrawPage
};


// ---------------
// Select Controls
// ---------------

//! Deselected radio button icon for select controls.
static uint8_t const CAL_PGM_DEF(WIDGETS_SelectCtrl_GFX_radioOff[WIDGETS_SELECTCTRL_ICONWIDTH]) = {
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,1,1,1,0,0 ),
	CREATE_BYTE( 0,0,1,0,0,0,1,0 ),
	CREATE_BYTE( 0,0,1,0,0,0,1,0 ),
	CREATE_BYTE( 0,0,1,0,0,0,1,0 ),
	CREATE_BYTE( 0,0,0,1,1,1,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 )
};

//! Selected radio button icon for select controls.
static uint8_t const CAL_PGM_DEF(WIDGETS_SelectCtrl_GFX_radioOn[WIDGETS_SELECTCTRL_ICONWIDTH]) = {
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,1,1,1,0,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,0,1,1,1,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 )
};

//! Deselected check box icon for select controls.
static uint8_t const CAL_PGM_DEF(WIDGETS_SelectCtrl_GFX_checkOff[WIDGETS_SELECTCTRL_ICONWIDTH]) = {
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,1,0,0,0,1,0 ),
	CREATE_BYTE( 0,0,1,0,0,0,1,0 ),
	CREATE_BYTE( 0,0,1,0,0,0,1,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 )
};

//! Selected check box icon for select controls.
static uint8_t const CAL_PGM_DEF(WIDGETS_SelectCtrl_GFX_checkOn[WIDGETS_SELECTCTRL_ICONWIDTH]) = {
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 )
};

//! Two-icon array gathering radio button icons for select controls.
//static uint8_t const __flash * __flash WIDGETS_SelectCtrl_radioIcons[2] = {
static uint8_t const CAL_PGM_DEF(* CAL_PGM(WIDGETS_SelectCtrl_radioIcons[2])) = {
	WIDGETS_SelectCtrl_GFX_radioOff,
	WIDGETS_SelectCtrl_GFX_radioOn
};

//! Two-icon array gathering check box icons for select controls.
static uint8_t const CAL_PGM_DEF(* CAL_PGM(WIDGETS_SelectCtrl_checkIcons[2])) = {
	WIDGETS_SelectCtrl_GFX_checkOff,
	WIDGETS_SelectCtrl_GFX_checkOn
};

//! Event and request handlers required to operate select control objects.
static FORMS_elementTraits_t const CAL_PGM_DEF(WIDGETS_SelectCtrl_Traits) = {
	WIDGETS_SelectCtrl_OnJoystick,
	WIDGETS_SelectCtrl_OnKeyboard,
	WIDGETS_SelectCtrl_OnGetFocus,
	WIDGETS_SelectCtrl_OnLoseFocus,
	WIDGETS_SelectCtrl_DrawPage
};



/***************************
 * Function implementations
 ***************************/

// -------------
// Spin Controls
// -------------

/*!
 * This function intializes a spin control object, including its contained
 * form element struct. To complete the initialization, the user must
 * assign a caption string using one of the SetCaption functions for form elements.
 * Also, for spin control with text lists, the user must
 * assign a string list to the object using one of the SetStringList
 * functions for spin control objects.
 *
 * \param  widget         Pointer to uninitialized widget.
 * \param  userId         Custom ID for this control.
 * \param  data           Pointer to widget's integer data.
 * \param  DrawData       Plugin for presenting the integer data.
 * \param  lowerLimit     Lower limit (inclusive) for the integer data.
 * \param  upperLimit     Upper limit (inclusive) for the integer data.
 * \param  stepSize       Minimum step size when stepping/spinning data.
 * \param  dataFieldSize  Size in characters for the representation of the data.
 */
void WIDGETS_SpinCtrl_Init( WIDGETS_SpinCtrl_t * widget,
		WIDGETS_id_t userId,
		WIDGETS_integer_t * data,
		WIDGETS_SpinCtrl_DrawDataFunc_t DrawData,
		WIDGETS_integer_t lowerLimit,
		WIDGETS_integer_t upperLimit,
		WIDGETS_integer_t stepSize,
		uint8_t dataFieldSize )
{
	// Initialize base element object.
	FORMS_InitElement( &(widget->element), 1, 0, true, (void *) widget, &WIDGETS_SpinCtrl_Traits );
	
	// Initialize spin control specifics.
	if (dataFieldSize == 0) { dataFieldSize = 1; } // Just to be sure.
	widget->userId = userId;
	widget->OnChange = NULL;
	widget->data = data;
	widget->DrawData = DrawData;
	widget->lowerLimit = lowerLimit;
	widget->upperLimit = upperLimit;
	widget->stepSize = stepSize;
	widget->accelerationCountdown = 0;
	widget->currentStepSize = stepSize;
	widget->dataFieldSize = dataFieldSize;
	widget->isDataFocused = false;
	widget->stringListLocation = WIDGETS_SRAM_STRINGS;
	widget->strings = NULL;
}


/*!
 * This function assigns an OnChange handler to a spin control object.
 * Providing a NULL pointer will remove the OnChange handler from the object.
 *
 * \param  widget    Pointer to initialized widget.
 * \param  OnChange  Function pointer for handler, or NULL to cancel.
 */
void WIDGETS_SpinCtrl_SetOnChange( WIDGETS_SpinCtrl_t * widget, WIDGETS_OnIntegerChangeFunc_t OnChange )
{
	widget->OnChange = OnChange;
}


/*!
 * This function assigns an SRAM array of SRAM strings to the widget.
 * Note that the strings nor the array is copied, so the text data needs to
 * be available and valid throughout the widget's lifetime.
 *
 * \param  widget   Pointer to initialized widget.
 * \param  strings  Pointer to SRAM array of SRAM strings.
 */
void WIDGETS_SpinCtrl_SetStringList( WIDGETS_SpinCtrl_t * widget, char const * * strings )
{
	widget->stringListLocation = WIDGETS_SRAM_STRINGS;
	widget->strings = strings;
}


/*!
 * This function assigns an SRAM array of Flash strings to the widget.
 * Note that the strings nor the array is copied, so the text data needs to
 * be available and valid throughout the widget's lifetime.
 *
 * \param  widget   Pointer to initialized widget.
 * \param  strings  Pointer to SRAM array of Flash strings.
 */
void WIDGETS_SpinCtrl_SetStringList_F( WIDGETS_SpinCtrl_t * widget, char const CAL_PGM(* * strings) )
{
	widget->stringListLocation = WIDGETS_FLASH_STRINGS;
	widget->strings_F = strings;
}


/*!
 * This function assigns a Flash array of Flash strings to the widget.
 * Note that the strings nor the array is copied, so the text data needs to
 * be available and valid throughout the widget's lifetime.
 *
 * \param  widget   Pointer to initialized widget.
 * \param  strings  Pointer to Flash array of Flash strings.
 */
///mt void WIDGETS_SpinCtrl_SetStringList_FF( WIDGETS_SpinCtrl_t * widget, char const __flash * __flash * strings )
void WIDGETS_SpinCtrl_SetStringList_FF( WIDGETS_SpinCtrl_t * widget, char const CAL_PGM(* CAL_PGM(* strings )))
{
	widget->stringListLocation = WIDGETS_FLASH_STRING_LIST;
	widget->strings_FF = strings;
}


/*!
 * This function is intended as a DrawData plugin for representing the integer
 * data as a decimal number.
 *
 * \param  widget   Pointer to spin control widget.
 * \param  lcdPage  Which 8-pixel LCD page to write to.
 */
void WIDGETS_SpinCtrl_DrawDec( WIDGETS_SpinCtrl_t const * widget, uint8_t lcdPage )
{
	uint8_t column = FORMS_LAST_COLUMN - TERMFONT_CHAR_WIDTH;
	uint8_t digitsLeft = widget->dataFieldSize;
	WIDGETS_integer_t number = *(widget->data);
	
	char digit = '0' + (number % 10);
	TERMFONT_DisplayChar( digit, lcdPage, column );
	column -= TERMFONT_CHAR_WIDTH;
	--digitsLeft;
	
	while ((number != 0) && (digitsLeft > 0)) {
		number /= 10;
		
		if (number != 0) {
			digit = '0' + (number % 10);
		} else {
			digit = ' ';
		}
		
		TERMFONT_DisplayChar( digit, lcdPage, column );
		column -= TERMFONT_CHAR_WIDTH;
		--digitsLeft;
	}
}


/*!
 * This function is intended as a DrawData plugin for representing the integer
 * data as a lower case hexadecimal number.
 *
 * \param  widget   Pointer to spin control widget.
 * \param  lcdPage  Which 8-pixel LCD page to write to.
 */
void WIDGETS_SpinCtrl_DrawLHex( WIDGETS_SpinCtrl_t const * widget, uint8_t lcdPage )
{
	uint8_t column = FORMS_LAST_COLUMN - TERMFONT_CHAR_WIDTH;
	uint8_t digitsLeft = widget->dataFieldSize;
	WIDGETS_integer_t number = *(widget->data);
	
	char digit = '0' + (number % 16);
	if (digit > '9') {
		digit = digit - '9' - 1 + 'a';
	}
	TERMFONT_DisplayChar( digit, lcdPage, column );
	column -= TERMFONT_CHAR_WIDTH;
	--digitsLeft;
	
	while ((number != 0) && (digitsLeft > 0)) {
		number /= 16;
		
		if (number != 0) {
			digit = '0' + (number % 16);
			if (digit > '9') {
				digit = digit - '9' - 1 + 'a';
			}
		} else {
			digit = ' ';
		}
		
		TERMFONT_DisplayChar( digit, lcdPage, column );
		column -= TERMFONT_CHAR_WIDTH;
		--digitsLeft;
	}
}


/*!
 * This function is intended as a DrawData plugin for representing the integer
 * data as a upper case hexadecimal number.
 *
 * \param  widget   Pointer to spin control widget.
 * \param  lcdPage  Which 8-pixel LCD page to write to.
 */
void WIDGETS_SpinCtrl_DrawUHex( WIDGETS_SpinCtrl_t const * widget, uint8_t lcdPage )
{
	uint8_t column = FORMS_LAST_COLUMN - TERMFONT_CHAR_WIDTH;
	uint8_t digitsLeft = widget->dataFieldSize;
	WIDGETS_integer_t number = *(widget->data);
	
	char digit = '0' + (number % 16);
	if (digit > '9') {
		digit = digit - '9' - 1 + 'A';
	}
	TERMFONT_DisplayChar( digit, lcdPage, column );
	column -= TERMFONT_CHAR_WIDTH;
	--digitsLeft;
	
	while ((number != 0) && (digitsLeft > 0)) {
		number /= 16;
		
		if (number != 0) {
			digit = '0' + (number % 16);
			if (digit > '9') {
				digit = digit - '9' - 1 + 'A';
			}
		} else {
			digit = ' ';
		}
		
		TERMFONT_DisplayChar( digit, lcdPage, column );
		column -= TERMFONT_CHAR_WIDTH;
		--digitsLeft;
	}
}


/*!
 * This function is intended as a DrawData plugin for representing the integer
 * data as text string indexed by the integer data. Note that a string list must
 * be assigned to the control in order for this to work.
 *
 * \param  widget   Pointer to spin control widget.
 * \param  lcdPage  Which 8-pixel LCD page to write to.
 */
void WIDGETS_SpinCtrl_DrawString( WIDGETS_SpinCtrl_t const * widget, uint8_t lcdPage )
{
	uint8_t column = FORMS_LAST_COLUMN - (TERMFONT_CHAR_WIDTH * widget->dataFieldSize);
	WIDGETS_integer_t index = *(widget->data);

	switch (widget->stringListLocation) {
		case WIDGETS_SRAM_STRINGS :
			TERMFONT_DisplayString( widget->strings[index], lcdPage, column );
		break;
		
		case WIDGETS_FLASH_STRINGS :
			TERMFONT_DisplayString_F( widget->strings_F[index], lcdPage, column );
		break;
		
		case WIDGETS_FLASH_STRING_LIST :
			///mt TERMFONT_DisplayString_F( widget->strings_FF[index], lcdPage, column );
			TERMFONT_DisplayString_F( CAL_pgm_read_pchar(&(widget->strings_FF[index])), lcdPage, column );
		break;
		
		default : break;
	}
}


// -------------
// Text Controls
// -------------

/*!
 * This function intializes a text control object, including its contained
 * form element struct. To complete the initialization, the user must
 * assign a caption string using one of the SetCaption functions for form elements.
 *
 * \param  widget         Pointer to uninitialized widget.
 * \param  userId         Custom ID for this control.
 * \param  data           Pointer to widget's text data.
 * \param  ChangeChar     Plugin for limiting the available character set.
 * \param  maxLength      Maximum number of characters in data buffer.
 * \param  dataFieldSize  Size in characters for the representation of the text.
 */
void WIDGETS_TextCtrl_Init( WIDGETS_TextCtrl_t * widget,
		WIDGETS_id_t userId,
		char * data,
		WIDGETS_TextCtrl_ChangeCharFunc_t ChangeChar,
		TERMFONT_stringSize_t maxLength,
		uint8_t dataFieldSize )
{
	// Initialize base element object.
	FORMS_InitElement( &(widget->element), 1, 0, true, (void *) widget, &WIDGETS_TextCtrl_Traits );
	
	// Initialize text control specifics.
	if (dataFieldSize == 0) { dataFieldSize = 1; } // Just to be sure.
	widget->userId = userId;
	widget->data = data;
	widget->ChangeChar = ChangeChar;
	widget->maxLength = maxLength;
	widget->dataFieldSize = dataFieldSize;
	widget->isDataFocused = false;
	widget->cursorPosition = 0;
	widget->scrollOffset = 0;
	widget->isCopying = false;
	widget->OnChange = NULL;
}


/*!
 * This function assigns an OnChange handler to a text control object.
 * Providing a NULL pointer will remove the OnChange handler from the object.
 *
 * \param  widget    Pointer to initialized widget.
 * \param  OnChange  Function pointer for handler, or NULL to cancel.
 */
void WIDGETS_TextCtrl_SetOnChange( WIDGETS_TextCtrl_t * widget, WIDGETS_OnTextChangeFunc_t OnChange )
{
	widget->OnChange = OnChange;
}


/*!
 * This function is intended as a ChangeChar plugin for limiting the available
 * character set to all printable ASCII characters.
 *
 * \param  widget    Pointer to text control widget.
 * \param  changeUp  True if plugin function should change to next character "upwards".
 */
void WIDGETS_TextCtrl_ChangeASCII( WIDGETS_TextCtrl_t const * widget, bool changeUp )
{
	///mt char * chPtr = widget->data + widget->cursorPosition;
	///comparison <127 always true for signed char
	///use a unsigned char to avoid warning
	uint8_t *chPtr = (uint8_t*)(widget->data + widget->cursorPosition);
	
	// Change up?
	if (changeUp) {
		if ((*chPtr >= 32) && (*chPtr < 127)) {
			++(*chPtr);
		} else {
			*chPtr = 32;
		}
	}
	// Change down, then.
	else {
		if ((*chPtr > 32) && (*chPtr <= 127)) {
			--(*chPtr);
		} else {
			*chPtr = 127;
		}
	}
}


/*!
 * This function is intended as a ChangeChar plugin for limiting the available
 * character set to decimal digits.
 *
 * \param  widget    Pointer to text control widget.
 * \param  changeUp  True if plugin function should change to next character "upwards".
 */
void WIDGETS_TextCtrl_ChangeDec( WIDGETS_TextCtrl_t const * widget, bool changeUp )
{
	char * chPtr = widget->data + widget->cursorPosition;
	
	// Change up?
	if (changeUp) {
		if (*chPtr < '9') {
			++(*chPtr);
		} else {
			*chPtr = '0';
		}
	}
	// Change down, then.
	else {
		if (*chPtr > '0') {
			--(*chPtr);
		} else {
			*chPtr = '9';
		}
	}
}


/*!
 * This function is intended as a ChangeChar plugin for limiting the available
 * character set to binary digits.
 *
 * \param  widget    Pointer to text control widget.
 * \param  changeUp  True if plugin function should change to next character "upwards".
 */
void WIDGETS_TextCtrl_ChangeBin( WIDGETS_TextCtrl_t const * widget, bool changeUp )
{
	char * chPtr = widget->data + widget->cursorPosition;
	
	if (*chPtr == '0') {
		*chPtr = '1';
	}
	else {
		*chPtr = '0';
	}
}


/*!
 * This function is intended as a ChangeChar plugin for limiting the available
 * character set to lower case hexadecimal digits.
 *
 * \param  widget    Pointer to text control widget.
 * \param  changeUp  True if plugin function should change to next character "upwards".
 */
void WIDGETS_TextCtrl_ChangeLHex( WIDGETS_TextCtrl_t const * widget, bool changeUp )
{
	char * chPtr = widget->data + widget->cursorPosition;
	
	// Change up?
	if (changeUp) {
		if (((*chPtr >= '0') && (*chPtr < '9')) ||
		    ((*chPtr >= 'a') && (*chPtr < 'f'))) {
			++(*chPtr);
		}
		else if (*chPtr == '9') {
			*chPtr = 'a';
		}
		else {
			*chPtr = '0';
		}
	}
	// Change down, then.
	else {
		if (((*chPtr > '0') && (*chPtr <= '9')) ||
		    ((*chPtr > 'a') && (*chPtr <= 'f'))) {
			--(*chPtr);
		}
		else if (*chPtr == 'a') {
			*chPtr = '9';
		}
		else {
			*chPtr = 'f';
		}
	}
}


/*!
 * This function is intended as a ChangeChar plugin for limiting the available
 * character set to upper case hexadecimal digits.
 *
 * \param  widget    Pointer to text control widget.
 * \param  changeUp  True if plugin function should change to next character "upwards".
 */
void WIDGETS_TextCtrl_ChangeUHex( WIDGETS_TextCtrl_t const * widget, bool changeUp )
{
	char * chPtr = widget->data + widget->cursorPosition;
	
	// Change up?
	if (changeUp) {
		if (((*chPtr >= '0') && (*chPtr < '9')) ||
		    ((*chPtr >= 'A') && (*chPtr < 'F'))) {
			++(*chPtr);
		}
		else if (*chPtr == '9') {
			*chPtr = 'A';
		}
		else {
			*chPtr = '0';
		}
	}
	// Change down, then.
	else {
		if (((*chPtr > '0') && (*chPtr <= '9')) ||
		    ((*chPtr > 'A') && (*chPtr <= 'F'))) {
			--(*chPtr);
		}
		else if (*chPtr == 'A') {
			*chPtr = '9';
		}
		else {
			*chPtr = 'F';
		}
	}
}


// -----------
// Button Rows
// -----------

/*!
 * This function intializes a button row object, including its contained
 * form element struct. To complete the initialization, the user must
 * assign a string list to the object using one of the SetStringList
 * functions for button row objects.
 *
 * \param  widget         Pointer to uninitialized widget.
 * \param  userIdList     Pointer to SRAM array of custom IDs to use for buttons.
 * \param  defaultButton  Default button index.
 * \param  buttonCount    Total number of buttons.
 */
void WIDGETS_ButtonRow_Init( WIDGETS_ButtonRow_t * widget,
		WIDGETS_id_t const * userIdList,
		FORMS_size_t defaultButton,
		FORMS_size_t buttonCount )
{
	// Initialize base element object.
	FORMS_InitElement( &(widget->element), 1, 0, true, (void *) widget, &WIDGETS_ButtonRow_Traits );
	
	// Initialize spin control specifics.
	widget->userIdList = userIdList;
	widget->currentButton = defaultButton;
	widget->buttonCount = buttonCount;
	widget->stringListLocation = WIDGETS_SRAM_STRINGS;
	widget->strings = NULL;
	widget->OnClick = NULL;
}


/*!
 * This function assigns an OnClick handler to a button row object.
 * Providing a NULL pointer will remove the OnClick handler from the object.
 *
 * \param  widget   Pointer to initialized widget.
 * \param  OnClick  Function pointer for handler, or NULL to cancel.
 */
void WIDGETS_ButtonRow_SetOnClick( WIDGETS_ButtonRow_t * widget, WIDGETS_OnClickFunc_t OnClick )
{
	widget->OnClick = OnClick;
}


/*!
 * This function assigns an SRAM array of SRAM strings to the widget.
 * Note that the strings nor the array is copied, so the text data needs to
 * be available and valid throughout the widget's lifetime.
 *
 * \param  widget   Pointer to initialized widget.
 * \param  strings  Pointer to SRAM array of SRAM strings.
 */
void WIDGETS_ButtonRow_SetStringList( WIDGETS_ButtonRow_t * widget, char const * * strings )
{
	widget->stringListLocation = WIDGETS_SRAM_STRINGS;
	widget->strings = strings;
}


/*!
 * This function assigns an SRAM array of Flash strings to the widget.
 * Note that the strings nor the array is copied, so the text data needs to
 * be available and valid throughout the widget's lifetime.
 *
 * \param  widget   Pointer to initialized widget.
 * \param  strings  Pointer to SRAM array of Flash strings.
 */
void WIDGETS_ButtonRow_SetStringList_F( WIDGETS_ButtonRow_t * widget, char const CAL_PGM(* * strings) )
{
	widget->stringListLocation = WIDGETS_FLASH_STRINGS;
	widget->strings_F = strings;
}


/*!
 * This function assigns a Flash array of Flash strings to the widget.
 * Note that the strings nor the array is copied, so the text data needs to
 * be available and valid throughout the widget's lifetime.
 *
 * \param  widget   Pointer to initialized widget.
 * \param  strings  Pointer to Flash array of Flash strings.
 */
void WIDGETS_ButtonRow_SetStringList_FF( WIDGETS_ButtonRow_t * widget, char const CAL_PGM(* CAL_PGM(* strings )))
{
	widget->stringListLocation = WIDGETS_FLASH_STRING_LIST;
	widget->strings_FF = strings;
}


// ------------
// Static Texts
// ------------

/*!
 * This function intializes a static text object, including its contained
 * form element struct. To complete the initialization, the user must
 * assign a caption string using one of the SetCaption functions for form elements.
 *
 * \param  widget        Pointer to uninitialized widget.
 * \param  userId        Custom ID for this control.
 * \param  height        Minimum height in characters.
 * \param  isActive      True if focus should stop on this control.
 * \param  inverseVideo  True if text should be rendered in inverse video.
 */
void WIDGETS_StaticText_Init( WIDGETS_StaticText_t * widget,
		WIDGETS_id_t userId,
		FORMS_size_t height,
		bool isActive,
		bool inverseVideo )
{
	// Initialize base element object.
	FORMS_InitElement( &(widget->element), height, 0, isActive, (void *) widget, &WIDGETS_StaticText_Traits );
	
	// Initialize static text specifics.
	widget->userId = userId;
	widget->OnClick = NULL;
	widget->inverseVideo = inverseVideo;
}


/*!
 * This function assigns an OnClick handler to a static text object.
 * Providing a NULL pointer will remove the OnClick handler from the object.
 *
 * \param  widget   Pointer to initialized widget.
 * \param  OnClick  Function pointer for handler, or NULL to cancel.
 */
void WIDGETS_StaticText_SetOnClick( WIDGETS_StaticText_t * widget, WIDGETS_OnClickFunc_t OnClick )
{
	widget->OnClick = OnClick;
}


// ----------
// Separators
// ----------

/*!
 * This function intializes a separator object, including its contained
 * form element struct.
 *
 * \param  widget      Pointer to uninitialized widget.
 */
void WIDGETS_Separator_Init( WIDGETS_Separator_t * widget )
{
	// Initialize base element object.
	FORMS_InitElement( &(widget->element), 1, 0, false, (void *) widget, &WIDGETS_Separator_Traits );
}


// ---------------
// Select Controls
// ---------------

/*!
 * This function initializes an exclusion group struct with an initial
 * select control. The user must make sure that the select control is actually
 * in a SELECTED state before operating a form or dialog box.
 *
 * \param  widget      Pointer to uninitialized widget.
 * \param  selectCtrl  Pointer to the select control that is selected in the group.
 */
void WIDGETS_ExclusionGroup_Init( WIDGETS_ExclusionGroup_t * widget, WIDGETS_SelectCtrl_t * selectCtrl )
{
	widget->currentSelected = selectCtrl;
}


/*!
 * This function returns the custom ID for the select control currently
 * selected in this group.
 *
 * \param  widget  Pointer to initialized widget.
 *
 * \return  Custom ID from the select control currently selected.
 */
WIDGETS_id_t WIDGETS_ExclusionGroup_GetId( WIDGETS_ExclusionGroup_t * widget )
{
	return widget->currentSelected->userId;
}


/*!
 * This function intializes a select control in check box mode, including its contained
 * form element struct. To complete the initialization, the user must
 * assign a caption string using one of the SetCaption functions for form elements.
 *
 * \param  widget  Pointer to uninitialized widget.
 * \param  userId  Custom ID for this control.
 * \param  data    Pointer to integer holding current state of control.
 */
void WIDGETS_SelectCtrl_InitCheck( WIDGETS_SelectCtrl_t * widget,
		WIDGETS_id_t userId,
		WIDGETS_integer_t * data )
{
	// Initialize base element object.
	FORMS_InitElement( &(widget->element), 1, 0, true, (void *) widget, &WIDGETS_SelectCtrl_Traits );
	
	// Initialize select control specifics.
	widget->userId = userId;
	widget->data = data;
	widget->iconList_FF = WIDGETS_SelectCtrl_checkIcons;
	widget->exclusionGroup = NULL;
	widget->OnChange = NULL; ///mt line added
}


/*!
 * This function intializes a select control in radio button mode, including its contained
 * form element struct. To complete the initialization, the user must
 * assign a caption string using one of the SetCaption functions for form elements.
 *
 * \param  widget          Pointer to uninitialized widget.
 * \param  userId          Custom ID for this control.
 * \param  data            Pointer to integer holding current state of control.
 * \param  exclusionGroup  Pointer to exclusion group to group within.
 */
void WIDGETS_SelectCtrl_InitRadio( WIDGETS_SelectCtrl_t * widget,
		WIDGETS_id_t userId,
		WIDGETS_integer_t * data,
		WIDGETS_ExclusionGroup_t * exclusionGroup )
{
	// Initialize base element object.
	FORMS_InitElement( &(widget->element), 1, 0, true, (void *) widget, &WIDGETS_SelectCtrl_Traits );
	
	// Initialize select control specifics.
	widget->userId = userId;
	widget->data = data;
	widget->iconList_FF = WIDGETS_SelectCtrl_radioIcons;
	widget->exclusionGroup = exclusionGroup;
}


/*!
 * This function assigns an OnChange handler to a select control object.
 * Providing a NULL pointer will remove the OnChange handler from the object.
 *
 * \param  widget    Pointer to initialized widget.
 * \param  OnChange  Function pointer for handler, or NULL to cancel.
 */
void WIDGETS_SelectCtrl_SetOnChange( WIDGETS_SelectCtrl_t * widget, WIDGETS_OnIntegerChangeFunc_t OnChange )
{
	widget->OnChange = OnChange;
}



/************************************
 * Internal function implementations
 ************************************/

// -------------
// Spin Controls
// -------------

/*!
 * This function handles joystick events for a spin control object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  event       Pointer to joystick event to process.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static bool WIDGETS_SpinCtrl_OnJoystick( FORMS_element_t * element,
		JOYSTICK_event_t const * event, FORMS_elementVisibility_t const * visibility )
{
	bool eventAccepted = false;
	
	// Get a pointer to spin control specific data.
	WIDGETS_SpinCtrl_t * widget = (WIDGETS_SpinCtrl_t *) (element->customData);

	// Left or enter deselects data field if not already deselected.
	if ((((event->clicked & JOYSTICK_LEFT) != 0x00) || ((event->released & JOYSTICK_ENTER) != 0x00)) &&
	     (widget->isDataFocused == true)) {
		if (visibility->firstPage == 0) {
			WIDGETS_SpinCtrl_ClrArrows( widget, visibility->firstLCDPage );
		}
		widget->isDataFocused = false;
		eventAccepted = true;
	}
	// Right or enter selects data field if not already selected.
	else if ((((event->clicked & JOYSTICK_RIGHT) != 0x00) || ((event->released & JOYSTICK_ENTER) != 0x00)) &&
	          (widget->isDataFocused == false)) {
		if (visibility->firstPage == 0) {
			WIDGETS_SpinCtrl_SetArrows( widget, visibility->firstLCDPage );
		}
		widget->isDataFocused = true;
		eventAccepted = true;
	}
	// Accept up/down if number is focused.
	else if (widget->isDataFocused == true) {
		// Stop repetitions if buttons released.
		if ((event->released & (JOYSTICK_UP | JOYSTICK_DOWN)) != 0x00) {
			FORMS_StopRepeating();
			widget->currentStepSize = widget->stepSize;
		}
		// Spin upwards if up clicked.
		else if ((event->clicked & JOYSTICK_UP) != 0x00) {
			WIDGETS_SpinCtrl_StepUp( widget );
			WIDGETS_SpinCtrl_UpdateAfterChange( widget, visibility );
		}
		// Spin downwards if down clicked.
		else if ((event->clicked & JOYSTICK_DOWN) != 0x00) {
			WIDGETS_SpinCtrl_StepDown( widget );
			WIDGETS_SpinCtrl_UpdateAfterChange( widget, visibility );
		}
		// Start repeating spin upwards if up held down.
		else if ((event->held & JOYSTICK_UP) != 0x00) {
			widget->accelerationCountdown = WIDGETS_SPIN_CTRL_ACCELERATION_DELAY;
			TIMING_AddRepCallbackEvent( TIMING_INFINITE_REPEAT,
										WIDGETS_REPDELAY,
										WIDGETS_SpinCtrl_StepUpCallback,
										FORMS_PrepareRepeatingElement( &(widget->element), visibility ) );
		}
		// Start repeating spin downwards if down held down.
		else if ((event->held & JOYSTICK_DOWN) != 0x00) {
			widget->accelerationCountdown = WIDGETS_SPIN_CTRL_ACCELERATION_DELAY;
			TIMING_AddRepCallbackEvent( TIMING_INFINITE_REPEAT,
										WIDGETS_REPDELAY,
										WIDGETS_SpinCtrl_StepDownCallback,
										FORMS_PrepareRepeatingElement( &(widget->element), visibility ) );
		}
		
		eventAccepted = true;
	}

	return eventAccepted;
}


/*!
 * This function handles keyboard events for a spin control object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  ch          Character code to process.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static bool WIDGETS_SpinCtrl_OnKeyboard( FORMS_element_t * element, char ch,
		FORMS_elementVisibility_t const * visibility )
{
	return false; // TODO handle keys.
}


/*!
 * This function handles GetFocus events for a spin control object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static void WIDGETS_SpinCtrl_OnGetFocus( FORMS_element_t * element,
		FORMS_elementVisibility_t const * visibility )
{
	// Only do graphics if visible.
	if (visibility->firstPage == 0) {
		// Get a pointer to spin control specific data.
		WIDGETS_SpinCtrl_t const * widget = (WIDGETS_SpinCtrl_t const *) (element->customData);
		
		// Print focus indicator.
		if (widget->isDataFocused) {
			WIDGETS_SpinCtrl_SetArrows( widget, visibility->firstLCDPage );
		}
	}
}


/*!
 * This function handles LoseFocus events for a spin control object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static void WIDGETS_SpinCtrl_OnLoseFocus( FORMS_element_t * element,
		FORMS_elementVisibility_t const * visibility )
{
	// Only do graphics if visible.
	if (visibility->firstPage == 0) {
		// Get a pointer to spin control specific data.
		WIDGETS_SpinCtrl_t const * widget = (WIDGETS_SpinCtrl_t const *) (element->customData);
		
		// Clear focus indicator.
		if (widget->isDataFocused) {
			WIDGETS_SpinCtrl_ClrArrows( widget, visibility->firstLCDPage );
		}
	}
}


/*!
 * This function handles draw/redraw requests for a spin control object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element       Pointer to form element part of the widget.
 * \param  internalPage  Which page of the widget to draw, in case of taller widgets.
 * \param  lcdPage       Which 8-pixel memory LCD page to write to.
 */
static void WIDGETS_SpinCtrl_DrawPage( FORMS_element_t const * element,
		FORMS_size_t internalPage, uint8_t lcdPage )
{
	// Get a pointer to spin control specific data.
	WIDGETS_SpinCtrl_t const * widget = (WIDGETS_SpinCtrl_t const *) (element->customData);
	
	// Draw element.
	if (internalPage == 0) { // Should never be anything else than page 0.
		// Draw caption first.
		if (widget->element.useFlashCaption) {
			TERMFONT_DisplayString_F( widget->element.caption_F, lcdPage, FORMS_FIRST_COLUMN );
		} else {
			TERMFONT_DisplayString( widget->element.caption, lcdPage, FORMS_FIRST_COLUMN );
		}
		
		// Draw digits.
		widget->DrawData( widget, lcdPage );
		
		// Print up/down arrow as focus indicator if focused, blank if not.
		if ((element->hasFocus == true) && (widget->isDataFocused == true)) {
			WIDGETS_SpinCtrl_SetArrows( widget, lcdPage );
		} else {
			WIDGETS_SpinCtrl_ClrArrows( widget, lcdPage );
		}
	}
}


/*!
 * This function draws the spin arrows icon on screen.
 *
 * \param  widget   Pointer to spin control widget.
 * \param  lcdPage  Which 8-pixel LCD page to write to.
 */
static void WIDGETS_SpinCtrl_SetArrows( WIDGETS_SpinCtrl_t const * widget, uint8_t lcdPage )
{
	uint8_t column = FORMS_LAST_COLUMN - ((widget->dataFieldSize + 1) * TERMFONT_CHAR_WIDTH);
	LCD_WritePage_F( WIDGETS_SpinCtrl_GFX_upDownArrow, lcdPage, column, TERMFONT_CHAR_WIDTH );
}


/*!
 * This function clears the spin arrows icon from screen.
 *
 * \param  widget   Pointer to spin control widget.
 * \param  lcdPage  Which 8-pixel LCD page to write to.
 */
static void WIDGETS_SpinCtrl_ClrArrows( WIDGETS_SpinCtrl_t const * widget, uint8_t lcdPage )
{
	uint8_t column = FORMS_LAST_COLUMN - ((widget->dataFieldSize + 1) * TERMFONT_CHAR_WIDTH);
	LCD_ClrPage( 0xff, lcdPage, column, TERMFONT_CHAR_WIDTH );
}


/*!
 * This function takes care of calling necessary graphics update functions
 * and event handler associated with this object after a status change.
 *
 * \param  widget      Pointer to initialized widget.
 * \param  visibility  Pointer to visiblitiy information for this object.
 */
static void WIDGETS_SpinCtrl_UpdateAfterChange( WIDGETS_SpinCtrl_t const * widget,
		FORMS_elementVisibility_t const * visibility )
{
	// Only do graphics if visible.
	if (visibility->firstPage == 0) {
		widget->DrawData( widget, visibility->firstLCDPage );
	}
	
	// Call handler if registered.
	if (widget->OnChange != NULL) {
		widget->OnChange( widget->userId, widget->data );
	}		
}


/*!
 * This function handles de delay countdown and incrementing of
 * the current step size when accelerating the spinning action.
 * This function is called every time the control spins one step.
 *
 * \param  widget  Pointer to initialized widget.
 */
static void WIDGETS_SpinCtrl_Accelerate( WIDGETS_SpinCtrl_t * widget )
{
	// Accelerate step size?
	if (widget->accelerationCountdown > 0) {
		--(widget->accelerationCountdown);
	} else {
		widget->accelerationCountdown = WIDGETS_SPIN_CTRL_ACCELERATION_DELAY;
		widget->currentStepSize *= WIDGETS_SPIN_CTRL_ACCELERATION_FACTOR;
	}	
}


/*!
 * This function steps up the data, making sure it does not go
 * beyond the upper limit.
 *
 * \param  widget  Pointer to initialized widget.
 */
static void WIDGETS_SpinCtrl_StepUp( WIDGETS_SpinCtrl_t * widget )
{
	if (*(widget->data) <= (widget->upperLimit - widget->currentStepSize)) {
		*(widget->data) += widget->currentStepSize;
	} else {
		*(widget->data) = widget->upperLimit;
	}
}


/*!
 * This function is used as callback from the timing library during
 * repeated stepping action. It takes care of accelerating step size,
 * stepping up the data, and updating everything afterwards.
 */
static void WIDGETS_SpinCtrl_StepUpCallback( void )
{
	WIDGETS_SpinCtrl_t * widget = (WIDGETS_SpinCtrl_t *) FORMS_GetRepeatingElement();
	FORMS_elementVisibility_t const * visibility = FORMS_GetRepeatingVisibility();

	WIDGETS_SpinCtrl_Accelerate( widget );	
	WIDGETS_SpinCtrl_StepUp( widget );
	WIDGETS_SpinCtrl_UpdateAfterChange( widget, visibility );
}


/*!
 * This function steps down the data, making sure it does not go
 * beyond the lower limit.
 *
 * \param  widget  Pointer to initialized widget.
 */
static void WIDGETS_SpinCtrl_StepDown( WIDGETS_SpinCtrl_t * widget )
{
	if (*(widget->data) >= (widget->lowerLimit + widget->currentStepSize)) {
		*(widget->data) -= widget->currentStepSize;
	} else {
		*(widget->data) = widget->lowerLimit;
	}
}


/*!
 * This function is used as callback from the timing library during
 * repeated stepping action. It takes care of accelerating step size,
 * stepping down the data, and updating everything afterwards.
 */
static void WIDGETS_SpinCtrl_StepDownCallback( void )
{
	WIDGETS_SpinCtrl_t * widget = (WIDGETS_SpinCtrl_t *) FORMS_GetRepeatingElement();
	FORMS_elementVisibility_t const * visibility = FORMS_GetRepeatingVisibility();

	WIDGETS_SpinCtrl_Accelerate( widget );	
	WIDGETS_SpinCtrl_StepDown( widget );
	WIDGETS_SpinCtrl_UpdateAfterChange( widget, visibility );
}


// -------------
// Text Controls
// -------------

/*!
 * This function handles joystick events for a text control object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  event       Pointer to joystick event to process.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static bool WIDGETS_TextCtrl_OnJoystick( FORMS_element_t * element, JOYSTICK_event_t const * event, FORMS_elementVisibility_t const * visibility )
{
	bool eventAccepted = false;
	
	// Get a pointer to select control specific data.
	WIDGETS_TextCtrl_t * widget = (WIDGETS_TextCtrl_t *) (element->customData);

	// Stop repetitions if buttons released.
	if ((event->released & (JOYSTICK_UP | JOYSTICK_DOWN | JOYSTICK_LEFT | JOYSTICK_RIGHT)) != 0x00) {
		FORMS_StopRepeating();
	}
		
	// Is data currently not focused?
	if (widget->isDataFocused == false) {
		// Right will give focus and reset cursor and scroll offset.
		if ((event->clicked & JOYSTICK_RIGHT) != 0x00) {
			widget->isDataFocused = true;
			widget->cursorPosition = 0;
			widget->scrollOffset = 0;
			eventAccepted = true;
		}
		// Enter will give focus to previous position.
		else if ((event->released & JOYSTICK_ENTER) != 0x00) {
			widget->isDataFocused = true;
			eventAccepted = true;
		}
	}
	// Is data currently focused?
	else {
		// Enter will remove data focus, or cancel copying if it was currently copying.
		if ((event->released & JOYSTICK_ENTER) != 0x00) {
			if (widget->isCopying == false) {
				widget->isDataFocused = false;
			} else {
				widget->isCopying = false;
			}
			eventAccepted = true;
		}
		// Left will move cursor left, or loose focus if at beginning.
		else if ((event->clicked & JOYSTICK_LEFT) != 0x00) {
			// Start text copy if ENTER down.
			if ((event->state & JOYSTICK_ENTER) != 0x00) {
				widget->isCopying = true;
			}

			WIDGETS_TextCtrl_MoveCursorLeft( widget, visibility );
			eventAccepted = true;
		}
		// Right will move cursor right, or stop at the end.
		else if ((event->clicked & JOYSTICK_RIGHT) != 0x00) {
			// Start text copy if ENTER down.
			if ((event->state & JOYSTICK_ENTER) != 0x00) {
				widget->isCopying = true;
			}

			WIDGETS_TextCtrl_MoveCursorRight( widget, visibility );
			eventAccepted = true;
		}
		// Up will change to next character, wrapping if necessary.
		else if ((event->clicked & JOYSTICK_UP) != 0x00) {
			widget->ChangeChar( widget, true );
			eventAccepted = true;
		}
		// Down will change to previous character, wrapping if necessary.
		else if ((event->clicked & JOYSTICK_DOWN) != 0x00) {
			widget->ChangeChar( widget, false );
			eventAccepted = true;
		}
		// If held up, start repeating change.
		else if ((event->held & JOYSTICK_UP) != 0x00) {
			TIMING_AddRepCallbackEvent( TIMING_INFINITE_REPEAT,
										WIDGETS_REPDELAY,
										WIDGETS_TextCtrl_StepUpCallback,
										FORMS_PrepareRepeatingElement( &(widget->element), visibility ) );
			eventAccepted = true;
		}
		// If held down, start repeating change.
		else if ((event->held & JOYSTICK_DOWN) != 0x00) {
			TIMING_AddRepCallbackEvent( TIMING_INFINITE_REPEAT,
										WIDGETS_REPDELAY,
										WIDGETS_TextCtrl_StepDownCallback,
										FORMS_PrepareRepeatingElement( &(widget->element), visibility ) );
			eventAccepted = true;
		}
		// If held left, start repeating move.
		else if ((event->held & JOYSTICK_LEFT) != 0x00) {
			TIMING_AddRepCallbackEvent( TIMING_INFINITE_REPEAT,
										WIDGETS_REPDELAY,
										WIDGETS_TextCtrl_MoveLeftCallback,
										FORMS_PrepareRepeatingElement( &(widget->element), visibility ) );
			eventAccepted = true;
		}
		// If held right, start repeating move.
		else if ((event->held & JOYSTICK_RIGHT) != 0x00) {
			TIMING_AddRepCallbackEvent( TIMING_INFINITE_REPEAT,
										WIDGETS_REPDELAY,
										WIDGETS_TextCtrl_MoveRightCallback,
										FORMS_PrepareRepeatingElement( &(widget->element), visibility ) );
			eventAccepted = true;
		}
	} // end: if (widget->isDataFocused == false) ... else ...

	if (eventAccepted) {
		WIDGETS_TextCtrl_UpdateGfx( widget, visibility->firstLCDPage );
	}

	return eventAccepted;
}


/*!
 * This function handles keyboard events for a text control object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  ch          Character code to process.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static bool WIDGETS_TextCtrl_OnKeyboard( FORMS_element_t * element, char ch, FORMS_elementVisibility_t const * visibility )
{
	return false; // TODO handle keys.
}


/*!
 * This function handles GetFocus events for a text control object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static void WIDGETS_TextCtrl_OnGetFocus( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility )
{
	// Only do graphics if visible.
	if (visibility->firstPage == 0) {
		// Get a pointer to text control specific data.
		WIDGETS_TextCtrl_t const * widget = (WIDGETS_TextCtrl_t const *) (element->customData);

		WIDGETS_TextCtrl_UpdateGfx( widget, visibility->firstLCDPage );
	}
}


/*!
 * This function handles LoseFocus events for a text control object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static void WIDGETS_TextCtrl_OnLoseFocus( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility )
{
	// Only do graphics if visible.
	if (visibility->firstPage == 0) {
		// Get a pointer to text control specific data.
		WIDGETS_TextCtrl_t const * widget = (WIDGETS_TextCtrl_t const *) (element->customData);

		WIDGETS_TextCtrl_UpdateGfx( widget, visibility->firstLCDPage );
	}
}


/*!
 * This function handles draw/redraw requests for a text control object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element       Pointer to form element part of the widget.
 * \param  internalPage  Which page of the widget to draw, in case of taller widgets.
 * \param  lcdPage       Which 8-pixel memory LCD page to write to.
 */
static void WIDGETS_TextCtrl_DrawPage( FORMS_element_t const * element,
		FORMS_size_t internalPage, uint8_t lcdPage )
{
	// Get a pointer to text control specific data.
	WIDGETS_TextCtrl_t const * widget = (WIDGETS_TextCtrl_t const *) (element->customData);
	
	// Draw element.
	if (internalPage == 0) { // Should never be anything else than page 0.
		WIDGETS_TextCtrl_UpdateGfx( widget, lcdPage );
	}
}


/*!
 * This function draws arrow icons for a control where the text is focused.
 *
 * \param  widget   Pointer to spin control widget.
 * \param  lcdPage  Which 8-pixel LCD page to write to.
 */
static void WIDGETS_TextCtrl_DrawActiveArrows( WIDGETS_TextCtrl_t const * widget, uint8_t lcdPage )
{
	// Start arrow.
	uint8_t startArrowColumn = FORMS_LAST_COLUMN - (TERMFONT_CHAR_WIDTH * (widget->dataFieldSize + 2));
	if (widget->scrollOffset > 0) {
		LCD_WritePage_F( WIDGETS_TextCtrl_GFX_activeOpenLeftArrow, lcdPage, startArrowColumn, TERMFONT_CHAR_WIDTH );
	} else {
		LCD_WritePage_F( WIDGETS_TextCtrl_GFX_activeClosedLeftArrow, lcdPage, startArrowColumn, TERMFONT_CHAR_WIDTH );
	}

	// End arrow.
	uint8_t endArrowColumn = FORMS_LAST_COLUMN - TERMFONT_CHAR_WIDTH;
	if (widget->scrollOffset < (widget->maxLength - widget->dataFieldSize)) {
		LCD_WritePage_F( WIDGETS_TextCtrl_GFX_activeOpenRightArrow, lcdPage, endArrowColumn, TERMFONT_CHAR_WIDTH );
	} else {
		LCD_WritePage_F( WIDGETS_TextCtrl_GFX_activeClosedRightArrow, lcdPage, endArrowColumn, TERMFONT_CHAR_WIDTH );
	}
}


/*!
 * This function draws arrow icons for a control where the text is not focused.
 *
 * \param  widget   Pointer to spin control widget.
 * \param  lcdPage  Which 8-pixel LCD page to write to.
 */
static void WIDGETS_TextCtrl_DrawPassiveArrows( WIDGETS_TextCtrl_t const * widget, uint8_t lcdPage )
{
	// Start arrow.
	uint8_t startArrowColumn = FORMS_LAST_COLUMN - (TERMFONT_CHAR_WIDTH * (widget->dataFieldSize + 2));
	if (widget->scrollOffset > 0) {
		LCD_WritePage_F( WIDGETS_TextCtrl_GFX_passiveOpenLeftArrow, lcdPage, startArrowColumn, TERMFONT_CHAR_WIDTH );
	} else {
		LCD_WritePage_F( WIDGETS_TextCtrl_GFX_passiveClosedLeftArrow, lcdPage, startArrowColumn, TERMFONT_CHAR_WIDTH );
	}

	// End arrow.
	uint8_t endArrowColumn = FORMS_LAST_COLUMN - TERMFONT_CHAR_WIDTH;
	if (widget->scrollOffset < (widget->maxLength - widget->dataFieldSize)) {
		LCD_WritePage_F( WIDGETS_TextCtrl_GFX_passiveOpenRightArrow, lcdPage, endArrowColumn, TERMFONT_CHAR_WIDTH );
	} else {
		LCD_WritePage_F( WIDGETS_TextCtrl_GFX_passiveClosedRightArrow, lcdPage, endArrowColumn, TERMFONT_CHAR_WIDTH );
	}
}


/*!
 * This function draws all elements of the widget, calling
 * necessary helper functions on the way.
 *
 * \param  widget   Pointer to spin control widget.
 * \param  lcdPage  Which 8-pixel LCD page to write to.
 */
static void WIDGETS_TextCtrl_UpdateGfx( WIDGETS_TextCtrl_t const * widget, uint8_t lcdPage )
{
	// Draw caption first.
	if (widget->element.useFlashCaption) {
		TERMFONT_DisplayString_F( widget->element.caption_F, lcdPage, FORMS_FIRST_COLUMN );
	} else {
		TERMFONT_DisplayString( widget->element.caption, lcdPage, FORMS_FIRST_COLUMN );
	}
		
	// Draw text, right adjust, make room for the end bracket and text.
	uint8_t column = FORMS_LAST_COLUMN - (TERMFONT_CHAR_WIDTH * (widget->dataFieldSize + 1));
	uint8_t charsLeft = widget->dataFieldSize;
	TERMFONT_stringSize_t currentPosition = widget->scrollOffset;
	while ((widget->data[currentPosition] != 0) && (charsLeft > 0)) {
		TERMFONT_DisplayChar( widget->data[currentPosition], lcdPage, column );
		if ((widget->isDataFocused == true) && (currentPosition == widget->cursorPosition)) {
			LCD_SetPage( (1<<7), lcdPage, column, TERMFONT_CHAR_WIDTH );
		}
		++currentPosition;
		--charsLeft;
		column += TERMFONT_CHAR_WIDTH;
	}
		
	// Print brackets as focus indicator if focused, blank if not.
	if (widget->isDataFocused) {
		WIDGETS_TextCtrl_DrawActiveArrows( widget, lcdPage );
	} else {
		WIDGETS_TextCtrl_DrawPassiveArrows( widget, lcdPage );
	}
}


/*!
 * This function is used as callback from the timing library during
 * repeated stepping action. It takes care changing the character at
 * the cursor position and updating graphics afterwards.
 */
static void WIDGETS_TextCtrl_StepUpCallback( void )
{
	WIDGETS_TextCtrl_t * widget = (WIDGETS_TextCtrl_t *) (FORMS_GetRepeatingElement()->customData);
	FORMS_elementVisibility_t const * visibility = FORMS_GetRepeatingVisibility();

	widget->ChangeChar( widget, true );

	// Only do graphics if visible.
	if (visibility->firstPage == 0) {
		WIDGETS_TextCtrl_UpdateGfx( widget, visibility->firstLCDPage );
	}
}


/*!
 * This function is used as callback from the timing library during
 * repeated stepping action. It takes care changing the character at
 * the cursor position and updating graphics afterwards.
 */
static void WIDGETS_TextCtrl_StepDownCallback( void )
{
	WIDGETS_TextCtrl_t * widget = (WIDGETS_TextCtrl_t *) (FORMS_GetRepeatingElement()->customData);
	FORMS_elementVisibility_t const * visibility = FORMS_GetRepeatingVisibility();

	widget->ChangeChar( widget, false );

	// Only do graphics if visible.
	if (visibility->firstPage == 0) {
		WIDGETS_TextCtrl_UpdateGfx( widget, visibility->firstLCDPage );
	}
}


/*!
 * This function moves cursor left, scrolling if necessary, and copying
 * text if widget is currently in copy mode. It also takes care of updating graphics.
 *
 * \param  widget      Pointer to initialized widget.
 * \param  visibility  Pointer to visiblitiy information for this object.
 */
static void WIDGETS_TextCtrl_MoveCursorLeft( WIDGETS_TextCtrl_t * widget, FORMS_elementVisibility_t const * visibility )
{
	// Move to left if not at beginning, scroll if necessary.
	if (widget->cursorPosition > 0) {
		// Move cursor.
		--(widget->cursorPosition);
		if (widget->cursorPosition < widget->scrollOffset) {
			widget->scrollOffset = widget->cursorPosition;
		}
		
		// Copy data if required.
		if (widget->isCopying) {
			widget->data[widget->cursorPosition] = widget->data[widget->cursorPosition + 1];
		}
	}
	// Loose focus if at beginning and not copying.
	else {
		if (widget->isCopying == false) {
			widget->isDataFocused = false;
		}
	}

	// Only do graphics if visible.
	if (visibility->firstPage == 0) {
		WIDGETS_TextCtrl_UpdateGfx( widget, visibility->firstLCDPage );
	}
}


/*!
 * This function moves cursor right, scrolling if necessary, and copying
 * text if widget is currently in copy mode. It also takes care of updating graphics.
 *
 * \param  widget      Pointer to initialized widget.
 * \param  visibility  Pointer to visiblitiy information for this object.
 */
static void WIDGETS_TextCtrl_MoveCursorRight( WIDGETS_TextCtrl_t * widget, FORMS_elementVisibility_t const * visibility )
{
	// Move to right if not at end, scroll if necessary.
	if (widget->cursorPosition < (widget->maxLength - 1)) {
		// Move cursor.
		++(widget->cursorPosition);
		if (widget->cursorPosition > (widget->scrollOffset + widget->dataFieldSize - 1)) {
			widget->scrollOffset = widget->cursorPosition - widget->dataFieldSize + 1;
		}

		// Copy data if required.
		if (widget->isCopying) {
			widget->data[widget->cursorPosition] = widget->data[widget->cursorPosition - 1];
		}
	}

	// Only do graphics if visible.
	if (visibility->firstPage == 0) {
		WIDGETS_TextCtrl_UpdateGfx( widget, visibility->firstLCDPage );
	}
}


/*!
 * This function is used as callback from the timing library during
 * repeated moving action. It just calls the move left function.
 */
static void WIDGETS_TextCtrl_MoveLeftCallback( void )
{
	WIDGETS_TextCtrl_t * widget = (WIDGETS_TextCtrl_t *) (FORMS_GetRepeatingElement()->customData);
	FORMS_elementVisibility_t const * visibility = FORMS_GetRepeatingVisibility();

	WIDGETS_TextCtrl_MoveCursorLeft( widget, visibility );
}


/*!
 * This function is used as callback from the timing library during
 * repeated moving action. It just calls the move right function.
 */
static void WIDGETS_TextCtrl_MoveRightCallback( void )
{
	WIDGETS_TextCtrl_t * widget = (WIDGETS_TextCtrl_t *) (FORMS_GetRepeatingElement()->customData);
	FORMS_elementVisibility_t const * visibility = FORMS_GetRepeatingVisibility();

	WIDGETS_TextCtrl_MoveCursorRight( widget, visibility );
}


// -----------
// Button Rows
// -----------

/*!
 * This function handles joystick events for a button row object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  event       Pointer to joystick event to process.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static bool WIDGETS_ButtonRow_OnJoystick( FORMS_element_t * element, JOYSTICK_event_t const * event, FORMS_elementVisibility_t const * visibility )
{
	bool eventAccepted = false;
	
	// Get a pointer to select control specific data.
	WIDGETS_ButtonRow_t * widget = (WIDGETS_ButtonRow_t *) (element->customData);

	// Check for left movement.
	if ((event->clicked & JOYSTICK_LEFT) != 0x00) {
		// Only move left if not already at left edge.
		if (widget->currentButton > 0) {
			--(widget->currentButton);
			WIDGETS_ButtonRow_UpdateGfx( widget, visibility->firstLCDPage, false, true );
			eventAccepted = true;
		}
	}
	// Check for right movement.
	else if ((event->clicked & JOYSTICK_RIGHT) != 0x00) {
		// Only move right if not already at right edge.
		if (widget->currentButton < (widget->buttonCount - 1)) {
			++(widget->currentButton);
			WIDGETS_ButtonRow_UpdateGfx( widget, visibility->firstLCDPage, false, true );
			eventAccepted = true;
		}
	}
	// Enter calls OnClick if set.
	else if (((event->clicked & JOYSTICK_ENTER) != 0x00) && (widget->OnClick != NULL)) {
		widget->OnClick( widget->userIdList[widget->currentButton] );
	}

	return eventAccepted;
}


/*!
 * This function handles keyboard events for a button row object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  ch          Character code to process.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static bool WIDGETS_ButtonRow_OnKeyboard( FORMS_element_t * element, char ch, FORMS_elementVisibility_t const * visibility )
{
	return false; // TODO handle keys.
}


/*!
 * This function handles GetFocus events for a button row object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static void WIDGETS_ButtonRow_OnGetFocus( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility )
{
	// Only do graphics if visible.
	if (visibility->firstPage == 0) {
		// Get a pointer to static text specific data.
		WIDGETS_ButtonRow_t const * widget = (WIDGETS_ButtonRow_t const *) (element->customData);
		
		// Draw brackets.
		WIDGETS_ButtonRow_UpdateGfx( widget, visibility->firstLCDPage, false, true );
	}
}


/*!
 * This function handles LoseFocus events for a button row object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static void WIDGETS_ButtonRow_OnLoseFocus( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility )
{
	// Only do graphics if visible.
	if (visibility->firstPage == 0) {
		// Get a pointer to static text specific data.
		WIDGETS_ButtonRow_t const * widget = (WIDGETS_ButtonRow_t const *) (element->customData);
		
		// Erase brackets.
		WIDGETS_ButtonRow_UpdateGfx( widget, visibility->firstLCDPage, false, false );
	}
}


/*!
 * This function handles draw/redraw requests for a button row object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element       Pointer to form element part of the widget.
 * \param  internalPage  Which page of the widget to draw, in case of taller widgets.
 * \param  lcdPage       Which 8-pixel memory LCD page to write to.
 */
static void WIDGETS_ButtonRow_DrawPage( FORMS_element_t const * element, FORMS_size_t internalPage, uint8_t lcdPage )
{
	// Get a pointer to static text specific data.
	WIDGETS_ButtonRow_t const * widget = (WIDGETS_ButtonRow_t const *) (element->customData);
	
	// Draw element.
	if (internalPage == 0) { // Should never be anything else than page 0.
		WIDGETS_ButtonRow_UpdateGfx( widget, lcdPage, true, element->hasFocus );
	}
}


/*!
 * This function draws the entire button row object on the LCD.
 *
 * \param  widget        Pointer to initialized widget.
 * \param  lcdPage       Which 8-pixel memory LCD page to write to.
 * \param  drawText      True if text should be drawn/updated.
 * \param  drawBrackets  True if button brackets should be drawn/updated.
 */
static void WIDGETS_ButtonRow_UpdateGfx( WIDGETS_ButtonRow_t const * widget, uint8_t lcdPage, bool drawText, bool drawBrackets )
{
	// Init current LCD column.
	uint8_t column = FORMS_FIRST_COLUMN;
	
	// Draw each button.
	for (FORMS_size_t i = 0; i < widget->buttonCount; ++i ) {
		// Draw start bracket for selected button.
		if ((i == widget->currentButton) && (drawBrackets == true)) {
			LCD_WritePage_F( WIDGETS_ButtonRow_GFX_startBracket, lcdPage, column, TERMFONT_CHAR_WIDTH );
		}
		// Or draw end bracket if this is the next after the selected button.
		else if ((i == (widget->currentButton + 1)) && (drawBrackets == true)) {
			LCD_WritePage_F( WIDGETS_ButtonRow_GFX_endBracket, lcdPage, column, TERMFONT_CHAR_WIDTH );
		}
		// Or clear spacer.
		else {
			LCD_ClrPage( 0xff, lcdPage, column, TERMFONT_CHAR_WIDTH );
		}
		column += TERMFONT_CHAR_WIDTH;
		
		// Draw button caption.
		switch (widget->stringListLocation) {
			case WIDGETS_SRAM_STRINGS :
				if (drawText == true) {
					TERMFONT_DisplayString( widget->strings[i], lcdPage, column );
				}
				column += TERMFONT_GetStringWidth( widget->strings[i] );
			break;
		
			case WIDGETS_FLASH_STRINGS :
				if (drawText == true) {
					TERMFONT_DisplayString_F( widget->strings_F[i], lcdPage, column );
				}
				column += TERMFONT_GetStringWidth_F( widget->strings_F[i] );
			break;
		
			case WIDGETS_FLASH_STRING_LIST :
				{
				char CAL_PGM(*pstrmt);
				pstrmt = CAL_pgm_read_pchar(&(widget->strings_FF[i]));
				if (drawText == true) {
					///mt TERMFONT_DisplayString_F( widget->strings_FF[i], lcdPage, column );
					TERMFONT_DisplayString_F( pstrmt, lcdPage, column );
				
				}
				///mt column += TERMFONT_GetStringWidth_F( widget->strings_FF[i] );
				column += TERMFONT_GetStringWidth_F( pstrmt );
				}
			break;
			
			default : break;
		}			
	} // end: for (...)
	
	// Draw end bracket after last button if it is selected.
	if ((widget->currentButton == (widget->buttonCount - 1)) && (drawBrackets == true)) {
		LCD_WritePage_F( WIDGETS_ButtonRow_GFX_endBracket, lcdPage, column, TERMFONT_CHAR_WIDTH );
	} 
	// Draw spacer after last button if it is not selected.
	else {
		LCD_ClrPage( 0xff, lcdPage, column, TERMFONT_CHAR_WIDTH );
	}				
}


// ------------
// Static Texts
// ------------

/*!
 * This function handles joystick events for a static text object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  event       Pointer to joystick event to process.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static bool WIDGETS_StaticText_OnJoystick( FORMS_element_t * element, JOYSTICK_event_t const * event, FORMS_elementVisibility_t const * visibility )
{
	bool eventAccepted = false;

	// Get a pointer to select control specific data.
	WIDGETS_StaticText_t * widget = (WIDGETS_StaticText_t *) (element->customData);

	// Up moves internal focus up of not at top.
	if ((element->focusedPage > 0) && ((event->clicked & JOYSTICK_UP) != 0x00)) {
		--(element->focusedPage);
		FORMS_EnsureFocusedIsVisible( visibility->form );
		eventAccepted = true;
	}
	// Down moves internal focus down of not at bottom.
	else if ((element->focusedPage < (element->height - 1)) && ((event->clicked & JOYSTICK_DOWN) != 0x00)) {
		++(element->focusedPage);
		FORMS_EnsureFocusedIsVisible( visibility->form );
		eventAccepted = true;
	}
	// Enter calls OnClick if set.
	else if (((event->clicked & JOYSTICK_ENTER) != 0x00) && (widget->OnClick != NULL)) {
		widget->OnClick( widget->userId );
	}
	
	return eventAccepted;
}


/*!
 * This function handles keyboard events for a static text object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  ch          Character code to process.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static bool WIDGETS_StaticText_OnKeyboard( FORMS_element_t * element, char ch, FORMS_elementVisibility_t const * visibility )
{
	return false; // TODO handle keys.
}


/*!
 * This function handles GetFocus events for a static text object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static void WIDGETS_StaticText_OnGetFocus( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility )
{
	return; // Nothing to do.
}


/*!
 * This function handles LoseFocus events for a static text object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static void WIDGETS_StaticText_OnLoseFocus( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility )
{
	return; // Nothing to do.
}


/*!
 * This function handles draw/redraw requests for a static text object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element       Pointer to form element part of the widget.
 * \param  internalPage  Which page of the widget to draw, in case of taller widgets.
 * \param  lcdPage       Which 8-pixel memory LCD page to write to.
 */
static void WIDGETS_StaticText_DrawPage( FORMS_element_t const * element, FORMS_size_t internalPage, uint8_t lcdPage )
{
	// Get a pointer to static text specific data.
	WIDGETS_StaticText_t const * widget = (WIDGETS_StaticText_t const *) (element->customData);
	
	// Draw element.
	const uint8_t charsPerFormLine = (FORMS_LAST_COLUMN - FORMS_FIRST_COLUMN + 1) / TERMFONT_CHAR_WIDTH;
	const TERMFONT_stringSize_t startPos = internalPage * charsPerFormLine;
	
	if (startPos < (element->captionSize - 1)) {
		uint8_t column = FORMS_FIRST_COLUMN;

		// Draw text.
		if (widget->element.useFlashCaption) {
			char const CAL_PGM(* chPtr) = element->caption_F + startPos;
			char ch;
			while (( (ch=CAL_pgm_read_char(chPtr)) != 0) && (column < FORMS_LAST_COLUMN)) {
				TERMFONT_DisplayChar( ch, lcdPage, column );
				++chPtr;
				column += TERMFONT_CHAR_WIDTH;
			}
		} else {
			char const * chPtr = element->caption + startPos;
			while ((*chPtr != 0) && (column < FORMS_LAST_COLUMN)) {
				TERMFONT_DisplayChar( *chPtr, lcdPage, column );
				++chPtr;
				column += TERMFONT_CHAR_WIDTH;
			}
		}
		
		// Inverse if required.
		if (widget->inverseVideo == true) {
			LCD_FlipPage( 0xff, lcdPage, FORMS_FIRST_COLUMN, FORMS_LAST_COLUMN - FORMS_FIRST_COLUMN + 1 );
		}
	}
}


// ----------
// Separators
// ----------

/*!
 * This function handles joystick events for a separator object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  event       Pointer to joystick event to process.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static bool WIDGETS_Separator_OnJoystick( FORMS_element_t * element, JOYSTICK_event_t const * event, FORMS_elementVisibility_t const * visibility )
{
	return false; // Nothing to do.
}


/*!
 * This function handles keyboard events for a separator object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  ch          Character code to process.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static bool WIDGETS_Separator_OnKeyboard( FORMS_element_t * element, char ch, FORMS_elementVisibility_t const * visibility )
{
	return false; // Nothing to do.
}


/*!
 * This function handles GetFocus events for a separator object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static void WIDGETS_Separator_OnGetFocus( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility )
{
	return; // Nothing to do.
}


/*!
 * This function handles LoseFocus events for a separator object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static void WIDGETS_Separator_OnLoseFocus( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility )
{
	return; // Nothing to do.
}


/*!
 * This function handles draw/redraw requests for a separator object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element       Pointer to form element part of the widget.
 * \param  internalPage  Which page of the widget to draw, in case of taller widgets.
 * \param  lcdPage       Which 8-pixel memory LCD page to write to.
 */
static void WIDGETS_Separator_DrawPage( FORMS_element_t const * element, FORMS_size_t internalPage, uint8_t lcdPage )
{
	// Draw element.
	if (internalPage == 0) { // Should never be anything else than page 0.
		LCD_SetHLine( FORMS_FIRST_COLUMN + WIDGETS_SEPARATOR_SPACING,
		              FORMS_LAST_COLUMN - WIDGETS_SEPARATOR_SPACING,
		              lcdPage * LCD_PAGE_HEIGHT + WIDGETS_SEPARATOR_POSITION );
	}
}


// ---------------
// Select Controls
// ---------------

/*!
 * This function handles joystick events for a select control object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  event       Pointer to joystick event to process.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static bool WIDGETS_SelectCtrl_OnJoystick( FORMS_element_t * element,
		JOYSTICK_event_t const * event, FORMS_elementVisibility_t const * visibility )
{
	bool eventAccepted = false;
	
	// Get a pointer to select control specific data.
	WIDGETS_SelectCtrl_t * widget = (WIDGETS_SelectCtrl_t *) (element->customData);

	// Enter changes data field.
	if ((event->clicked & JOYSTICK_ENTER) != 0x00) {
		// If selected, only deselect if not part of exclusion group.
		if (*(widget->data) == WIDGETS_SELECTCTRL_SELECTED) {
			if (widget->exclusionGroup == NULL) {
				WIDGETS_SelectCtrl_Deselect( widget, visibility );
			}
		}
		// If deselected, select and notify exclusion group if used.
		else {
			if (widget->exclusionGroup != NULL) {
				// Deselect other.
				FORMS_elementVisibility_t otherVisibility;
				FORMS_CalculateVisibility( visibility->form, &(widget->exclusionGroup->currentSelected->element), &otherVisibility );
				WIDGETS_SelectCtrl_Deselect( widget->exclusionGroup->currentSelected, &otherVisibility );
				
				// Change group to point to widget widget.
				widget->exclusionGroup->currentSelected = widget;
			}
			WIDGETS_SelectCtrl_Select( widget, visibility );
		}
			
		eventAccepted = true;
	}

	return eventAccepted;
}


/*!
 * This function handles keyboard events for a select control object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  ch          Character code to process.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static bool WIDGETS_SelectCtrl_OnKeyboard( FORMS_element_t * element, char ch,
		FORMS_elementVisibility_t const * visibility )
{
	return false; // TODO handle keys.
}


/*!
 * This function handles GetFocus events for a select control object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static void WIDGETS_SelectCtrl_OnGetFocus( FORMS_element_t * element,
		FORMS_elementVisibility_t const * visibility )
{
	return; // Nothing to do.
}


/*!
 * This function handles LoseFocus events for a select control object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element     Pointer to form element part of the widget.
 * \param  visibility  Pointer to struct holding information on this widgets visibility on screen.
 */
static void WIDGETS_SelectCtrl_OnLoseFocus( FORMS_element_t * element,
		FORMS_elementVisibility_t const * visibility )
{
	return; // Nothing to do.
}


/*!
 * This function handles draw/redraw requests for a select control object.
 * It is intented to be part of the traits struct for this object type,
 * and thus accepts form element pointers instead of the correct widget type.
 *
 * \param  element       Pointer to form element part of the widget.
 * \param  internalPage  Which page of the widget to draw, in case of taller widgets.
 * \param  lcdPage       Which 8-pixel memory LCD page to write to.
 */
static void WIDGETS_SelectCtrl_DrawPage( FORMS_element_t const * element,
		FORMS_size_t internalPage, uint8_t lcdPage )
{
	// Get a pointer to spin control specific data.
	WIDGETS_SelectCtrl_t const * widget = (WIDGETS_SelectCtrl_t const *) (element->customData);
	
	// Draw element.
	if (internalPage == 0) { // Should never be anything else than page 0.
		// Draw icon.
		WIDGETS_SelectCtrl_DrawIcon( widget, lcdPage );

		// Draw caption.
		if (widget->element.useFlashCaption) {
			TERMFONT_DisplayString_F( widget->element.caption_F, lcdPage,
			                          FORMS_FIRST_COLUMN + WIDGETS_SELECTCTRL_ICONWIDTH );
		} else {
			TERMFONT_DisplayString( widget->element.caption, lcdPage,
			                        FORMS_FIRST_COLUMN + WIDGETS_SELECTCTRL_ICONWIDTH );
		}		
	}
}


/*!
 * This function draws the select control icon on screen.
 *
 * \param  widget   Pointer to initialized widget.
 * \param  lcdPage  Which 8-pixel memory LCD page to write to.
 */
static void WIDGETS_SelectCtrl_DrawIcon( WIDGETS_SelectCtrl_t const * widget, uint8_t lcdPage )
{
	uint8_t column = FORMS_FIRST_COLUMN;

	// Find correct icon index.
	uint8_t iconIdx;	
	if (*(widget->data) == WIDGETS_SELECTCTRL_DESELECTED) {
		iconIdx = 0; // "deselected" icon at index 0 always.
	} else {
		iconIdx = 1; // "selected" icon at index 1 always.
	}

	// Draw icon.
	///mt LCD_WritePage_F( widget->iconList_FF[iconIdx], lcdPage, column, WIDGETS_SELECTCTRL_ICONWIDTH );
	LCD_WritePage_F( CAL_pgm_read_puint8(&(widget->iconList_FF[iconIdx])), lcdPage, column, WIDGETS_SELECTCTRL_ICONWIDTH );
}


/*!
 * This function takes care of calling necessary graphics update functions
 * and event handler associated with this object after a status change.
 *
 * \param  widget      Pointer to initialized widget.
 * \param  visibility  Pointer to visiblitiy information for this object.
 */
static void WIDGETS_SelectCtrl_UpdateAfterChange( WIDGETS_SelectCtrl_t const * widget,
		FORMS_elementVisibility_t const * visibility )
{
	// Only do graphics if visible.
	if (visibility->firstPage == 0) {
		WIDGETS_SelectCtrl_DrawIcon( widget, visibility->firstLCDPage );
	}
	
	// Call handler if registered.
	if (widget->OnChange != NULL) {
		widget->OnChange( widget->userId, widget->data );
	}		
}


/*!
 * This function takes care of calling necesary internal function in the correct
 * order to select the control.
 *
 * \param  widget      Pointer to initialized widget.
 * \param  visibility  Pointer to visiblitiy information for this object.
 */
static void WIDGETS_SelectCtrl_Select( WIDGETS_SelectCtrl_t * widget, FORMS_elementVisibility_t const * visibility )
{
	// Select if currently deselected.
	if (*(widget->data) == WIDGETS_SELECTCTRL_DESELECTED) {
		*(widget->data) = WIDGETS_SELECTCTRL_SELECTED;
		WIDGETS_SelectCtrl_UpdateAfterChange( widget, visibility );
	}
}


/*!
 * This function takes care of calling necesary internal function in the correct
 * order to deselect the control.
 *
 * \param  widget      Pointer to initialized widget.
 * \param  visibility  Pointer to visiblitiy information for this object.
 */
static void WIDGETS_SelectCtrl_Deselect( WIDGETS_SelectCtrl_t * widget, FORMS_elementVisibility_t const * visibility )
{
	// Select if currently deselected.
	if (*(widget->data) == WIDGETS_SELECTCTRL_SELECTED) {
		*(widget->data) = WIDGETS_SELECTCTRL_DESELECTED;
		WIDGETS_SelectCtrl_UpdateAfterChange( widget, visibility );
	}
}


// end of file
