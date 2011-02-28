// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  Graphical forms library source file.
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
 * $Revision: 2436 $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/forms_lib/forms_lib.c $
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

#include "forms_lib.h"
#include <timing_lib.h>
#include <lcd_lib.h>
#include <s6b1713_driver.h>
#include <rtc_driver.h>




/********************************
 * Private constants and defines
 ********************************/

//! Delay in ticks to use when joystick press-and-hold causes repeated navigation events.
#define FORMS_REPEAT_DELAY (RTC_TICKS_PER_SECOND / 8)



/******************************
 * Private types and variables
 ******************************/

/*!
 * This struct holds the state information for any form
 * or element that is currently in a repeated navigation
 * mode, such as press-and-hold causing repeated browsing
 * inside a form. The struct is unused if the form or element
 * pointer is NULL, which is also the default initialization value.
 *
 * The purpose of this form is to keep a pointer to
 * the form or element in repeating state, and to hold
 * the timing event node for the repeating action. Also, for
 * repeating elements, the precalculated visibility block
 * is kept inside the repeat state struct.
 *
 * Note that this struct is shared among all
 * forms and elements in the application. This is ok, since only
 * one form will be on-screen at any given time, and only one
 * element can have focus inside the form.
 */
static struct FORMS_repeatState_struct
{
	union {
		FORMS_form_t * form;  //!< Form currently in repeating state.
		FORMS_element_t * element;  //!< Element currently in repeating state.
	};
	TIMING_event_t timingEvent;  //!< The timing event node required for the repeating action.
	FORMS_elementVisibility_t visibility;  //!< Precalculated visibility block when element is repeating.
} FORMS_repeatState;



/****************
 * Form graphics
 ****************/

//! Small bitmap used for drawing the focus indicator.
static uint8_t const CAL_PGM_DEF(FORMS_focusIndicator[FORMS_FIRST_COLUMN]) = {
	CREATE_BYTE( 0,0,1,1,1,1,1,0 ),
	CREATE_BYTE( 0,0,0,1,1,1,0,0 ),
	CREATE_BYTE( 0,0,0,0,1,0,0,0 ),
	CREATE_BYTE( 0,0,0,0,0,0,0,0 )
};



/******************************
 * Private function prototypes
 ******************************/

//! Callback used for repeated scroll-up events.
static void FORMS_ScrollUpCallback( void );
//! Callback used for repeated scroll-down events.
static void FORMS_ScrollDownCallback( void );
//! Callback used for repeated navigate-up events.
static void FORMS_NavigateUpCallback( void );
//! Callback used for repeated navigate-down events.
static void FORMS_NavigateDownCallback( void );
//! Calculate or recalculate start pages for all elements and total form height.
static void FORMS_CalculateStartPages( FORMS_form_t * form );
//! Draw or redraw the scrollbar.
static void FORMS_DrawScrollbar( FORMS_form_t const * form );
//! Draw or redraw the focus indicator.
static void FORMS_DrawFocusIndicator( FORMS_form_t const * form );



/***************************
 * Function implementations
 ***************************/

/*!
 * This function initializes a form struct in memory.
 * Call this function on a form struct before doing anything
 * else with it.
 *
 * \param  form          Pointer to uninitialized form struct.
 * \param  useScrollbar  True if form should show a scrollbar on right-hand side.
 */
void FORMS_Init( FORMS_form_t * form, bool useScrollbar )
{
	form->focusedElement = NULL;
	form->firstVisiblePage = 0;
	form->height = 0;
	form->useScrollbar = useScrollbar;
	form->needsRecalculation = false;
	form->elementCount = 0;
	form->firstElement = NULL;
	form->lastElement = NULL;
	form->isScrolling = false;
}


/*!
 * This function cleans up the LCD screen after a form is finished.
 * It basically clears the screen and resets any hardware LCD scrolling
 * to default state.
 */
void FORMS_CleanUp( void )
{
	LCD_SetScreen( 0x00 );
	LCD_SetTopPage( 0, 0 );
}


/*!
 * This function sets the hardware LCD scrolling back to default state
 * a redraws the form with the new settings. This function is convenient if
 * you want to overlay some graphics on top of a form temporarily, such as
 * a pop-up box, without having to compensate for hardware LCD scrolling.
 *
 * \param  form  Pointer to the form currently being on screen.
 */
void FORMS_NormalizeLCDScroll( FORMS_form_t * form )
{
	LCD_SetTopPage( 0, 0 );
	FORMS_Draw( form );
}


/*!
 * This function initializes a form element struct in memory.
 * Call this function on a form element struct before doing anything
 * else with it. Normally, this function will be called by the initialization
 * functions for a specific widget, so the user should not have to worry about
 * this. However, if you are making your own widgets, make sure your widget
 * initialization function calls this element initialization function first thing.
 *
 * \param  element      Pointer to uninitialized form element struct.
 * \param  height       Height of element, in number of LCD pages (8-pixel pages).
 * \param  focusedPage  Which page inside the element (first is 0), to give focus to begin with.
 * \param  isActive     True if this element should be browsable (not skipped) in the form.
 * \param  customData   Pointer to be usd to associate specific widget data to this element.
 * \param  traits       Set of callbacks and function pointers implemented for specific widgets.
 */
