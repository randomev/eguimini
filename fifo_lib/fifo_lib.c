// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  FIFO library source file
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
 * $Revision: 2416 $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/fifo_lib/fifo_lib.c $
 * $Date: 2007-09-17 02:23:27 -0600 (ma, 17 sep 2007) $  \n
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

#include "fifo_lib.h"


/*************************************************************************************
 * Internal functions for data access. Used by all other insert and remove operations.
 *************************************************************************************/

/*! \brief Puts one byte of data in the FIFO
 *
 * Puts one byte of data into the FIFO.
 * If FIFO is full, it waits until something is removed
 * If the FIFO was empty, it adds the data and calls the handle's consumer
 * 
 * \param  handle  Which FIFO to add data to
 * \param  data  Data to add
*/
static void FIFO_InternalPutData( FIFO_handle_t volatile * handle, FIFO_data_t data )
{
	// If full, wait until someone (e.g. interrupt handler) removes data.
	do {} while (handle->itemsFree == 0);
	
	// Insert one data item at head and move head pointer.
	*(handle->head) = data;
	FIFO_data_t * bufferEnd = handle->bufferEnd; // Need to get this first, since struct is volatile.
	if (handle->head == bufferEnd) {
		handle->head = handle->bufferStart;
	} else {
		++(handle->head);
	}

	bool shouldCallConsumer = false; // Will be set to true if FIFO was empty when Put was called.

	// Critical section: Update data counters.
	uint8_t storedSREG = SREG;
	CAL_disable_interrupt();
	if (handle->itemsUsed == 0) { shouldCallConsumer = true; }
	--(handle->itemsFree);
	++(handle->itemsUsed);
	SREG = storedSREG;
	
	// Notify registered consumer.
	if (shouldCallConsumer == true) {
		if (handle->ConsumerCallback != NULL) {
			handle->ConsumerCallback( handle );
		}
	}
}

/*! \brief Gets one byte from a FIFO
 * 
 * Gets one byte from the FIFO
 * If empty, waits until something is added
 * If FIFO was full when called, it calls the consumer of the handle
 *
 * \param  handle  From which FIFO to get data
 *
 * \retval  Data  got from the handle
*/
static FIFO_data_t FIFO_InternalGetData( FIFO_handle_t volatile * handle )
{
	// If empty, wait until someone (e.g. interrupt handler) insert data.
	do {} while (handle->itemsUsed == 0);
	
	// Move tail pointer.
	FIFO_data_t * bufferEnd = handle->bufferEnd; // Need to get this first, since struct is volatile.
	if (handle->tail == bufferEnd) {
		handle->tail = handle->bufferStart;
	} else {
		++(handle->tail);
	}
	
	bool shouldCallProducer = false; // Will be set to true if FIFO was full when Get was called.

	// Critical section: Update data counters.
	uint8_t storedSREG = SREG;
	CAL_disable_interrupt();
	if (handle->itemsFree == 0) { shouldCallProducer = true; }
	++(handle->itemsFree);
	--(handle->itemsUsed);
	SREG = storedSREG;

	// Notify registered producer.
	if (shouldCallProducer == true) {
		if (handle->ProducerCallback != NULL) {
			handle->ProducerCallback( handle );
		}
	}

	// Return data item at current tail position.
	return *(handle->tail);
}



/***************************
 * Function implementations.
 ***************************/

/*! \brief  Initialize a FIFO structure with initial states and buffer association.
 *
 *  This function sets up the FIFO structure pointed to by "handle" to an
 *  initial state, given the buffer pointer "buffer" and its size "size".
 *
 *  The caller must provide memory for the FIFO structure and the buffer itself.
 *  Also, the caller must ensure that the buffer memory is valid during the
 *  entire lifetime of the FIFO structure.
 *
 *  The memory buffer must be of type FIFO_data_t, or must be casted explicitly
 *  to this type when calling FIFO_Init.
 *
 *  \param  handle  Pointer to FIFO state structure.
 *  \param  buffer  Pointer to memory buffer to be used by the FIFO.
 *  \param  size    Size of the memory buffer in bytes (actually number of FIFO_data_t's).
 */
void FIFO_Init( FIFO_handle_t * handle, FIFO_data_t * buffer, FIFO_size_t size )
{
	// Initialize to an empty state, ready for data.
	handle->bufferStart = buffer;
	handle->bufferEnd = buffer + size - 1;
	
	handle->head = handle->bufferStart;
	handle->tail = handle->bufferEnd;
	
	handle->itemsFree = size;
	handle->itemsUsed = 0;
	
	handle->ProducerCallback = NULL;
	handle->ConsumerCallback = NULL;
}


