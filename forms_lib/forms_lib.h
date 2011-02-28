// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  Graphical forms library header file.
 *
 *      The forms library provides functions for building and operating
 *      graphical user interface forms. It provides the basic structures for
 *      building form elements, such as check boxes, labels, and buttons.
 *      Other libraries, such as the Widgets Library use these basic structures
 *      for building complex graphical widgets.
 *
 *      Even though the forms library does not provide complete widgets itself,
 *      it provides the complete engine for stacking form elements into large
 *      forms and operating the form from incoming navigation events, typically
 *      from the Joystick Driver. The engine takes care of processing navigation
 *      events to browse through elements in the form, feeding events to the
 *      currently focused element, changing focus and issuing redraw request to
 *      updated areas on the LCD. It also utilizes the hardware scroll feature
 *      of the LCD Controller to efficiently support vertical scrolling for
 *      forms that are taller than the LCD screen.
 *
 *      In order to implement press-and-hold features such as repeated scrolling
 *      or browsing through characters in a widget, this library provides one
 *      single timing event that can be aquired/locked by a form or an element.
 *      Typically, the event handler will act upon a press-and-hold event by
 *      aquiring the timing event and handing over ncessary pointers and other
 *      information. The timing event will then take care of the repeated
 *      scrolling or other operation. When a release event arrives, the timing
 *      event is released.
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
 * $Revision: 2284 $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/forms_lib/forms_lib.h $
 * $Date: 2007-08-15 13:58:12 -0600 (on, 15 aug 2007) $  \n
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
#ifndef FORMS_LIB_H
#define FORMS_LIB_H

#include <stdint.h>
#include <stdbool.h>
#include <cal.h>

#include <joystick_driver.h>

#include <timing_lib.h>
#include <termfont_lib.h>



/************************
 * Constants and defines
 ************************/

//! First LCD column with element graphics. Decides margin available for focus indicator.
#define FORMS_FIRST_COLUMN 4
//! Last LCD column with element graphics. Decides margin available for scrollbar.
#define FORMS_LAST_COLUMN (LCD_WIDTH-4)
//! Characers per LCD line, excluding the space used by focus indicator and scrollbar.
#define FORMS_CHARS_PER_LINE ((FORMS_LAST_COLUMN - FORMS_FIRST_COLUMN + 1) / TERMFONT_CHAR_WIDTH)



/*************************************************************
 * Simple types, forward definitions and convenience typedefs
 *************************************************************/

//! Datatype counting form elements and pages. Change if you need forms larger than 255 pages.
typedef uint8_t FORMS_size_t;


//! One element of a form. Holds information about size, position, focus, event handlers etc.
struct FORMS_element_struct;
//! A container for event handlers and callbacks required to extend an element to become a widget.
struct FORMS_elementTraits_struct;
//! Holds information about which parts, if any, of an element are visible when updating graphics.
struct FORMS_elementVisibility_struct;
//! Holds the list of elements and current state of an entire form.
struct FORMS_form_struct;


// Convenient typedefs used to avoid having to type "struct" all the time.
typedef struct FORMS_element_struct           FORMS_element_t;
typedef struct FORMS_elementTraits_struct     FORMS_elementTraits_t;
typedef struct FORMS_elementVisibility_struct FORMS_elementVisibility_t;
typedef struct FORMS_form_struct              FORMS_form_t;


//! Joystick event handler function pointer for use with FORMS_elementTraits_t.
typedef bool (* FORMS_OnJoystickEventFunc_t) ( FORMS_element_t * element, JOYSTICK_event_t const * event, FORMS_elementVisibility_t const * visibility );
//! Keyboard event handler function pointer for use with FORMS_elementTraits_t.
typedef bool (* FORMS_OnKeyboardEventFunc_t) ( FORMS_element_t * element, char ch, FORMS_elementVisibility_t const * visibility );
//! Focus change event handler function pointer for use with FORMS_elementTraits_t.
typedef void (* FORMS_OnFocusEventFunc_t)    ( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility );
//! Draw page callback function pointer for use with FORMS_elementTraits_t.
typedef void (* FORMS_DrawPageFunc_t)        ( FORMS_element_t const * element, FORMS_size_t internalPage, uint8_t lcdPage );



