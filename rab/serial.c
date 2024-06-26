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

typedef struct
{
    const byte *tx_data;
    volatile word tx_size;

#define RX_SIZE 16 // must be a power of 2!!
#define RX_MASK (RX_SIZE - 1)

    byte rx_data[RX_SIZE];
    volatile byte rx_in, rx_out;
}
serial;

static serial ctx_a, ctx_b, ctx_e;

////////////////////////////////////////////////////////////////////////////////
static word get_div(dword baud)
{
    return (dword)19200 * div_19200 / baud;
}

////////////////////////////////////////////////////////////////////////////////
static inline byte get1(serial *ctx)
{
    byte d;
    _critical
    {
        d = ctx->rx_data[ctx->rx_out];
        ctx->rx_out = (ctx->rx_out + 1) & RX_MASK;
    }
    return d;
}

////////////////////////////////////////////////////////////////////////////////
static int getc(serial *ctx, int timeout)
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
        word count; // intermediate variable for "atomic" comparisons below

        if (end < start) do
        {
            if (ctx->rx_out != ctx->rx_in) return get1(ctx);
            count = msec_count;
        }
        while (count >= start);

        do
        {
            if (ctx->rx_out != ctx->rx_in) return get1(ctx);
            count = msec_count;
        }
        while (count < end);

        return TIMEOUT;
    }
}

