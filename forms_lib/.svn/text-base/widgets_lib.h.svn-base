// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  GUI widgets library header file.
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
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/forms_lib/widgets_lib.h $
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
#ifndef WIDGETS_LIB_H
#define WIDGETS_LIB_H

#include <cal.h>
#include <forms_lib.h>


/************************
 * Constants and defines
 ************************/

//! Constant value used for representing deselected/unchecked SelectCtrl widgets.
#define WIDGETS_SELECTCTRL_DESELECTED ((WIDGETS_integer_t) false)
//! Constant value used for representing selected/checked SelectCtrl widgets.
#define WIDGETS_SELECTCTRL_SELECTED   ((WIDGETS_integer_t) true)



/*******************************************************
 * Forward struct declarations and convenience typedefs
 *******************************************************/

struct WIDGETS_SpinCtrl_struct;  //!< Spin button widget.
struct WIDGETS_TextCtrl_struct;  //!< Text input widget.
struct WIDGETS_ButtonRow_struct;  //!< Clickable button widget.
struct WIDGETS_StaticText_struct;  //!< Clickable static text widget.
struct WIDGETS_Separator_struct;  //!< Passive separator line widget.
struct WIDGETS_ExclusionGroup_struct;  //!< Helper object for radio buttons.
struct WIDGETS_SelectCtrl_struct;  //!< Radio buttons or check boxes.

typedef struct WIDGETS_SpinCtrl_struct       WIDGETS_SpinCtrl_t;
typedef struct WIDGETS_TextCtrl_struct       WIDGETS_TextCtrl_t;
typedef struct WIDGETS_ButtonRow_struct      WIDGETS_ButtonRow_t;
typedef struct WIDGETS_StaticText_struct     WIDGETS_StaticText_t;
typedef struct WIDGETS_Separator_struct      WIDGETS_Separator_t;
typedef struct WIDGETS_ExclusionGroup_struct WIDGETS_ExclusionGroup_t;
typedef struct WIDGETS_SelectCtrl_struct     WIDGETS_SelectCtrl_t;



/********************
 * Misc useful types
 ********************/

//! Integer type used across all widget types, working as a common type for values of all kinds.
typedef int32_t WIDGETS_integer_t;
//! Used for custom IDs for referencing widgets.
typedef uint16_t WIDGETS_id_t;

//! Callback type used by clickable widgets, such as buttons and static texts.
typedef void (* WIDGETS_OnClickFunc_t)( WIDGETS_id_t userId );
//! Callback type used by widgets which changing values, such as spin controls.
typedef void (* WIDGETS_OnIntegerChangeFunc_t)( WIDGETS_id_t userId, WIDGETS_integer_t const * data );
//! Callback type used by widgets with editable text.
typedef void (* WIDGETS_OnTextChangeFunc_t)( WIDGETS_id_t userId, char const * data );
//! Plugin function type for widgets that need a way to present a value on screen.
typedef void (* WIDGETS_SpinCtrl_DrawDataFunc_t)( WIDGETS_SpinCtrl_t const * widget, uint8_t lcdPage );
//! Plugin function type for widgets with editable text. Used to limit possible characters.
typedef void (* WIDGETS_TextCtrl_ChangeCharFunc_t)( WIDGETS_TextCtrl_t const * widget, bool changeUp );

//! Enumerator for indicating where strings and lists are located.
typedef enum WIDGETS_StringListLocation_enum
{
	WIDGETS_SRAM_STRINGS,      //!< SRAM-based list of SRAM-based strings.
	WIDGETS_FLASH_STRINGS,     //!< SRAM-based list of Flash-based strings.
	WIDGETS_FLASH_STRING_LIST  //!< Flash-based list of Flash-based strings.
} WIDGETS_StringListLocation_t;



/********************************
 * Widget object structure types
 ********************************/

/*!
 * This struct holds current state and other information required to
 * operate a spin control widget. The spin control is used to browse
 * through a data set, either a range of numbers, or through a list of
 * text strings.
 *
 * The spin control itself only understands a range of
 * consequtive numbers, but using plugin functions, the numbers can be
 * presented as is, converted to hex, bin or even be used as an index into
 * an array of text strings.
 *
 * When stepping/spinning up/down, the widget has a minimum step size, but
 * if the up/down button is held down, the step size doubles a regular
 * intervals, accelerating the stepping/spinning.
 */