/*********************
 * Struct definitions
 *********************/

/*!
 * This struct describes one element of the form.
 *
 * A form is built from elements stacked on top of each other and
 * this struct contains information about size and position inside the form.
 * These parameters will be automatically updated by the form when elements
 * are inserted and removed, along with the next and prev pointers.
 * Also, the form will update the focus variables when the user navigates
 * the form.
 *
 * The element itself is not a visible object, so this struct provides
 * a custom pointer variable and a pointer to a FORMS_elementTraits_t block,
 * providing the extension slot for extending elements to proper GUI widgets.
 *
 * The traits block contains function pointers for event handlers, drawing
 * functions etc, so that the form knows where to pass on incoming events and
 * how to draw and updated the element. The custom pointer is set to point to
 * a data block containing the specific state of a widget.
 */
struct FORMS_element_struct
{
	FORMS_size_t startPage;  //!< Page index inside form at which this elements starts.
	FORMS_size_t height;  //!< Height, in pages, of this element.
	FORMS_size_t focusedPage;  //!< Currently focused page inside this element, if focused.
	void * customData;  //!< Use this custom pointer to extend elements to full widgets.
	FORMS_elementTraits_t const CAL_PGM(* traits);  //!< Information about how to handle this element.

	bool isActive : 1;  //!< Form navigation skips this element if not active.
	bool hasFocus : 1;  //!< True if any page in this element is currently focused.
	bool useFlashCaption : 1;  //!< True if caption text should be read from Flash.
	
	union {
		char const * caption;  //!< Optional pointer to element caption text in SRAM.
		char const CAL_PGM(* caption_F);  //!< Optional pointer to element caption text in Flash.
	};
	TERMFONT_stringSize_t captionSize;  //!< Number of chars in caption string.
	
	FORMS_element_t * prev;  //!< Pointer to previous element in form.
	FORMS_element_t * next;  //!< Pointer to next element in form.
};


/*!
 * This struct contains function pointers for event handlers and other functions
 * required for extending an element to a proper GUI widget.
 *
 * Each element in a form conatins a pointer to a traits block in Flash.
 * Therefore, every widget type should have a corresponding traits block located
 * in Flash. The pointer to this block should then be passed on the element
 * initialization function.
 *
 * The form will call the functions pointed to by the traits block whenever
 * events need to be processed or the graphics requires and update. The
 * functions will get a pointer to the element itself, and by reading the
 * custom pointer within the element, the specific state variables of the widget
 * can be accessed. This would require an explicit type cast of the custom
 * pointer to the specific widget type.
 */
struct FORMS_elementTraits_struct
{
	//! Joystick navigation event handler.
	FORMS_OnJoystickEventFunc_t OnJoystick;
	//! Keyboard input event handler.
	FORMS_OnKeyboardEventFunc_t OnKeyboard;
	//! Event handler called when element goes from non-focused to focused state.
	FORMS_OnFocusEventFunc_t OnGetFocus;
	//! Event handler called when element goes from focused to non-focused state.
	FORMS_OnFocusEventFunc_t OnLoseFocus;
	//! Callback function used by the form to draw/refresh one page of the element.
	FORMS_DrawPageFunc_t DrawPage;
};


/*!
 * This struct contains the necessary information to decide what parts of
 * an element is visible on-screen or not for larger forms. When elements
 * needs to update graphics, a visibility struct is prepared and passed to
 * the grahics update functions for that element.
 *
 * If the element is not visible at all, the first and last visible page
 * variables have invalid values, beoynd last page of element.
 *
 * In case elements want to make other elements refresh their graphics,
 * they can use the form pointer in the struct to calculate a second visibility
 * block and pass that to the other elements drawing functions.
 */
struct FORMS_elementVisibility_struct
{
	FORMS_size_t firstPage;  //!< First visisble page of element.
	FORMS_size_t lastPage;  //!< Last visible page of element.
	uint8_t firstLCDPage;  //!< LCD page that corresponds to the first visible element page.
	FORMS_form_t * form;  //!< Reference to the parent form.
};


/*!
 * This struct holds the current state of the entire form, with current
 * focused element, visible pages, scrollbar states and of course the
 * double linked list of the elements themselves.
 */
