////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef IVT_H
#define IVT_H

#include "types.h"

enum
{
    INT_PERIODIC= 0x00,
    INT_TIMERA  = 0x0a,
    INT_TIMERB  = 0x0b,
    INT_SERIALA = 0x0c,
    INT_SERIALB = 0x0d,
    INT_SERIALC = 0x0e,
    INT_SERIALD = 0x0f,
    INT_SERIALE = 0x1c,
    INT_SERIALF = 0x1d,

    EXTERN0     = 0x00,
    EXTERN1     = 0x01,
};

int ivt_intern_isr(byte num, void *isr) _sdcccall;
int ivt_extern_isr(byte num, void *isr) _sdcccall;

////////////////////////////////////////////////////////////////////////////////
#endif
