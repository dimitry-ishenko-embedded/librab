////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef TIMERA_H
#define TIMERA_H

#include "types.h"

void tima_init() _sdcccall;
void tima1_proc(void *proc) _sdcccall;
void tima2_proc(void *proc) _sdcccall;
void tima3_proc(void *proc) _sdcccall;
void tima4_proc(void *proc) _sdcccall;
void tima5_proc(void *proc) _sdcccall;
void tima6_proc(void *proc) _sdcccall;
void tima7_proc(void *proc) _sdcccall;

////////////////////////////////////////////////////////////////////////////////
#endif