////////////////////////////////////////////////////////////////////////////////
static word recv(serial *ctx, byte *data, word max_size, int timeout)
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
        word count; // intermediate variable for "atomic" comparisons below

        byte *p = data, *e = p + max_size;

        if (end < start)
        {
            for (; p != e; ++p, ++n)
            {
                while (ctx->rx_out == ctx->rx_in)
                {
                    count = msec_count;
                    if (count < start) goto done_1;
                }
                *p = get1(ctx);
            }
        done_1:;
        }

        for (; p != e; ++p, ++n)
        {
            while (ctx->rx_out == ctx->rx_in)
            {
                count = msec_count;
                if (count >= end) goto done_2;
            }
            *p = get1(ctx);
        }
        done_2:;
    }

    return n;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static void isr_sera() _critical _interrupt
{
    byte rs = SASR;
    if (rs & 0x80) // have rx
    {
        ctx_a.rx_data[ctx_a.rx_in] = SADR;
        ctx_a.rx_in = (ctx_a.rx_in + 1) & RX_MASK;

        // overflow? bump rx_out
        if (ctx_a.rx_in == ctx_a.rx_out) ctx_a.rx_out = (ctx_a.rx_out + 1) & RX_MASK;
    }
    if (!(rs & 0x08)) // tx done
    {
        if (ctx_a.tx_size > 0)
        {
            if (ctx_a.tx_size > 1) SADR = *++ctx_a.tx_data;
            --ctx_a.tx_size;
        }
        SASR = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
int sera_open(dword baud)
{
    word div = get_div(baud);
    if (div > 255) return FAIL;

    ivt_intern_isr(INT_SERA, &isr_sera);

    SACR = SA_IN_PC | SA_8BIT | INT_PRIO1;
    TAT4R = div;

    return OK;
}

////////////////////////////////////////////////////////////////////////////////
void sera_flush() _critical { ctx_a.rx_out = ctx_a.rx_in; }
void sera_close() { SACR = INT_DISABLE; }

////////////////////////////////////////////////////////////////////////////////
void sera_putc(byte c)
{
    ctx_a.tx_size = 1;
    SADR = c;
    while (ctx_a.tx_size);
}

////////////////////////////////////////////////////////////////////////////////
void sera_send(const void *data, word size)
{
    if (!size) return;

    ctx_a.tx_data = data;
    ctx_a.tx_size = size;
    SADR = *(const byte *)data;

    while (ctx_a.tx_size);
}

////////////////////////////////////////////////////////////////////////////////
void sera_puts(const char *s) { sera_send(s, strlen(s)); }

int  sera_getc(int timeout) { return getc(&ctx_a, timeout); }
word sera_recv(void *data, word size, int timeout) { return recv(&ctx_a, data, size, timeout); }
int  sera_recv_all(void *data, word size, int timeout) { return sera_recv(data, size, timeout) == size ? OK : TIMEOUT; }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static void isr_serb() _critical _interrupt
{
    byte rs = SBSR;
    if (rs & 0x80) // have rx
    {
        ctx_b.rx_data[ctx_b.rx_in] = SBDR;
        ctx_b.rx_in = (ctx_b.rx_in + 1) & RX_MASK;

        // overflow? bump rx_out
        if (ctx_b.rx_in == ctx_b.rx_out) ctx_b.rx_out = (ctx_b.rx_out + 1) & RX_MASK;
    }
    if (!(rs & 0x08)) // tx done
    {
        if (ctx_b.tx_size > 0)
        {
            if (ctx_b.tx_size > 1) SBDR = *++ctx_b.tx_data;
            --ctx_b.tx_size;
        }
        SBSR = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
int serb_open(dword baud)
{
    word div = get_div(baud);
    if (div > 255) return FAIL;

    ivt_intern_isr(INT_SERB, &isr_serb);

    SBCR = SB_IN_PC | SB_8BIT | INT_PRIO1;
    TAT5R = div;

    return OK;
}

////////////////////////////////////////////////////////////////////////////////
void serb_flush() _critical { ctx_b.rx_out = ctx_b.rx_in; }
void serb_close() { SBCR = INT_DISABLE; }

////////////////////////////////////////////////////////////////////////////////
void serb_putc(byte c)
{
    ctx_b.tx_size = 1;
    SBDR = c;
    while (ctx_b.tx_size);
}

////////////////////////////////////////////////////////////////////////////////
void serb_send(const void *data, word size)
{
    if (!size) return;

    ctx_b.tx_data = data;
    ctx_b.tx_size = size;
    SBDR = *(const byte *)data;

    while (ctx_b.tx_size);
}

////////////////////////////////////////////////////////////////////////////////
void serb_puts(const char *s) { serb_send(s, strlen(s)); }

int  serb_getc(int timeout) { return getc(&ctx_b, timeout); }
word serb_recv(void *data, word size, int timeout) { return recv(&ctx_b, data, size, timeout); }
int  serb_recv_all(void *data, word size, int timeout) { return serb_recv(data, size, timeout) == size ? OK : TIMEOUT; }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static void isr_sere() _critical _interrupt
{
    byte rs = SESR;
    if (rs & 0x80) // have rx
    {
        ctx_e.rx_data[ctx_e.rx_in] = SEDR;
        ctx_e.rx_in = (ctx_e.rx_in + 1) & RX_MASK;

        // overflow? bump rx_out
        if (ctx_e.rx_in == ctx_e.rx_out) ctx_e.rx_out = (ctx_e.rx_out + 1) & RX_MASK;
    }
    if (!(rs & 0x08)) // tx done
    {
        if (ctx_e.tx_size > 0)
        {
            if (ctx_e.tx_size > 1) SEDR = *++ctx_e.tx_data;
            --ctx_e.tx_size;
        }
        SESR = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
int sere_open(dword baud)
{
    word div = get_div(baud);
    if (div > 255) return FAIL;

    ivt_intern_isr(INT_SERE, &isr_sere);

    SECR = SE_IN_PG7 | SE_8BIT | INT_PRIO1;
    TAT2R = div;

    return OK;
}

////////////////////////////////////////////////////////////////////////////////
void sere_flush() _critical { ctx_e.rx_out = ctx_e.rx_in; }
void sere_close() { SECR = INT_DISABLE; }

////////////////////////////////////////////////////////////////////////////////
void sere_putc(byte c)
{
    ctx_e.tx_size = 1;
    SEDR = c;
    while (ctx_e.tx_size);
}

////////////////////////////////////////////////////////////////////////////////
void sere_send(const void *data, word size)
{
    if (!size) return;

    ctx_e.tx_data = data;
    ctx_e.tx_size = size;
    SEDR = *(const byte *)data;

    while (ctx_e.tx_size);
}

////////////////////////////////////////////////////////////////////////////////
void sere_puts(const char *s) { sere_send(s, strlen(s)); }

int  sere_getc(int timeout) { return getc(&ctx_e, timeout); }
word sere_recv(void *data, word size, int timeout) { return recv(&ctx_e, data, size, timeout); }
int  sere_recv_all(void *data, word size, int timeout) { return sere_recv(data, size, timeout) == size ? OK : TIMEOUT; }