struct WIDGETS_SpinCtrl_struct
{
	FORMS_element_t element;                   //!< The basic form element information.
	WIDGETS_id_t userId;                       //!< Custom ID for this widget.
	WIDGETS_OnIntegerChangeFunc_t OnChange;    //!< Callback for handling data changes.
	WIDGETS_integer_t * data;                  //!< Pointer to widget's integer data.
	WIDGETS_SpinCtrl_DrawDataFunc_t DrawData;  //!< Plugin for presenting the integer data.
	WIDGETS_integer_t lowerLimit;              //!< Lower limit (inclusive) for the integer data.
	WIDGETS_integer_t upperLimit;              //!< Upper limit (inclusive) for the integer data.
	WIDGETS_integer_t stepSize;                //!< Minimum step size when stepping/spinning data.
	WIDGETS_integer_t accelerationCountdown;   //!< Steps left before doubling the step size.
	WIDGETS_integer_t currentStepSize;         //!< Current step size while accelerating.
	uint8_t dataFieldSize;                     //!< Size in characters for the representation of the data.
	bool isDataFocused;                        //!< True if data is focused, ie. will spin on up/down events.

	WIDGETS_StringListLocation_t stringListLocation;  //!< Where to find text strings, if used.
	union {
		char const * * strings;                       //!< SRAM array of SRAM strings.
		char const CAL_PGM(* * strings_F);            //!< SRAM array of Flash strings.
		// char const __flash * __flash * strings_FF; //!< Flash array of Flash strings.
		char const CAL_PGM(* CAL_PGM(* strings_FF));  //!< Flash array of Flash strings.
	};
};


/*!
 * This struct holds current state and other information required to
 * operate a text control widget. The text control is used to input text
 * from the user, by letting the user move left/right in a text string,
 * and up/down to change the character at the cursor position. Pressing
 * CENTER while moving left/right copies characters to adjacent positions.
 *
 * Plugin functions are used to limit the available characters, as the plugin
 * decides what character to change to when moving up/down.
 */
struct WIDGETS_TextCtrl_struct
{
	FORMS_element_t element;                       //!< The basic form element information.
	WIDGETS_id_t userId;                           //!< Custom ID for this widget.
	WIDGETS_OnTextChangeFunc_t OnChange;           //!< Callback for handling data changes.
	char * data;                                   //!< Pointer to widget's text data.
	WIDGETS_TextCtrl_ChangeCharFunc_t ChangeChar;  //!< Plugin for character change.
	TERMFONT_stringSize_t maxLength;               //!< Maximum number of characters.
	TERMFONT_stringSize_t cursorPosition;          //!< Current position within text string.
	TERMFONT_stringSize_t scrollOffset;            //!< Index of first character visible on screen.
	uint8_t dataFieldSize;                         //!< Size in characters of visible text field.
	bool isDataFocused;                            //!< True if data is focused, ie. will change or move in text.
	bool isCopying;                                //!< True if currently in copy-to-adjacent mode.
};


/*!
 * This struct holds current state and other information required to
 * operate a button row widget. The button row contains a set of buttons, which
 * the user can move between and click on. The button row widget accepts a list
 * of strings, and shows buttons accordingly. The buttons can be given individual
 * custom IDs, and an OnClick plugin will handle the click event.
 *
 * Normally, a button row will have an OnClick callback that ends a modal
 * dialog box, returning the current button's ID to the dialog box manager.
 */
struct WIDGETS_ButtonRow_struct
{
	FORMS_element_t element;          //!< The basic form element information.
	FORMS_size_t currentButton;       //!< Index of current button selected.
	FORMS_size_t buttonCount;         //!< Total number of buttons.
	WIDGETS_id_t const * userIdList;  //!< List of custom IDs for buttons.
	WIDGETS_OnClickFunc_t OnClick;	  //!< Callback for handling click events.

	WIDGETS_StringListLocation_t stringListLocation;  //!< Where to find text strings.
	union {
		char const * * strings;                       //!< SRAM array of SRAM strings.
		char const CAL_PGM(* * strings_F);            //!< SRAM array of Flash strings.
		char const CAL_PGM(* CAL_PGM(* strings_FF));  //!< Flash array of Flash strings.
	};
};


