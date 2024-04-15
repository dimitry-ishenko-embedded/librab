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
int  sera_open(dword baud);
void sera_flush();

void sera_putc(byte c);
void sera_send(const void *data, word size);
void sera_puts(const char *s);

int  sera_getc(int timeout);
word sera_recv(void *data, word size, int timeout);
int  sera_recv_all(void *data, word size, int timeout);

////////////////////////////////////////////////////////////////////////////////
int  serb_open(dword baud);
void serb_flush();

void serb_putc(byte c);
void serb_send(const void *data, word size);
void serb_puts(const char *s);

int  serb_getc(int timeout);
word serb_recv(void *data, word size, int timeout);
int  serb_recv_all(void *data, word size, int timeout);

////////////////////////////////////////////////////////////////////////////////
#endif