/*
 * \param  handle  Pointer to a FIFO struct
 * \param  ProducerCallback  Function to call when FIFO goes from full to non-full
 */
void FIFO_RegisterProducer( FIFO_handle_t * handle, FIFO_ProducerCallback_t ProducerCallback )
{
	handle->ProducerCallback = ProducerCallback;
}


/*
 * \param  handle  Pointer to a FIFO struct
 * \param  ConsumerCallback  Function to call when FIFO goes from empty to non-empty
 */
void FIFO_RegisterConsumer( FIFO_handle_t * handle, FIFO_ConsumerCallback_t ConsumerCallback )
{
	handle->ConsumerCallback = ConsumerCallback;
}


/*
 * \param  handler  Which FIFO to flush
 */
void FIFO_Flush( FIFO_handle_t * handle )
{
	// Read and discard all remaining contents.
	while (FIFO_IsEmpty( handle ) == false) {
		uint8_t volatile dummy = FIFO_GetUint8( handle );
	}
}

/*!
 *  This function repetitively removes data from FIFO pointed to by "source"
 *  and insterts into FIFO pointed to by "destination". This is repeated "size"
 *  times. Both the remove and the insert operation is blocking, which means
 *  that if there is no data to remove or no room for inserting data, the
 *  function waits until there is. This could be done by e.g. an interrupt
 *  handler inserting or removing data, respectively.
 *
 *  \param  source       Pointer to source FIFO state structure.
 *  \param  destination  Pointer to destination FIFO state structure.
 *  \param  size         Number of items (bytes) to transfer.
 */
void FIFO_Transfer( FIFO_handle_t * source, FIFO_handle_t * destination, FIFO_size_t size )
{
	// Repetitively remove data from source and insert into destination.
	while (size > 0) {
		FIFO_InternalPutData( destination, FIFO_InternalGetData( source ) );
		--size;
	}
}

/*
 * \param  handle  Pointer to FIFO to put data in
 * \param  data    8-bit unsigned integer to put into FIFO
 */
void FIFO_PutUint8( FIFO_handle_t * handle, uint8_t data )
{
	// Cast data to FIFO data type and insert.
	FIFO_InternalPutData( handle, (FIFO_data_t) data );
}


/*
 * \param  handle  Pointer to FIFO to put data in
 * \param  data    8-bit signed integer to put into FIFO
 */
void FIFO_PutInt8( FIFO_handle_t * handle, int8_t data )
{
	// Cast data to FIFO data type and insert.
	FIFO_InternalPutData( handle, (FIFO_data_t) data );
}

/*
 * \param  handle  Pointer to FIFO to put data in
 * \param  data    16-bit unsigned integer to put into FIFO
 */
void FIFO_PutUint16( FIFO_handle_t * handle, uint16_t data )
{
	// Split data and put bytes into FIFO.
	FIFO_InternalPutData( handle, (FIFO_data_t) ((data >> (0*8)) & 0xff) );
	FIFO_InternalPutData( handle, (FIFO_data_t) ((data >> (1*8)) & 0xff) );
}


/*
 * \param  handle  Pointer to FIFO to put data in
 * \param  data    16-bit signed integer to put into FIFO
 */
void FIFO_PutInt16( FIFO_handle_t * handle, int16_t data )
{
	// Split data and put bytes into FIFO.
	FIFO_InternalPutData( handle, (FIFO_data_t) ((data >> (0*8)) & 0xff) );
	FIFO_InternalPutData( handle, (FIFO_data_t) ((data >> (1*8)) & 0xff) );
}


/*
 * \param  handle  Pointer to FIFO to put data in
 * \param  data    32-bit unsigned integer to put into FIFO
 */
void FIFO_PutUint32( FIFO_handle_t * handle, uint32_t data )
{
	// Split data and put bytes into FIFO.
	FIFO_InternalPutData( handle, (FIFO_data_t) ((data >> (0*8)) & 0xff) );
	FIFO_InternalPutData( handle, (FIFO_data_t) ((data >> (1*8)) & 0xff) );
	FIFO_InternalPutData( handle, (FIFO_data_t) ((data >> (2*8)) & 0xff) );
	FIFO_InternalPutData( handle, (FIFO_data_t) ((data >> (3*8)) & 0xff) );
}


/*
 * \param  handle  Pointer to FIFO to put data in
 * \param  data    32-bit signed integer to put into FIFO
 */
