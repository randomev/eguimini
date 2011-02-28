/// This file has been prepared for Doxygen automatic documentation generation.
/*! \file *********************************************************************
 *
 * \brief  DB101 memory helper application used by the main function
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
 * $Revision: 2347 $
 * $Date: 2007-09-11 00:18:39 -0600 (ti, 11 sep 2007) $
 * $URL: http://revisor.norway.atmel.com/AppsAVR8/avr482_db101_software_users_guide/tags/release_A_code_20070917/IAR/production_demo_rev_A/memory.c $
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


#include <memblock_lib.h>


#define MEMORY_BAGCOUNT 4

#define MEMORY_BLOCKSIZE0 16
#define MEMORY_BLOCKSIZE1 32
#define MEMORY_BLOCKSIZE2 64
#define MEMORY_BLOCKSIZE3 1024

#define MEMORY_BLOCKCOUNT0 16
#define MEMORY_BLOCKCOUNT1 32
#define MEMORY_BLOCKCOUNT2 16
#define MEMORY_BLOCKCOUNT3 3


static uint8_t MEMORY_block0[ MEMBLOCK_BAGSIZE(MEMORY_BLOCKSIZE0,MEMORY_BLOCKCOUNT0) ];
static uint8_t MEMORY_block1[ MEMBLOCK_BAGSIZE(MEMORY_BLOCKSIZE1,MEMORY_BLOCKCOUNT1) ];
static uint8_t MEMORY_block2[ MEMBLOCK_BAGSIZE(MEMORY_BLOCKSIZE2,MEMORY_BLOCKCOUNT2) ];
static uint8_t MEMORY_block3[ MEMBLOCK_BAGSIZE(MEMORY_BLOCKSIZE3,MEMORY_BLOCKCOUNT3) ];

static MEMBLOCK_bag_t MEMORY_blockBags[ MEMORY_BAGCOUNT ];

void InitMemory( void )
{
	MEMBLOCK_InitManager( MEMORY_BAGCOUNT, MEMORY_blockBags );
	MEMBLOCK_InitBag( 0, MEMORY_BLOCKSIZE0, MEMORY_BLOCKCOUNT0, MEMORY_block0 );
	MEMBLOCK_InitBag( 1, MEMORY_BLOCKSIZE1, MEMORY_BLOCKCOUNT1, MEMORY_block1 );
	MEMBLOCK_InitBag( 2, MEMORY_BLOCKSIZE2, MEMORY_BLOCKCOUNT2, MEMORY_block2 );
	MEMBLOCK_InitBag( 3, MEMORY_BLOCKSIZE3, MEMORY_BLOCKCOUNT3, MEMORY_block3 );
}
