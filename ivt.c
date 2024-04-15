////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "ivt.h"

static byte ivt;

static void install(byte ivt, byte num, void *isr) _sdcccall
{
    byte *pd = (byte *)((ivt << 8) | (num << 4));
    word pi = (word)(isr);

    _critical
    {
        *pd++ = 0xc3; // jp opcode
        *pd++ = pi;
        *pd++ = pi >> 8;
    }
}

int ivt_intern_isr(byte num, void *isr) _sdcccall
{
    if (num > 0x1f) return FAIL;

_asm
    ld a, iir
    ld (_ivt), a
_endasm;
    install(ivt, num, isr);
    return OK;
}

int ivt_extern_isr(byte num, void *isr) _sdcccall
{
    if (num > 1) return FAIL;

_asm
    ld a, eir
    ld (_ivt), a
_endasm;
    install(ivt, num, isr);
    return OK;
}