/*!
 * This struct holds current state and other information required to
 * operate a static text widget. The static text shows single- og multiline
 * text, no changes possible. However, the text is clickable, and can tehrefore
 * be used to end modal dialog boxes, eg. for a list of menu items.
 */
struct WIDGETS_StaticText_struct
{
	FORMS_element_t element;        //!< The basic form element information.
	WIDGETS_id_t userId;            //!< Custom ID for this widget.
	WIDGETS_OnClickFunc_t OnClick;  //!< Callback for handling click events.
	bool inverseVideo;              //!< True if text should ble shown in inverse video.
};


/*!
 * This struct holds current state and other information required to
 * operate a separator bar widget. The separator bar can be used as a natural
 * divider for logically grouping form elements. It is passive, and cannot be
 * focused or clicked on.
 */
struct WIDGETS_Separator_struct
{
	FORMS_element_t element;  //!< The basic form element information.
};


/*!
 * This struct holds current state and information required by the
 * WIDGETS_SelectCtrl_struct object to implement the radio button feature.
 * Every radio button in a single exclusion group keeps a reference to this
 * struct, which in turn keeps track of the radio button currently selected.
 */
struct WIDGETS_ExclusionGroup_struct
{
	WIDGETS_SelectCtrl_t * currentSelected;  //!< Current selected entry in the group.
};


/*!
 * This struct holds current state and information required by to
 * operate a select control widget. The select control is a textual
 * item with a selection box to the left. A number of select controls
 * can be assoicated with a WIDGETS_ExclusionGroup object in order to
 * implement a list of mutually exclusive selections.
 */
struct WIDGETS_SelectCtrl_struct
{
	FORMS_element_t element;                        //!< The basic form element information.
	WIDGETS_id_t userId;                            //!< Custom ID for this widget.
	WIDGETS_OnIntegerChangeFunc_t OnChange;         //!< Callback for handling data changes.
	WIDGETS_integer_t * data;                       //!< Pointer to widget's integer data, SELECTED/DESELECTED.
	uint8_t const CAL_PGM(* CAL_PGM(* iconList_FF));  //!< Pointor to two-icon Flash array for check box images.
	WIDGETS_ExclusionGroup_t * exclusionGroup;      //!< Optional reference to exclusion group.
};



/*********************
 * Function protoypes
 *********************/

//! Initialize a spin control object.
void WIDGETS_SpinCtrl_Init( WIDGETS_SpinCtrl_t * widget,
		WIDGETS_id_t userId,
		WIDGETS_integer_t * data,
		WIDGETS_SpinCtrl_DrawDataFunc_t DrawData,
		WIDGETS_integer_t lowerLimit,
		WIDGETS_integer_t upperLimit,
		WIDGETS_integer_t stepSize,
		uint8_t dataFieldSize );
//! Provide an OnChange handler for a spin control object.
void WIDGETS_SpinCtrl_SetOnChange( WIDGETS_SpinCtrl_t * widget, WIDGETS_OnIntegerChangeFunc_t OnChange );
//! Associate an SRAM array of SRAM strings to a spin control object.
void WIDGETS_SpinCtrl_SetStringList( WIDGETS_SpinCtrl_t * widget, char const * * strings );
//! Associate an SRAM array of Flash strings to a spin control object.
void WIDGETS_SpinCtrl_SetStringList_F( WIDGETS_SpinCtrl_t * widget, char const CAL_PGM(* * strings) );
//! Associate a Flash array of Flash strings to a spin control object.
void WIDGETS_SpinCtrl_SetStringList_FF( WIDGETS_SpinCtrl_t * widget, char const CAL_PGM(* CAL_PGM(* strings)) );
//! Plugin function for decimal number presentation.
void WIDGETS_SpinCtrl_DrawDec( WIDGETS_SpinCtrl_t const * widget, uint8_t lcdPage );
//! Plugin function for lower case hexadecimal number presentation.
void WIDGETS_SpinCtrl_DrawLHex( WIDGETS_SpinCtrl_t const * widget, uint8_t lcdPage );
//! Plugin function for upper case hexadecimal number presentation.
void WIDGETS_SpinCtrl_DrawUHex( WIDGETS_SpinCtrl_t const * widget, uint8_t lcdPage );
//! Plugin function for presenting data by indexing into string list.
void WIDGETS_SpinCtrl_DrawString( WIDGETS_SpinCtrl_t const * widget, uint8_t lcdPage );

