// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  FIFO library header file
 *
 *      The FIFO queue library provides functions for managing a
 *      first-in-first-out byte-oriented queue. The FIFO queue library does not
 *      allocate memory itself, so that user must provide free memory buffers
 *      when new FIFO queues are initialized. The FIFO queue is then used as a
 *      circular buffer with Put and Get routines for standard and custom data
 *      types of any size.
 *
 *      Note that the FIFO queues are internally byte-oriented, which means that
 *      there is no guarantee that a larger block of data is not split up if the
 *      internal buffer pointers wraps back to start from the end of the memory
 *      buffer. However, being byte-oriented queues, the user is not limited to
 *      only one type of data in the queue. If great care is taken, the queue
 *      can contain any mix of data types.
 *
 *      Both the remove and the insert functions are blocking, which means
 *      that if there is no data to remove or no room for inserting data, the
 *      functions wait until there is. This could be done by e.g. an interrupt
 *      handler inserting or removing data, respectively. Insert and remove
 *      operations from within interrupt handlers should use the macros
 *      "FIFO_QuickPutData" and "FIFO_QuickGetData" in order to avoid the
 *      function call overhead and not manipulate interrupt status.
 *
 *      For future compatibility, never access the FIFO struct members directly.
 *      Always use the provided functions instead. Also make sure you use the
 *      data type FIFO_data_t when allocating memory buffers, instead of uint8_t
 *      or similar byte-types.
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
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/fifo_lib/fifo_lib.h $
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

#ifndef FIFO_LIB_H
#define FIFO_LIB_H

#include <stdint.h>
#include <common.h>


/***********************
 * Typedefs and structs.
 ***********************/
 
typedef uint8_t FIFO_data_t;  //!< Data type for FIFO storage buffer.
typedef uint8_t FIFO_size_t;  //!< Data type for all sizes regarding FIFO buffers and blocks.

struct FIFO_handle_struct;  //!< Forward definition used by the callback typedefs below.
typedef struct FIFO_handle_struct FIFO_handle_t;  //!< Forward definition used by the callback typedefs below.

typedef void (* FIFO_ProducerCallback_t)( FIFO_handle_t volatile * fifo );  //!< Function called when FIFO goes from full to not-full.
typedef void (* FIFO_ConsumerCallback_t)( FIFO_handle_t volatile * fifo );  //!< Function called when FIFO goes from empty to not-empty.

//! The FIFO control block. Holds all information regarding one FIFO instance.
struct FIFO_handle_struct
{
	FIFO_data_t * bufferStart;  //!< Pointer to first location in memory buffer.
	FIFO_data_t * bufferEnd;    //!< Pointer to last location in memory buffer.
	FIFO_data_t * head;         //!< Pointer to location into which next data is inserted.
	FIFO_data_t * tail;         //!< Pointer to location from which last data was removed.
	FIFO_size_t itemsFree;      //!< Number of items that can be inserted before buffer is full.
	FIFO_size_t itemsUsed;      //!< Number of ltems that can be removed before buffer is empty.
	FIFO_ProducerCallback_t ProducerCallback;  //!< Function called when FIFO goes from full to not-full.
	FIFO_ConsumerCallback_t ConsumerCallback;  //!< Function called when FIFO goes from empty to not-empty.
};



/*********************************************************************************************
 * Function prototypes. The Put and Get functions are generally documented in the file header.
 *********************************************************************************************/

//! Initialize a FIFO structure with initial states and buffer association.
void FIFO_Init( FIFO_handle_t * handle, FIFO_data_t * buffer, FIFO_size_t size );

//! Assign new (replacing old) producer callback.
void FIFO_RegisterProducer( FIFO_handle_t * handle, FIFO_ProducerCallback_t ProducerCallback );
//! Assign new (replacing old) consumer callback.
void FIFO_RegisterConsumer( FIFO_handle_t * handle, FIFO_ConsumerCallback_t ConsumerCallback );
//! Remove all items from FIFO.
void FIFO_Flush( FIFO_handle_t * handle );

//! Transfer data from one FIFO to another.
void FIFO_Transfer( FIFO_handle_t * source, FIFO_handle_t * destination, FIFO_size_t size );

//! Insert an unsigned byte into FIFO.
void FIFO_PutUint8 ( FIFO_handle_t * handle, uint8_t  data );
//! Insert a signed byte into FIFO.
void FIFO_PutInt8  ( FIFO_handle_t * handle, int8_t   data );
//! Insert an unsigned word into FIFO.
void FIFO_PutUint16( FIFO_handle_t * handle, uint16_t data );
//! Insert a signed word into FIFO.
void FIFO_PutInt16 ( FIFO_handle_t * handle, int16_t  data );
//! Insert an unsigned dword into FIFO.
void FIFO_PutUint32( FIFO_handle_t * handle, uint32_t data );
//! Insert a signed dword into FIFO.
void FIFO_PutInt32 ( FIFO_handle_t * handle, int32_t  data );
//! Insert a data block with "size" bytes from memory pointed to by "data" into FIFO.
void FIFO_PutBlock ( FIFO_handle_t * handle, const void * data, FIFO_size_t size );  

