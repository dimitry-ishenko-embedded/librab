////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef BOARD_H
#define BOARD_H

#include "types.h"

// milli-second counter (overflows every ~65 sec)
extern word _at (0xdff8) msec_count;

// divisor for 19200 baud
extern byte _at (0xdffa) div_19200;

void board_init();
void sleep(word msec);

////////////////////////////////////////////////////////////////////////////////
#endif
