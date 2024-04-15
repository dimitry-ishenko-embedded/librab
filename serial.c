////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "board.h"
#include "ivt.h"
#include "reg.h"
#include "serial.h"

#include <string.h>

////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    const byte *tx_data;
    word tx_size;

#define RX_SIZE 16 // must be a power of 2!!
#define RX_MASK (RX_SIZE - 1)

    byte rx[RX_SIZE];
    byte rx_in, rx_out;

    word rd, rs; // data and status registers
}
serial;

static serial sera = { .rd = 0x00c0 /*SADR*/, .rs = 0x00c3 /*SASR*/ };
static serial serb = { .rd = 0x00d0 /*SBDR*/, .rs = 0x00d3 /*SBSR*/ };
static serial serc = { .rd = 0x00e0 /*SCDR*/, .rs = 0x00e3 /*SCSR*/ };
static serial serd = { .rd = 0x00f0 /*SDDR*/, .rs = 0x00f3 /*SDSR*/ };

static word get_div(dword baud) _sdcccall
{
    return (dword)19200 * div_19200 / baud;
}

////////////////////////////////////////////////////////////////////////////////
static void flush(serial *ctx) _sdcccall
{
    _critical { ctx->rx_out = ctx->rx_in; }
}

////////////////////////////////////////////////////////////////////////////////
static void putc(serial *ctx, byte c) _sdcccall
{
    ctx->tx_size = 1;
    reg_write(ctx->rd, c);

    while (ctx->tx_size);
}

static void send(serial *ctx, const byte *data, word size) _sdcccall
{
    if (!size) return;

    ctx->tx_data = data;
    ctx->tx_size = size;
    reg_write(ctx->rd, *data);

    while (ctx->tx_size);
}

////////////////////////////////////////////////////////////////////////////////
static inline byte get1(serial *ctx) _sdcccall
{
    byte d;
    _critical
    {
        d = ctx->rx[ctx->rx_out];
        ctx->rx_out = ++ctx->rx_out & RX_MASK;
    }
    return d;
}

static int getc(serial *ctx, int timeout) _sdcccall
{
    if (timeout < 0) // get one
    {
        while (ctx->rx_out == ctx->rx_in);
        return get1(ctx);
    }
    else if (timeout == 0) // get cached
    {
        return (ctx->rx_out != ctx->rx_in) ? get1(ctx) : TIMEOUT;
    }
    else // get timed
    {
        word start = msec_count;
        word end = start + timeout;

        if (end < start) do
            if (ctx->rx_out != ctx->rx_in) return get1(ctx);
        while (msec_count >= start);

        do
            if (ctx->rx_out != ctx->rx_in) return get1(ctx);
        while (msec_count < end);

        return TIMEOUT;
    }
}

static word recv(serial *ctx, byte *data, word max_size, int timeout) _sdcccall
{
    word n = 0;
    if (timeout < 0) // recv all
    {
        for (byte *p = data, *e = p + max_size; p != e; ++p, ++n)
        {
            while (ctx->rx_out == ctx->rx_in);
            *p = get1(ctx);
        }
    }
    else if (timeout == 0) // recv cached
    {
        for (byte *p = data, *e = p + max_size; p != e; ++p, ++n)
        {
            if (ctx->rx_out == ctx->rx_in) break;
            *p = get1(ctx);
        }
    }
    else // recv timed
    {
        word start = msec_count;
        word end = start + timeout;

        byte *p = data, *e = p + max_size;

        if (end < start)
        {
            for (; p != e; ++p, ++n)
            {
                while (ctx->rx_out == ctx->rx_in)
                    if (msec_count < start) goto done1;
                *p = get1(ctx);
            }
        }

done1:
        for (; p != e; ++p, ++n)
        {
            while (ctx->rx_out == ctx->rx_in)
                if (msec_count >= end) goto done2;
            *p = get1(ctx);
        }
    }

done2:
    return n;
}

////////////////////////////////////////////////////////////////////////////////
static void isr_serial(serial *ctx) _sdcccall
{
    byte rs = reg_read(ctx->rs);
    if (rs & 0x80) // have rx
    {
        ctx->rx[ctx->rx_in] = reg_read(ctx->rd);
        ctx->rx_in = ++ctx->rx_in & RX_MASK;

        // overflow? bump rx_out
        if (ctx->rx_in == ctx->rx_out) ctx->rx_out = ++ctx->rx_out & RX_MASK;
    }
    if (!(rs & 0x08)) // tx done
    {
        if (ctx->tx_size > 0)
        {
            if (ctx->tx_size > 1) reg_write(ctx->rd, *++ctx->tx_data);
            --ctx->tx_size;
        }
        reg_write(ctx->rs, 0);
    }
}

static void isr_sera() _critical _interrupt { isr_serial(&sera); }
static void isr_serb() _critical _interrupt { isr_serial(&serb); }
static void isr_serc() _critical _interrupt { isr_serial(&serc); }
static void isr_serd() _critical _interrupt { isr_serial(&serd); }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int sera_open(dword baud) _sdcccall
{
    word div = get_div(baud);
    if (div > 255) return FAIL;

    ivt_intern_isr(INT_SERIALA, &isr_sera);

    SACR = SERIALA_PORTC_IN | SERIALA_MODE_8BIT | SERIALA_INT_PRIO1;
    TAT4R = div;

    return OK;
}

void sera_flush() _sdcccall { flush(&sera); }

void sera_putc(byte c) _sdcccall { putc(&sera, c); }
void sera_send(const void *data, word size) _sdcccall { send(&sera, data, size); }
void sera_puts(const char *s) _sdcccall { send(&sera, (byte *)s, strlen(s)); }

int  sera_getc(int timeout) _sdcccall { return getc(&sera, timeout); }
word sera_recv(void *data, word size, int timeout) _sdcccall
{
    return recv(&sera, data, size, timeout);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int serb_open(dword baud) _sdcccall
{
    word div = get_div(baud);
    if (div > 255) return FAIL;

    ivt_intern_isr(INT_SERIALB, &isr_serb);

    SBCR = SERIALB_PORTC_IN | SERIALB_MODE_8BIT | SERIALB_INT_PRIO1;
    TAT5R = div;

    return OK;
}

void serb_flush() _sdcccall { flush(&serb); }

void serb_putc(byte c) _sdcccall { putc(&serb, c); }
void serb_send(const void *data, word size) _sdcccall { send(&serb, data, size); }
void serb_puts(const char *s) _sdcccall { send(&serb, (byte *)s, strlen(s)); }

int  serb_getc(int timeout) _sdcccall { return getc(&serb, timeout); }
word serb_recv(void *data, word size, int timeout) _sdcccall
{
    return recv(&serb, data, size, timeout);
}
