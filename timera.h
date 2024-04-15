////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef TIMERA_H
#define TIMERA_H

#include "types.h"

enum
{
    TIMERA1 = 1,
    TIMERA2,
    TIMERA3,
    TIMERA4,
    TIMERA5,
    TIMERA6,
    TIMERA7,
};

void timera_init() _sdcccall;
int timera_isr(byte n, void *isr) _sdcccall;

////////////////////////////////////////////////////////////////////////////////
#endif