void FORMS_InitElement(
		FORMS_element_t * element,
		FORMS_size_t height,
		FORMS_size_t focusedPage,
		bool isActive,
		void * customData,
		FORMS_elementTraits_t const CAL_PGM(* traits) )
{
	element->height = height;
	element->focusedPage = focusedPage;
	element->isActive = isActive;
	element->customData = customData;
	element->traits = traits;
	element->useFlashCaption = false;
	element->caption = NULL;
	element->hasFocus = false;
}


/*!
 * This function assigns an SRAM based string as a caption for a form element.
 * The text length and height are calculated and the element height increased
 * if required. Note that the text is NOT copied, so the source data needs to
 * be valid and available until the caption is changed or the element destroyed.
 *
 * \param  element  Pointer to element of which to set or change caption.
 * \param  caption  Pointer to a null-terminated string in SRAM.
 */
void FORMS_SetCaption( FORMS_element_t * element, char const * caption )
{
	element->useFlashCaption = false;
	element->caption = caption;
	
	// Find number of characters in caption.
	TERMFONT_stringSize_t captionSize = 0;
	char const * chPtr = caption;
	while (*chPtr != 0) {
		++captionSize;
		++chPtr;
	}
	element->captionSize = captionSize;
	
	// Increase element height if required.
	uint8_t const requiredLines = (captionSize + FORMS_CHARS_PER_LINE - 1) / FORMS_CHARS_PER_LINE;
	if (requiredLines > element->height) {
		element->height = requiredLines;
	}
}


/*!
 * This function assigns a Flash based string as a caption for a form element.
 * The text length and height are calculated and the element height increased
 * if required. Note that the text is NOT copied, so the source data needs to
 * be valid and available until the caption is changed or the element destroyed.
 *
 * \param  element    Pointer to element of which to set or change caption.
 * \param  caption_F  Pointer to a null-terminated string in SRAM.
 */
void FORMS_SetCaption_F( FORMS_element_t * element, char const CAL_PGM(* caption_F) )
{
	element->useFlashCaption = true;
	element->caption_F = caption_F;

	// Find number of characters in caption.
	TERMFONT_stringSize_t captionSize = 0;
	char const CAL_PGM(* chPtr) = caption_F;
	while ( CAL_pgm_read_char(chPtr) != 0) {
		++captionSize;
		++chPtr;
	}
	element->captionSize = captionSize;
	
	// Increase element height if required.
	uint8_t const requiredLines = (captionSize + FORMS_CHARS_PER_LINE - 1) / FORMS_CHARS_PER_LINE;
	if (requiredLines > element->height) {
		element->height = requiredLines;
	}
}


/*!
 * This function adds a prepared form element to the top of an existing form,
 * updates necessary variables and marks the form as updated, so that heights
 * will be automatically recalculated on the next draw.
 *
 * \param  form        Pointer to existing form to add element to.
 * \param  newElement  Pointer to form element to be added to form.
 */
void FORMS_AddTopElement( FORMS_form_t * form, FORMS_element_t * newElement )
{
	// Add the element to top of list.
	if (form->elementCount == 0) {
		form->firstElement = newElement;
		form->lastElement = newElement;
		newElement->prev = NULL;
		newElement->next = NULL;
		newElement->hasFocus = true;
		form->focusedElement = newElement;
	} else {
		form->firstElement->prev = newElement;
		newElement->next = form->firstElement;
		newElement->prev = NULL;
		form->firstElement = newElement;
	}

	++(form->elementCount);
	
	// Mark form for recalculation on next draw.
	form->needsRecalculation = true;
}


/*!
 * This function adds a prepared form element to the bottom of an existing form,
 * updates necessary variables and marks the form as updated, so that heights
 * will be automatically recalculated on the next draw.
 *
 * \param  form        Pointer to existing form to add element to.
 * \param  newElement  Pointer to form element to be added to form.
 */
void FORMS_AddBottomElement( FORMS_form_t * form, FORMS_element_t * newElement )
{
	// Add the element to bottom of list.
	if (form->elementCount == 0) {
		form->firstElement = newElement;
		form->lastElement = newElement;
		newElement->prev = NULL;
		newElement->next = NULL;
		newElement->hasFocus = true;
		form->focusedElement = newElement;
	} else {
		form->lastElement->next = newElement;
		newElement->prev = form->lastElement;
		newElement->next = NULL;
		form->lastElement = newElement;
	}
	
	++(form->elementCount);

	// Mark form for recalculation on next draw.
	form->needsRecalculation = true;
}


/*!
 * This function removes an element from a form. If the element is not
 * found within the form, this function leaves the form untouched and exits.
 * If the element is found and removed, this function also marks the form as
 * updated, so that heights will be automatically recalculated on the next draw.
 *
 * \param  form                Pointer to form containing the element.
 * \param  elementToBeRemoved  Pointer to element that should be removed.
 */
