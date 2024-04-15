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

static serial ctx_a = { .rd = _SADR, .rs = _SASR };
static serial ctx_b = { .rd = _SBDR, .rs = _SBSR };

static word get_div(dword baud) _sdcccall
{
    return (dword)19200 * div_19200 / baud;
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

////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
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

static void isr_sera() _critical _interrupt { isr_serial(&ctx_a); }
static void isr_serb() _critical _interrupt { isr_serial(&ctx_b); }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int sera_open(dword baud) _sdcccall
{
    word div = get_div(baud);
    if (div > 255) return FAIL;

    ivt_intern_isr(INT_SERIALA, &isr_sera);

    SACR = SERIALA_PORTC_IN | SERIALA_MODE_8BIT | INT_PRIO1;
    TAT4R = div;

    return OK;
}

void sera_flush() _sdcccall _critical { ctx_a.rx_out = ctx_a.rx_in; }

////////////////////////////////////////////////////////////////////////////////
void sera_putc(byte c) _sdcccall
{
    ctx_a.tx_size = 1;
    SADR = c;
    while (ctx_a.tx_size);
}

void sera_send(const void *data, word size) _sdcccall
{
    if (!size) return;

    ctx_a.tx_data = data;
    ctx_a.tx_size = size;
    SADR = *(const byte *)data;

    while (ctx_a.tx_size);
}

void sera_puts(const char *s) _sdcccall { sera_send(s, strlen(s)); }

////////////////////////////////////////////////////////////////////////////////
int  sera_getc(int timeout) _sdcccall { return getc(&ctx_a, timeout); }
word sera_recv(void *data, word size, int timeout) _sdcccall { return recv(&ctx_a, data, size, timeout); }
int  sera_recv_all(void *data, word size, int timeout) _sdcccall { return sera_recv(data, size, timeout) == size ? OK : TIMEOUT; }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int serb_open(dword baud) _sdcccall
{
    word div = get_div(baud);
    if (div > 255) return FAIL;

    ivt_intern_isr(INT_SERIALB, &isr_serb);

    SBCR = SERIALB_PORTC_IN | SERIALB_MODE_8BIT | INT_PRIO1;
    TAT5R = div;

    return OK;
}

void serb_flush() _sdcccall _critical { ctx_b.rx_out = ctx_b.rx_in; }

////////////////////////////////////////////////////////////////////////////////
void serb_putc(byte c) _sdcccall
{
    ctx_b.tx_size = 1;
    SBDR = c;
    while (ctx_b.tx_size);
}

void serb_send(const void *data, word size) _sdcccall
{
    if (!size) return;

    ctx_b.tx_data = data;
    ctx_b.tx_size = size;
    SBDR = *(const byte *)data;

    while (ctx_b.tx_size);
}

void serb_puts(const char *s) _sdcccall { serb_send(s, strlen(s)); }

////////////////////////////////////////////////////////////////////////////////
int  serb_getc(int timeout) _sdcccall { return getc(&ctx_b, timeout); }
word serb_recv(void *data, word size, int timeout) _sdcccall { return recv(&ctx_b, data, size, timeout); }
int  serb_recv_all(void *data, word size, int timeout) _sdcccall { return serb_recv(data, size, timeout) == size ? OK : TIMEOUT; }
