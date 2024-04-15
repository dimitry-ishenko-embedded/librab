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
    INT_PERIOD = 0x00,
    INT_TIMA = 0x0a,
    INT_TIMB = 0x0b,
    INT_SERA = 0x0c,
    INT_SERB = 0x0d,
    INT_SERC = 0x0e,
    INT_SERD = 0x0f,
    INT_SERE = 0x1c,
    INT_SERF = 0x1d,

    INT_EXT0 = 0x00,
    INT_EXT1 = 0x01,
};

int ivt_intern_isr(byte num, void *isr);
int ivt_extern_isr(byte num, void *isr);

////////////////////////////////////////////////////////////////////////////////
#endif