void FORMS_RemoveElement( FORMS_form_t * form, FORMS_element_t * elementToBeRemoved )
{
	// Setup iteration variables.
	FORMS_element_t const * element = form->firstElement;
	FORMS_size_t elementsLeft = form->elementCount;

	// Find element.
	while ((elementsLeft > 0) && (element != elementToBeRemoved)) {
		element = element->next;
		--elementsLeft;
	}

	// Did we find it?
	if (element == elementToBeRemoved) {
		// Was it the last one?
		if (form->elementCount == 1) {
			form->firstElement = NULL;
			form->lastElement = NULL;
		}
		// Was it at the top?
		else if (elementToBeRemoved == form->firstElement) {
			// Safely change focus to next element.
			if (elementToBeRemoved->hasFocus) {
				FORMS_SetFocusedElement( form, elementToBeRemoved->next );
			}
			
			// Remove from top.
			form->firstElement = elementToBeRemoved->next;
		}
		// Was it at the bottom?
		else if (elementToBeRemoved == form->lastElement) {
			// Safely change focus to previous element.
			if (elementToBeRemoved->hasFocus) {
				FORMS_SetFocusedElement( form, elementToBeRemoved->prev );
			}
			
			// Remove from bottom.
			form->lastElement = elementToBeRemoved->prev;
		}
		// In the middle somewhere, then.
		else {
			// Safely change focus to next element.
			if (elementToBeRemoved->hasFocus) {
				FORMS_SetFocusedElement( form, elementToBeRemoved->next );
			}

			// Remove from list.			
			elementToBeRemoved->next->prev = elementToBeRemoved->prev;
			elementToBeRemoved->prev->next = elementToBeRemoved->next;
		}

		// Clean up.
		elementToBeRemoved->prev = NULL;
		elementToBeRemoved->next = NULL;
		--(form->elementCount);

		// Mark form for recalculation on next draw.
		form->needsRecalculation = true;
	}
}


/*!
 * This function changes which element in the form that has focus. After changing
 * focus, the function makes sure that the new focused element is visible, scrolling
 * the form if necessary.
 * Note that this function does not check if the new element to focus is
 * actually a part of the form referred to. It wil fail miserably if this
 * condition is not met.
 *
 * \param  form            Pointer to the form for which to change focus.
 * \param  elementToFocus  Pointer to new element to focus. Must be in the form already.
 */
void FORMS_SetFocusedElement( FORMS_form_t * form, FORMS_element_t * elementToFocus )
{
	if (form->focusedElement != elementToFocus) {
		FORMS_elementVisibility_t visibility;
		FORMS_OnFocusEventFunc_t focusEventFunc; ///mt
		if ( form->focusedElement != NULL ) {    ///mt 
			// Prepare visibility for element that loses focus and notify it.
			FORMS_CalculateVisibility( form, form->focusedElement, &visibility );
			///mt form->focusedElement->traits->OnLoseFocus( form->focusedElement, &visibility );
			///mt uff, well, not nice but works
			//focusEventFunc = (FORMS_OnFocusEventFunc_t)(CAL_pgm_read_pvoid(&(form->focusedElement->traits->OnLoseFocus)));
			focusEventFunc = (FORMS_OnFocusEventFunc_t)(CAL_pgm_read_pvoid(&(form->focusedElement->traits->OnLoseFocus)));
			focusEventFunc( form->focusedElement, &visibility );
			// Change focus.
			form->focusedElement->hasFocus = false;
		}

		form->focusedElement = elementToFocus;
		form->focusedElement->hasFocus = true;
		
		// Prepare visibility for and notify recently focused element.
		FORMS_CalculateVisibility( form, form->focusedElement, &visibility );
		///mt form->focusedElement->traits->OnGetFocus( form->focusedElement, &visibility );
		focusEventFunc = (FORMS_OnFocusEventFunc_t)(CAL_pgm_read_pvoid(&(form->focusedElement->traits->OnGetFocus)));
		focusEventFunc( form->focusedElement, &visibility );

		// Then scroll form to make sure it is visible.
		FORMS_EnsureFocusedIsVisible( form );
	}
}


/*!
 * This function draws all visible contents of a form on the LCD.
 * Call this function whenever there is a need to refresh the screen,
 * after displaying other graphics on top, for instance.
 *
 * \param  form  Pointer to form to draw.
 */
void FORMS_Draw( FORMS_form_t * form )
{
	// Clear screen first.
	LCD_SetScreen( 0x00 );
	
	// Exit if there is nothing to draw for some reason.
	if (form->elementCount == 0) {return; }

	// Calculate start pages if required.
	if (form->needsRecalculation) {
		FORMS_CalculateStartPages( form );
	}

	// Setup iteration variables.
	FORMS_element_t const * element = form->firstElement;
	FORMS_size_t elementsLeft = form->elementCount;

	// Find first element that have parts to draw on LCD.
	while ((elementsLeft > 0) &&
	       ((element->startPage + element->height - 1) < form->firstVisiblePage)) {
		element = element->next;
		--elementsLeft;
	}

	// Find first page to draw within element.
	FORMS_size_t internalPage = form->firstVisiblePage - element->startPage;

	// Draw nodes page by page until end of LCD reached or no more elements left.
	uint8_t lcdPage = LCD_GetTopPage();
	uint8_t lcdPagesLeft = LCD_PAGE_COUNT;
	while ((lcdPagesLeft > 0) && (elementsLeft > 0)) {
		while ((lcdPagesLeft > 0) && (internalPage < element->height)) {

			///mt element->traits->DrawPage( element, internalPage, lcdPage );
			FORMS_DrawPageFunc_t drawPageFunc;
			drawPageFunc = (FORMS_DrawPageFunc_t)(CAL_pgm_read_pvoid(&(element->traits->DrawPage)));
			drawPageFunc( element, internalPage, lcdPage );

			++internalPage;
			++lcdPage;
			if (lcdPage >= LCD_PAGE_COUNT) {
				lcdPage = 0;
			}
			--lcdPagesLeft;
		}
		internalPage = 0;
		element = element->next;
		--elementsLeft;
	}	

	// Draw non-element form graphics.
	if (form->useScrollbar) {
		FORMS_DrawScrollbar( form );
	}
	FORMS_DrawFocusIndicator( form );
}


