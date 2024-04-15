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
static void (*service[TIMERA7 + 1])();

static void isr_timera() _critical _interrupt
{
    byte cs = TACSR >> 1;
    for (byte n = TIMERA1; n <= TIMERA7; ++n, cs >>= 1) if ((cs & 1) && service[n]) service[n]();
}

void tima_init() _sdcccall
{
    ivt_intern_isr(INT_TIMERA, isr_timera);
    TACR = (TACS |= INT_PRIO2);
}

////////////////////////////////////////////////////////////////////////////////
#pragma save
#pragma disable_warning 244

int tima_service(byte n, void *srv) _sdcccall
{
    if (n < TIMERA1 || n > TIMERA7) return FAIL;

    _critical { service[n] = srv; }
    return OK;
}

#pragma restore

////////////////////////////////////////////////////////////////////////////////
static word TATxR[] = { 0, _TAT1R, _TAT2R, _TAT3R, _TAT4R, _TAT5R, _TAT6R, _TAT7R };

int tima_count(byte n, byte c) _sdcccall
{
    if (n < TIMERA1 || n > TIMERA7) return FAIL;

    reg_write(TATxR[n], c);
    return OK;
}

////////////////////////////////////////////////////////////////////////////////
int tima_clock_a1(byte n, bool b) _sdcccall
{
    if (n < TIMERA2 || n > TIMERA7) return FAIL;

    byte m = 1 << (n - TIMERA1 + 1);
    TACR = TACS = b ? TACS | m : TACS & ~m;
    return OK;
}

////////////////////////////////////////////////////////////////////////////////
int tima_enable(byte n, bool b) _sdcccall
{
    if (n < TIMERA1 || n > TIMERA7) return FAIL;

    byte m = 1 << (n - TIMERA1 + 1);
    TACSR = TACSS = b ? TACSS | m : TACSS & ~m;
    return OK;
}
