////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "reg.h"

byte TACSRW, TAPRW, TACRW;

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