/*!
 * This function ensures that the currently focused element of a form
 * is visible, scrolling the form as necessary to bring the focused element onto
 * the LCD screen. This function also takes care of redrawing parts of the LCD
 * the requires it.
 *
 * \param  form  Pointer to the form to operate on.
 */
void FORMS_EnsureFocusedIsVisible( FORMS_form_t * form )
{
	// Calculate start pages if required.
	if (form->needsRecalculation) {
		FORMS_CalculateStartPages( form );
	}

	// We use this a lot below, so we create a shortcut to it here. Easier to read.
	FORMS_element_t const * const element = form->focusedElement;
	
	// Check if focused element is above visible area.
	if ((element->startPage + element->focusedPage) < form->firstVisiblePage) {
		// How much do we need to scroll in order to show focused page within element?
		FORMS_size_t scrollAmount = form->firstVisiblePage -
		                              (element->startPage + element->focusedPage);
		
		// If we can show the entire element, we scroll until entire element is visisble.
		if (element->height <= LCD_PAGE_COUNT) {
			scrollAmount += element->focusedPage;
		}
		
		// Now, scroll the desired amount of pages.
		FORMS_ScrollDown( form, scrollAmount );
	}
	// If not, check if focused element is below visible area.
	else if ((element->startPage + element->focusedPage) >
	         (form->firstVisiblePage + LCD_PAGE_COUNT - 1)) {
		// How much do we need to scroll in order to show focused page within element?
		FORMS_size_t scrollAmount = (element->startPage + element->focusedPage) -
		                              (form->firstVisiblePage + LCD_PAGE_COUNT - 1);
		
		// If we can show the entire element, we scroll until entire element is visisble.
		if (element->height <= LCD_PAGE_COUNT) {
			scrollAmount += element->height - element->focusedPage - 1;
		}

		// Now, scroll the desired amount of pages.
		FORMS_ScrollUp( form, scrollAmount );
	}
	
	// Redraw focus indicator as well, in case an element changed its internal focus.
	FORMS_DrawFocusIndicator( form );
}


/*!
 * This function is a helper function used to fill in information in an element visibility
 * structure. Given a form and an element within that form, this function calculates
 * which parts, if any, of the element that are visible if the form is drawn on the LCD.
 * This function is typically used within drawing and refresh functions for specific widgets.
 * Note that there is no check if the element is actually a member of the form.
 *
 * \param  form        Pointer to form.
 * \param  element     Pointer to element within form.
 * \param  visibility  Pointer to uninitialized element visibility struct.
 */
void FORMS_CalculateVisibility( FORMS_form_t * form, FORMS_element_t const * element, FORMS_elementVisibility_t * visibility )
{
	// Copy form pointer for later reference.
	visibility->form = form;
	
	// Check if element is outside visible area.
	if (((element->startPage + element->height - 1) < form->firstVisiblePage) ||
	    (element->startPage > (form->firstVisiblePage + LCD_PAGE_COUNT - 1))) {
		visibility->firstPage = element->height; // Past end of element.
		visibility->lastPage = element->height;
	}
	// If not, check how much is visible.
	else {
		// Check if top of element is above top of visible area.
		if (element->startPage < form->firstVisiblePage) {
			visibility->firstPage = form->firstVisiblePage - element->startPage;
		} else {
			visibility->firstPage = 0;
		}
		// Calculate which LCD page corresponds to first visible element page.
		visibility->firstLCDPage = LCD_ROW2PAGE(
				(element->startPage + visibility->firstPage) -
				form->firstVisiblePage );
		
		// Check if bottom of element is below bottom of visible area.
		if ((element->startPage + element->height) > (form->firstVisiblePage + LCD_PAGE_COUNT)) {
			visibility->lastPage = (form->firstVisiblePage + LCD_PAGE_COUNT - 1) -
					element->startPage;
		} else {
			visibility->lastPage = element->height - 1;
		}
	}
}


/*!
 * This function handles a joystick event for a form. It takes care of updating
 * internal states, performing navigation and scrolling if required, or directing
 * joystick events to the currently focused element in the form.
 * It also takes care of redrawing necessary element and graphics.
 * This function potentially takes some time to execute, depending on the
 * complexity of the form elements involved, and should therefore not be called
 * from an interrupt handler. Instead, if joystick events are created from an
 * interrupt handler, queue the events up and pass them on to this function
 * outside the interrupt handler domain.
 *
 * \param  form   Pointer to form that should receive the joystick event.
 * \param  event  Pointer to joystick event information. Will not be changed.
 */
