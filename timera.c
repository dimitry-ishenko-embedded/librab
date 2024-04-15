////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "ivt.h"
#include "reg.h"
#include "timera.h"

////////////////////////////////////////////////////////////////////////////////
static void (*handle[TIMERA7 + 1])();

static void isr_timera() _critical _interrupt
{
    byte cs = TACSR >> 1;
    for (byte n = TIMERA1; n <= TIMERA7; ++n, cs >>= 1) if ((cs & 1) && handle[n]) handle[n]();
}

void tima_init() _sdcccall
{
    ivt_intern_isr(INT_TIMERA, isr_timera);
    TACR = (TACS |= INT_PRIO2);
}

////////////////////////////////////////////////////////////////////////////////
#pragma save
#pragma disable_warning 244

int tima_handle(byte n, void *cb) _sdcccall
{
    if (n < TIMERA1 || n > TIMERA7) return FAIL;

    _critical { handle[n] = cb; }
    return OK;
}

#pragma restore