struct FORMS_form_struct
{
	FORMS_element_t * focusedElement;  //!< Currently focused element.
	FORMS_size_t firstVisiblePage;  //!< The form page the is currently on the top LCD row.
	FORMS_size_t height;  //!< Total height, in pages, of the form, including all elements.
	bool useScrollbar : 1;  //!< True if scrollbar should be drawn.
	bool needsRecalculation : 1;  //!< True if contents have been changed and height etc needs recalculation.
	bool isScrolling : 1;  //!< True if form is currently in scroll-without-focus-change mode.
	
	FORMS_size_t elementCount;  //!< Number of elements in form.
	FORMS_element_t * firstElement;  //!< First element in double linked list.
	FORMS_element_t * lastElement;  //!< Last element in double linked list.
};



/**********************
 * Function prototypes
 **********************/

//! Initialize a form struct and make it ready for adding elements.
void FORMS_Init( FORMS_form_t * form, bool useScrollbar );
//! Clear screen and restore default LCD hardware scrolling.
void FORMS_CleanUp( void );
//! Restore default LCD hardware scrolling and redraw form.
void FORMS_NormalizeLCDScroll( FORMS_form_t * form );

//! Initialize a form element struct. Should only be called from initialization functions for widgets.
void FORMS_InitElement(
		FORMS_element_t * element,
		FORMS_size_t height,
		FORMS_size_t focusedPage,
		bool isActive,
		void * customData,
		FORMS_elementTraits_t const CAL_PGM(* traits ));
//! Set a caption text from SRAM for a form element.
void FORMS_SetCaption( FORMS_element_t * element, char const * caption );
//! Set a caption text from Flash for a form element.
void FORMS_SetCaption_F( FORMS_element_t * element, char const CAL_PGM(* caption_F) );

//! Add an element at the top of the form.
void FORMS_AddTopElement( FORMS_form_t * form, FORMS_element_t * newElement );
//! Add an element at the bottom of the form.
void FORMS_AddBottomElement( FORMS_form_t * form, FORMS_element_t * newElement );
//! Remove element from form.
void FORMS_RemoveElement( FORMS_form_t * form, FORMS_element_t * elementToBeRemoved );
//! Set the element to be currently focused.
void FORMS_SetFocusedElement( FORMS_form_t * form, FORMS_element_t * elementToFocus );

//! Draw the form and its contents.
void FORMS_Draw( FORMS_form_t * form );
//! Scroll the form if necessary, to bring focused element into view.
void FORMS_EnsureFocusedIsVisible( FORMS_form_t * form );
//! Prepare a visibility information block from a form and an element.
void FORMS_CalculateVisibility( FORMS_form_t * form, FORMS_element_t const * element, FORMS_elementVisibility_t * visibility );

//! Joystick event handler function for an entire form.
void FORMS_OnJoystick( FORMS_form_t * form, JOYSTICK_event_t const * event );
//! Scroll form up a number of steps, without changing focus.
void FORMS_ScrollUp( FORMS_form_t * form, FORMS_size_t scrollAmount );
//! Scroll form down a number of steps. without changing focus.
void FORMS_ScrollDown( FORMS_form_t * form, FORMS_size_t scrollAmount );
//! Move focus to previous element, scrolling if necessary.
void FORMS_NavigateUp( FORMS_form_t * form );
//! Move focus to next element, scrolling if necessary.
void FORMS_NavigateDown( FORMS_form_t * form );

//! Aquire the form library's timing event for a repeating event for a form, such as navigation.
TIMING_event_t * FORMS_PrepareRepeatingForm( FORMS_form_t * form );
//! Aquire the form library's timing event for a repeating event for an element, such as spinning numbers.
TIMING_event_t * FORMS_PrepareRepeatingElement( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility );
//! Get pointer to the form currently having the timing event.
FORMS_form_t * FORMS_GetRepeatingForm( void );
//! Get pointer to the element currently having the timing event.
FORMS_element_t * FORMS_GetRepeatingElement( void );
//! Get pointer to visibility information block associated with the timing event.
FORMS_elementVisibility_t const * FORMS_GetRepeatingVisibility( void );
//! Release the form library's timing event.
void FORMS_StopRepeating( void );


#endif
// end of file