void FORMS_OnJoystick( FORMS_form_t * form, JOYSTICK_event_t const * event )
{
	// Let currently focused element process event first, unless we are scrolling the form right now.
	bool eventProcessedByElement = false;
	if ((form->elementCount > 0) && (form->isScrolling == false)) {
		// Prepare element visibility in case it wants to do anything graphical.
		FORMS_elementVisibility_t visibility;
		FORMS_CalculateVisibility( form, form->focusedElement, &visibility );
		
		///mt eventProcessedByElement = form->focusedElement->traits->OnJoystick( form->focusedElement, event, &visibility );
		FORMS_OnJoystickEventFunc_t joystickEventFunc;
		joystickEventFunc = (FORMS_OnJoystickEventFunc_t)(CAL_pgm_read_pvoid(&(form->focusedElement->traits->OnJoystick)));
		eventProcessedByElement = joystickEventFunc( form->focusedElement, event, &visibility );
		
	}

	// If element did not use event, the form itself can process it.
	if (eventProcessedByElement == false) {
		// Stop repetitions if active.
		if ((event->released & (JOYSTICK_UP | JOYSTICK_DOWN)) != 0x00) {
			FORMS_StopRepeating();
		}
		
		// Check for scroll without navigation first.
		if ((event->state & JOYSTICK_ENTER) != 0x00) {
			// Check for single step scroll up.
			if ((event->clicked & JOYSTICK_UP) != 0x00) {
				FORMS_ScrollDown( form, 1 );
				form->isScrolling = true;		
			}
			// Check for single step scroll down.
			else if ((event->clicked & JOYSTICK_DOWN) != 0x00) {
				FORMS_ScrollUp( form, 1 );
				form->isScrolling = true;		
			}
			// Check for press-and-hold up, which means repeated scroll.
			else if ((event->held & JOYSTICK_UP) != 0x00) {
				TIMING_AddRepCallbackEvent( TIMING_INFINITE_REPEAT,
				                            FORMS_REPEAT_DELAY,
				                            FORMS_ScrollDownCallback,
				                            FORMS_PrepareRepeatingForm( form ) );
				form->isScrolling = true;		
			}
			// Check for press-and-hold down, which means repeated scroll.
			else if ((event->held & JOYSTICK_DOWN) != 0x00) {
				TIMING_AddRepCallbackEvent( TIMING_INFINITE_REPEAT,
				                            FORMS_REPEAT_DELAY,
				                            FORMS_ScrollUpCallback,
				                            FORMS_PrepareRepeatingForm( form ) );
				form->isScrolling = true;		
			}
		}
		// If not scrolling, check for plain navigation. Left/right only ensures
		// focused element is visible.
		else {
			form->isScrolling = false;
			
			// Check for single step navigation first.
			if ((event->clicked & JOYSTICK_UP) != 0x00) {
				FORMS_NavigateUp( form );
			}
			else if ((event->clicked & JOYSTICK_DOWN) != 0x00) {
				FORMS_NavigateDown( form );
			}
			else if ((event->clicked & (JOYSTICK_LEFT | JOYSTICK_RIGHT)) != 0x00) {
				FORMS_EnsureFocusedIsVisible( form );
			}
			// Then check for press-and-hold, which means repeated navigation.
			else if ((event->held & JOYSTICK_UP) != 0x00) {
				TIMING_AddRepCallbackEvent( TIMING_INFINITE_REPEAT,
				                            FORMS_REPEAT_DELAY,
				                            FORMS_NavigateUpCallback,
				                            FORMS_PrepareRepeatingForm( form ) );
			}
			else if ((event->held & JOYSTICK_DOWN) != 0x00) {
				TIMING_AddRepCallbackEvent( TIMING_INFINITE_REPEAT,
				                            FORMS_REPEAT_DELAY,
				                            FORMS_NavigateDownCallback,
				                            FORMS_PrepareRepeatingForm( form ) );
			}
		}
	}
}


/*!
 * This function scrolls the forms contents up a certain amount of
 * 8-pixel pages, utilizing the hardware scrolling capabilities of the LCD.
 * Note that this function expects that the form is already displayed on screen.
 *
 * \param  form          Pointer to form.
 * \param  scrollAmount  Number of pages to scroll.
 */
void FORMS_ScrollUp( FORMS_form_t * form, FORMS_size_t scrollAmount )
{
	// Calculate start pages if required.
	if (form->needsRecalculation) {
		FORMS_CalculateStartPages( form );
	}
	
	// Make sure we do not scroll past bottom of form.
	if (form->height <= LCD_PAGE_COUNT) {
		scrollAmount = 0;
	}
	else if (scrollAmount > (form->height - form->firstVisiblePage - LCD_PAGE_COUNT)) {
		scrollAmount = form->height - form->firstVisiblePage - LCD_PAGE_COUNT;
	}

	// Setup iteration variables.
	FORMS_element_t const * element = form->firstElement;
	FORMS_size_t elementsLeft = form->elementCount;

	// Find first element that have parts just below the LCD.
	while ((elementsLeft > 0) &&
	       ((element->startPage + element->height - 1) < (form->firstVisiblePage + LCD_PAGE_COUNT))) {
		element = element->next;
		--elementsLeft;
	}

	// Find first page to draw within element.
	FORMS_size_t internalPage = (form->firstVisiblePage + LCD_PAGE_COUNT) -
	                              element->startPage;

	// Now, scroll page by page.
	while (scrollAmount > 0) {
		// Current top LCD page will become bottom after scrolling, so we save it for later.
		uint8_t oldTopPage = LCD_GetTopPage();

		// Let LCD hardware scroll (actually rotate) eight pixels for us.
		if (LCD_GetTopPage() < (LCD_PAGE_COUNT - 1)) {
			LCD_SetTopPage( LCD_GetTopPage() + 1, 0 );
		} else {
			LCD_SetTopPage( 0, 0 );
		}
		++(form->firstVisiblePage);
		
		// Clear old graphics and ask element to fill in new bottom page contents.
		LCD_ClrPage( 0xff, oldTopPage, FORMS_FIRST_COLUMN, FORMS_LAST_COLUMN - FORMS_FIRST_COLUMN + 1 );
		
		///mt element->traits->DrawPage( element, internalPage, oldTopPage );
		FORMS_DrawPageFunc_t drawPageFunc;
		drawPageFunc = (FORMS_DrawPageFunc_t)(CAL_pgm_read_pvoid(&(element->traits->DrawPage)));
		drawPageFunc( element, internalPage, oldTopPage );
		
		// Update non-element form graphics.
		if (form->useScrollbar) {
			FORMS_DrawScrollbar( form );
		}
		FORMS_DrawFocusIndicator( form );

		// One less page to scroll. Iterate through elements if more scrolling to come.
		--scrollAmount;
		if (scrollAmount > 0) {
			if (internalPage < (element->height - 1)) {
				++internalPage;
			} else {
				element = element->next;
				internalPage = 0;
			}
		}
	}
}


