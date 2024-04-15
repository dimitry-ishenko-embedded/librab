////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdint.h>

#ifndef _asm
#  define _asm __asm
#endif

#ifndef _at
#  define _at(addr) __at (addr)
#endif

#ifndef _critical
#  define _critical __critical
#endif

#ifndef _endasm
#  define _endasm __endasm
#endif

#ifndef _interrupt
#  define _interrupt __interrupt
#endif

#ifndef _naked
#  define _naked __naked
#endif

#ifndef _reg
#  define _reg __sfr __banked
#endif

#ifndef _sdcccall
#  define _sdcccall __sdcccall(1)
#endif

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;

enum
{
    off = false,
    on = true,
    hi = true,
};

enum
{
    OK = 0,
    FAIL = -1,
    TIMEOUT = -2,
};

////////////////////////////////////////////////////////////////////////////////
#endif