//! Initialize a text control object.
void WIDGETS_TextCtrl_Init( WIDGETS_TextCtrl_t * widget,
		WIDGETS_id_t userId,
		char * data,
		WIDGETS_TextCtrl_ChangeCharFunc_t ChangeChar,
		TERMFONT_stringSize_t maxLength,
		uint8_t dataFieldSize );
//! Provide an OnChange handler for a text control object.
void WIDGETS_TextCtrl_SetOnChange( WIDGETS_TextCtrl_t * widget, WIDGETS_OnTextChangeFunc_t OnChange );
//! Plugin function for using all printable characters.
void WIDGETS_TextCtrl_ChangeASCII( WIDGETS_TextCtrl_t const * widget, bool changeUp );
//! Plugin function for using decimal digits only.
void WIDGETS_TextCtrl_ChangeDec( WIDGETS_TextCtrl_t const * widget, bool changeUp );
//! Plugin function for using binary digits only.
void WIDGETS_TextCtrl_ChangeBin( WIDGETS_TextCtrl_t const * widget, bool changeUp );
//! Plugin function for using lower case hexadecimal digits only.
void WIDGETS_TextCtrl_ChangeLHex( WIDGETS_TextCtrl_t const * widget, bool changeUp );
//! Plugin function for using upper case hexadecimal digits only.
void WIDGETS_TextCtrl_ChangeUHex( WIDGETS_TextCtrl_t const * widget, bool changeUp );

//! Initialize a button row object.
void WIDGETS_ButtonRow_Init( WIDGETS_ButtonRow_t * widget,
		WIDGETS_id_t const * userIdList,
		FORMS_size_t defaultButton,
		FORMS_size_t buttonCount );
//! Provide an OnClick handler for a button row object.
void WIDGETS_ButtonRow_SetOnClick( WIDGETS_ButtonRow_t * widget, WIDGETS_OnClickFunc_t OnClick );
//! Associate an SRAM array of SRAM strings to a button row object.
void WIDGETS_ButtonRow_SetStringList( WIDGETS_ButtonRow_t * widget, char const * * strings );
//! Associate an SRAM array of Flash strings to a button row object.
void WIDGETS_ButtonRow_SetStringList_F( WIDGETS_ButtonRow_t * widget, char const CAL_PGM(* * strings) );
//! Associate a Flash array of Flash strings to a button row object.
void WIDGETS_ButtonRow_SetStringList_FF( WIDGETS_ButtonRow_t * widget, char const CAL_PGM(* CAL_PGM(* strings)) );

//! Initialize a static text object.
void WIDGETS_StaticText_Init( WIDGETS_StaticText_t * widget,
		WIDGETS_id_t userId,
		FORMS_size_t height,
		bool isActive,
		bool inverseVideo );
//! Provide an OnClick handler for a static text object.
void WIDGETS_StaticText_SetOnClick( WIDGETS_StaticText_t * widget, WIDGETS_OnClickFunc_t OnClick );

//! Initialize a separator object.
void WIDGETS_Separator_Init( WIDGETS_Separator_t * widget );

//! Initialize an exclusion group object.
void WIDGETS_ExclusionGroup_Init( WIDGETS_ExclusionGroup_t * widget, WIDGETS_SelectCtrl_t * selectCtrl );
//! Get custom ID from current selected element in an exclusion group.
WIDGETS_id_t WIDGETS_ExclusionGroup_GetId( WIDGETS_ExclusionGroup_t * widget );

//! Initialize a "check box" type select control object.
void WIDGETS_SelectCtrl_InitCheck( WIDGETS_SelectCtrl_t * widget,
		WIDGETS_id_t userId,
		WIDGETS_integer_t * data );
//! Initialize a "radio button" type select control object.
void WIDGETS_SelectCtrl_InitRadio( WIDGETS_SelectCtrl_t * widget,
		WIDGETS_id_t userId,
		WIDGETS_integer_t * data,
		WIDGETS_ExclusionGroup_t * exclusionGroup );
//! Provide an OnChange handler for a select control object.
void WIDGETS_SelectCtrl_SetOnChange( WIDGETS_SelectCtrl_t * widget, WIDGETS_OnIntegerChangeFunc_t OnChange );


#endif
// end of file