/*!
 * This function scrolls the forms contents down a certain amount of
 * 8-pixel pages, utilizing the hardware scrolling capabilities of the LCD.
 * Note that this function expects that the form is already displayed on screen.
 *
 * \param  form          Pointer to form.
 * \param  scrollAmount  Number of pages to scroll.
 */
void FORMS_ScrollDown( FORMS_form_t * form, FORMS_size_t scrollAmount )
{
	// Calculate start pages if required.
	if (form->needsRecalculation) {
		FORMS_CalculateStartPages( form );
	}

	// Make sure we do not scroll past top of form.
	if (scrollAmount > form->firstVisiblePage) {
		scrollAmount = form->firstVisiblePage;
	}

	// Setup iteration variables.
	FORMS_element_t const * element = form->firstElement;
	FORMS_size_t elementsLeft = form->elementCount;

	// Find first element that have parts just above the LCD.
	while ((elementsLeft > 0) &&
	       ((element->startPage + element->height - 1) < (form->firstVisiblePage - 1))) {
		element = element->next;
		--elementsLeft;
	}

	// Find first page to draw within element.
	FORMS_size_t internalPage = (form->firstVisiblePage - 1) - element->startPage;

	// Now, scroll page by page.
	while (scrollAmount > 0) {
		// Let LCD hardware scroll (actually rotate) eight pixels for us.
		if (LCD_GetTopPage() > 0) {
			LCD_SetTopPage( LCD_GetTopPage() - 1, 0 );
		} else {
			LCD_SetTopPage( LCD_PAGE_COUNT - 1, 0 );
		}
		--(form->firstVisiblePage);

		// Clear old graphics and ask element to fill in new top page contents.
		LCD_ClrPage( 0xff, LCD_GetTopPage(), FORMS_FIRST_COLUMN, FORMS_LAST_COLUMN - FORMS_FIRST_COLUMN + 1 );
		
		///mt element->traits->DrawPage( element, internalPage, LCD_GetTopPage() );
		FORMS_DrawPageFunc_t drawPageFunc;
		drawPageFunc = (FORMS_DrawPageFunc_t)(CAL_pgm_read_pvoid(&(element->traits->DrawPage)));
		drawPageFunc( element, internalPage, LCD_GetTopPage() );

		// Update non-element form graphics.
		if (form->useScrollbar) {
			FORMS_DrawScrollbar( form );
		}
		FORMS_DrawFocusIndicator( form );

		// One less page to scroll. Iterate through elements if more scrolling to come.
		--scrollAmount;
		if (scrollAmount > 0) {
			if (internalPage > 0) {
				--internalPage;
			} else {
				element = element->prev;
				internalPage = element->height - 1;
			}
		}
	}
}


/*!
 * This function changes the form's focus to the next active element upwards.
 * Note that this function will not navigate inside large elements, but rather
 * forces another element into focus.
 *
 * \param  form  Pointer to form.
 */
void FORMS_NavigateUp( FORMS_form_t * form )
{
	// Exit if there are no elements in form.
	if (form->elementCount == 0) { return; }

	// Calculate start pages if required.
	if (form->needsRecalculation) {
		FORMS_CalculateStartPages( form );
	}

	// Change focus if not already on top.
	if (form->focusedElement != form->firstElement) {
		// Find first element that is active.
		FORMS_element_t * elementToFocus = form->focusedElement;
		do {
			elementToFocus = elementToFocus->prev;
		} while ((elementToFocus != form->firstElement) && (elementToFocus->isActive == false));

		// Notify old and new element about focus change if we found an active element.
		if ((elementToFocus != form->focusedElement) && (elementToFocus->isActive)) {
			FORMS_SetFocusedElement( form, elementToFocus );
		}
	}

	// Draw non-element form graphics.
	FORMS_DrawFocusIndicator( form );

	// Scroll graphics if necessary. Even if we did not change focus, we might
	// need to scroll, in case the form has been scrolled without navigating.
	FORMS_EnsureFocusedIsVisible( form );
}


/*!
 * This function changes the form's focus to the next active element downwards.
 * Note that this function will not navigate inside large elements, but rather
 * forces another element into focus.
 *
 * \param  form  Pointer to form.
 */
