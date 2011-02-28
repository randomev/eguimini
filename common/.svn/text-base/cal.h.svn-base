/* 
   cal - "Compiler Abstraction Layer"
   for GNU avr-gcc/avr-libc and IAR AVR toolchain
   
   Copyright (c) 2007,2008 Martin Thomas, Kaiserslautern, Germany
   Copyright (c) 2008 Kevin Rosenberg
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. 
*/



#ifndef COMPILER_ABSTRACTION_LAYER_H_
#define COMPILER_ABSTRACTION_LAYER_H_

#if defined(__GNUC__)
#define CAL_FOR_GCC
#elif defined(__ICCAVR__)
#define CAL_FOR_IAR
#else
#error "unsupported toolchain"
#endif

#include <stdint.h>


/* ------------- IAR ------------- */

#if defined(CAL_FOR_IAR)

#include <ioavr.h>
#include <inavr.h>


#define CAL_DO_PRAGMA(CAL_pragma_par) _Pragma(#CAL_pragma_par)

#define CAL_ISR(CAL_vectname)\
CAL_DO_PRAGMA(vector = CAL_vectname)\
__interrupt void CAL_vectname##_handler(void)

#define CAL_ALWAYS_INLINE_DECLARE(CAL_function)\
CAL_function

#define CAL_ALWAYS_INLINE_DEFINE(CAL_function)\
CAL_DO_PRAGMA(inline = forced)\
CAL_function

#define CAL_ALWAYS_INLINE(CAL_function)\
CAL_DO_PRAGMA(inline = forced)\
CAL_function

#define CAL_NO_RETURN(CAL_function) __noreturn CAL_function

#define CAL_MAIN() __task void main(void)
#define CAL_MAIN_LAST()

#define CAL_enable_interrupt()    __enable_interrupt()
#define CAL_disable_interrupt()   __disable_interrupt()
#define CAL_sleep()               __sleep()
#define CAL_no_operation()        __no_operation()

#define CAL_PGM(CAL_name)                 __flash CAL_name
#define CAL_PGM_DEF(CAL_name)             __flash CAL_name
#define CAL_PSTR(CAL_name)                CAL_name
#define CAL_pgm_read_byte(CAL_address)    (*CAL_address)
#define CAL_pgm_read_char(CAL_address)    ((char)(*CAL_address))
#define CAL_pgm_read_word(CAL_address)    (*CAL_address)
#define CAL_pgm_read_pchar(CAL_address)   ((char __flash *)(*CAL_address))
#define CAL_pgm_read_puint8(CAL_address)  ((uint8_t __flash *)(*CAL_address))
#define CAL_pgm_read_puint16(CAL_address) ((uint16_t __flash *)(*CAL_address))
#define CAL_pgm_read_enum(CAL_address)    (*CAL_address)
#define CAL_pgm_read_pvoid(CAL_address)   ((void const __flash *)(*CAL_address))

#define CAL_delay_cycles( _clk )  __delay_cycles(_clk) 


/* ------------- GNU ------------- */

#elif defined(CAL_FOR_GCC)

#include <avr/io.h>
#include <avr/interrupt.h> /* sei(), cli(); ISR() */
#include <avr/sleep.h>     /* sleep_cpu() */
#include <avr/pgmspace.h>  /* PROGMEM pgm_read_* */

#ifndef F_CPU
#warning "F_CPU not defined so far - will do now"
#define F_CPU 7372800
#endif
#include "delay_x_gcc.h"   /* _delay_cycles (by Hans-Juergen Heinrichs) */


#define CAL_ISR(CAL_vectname)\
ISR(CAL_vectname)

#define CAL_ALWAYS_INLINE_DECLARE(CAL_function)\
CAL_function __attribute__((always_inline))

#define CAL_ALWAYS_INLINE_DEFINE(CAL_function)\
CAL_function

#define CAL_ALWAYS_INLINE(CAL_function)\
CAL_function /* __attribute__((always_inline)) */;\
CAL_function 

#define CAL_NO_RETURN(CAL_function)\
CAL_function __attribute__((noreturn));\
CAL_function

#define CAL_MAIN()\
int main(void) /* avoid warning and do not use: __attribute__((noreturn)) */;\
int main (void)
#define CAL_MAIN_LAST() return 0

#define CAL_enable_interrupt()   sei()
#define CAL_disable_interrupt()  cli()
#define CAL_sleep()              sleep_cpu()
#define CAL_no_operation()       __asm__ __volatile__ ("nop" ::)

#define CAL_PGM(CAL_name)                 CAL_name
#define CAL_PGM_DEF(CAL_name)             CAL_name PROGMEM
#define CAL_pgm_read_byte(CAL_address)    (pgm_read_byte( CAL_address ))
#define CAL_pgm_read_char(CAL_address)    ((char)(pgm_read_byte( CAL_address )))
#define CAL_pgm_read_word(CAL_address)    (pgm_read_word( CAL_address ))
#define CAL_pgm_read_pchar(CAL_address)   ((char*)(pgm_read_word(CAL_address)))
#define CAL_pgm_read_puint8(CAL_address)  ((uint8_t*)(pgm_read_word(CAL_address)))
#define CAL_pgm_read_puint16(CAL_address) ((uint16_t*)(pgm_read_word(CAL_address)))
// expect -fshort-enums is *not* enabled so 
// enum-type is unsigned int or int by default
#define CAL_pgm_read_enum(CAL_address)    (unsigned int)(pgm_read_word(CAL_address))
#define CAL_pgm_read_pvoid(CAL_address)   ((void*)(pgm_read_word(CAL_address)))

#ifdef CAL_DEBUG_
#warning "Debug/Sim short delays only"
#define CAL_delay_cycles( _clk )  _delay_cycles( 1 )
#else
#define CAL_delay_cycles( _clk )  _delay_cycles( _clk )
#endif


#else
#error "unsupported toolchain"
#endif

#endif /* COMPILER_ABSTRACTION_LAYER_H_ */
