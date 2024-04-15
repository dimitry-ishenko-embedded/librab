////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef SERIAL_H
#define SERIAL_H

#include "types.h"

////////////////////////////////////////////////////////////////////////////////
int  sera_open(dword baud) _sdcccall;
void sera_flush() _sdcccall;

void sera_putc(byte c) _sdcccall;
void sera_send(const void *data, word size) _sdcccall;
void sera_puts(const char *s) _sdcccall;

int  sera_getc(int timeout) _sdcccall;
word sera_recv(void *data, word size, int timeout) _sdcccall;

////////////////////////////////////////////////////////////////////////////////
int  serb_open(dword baud) _sdcccall;
void serb_flush() _sdcccall;

void serb_putc(byte c) _sdcccall;
void serb_send(const void *data, word size) _sdcccall;
void serb_puts(const char *s) _sdcccall;

int  serb_getc(int timeout) _sdcccall;
word serb_recv(void *data, word size, int timeout) _sdcccall;

////////////////////////////////////////////////////////////////////////////////
#endif