void FORMS_NavigateDown( FORMS_form_t * form )
{
	// Exit if there are no elements in form.
	if (form->elementCount == 0) { return; }

	// Calculate start pages if required.
	if (form->needsRecalculation) {
		FORMS_CalculateStartPages( form );
	}

	// Change focus if not already on bottom.
	if (form->focusedElement != form->lastElement) {
		// Find first element that is active.
		FORMS_element_t * elementToFocus = form->focusedElement;
		do {
			elementToFocus = elementToFocus->next;
		} while ((elementToFocus != form->lastElement) && (elementToFocus->isActive == false));

		// Notify old and new element about focus change if we found an active element.
		if ((elementToFocus != form->focusedElement) && (elementToFocus->isActive)) {
			FORMS_SetFocusedElement( form, elementToFocus );
		}
	}

	// Draw non-element form graphics.
	FORMS_DrawFocusIndicator( form );

	// Scroll graphics if necessary. Even if we did not change focus, we might
	// need to scroll, in case the form has been scrolled without navigating.
	FORMS_EnsureFocusedIsVisible( form );
}


/*!
 * This function aquires the globally shared repeatState struct and associates
 * a form pointer to it. The repeatState can then be used for repeating actions
 * on that form, such as repeating navigation or scrolling.
 *
 * This function returns a pointer to the timing event assoicated with the
 * repeat state. This timing event should be passed on to the timing library
 * when setting up the delays and repeat counts.
 *
 * Note that this function will cancel any ongoing repeat actions.
 *
 * \param  form  Pointer to form that should be associated with the repeatState.
 *
 * \return  Pointer to the timing event struct.
 */
TIMING_event_t * FORMS_PrepareRepeatingForm( FORMS_form_t * form )
{
	BEGIN_CRITICAL_SECTION
		if ((FORMS_repeatState.form != NULL) || (FORMS_repeatState.element != NULL)) {
			TIMING_RemoveEvent( &(FORMS_repeatState.timingEvent) );
		}
	END_CRITICAL_SECTION
	
	FORMS_repeatState.form = form;
	
	return &(FORMS_repeatState.timingEvent);
}


/*!
 * This function aquires the globally shared repeatState struct and associates
 * a form element pointer to it, along with a prepared element visibility struct.
 * The repeatState can then be used for repeating actions on that element, such
 * as repeated spinning, text, browsing etc.
 *
 * This function returns a pointer to the timing event assoicated with the
 * repeat state. This timing event should be passed on to the timing library
 * when setting up the delays and repeat counts.
 *
 * Note that this function will cancel any ongoing repeat actions.
 *
 * \param  element     Pointer to form element that should be associated with the repeatState.
 * \param  visibility  Pointer to prepared element visibility struct.
 *
 * \return  Pointer to the timing event struct.
 */
TIMING_event_t * FORMS_PrepareRepeatingElement( FORMS_element_t * element, FORMS_elementVisibility_t const * visibility )
{
	BEGIN_CRITICAL_SECTION
		if ((FORMS_repeatState.form != NULL) || (FORMS_repeatState.element != NULL)) {
			TIMING_RemoveEvent( &(FORMS_repeatState.timingEvent) );
		}
	END_CRITICAL_SECTION
	
	FORMS_repeatState.element = element;
	FORMS_repeatState.visibility = *visibility;
	
	return &(FORMS_repeatState.timingEvent);
}


/*!
 * This function returns the form pointer currently associated
 * with the global repeatState struct. Note that there is no check
 * whether any repeat action is currently active, nor that it is
 * associated with a form or not.
 *
 * \return  Pointer to form associated with repeatState.
 */
FORMS_form_t * FORMS_GetRepeatingForm( void )
{
	return FORMS_repeatState.form;
}


/*!
 * This function returns the form element pointer currently associated
 * with the global repeatState struct. Note that there is no check
 * whether any repeat action is currently active, nor that it is
 * associated with a form element or not.
 *
 * \return  Pointer to form element associated with repeatState.
 */
FORMS_element_t * FORMS_GetRepeatingElement( void )
{
	return FORMS_repeatState.element;
}


/*!
 * This function returns the element visibility struct pointer currently associated
 * with the global repeatState struct. Note that there is no check
 * whether any repeat action is currently active, nor that it is
 * associated with a form element or not.
 *
 * \return  Pointer to element visibility struct associated with repeatState.
 */
FORMS_elementVisibility_t const * FORMS_GetRepeatingVisibility( void )
{
	return &(FORMS_repeatState.visibility);
}


/*!
 * This function cancels any ongoing repeat action and removes the timing event
 * from the timing library queue.
 */
void FORMS_StopRepeating( void )
{
	BEGIN_CRITICAL_SECTION
		if ((FORMS_repeatState.form != NULL) || (FORMS_repeatState.element != NULL)) {
			TIMING_RemoveEvent( &(FORMS_repeatState.timingEvent) );
		}
	END_CRITICAL_SECTION
}


/*!
 * This function calls the scroll up function for the form currently
 * associated with the global repeat state. This function is used as the
 * callback when using the timing library to perform the repeated action.
 */
static void FORMS_ScrollUpCallback( void )
{
	FORMS_form_t * form = FORMS_GetRepeatingForm();
	FORMS_ScrollUp( form, 1 );
}


/*!
 * This function calls the scroll down function for the form currently
 * associated with the global repeat state. This function is used as the
 * callback when using the timing library to perform the repeated action.
 */
static void FORMS_ScrollDownCallback( void )
{
	FORMS_form_t * form = (FORMS_form_t *) FORMS_GetRepeatingForm();
	FORMS_ScrollDown( form, 1 );
}


/*!
 * This function calls the navigate up function for the form currently
 * associated with the global repeat state. This function is used as the
 * callback when using the timing library to perform the repeated action.
 */
static void FORMS_NavigateUpCallback( void )
{
	FORMS_form_t * form = (FORMS_form_t *) FORMS_GetRepeatingForm();
	FORMS_NavigateUp( form );
}


