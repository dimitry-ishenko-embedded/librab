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
enum
{
    TA1 = 1,
    TA2,
    TA3,
    TA4,
    TA5,
    TA6,
    TA7,
};

static void (*ctx[TA7 + 1])();

////////////////////////////////////////////////////////////////////////////////
static void isr_tima() _critical _interrupt
{
    byte cs = TACSR >> 1;
    for (byte n = TA1; n <= TA7; ++n, cs >>= 1) if (cs & 1) ctx[n]();
}

void tima_init() _sdcccall
{
    ivt_intern_isr(INT_TIMA, isr_tima);
    TACR = (TACS |= INT_PRIO1);
}

#pragma save
#pragma disable_warning 244

static void tima_proc(byte n, void *proc) _sdcccall
{
    if (proc)
    {
        ctx[n] = proc;
        TACSR = (TACSS |= (1 << n));
    }
    else TACSR = (TACSS &= ~(1 << n));
}

#pragma restore

void tima1_proc(void *proc) _sdcccall { tima_proc(TA1, proc); }
void tima2_proc(void *proc) _sdcccall { tima_proc(TA2, proc); }
void tima3_proc(void *proc) _sdcccall { tima_proc(TA3, proc); }
void tima4_proc(void *proc) _sdcccall { tima_proc(TA4, proc); }
void tima5_proc(void *proc) _sdcccall { tima_proc(TA5, proc); }
void tima6_proc(void *proc) _sdcccall { tima_proc(TA6, proc); }
void tima7_proc(void *proc) _sdcccall { tima_proc(TA7, proc); }