void FIFO_PutInt32( FIFO_handle_t * handle, int32_t data )
{
	// Split data and put bytes into FIFO.
	FIFO_InternalPutData( handle, (FIFO_data_t) ((data >> (0*8)) & 0xff) );
	FIFO_InternalPutData( handle, (FIFO_data_t) ((data >> (1*8)) & 0xff) );
	FIFO_InternalPutData( handle, (FIFO_data_t) ((data >> (2*8)) & 0xff) );
	FIFO_InternalPutData( handle, (FIFO_data_t) ((data >> (3*8)) & 0xff) );
}


/*
 * \param  handle  Pointer to FIFO to put data in
 * \param  data    Pointer to beginning of memory block
 * \param  size    How many bytes of data to put into FIFO from data
 */
void FIFO_PutBlock( FIFO_handle_t * handle, const void * data, FIFO_size_t size )
{
	// Cast untyped data pointer to proper buffer pointer.
	FIFO_data_t * dataPtr = (FIFO_data_t *) data;
	
	// Put byte by byte from buffer into FIFO.
	while (size > 0) {
		FIFO_InternalPutData( handle, *dataPtr );
		++dataPtr;
		--size;
	}
}

/*
 * \param  handle  Pointer to FIFO to get data from
 *
 * \return  Unsigned  8-bit integer containing data from the FIFO
 */
uint8_t FIFO_GetUint8( FIFO_handle_t * handle )
{
	// Get data item and cast to return type.
	uint8_t data;
	data =  ((uint8_t) FIFO_InternalGetData( handle )) << (8*0);
	
	return data;
}


/*
 * \param  handle  Pointer to FIFO to get data from
 *
 * \return  Signed  8-bit integer containing data from the FIFO
 */
int8_t FIFO_GetInt8( FIFO_handle_t * handle )
{
	// Get data item and cast to return type.
	int8_t data;
	data =  ((int8_t) FIFO_InternalGetData( handle )) << (8*0);
	
	return data;
}


/*
 * \param  handle  Pointer to FIFO to get data from
 *
 * \return  Unsigned  16-bit integer containing data from the FIFO
 */
uint16_t FIFO_GetUint16( FIFO_handle_t * handle )
{
	// Get bytes and combine into return data type.
	uint16_t data;
	data =  ((uint16_t) FIFO_InternalGetData( handle )) << (8*0);
	data |= ((uint16_t) FIFO_InternalGetData( handle )) << (8*1);
	
	return data;
}


/*
 * \param  handle  Pointer to FIFO to get data from
 *
 * \return  Signed  16-bit integer containing data from the FIFO
 */
int16_t FIFO_GetInt16( FIFO_handle_t * handle )
{
	// Get bytes and combine into return data type.
	int16_t data;
	data =  ((int16_t) FIFO_InternalGetData( handle )) << (8*0);
	data |= ((int16_t) FIFO_InternalGetData( handle )) << (8*1);
	
	return data;
}


/*
 * \param  handle  Pointer to FIFO to get data from
 *
 * \return  Unsigned  32-bit integer containing data from the FIFO
 */
uint32_t FIFO_GetUint32( FIFO_handle_t * handle )
{
	// Get bytes and combine into return data type.
	uint32_t data;
	data =  ((uint32_t) FIFO_InternalGetData( handle )) << (8*0);
	data |= ((uint32_t) FIFO_InternalGetData( handle )) << (8*1);
	data |= ((uint32_t) FIFO_InternalGetData( handle )) << (8*2);
	data |= ((uint32_t) FIFO_InternalGetData( handle )) << (8*3);
	
	return data;
}


/*
 * \param  handle  Pointer to FIFO to get data from
 *
 * \return  Signed  32-bit integer containing data from the FIFO
 */
int32_t FIFO_GetInt32( FIFO_handle_t * handle )
{
	// Get bytes and combine into return data type.
	int32_t data;
	data =  ((int32_t) FIFO_InternalGetData( handle )) << (8*0);
	data |= ((int32_t) FIFO_InternalGetData( handle )) << (8*1);
	data |= ((int32_t) FIFO_InternalGetData( handle )) << (8*2);
	data |= ((int32_t) FIFO_InternalGetData( handle )) << (8*3);
	
	return data;
}


/*
 * \param  handle  Pointer to FIFI to get data from
 * \param  data    Pointer to where to save the data from the FIFO
 * \param  size    How many bytes to read from the FIFO
 */
void FIFO_GetBlock( FIFO_handle_t * handle, void * data, FIFO_size_t size )
{
	// Cast untyped data pointer to proper buffer pointer.
	FIFO_data_t * dataPtr = (FIFO_data_t *) data;
	
	// Put byte by byte from FIFO into buffer.
	while (size > 0) {
		*dataPtr = FIFO_InternalGetData( handle );
		++dataPtr;
		--size;
	}
}
