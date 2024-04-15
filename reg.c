////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "reg.h"

// shadow registers
byte _at (0xdfa0) TACSS;
byte _at (0xdfa1) TAPS;
byte _at (0xdfa4) TACS;

////////////////////////////////////////////////////////////////////////////////
#pragma save
#pragma disable_warning 85

void reg_write(word r, byte d) _sdcccall
{
_asm
    ioi
    ld (hl), a
_endasm;
}

#pragma restore

////////////////////////////////////////////////////////////////////////////////
#pragma save
#pragma disable_warning 59
#pragma disable_warning 85

byte reg_read(word r) _sdcccall
{
_asm
    ioi
    ld a, (hl)
_endasm;
}

#pragma restore
