////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "ivt.h"

static byte *ivt;

////////////////////////////////////////////////////////////////////////////////
int ivt_intern_isr(byte num, void *isr) _sdcccall
{
    if (num > 0x1f) return FAIL;

_asm
    xor a, a
    ld (_ivt + 0), a
    ld a, iir
    ld (_ivt + 1), a
_endasm;

    byte *p = ivt + (num << 4);
    _critical
    {
        *p = 0xc3; // jp opcode
        *(void **)(p + 1) = isr;
    }
    return OK;
}

////////////////////////////////////////////////////////////////////////////////
int ivt_extern_isr(byte num, void *isr) _sdcccall
{
    if (num > 1) return FAIL;

_asm
    xor a, a
    ld (_ivt + 0), a
    ld a, eir
    ld (_ivt + 1), a
_endasm;

    byte *p = ivt + (num << 4);
    _critical
    {
        *p = 0xc3; // jp opcode
        *(void **)(p + 1) = isr;
    }
    return OK;
}