//! Remove an unsigned byte from FIFO.
uint8_t  FIFO_GetUint8 ( FIFO_handle_t * handle );
//! Remove a signed byte from FIFO.
int8_t   FIFO_GetInt8  ( FIFO_handle_t * handle );
//! Remove an unsigned word from FIFO.
uint16_t FIFO_GetUint16( FIFO_handle_t * handle );
//! Remove a signed word from FIFO.
int16_t  FIFO_GetInt16 ( FIFO_handle_t * handle );
//! Remove an unsigned dword from FIFO.
uint32_t FIFO_GetUint32( FIFO_handle_t * handle );
//! Remove a signed dword from FIFO.
int32_t  FIFO_GetInt32 ( FIFO_handle_t * handle );
//! Remove a data block with "size" bytes from FIFO into memory pointed to by "data".
void     FIFO_GetBlock ( FIFO_handle_t * handle, void * data, FIFO_size_t size );



/*********************
 * Convenience macros.
 *********************/

//! Get number of items that can be inserted before FIFO is full.
#define FIFO_GetItemsFree(handle)     ((handle)->itemsFree)
//! Get number of items that can be removed before FIFO is empty.
#define FIFO_GetItemsUsed(handle)     ((handle)->itemsUsed)
//! Return true if FIFO is full.
#define FIFO_IsFull(handle)           (FIFO_GetItemsFree( (handle) ) == 0)
//! Return true if FIFO is empty.
#define FIFO_IsEmpty(handle)          (FIFO_GetItemsUsed( (handle) ) == 0)
//! Return true if there is room for inserting value of type "datatype" into FIFO.
#define FIFO_HasRoomFor(handle,datatype)  (FIFO_GetItemsFree( (handle) ) >= sizeof(datatype))
//! Return true if there is enough data to remove value of type "datatype" from FIFO.
#define FIFO_HasData(handle,datatype)  (FIFO_GetItemsUsed( (handle) ) >= sizeof(datatype))
//! Put one arbitrary data type into FIFO.
#define FIFO_PutData(handle,ptr)  FIFO_PutBlock( (handle), (void *) (ptr), sizeof(*(ptr)) );
//! Get one arbitrary data type from FIFO.
#define FIFO_GetData(handle,ptr)  FIFO_GetBlock( (handle), (void *) (ptr), sizeof(*(ptr)) );


/**************************************************************************
 * Frequenctly used functions implemented as macros for speed optimization.
 **************************************************************************/

/*! \brief  Macro for inserting FIFO data from within interrupt handlers.
 *
 *  This macro inserts one data item "data" into the FIFO referred to
 *  by the "handle" pointer. It is very similar to the internal "FIFO_PutData".
 *
 *  Since this macro does not check whether the FIFO has room for data or not,
 *  the caller must enure this by checking first, or by design.
 *
 *  This macro modifies the FIFO state without disabling interrupts. This means
 *  that this macro should only be called from within an interrupt handler.
 *  If called from application code, make sure no other interrupts would access
 *  the FIFO at the same time, otherwise the FIFO could be corrupted.
 */
#define FIFO_QuickPutData(handle,data)           \
{	                                             \
	*((handle)->head) = (data);                  \
	if ((handle)->head == (handle)->bufferEnd) { \
		(handle)->head = (handle)->bufferStart;  \
	} else {                                     \
		++((handle)->head);                      \
	}                                            \
	                                             \
	--((handle)->itemsFree);                     \
	++((handle)->itemsUsed);                     \
}
// end


/*! \brief  Macro for removing FIFO data from within interrupt handlers.
 *
 *  This macro removes one data item "data" from the FIFO referred to
 *  by the "handle" pointer. It is very similar to the internal "FIFO_GetData".
 *
 *  Since this macro does not check whether the FIFO has data or not,
 *  the caller must enure this by checking first, or by design.
 *
 *  This macro modifies the FIFO state without disabling interrupts. This means
 *  that this macro should only be called from within an interrupt handler.
 *  If called from application code, make sure no other interrupts would access
 *  the FIFO at the same time, otherwise the FIFO could be corrupted.
 *
 *  The macro is implemented using the ternary "?:" operator and the
 *  comma operator. This is due to the macro being used as an expression
 *  returning a value. "if/else"-constructs are replaced by the ternary
 *  "?:" operator. All statements are separated by commas instead of
 *  semicolons, in order to evaluate all statements, but letting the whole
 *  macro return the value of the last statement only.
 *
 *  Refer to a C syntax reference for details on the ternary "?:" operator and
 *  the comma operator.
 */
#define FIFO_QuickGetData(handle)                \
(	                                             \
	((handle)->tail == (handle)->bufferEnd) ? (  \
		(handle)->tail = (handle)->bufferStart   \
	) : (                                        \
		++((handle)->tail)                       \
	),	                                         \
	                                             \
	++((handle)->itemsFree),                     \
	--((handle)->itemsUsed),                     \
                                                 \
	*((handle)->tail)                            \
)	                                             \
// end

//! Convenience macro to insert "uint8_t" data using the "FIFO_QuickPutData" macro.
#define FIFO_QuickPutUint8(handle,data)  FIFO_QuickPutData( (handle), (FIFO_data_t) (data) )
//! Convenience macro to remove "uint8_t" data using the "FIFO_QuickGetData" macro.
#define FIFO_QuickGetUint8(handle)       ((uint8_t) FIFO_QuickGetData( (handle) ))



#endif