/*!
 * This function calls the navigate down function for the form currently
 * associated with the global repeat state. This function is used as the
 * callback when using the timing library to perform the repeated action.
 */
static void FORMS_NavigateDownCallback( void )
{
	FORMS_form_t * form = (FORMS_form_t *) FORMS_GetRepeatingForm();
	FORMS_NavigateDown( form );
}

/*!
 * This function walks through all elements of a form and recalculates each
 * element's start page, and finally the resulting totalt height of the form.
 * Form drawing functions automatically call this function when elements have
 * been added or removed from the form.
 *
 * \param  form  Pointer to form.
 */
static void FORMS_CalculateStartPages( FORMS_form_t * form )
{
	// Setup iteration variables.
	FORMS_element_t * element = form->firstElement;
	FORMS_size_t elementsLeft = form->elementCount;
	FORMS_size_t currentHeight = 0;

	// Set start pages - element by element.
	while (elementsLeft > 0) {
		element->startPage = currentHeight;
		currentHeight += element->height;
		element = element->next;
		--elementsLeft;
	}

	// Now, we know the height of the entire form.
	form->height = currentHeight;
	
	// Make sure form is not scrolled beyond bottom now when we have recalculated.
	if (form->height < LCD_PAGE_COUNT) {
		form->firstVisiblePage = 0;
	}
	else if ((form->firstVisiblePage + LCD_PAGE_COUNT) > form->height) {
		form->firstVisiblePage = currentHeight - LCD_PAGE_COUNT;
	}
	
	// Indicate that we have recalculated start pages.
	form->needsRecalculation = false;
}


/*!
 * This function is called by drawing and refreshing functions as
 * one of the final steps after navigation and/or scrolling has happened.
 * It draws the divider line at the right edge of the screen, and the scroll
 * position indicator if required.
 *
 * \param  form  Pointer to form.
 */
static void FORMS_DrawScrollbar( FORMS_form_t const * form )
{
	// Draw vertical separator and clear old bar.
	LCD_SetVLine( LCD_WIDTH - 3, 0, LCD_HEIGHT - 1 );
	LCD_ClrVLine( LCD_WIDTH - 2, 0, LCD_HEIGHT - 1 );
	LCD_ClrVLine( LCD_WIDTH - 1, 0, LCD_HEIGHT - 1 );

	// Draw scroll position indicator if form is not empty.
	if (form->height > LCD_PAGE_COUNT) {
		// Indicator height shows the size of the visible parts
		// related to the entire form height.
		uint16_t heightOfBar = (uint16_t) LCD_HEIGHT * LCD_PAGE_COUNT / form->height;
		
		// Make sure the bar is not too small or too large.
		if (heightOfBar == 0) {
			heightOfBar = 1;
		}
		else if (heightOfBar > LCD_HEIGHT) {
			heightOfBar = LCD_HEIGHT;
		}

		// Position shows the relative position of the visible area within the entire form.
		uint16_t startOfBar = form->firstVisiblePage * (LCD_HEIGHT - heightOfBar) /
		                      (form->height - LCD_PAGE_COUNT);

		// Adjust start according to LCD hardware scrolling.
		startOfBar = (startOfBar + (LCD_GetTopPage() * LCD_PAGE_HEIGHT)) % LCD_HEIGHT;

		// If bar wraps from bottom to top after adjusting it, draw it in two steps.
		if ((startOfBar + heightOfBar) > LCD_HEIGHT) {
			LCD_SetVLine( LCD_WIDTH - 2, startOfBar, LCD_HEIGHT - 1 );
			LCD_SetVLine( LCD_WIDTH - 1, startOfBar, LCD_HEIGHT - 1 );
			LCD_SetVLine( LCD_WIDTH - 2, 0, (startOfBar + heightOfBar) - LCD_HEIGHT - 1 );
			LCD_SetVLine( LCD_WIDTH - 1, 0, (startOfBar + heightOfBar) - LCD_HEIGHT - 1 );
		} else {
			LCD_SetVLine( LCD_WIDTH - 2, startOfBar, startOfBar + heightOfBar - 1 );
			LCD_SetVLine( LCD_WIDTH - 1, startOfBar, startOfBar + heightOfBar - 1 );
		}
	}
}


/*!
 * This function is called by drawing and refreshing functions as
 * one of the final steps after navigation and/or scrolling has happened.
 * It draws the focus arrow next to the current focused page on screen.
 *
 * \param  form  Pointer to form.
 */
static void FORMS_DrawFocusIndicator( FORMS_form_t const * form )
{
	// Clear old indicator.
	for (uint8_t col = 0; col < FORMS_FIRST_COLUMN; ++col) {
		LCD_ClrVLine( col, 0, LCD_HEIGHT );
	}

	// Check that focused element is within visible area.
	FORMS_size_t focusedPage = form->focusedElement->startPage +
	                             form->focusedElement->focusedPage;
	if ((focusedPage >= form->firstVisiblePage) &&
	    (focusedPage < (form->firstVisiblePage + LCD_PAGE_COUNT))) {
		// Find start position for indicator, adjust for LCD hardware scrolling.
		uint8_t lcdPage = LCD_ROW2PAGE( focusedPage - form->firstVisiblePage );

		// Draw indicator.
		LCD_WritePage_F( FORMS_focusIndicator, lcdPage, 0, FORMS_FIRST_COLUMN );
	}
}


// end of file
