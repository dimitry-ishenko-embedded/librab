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
typedef void (*isr_t)();
static isr_t tima_isr[TIMERA7 + 1];

static void isr_timera() _critical _interrupt
{
    byte cs = TACSR >> 1;
    for (byte n = TIMERA1; n <= TIMERA7; ++n, cs >>= 1) if ((cs & 1) && tima_isr[n]) tima_isr[n]();
}

void timera_init() _sdcccall
{
    ivt_intern_isr(INT_TIMERA, isr_timera);
    TACR = (TACRW |= INT_PRIO2);
}

////////////////////////////////////////////////////////////////////////////////
#pragma save
#pragma disable_warning 244

int timera_isr(byte n, void *isr) _sdcccall
{
    if (n < TIMERA1 || n > TIMERA7) return FAIL;

    _critical { tima_isr[n] = (isr_t)isr; }
    return OK;
}

#pragma restore