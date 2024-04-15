////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "board.h"
#include "ivt.h"
#include "reg.h"

// milli-second counter (overflows every ~65 sec)
volatile word _at (0xdff8) msec_count;

// divisor for 19200 baud
byte _at (0xdffa) div_19200;

////////////////////////////////////////////////////////////////////////////////
static byte _at (0xdffb) tic;
static byte _at (0xdffc) toc;

static void isr_period() _naked
{
_asm
    push af
    push hl
    ioi
    ld a, (_GCSR)

; runs every 488 µsec or at ~2048 Hz
; skip every other tick to get 1024 Hz
    ld hl, #_tic
    inc (hl)
    bit 0, (hl)
    jr nz, .per_exit

; runs at 1024 Hz
; skip 6 ticks out of 256 to get 1000 Hz (= 1024 * 250 / 256)
    ld a, (_toc)
    add a, #250
    ld (_toc), a
    jr nc, .per_exit

; runs at 1000 Hz or every msec
    ld hl, (_msec_count)
    inc hl
    ld (_msec_count), hl

.per_exit:
    pop hl
    pop af
    ipres
    ret
_endasm;
}

////////////////////////////////////////////////////////////////////////////////
static void calc_div_19200() _critical
{
_asm
; wait for the lowest 11 bits to be cleared
    ld de, #0x7ff
div_zero:
    ioi
    ld (_RTC0R), a
    ioi
    ld hl, (_RTC0R)
    and hl, de
    jr nz, div_zero

    ld de, #0
div_calc:
    inc de

; expend time
    ld b, #0x7b
div_loop:
    ld a, #0x5a
    ioi
    ld (_WDTCR), a
    djnz div_loop

; wait for bit 11 to become "1"
    ioi
    ld (_RTC0R), a
    ioi
    ld hl, (_RTC0R)
    bit 2, h
    jr z, div_calc

; divide by 8
    rr de
    rr de
    rr de

    ld a, e
    ld (_div_19200), a
_endasm;
}

////////////////////////////////////////////////////////////////////////////////
void board_init()
{
    // IMPORTANT:
    // This function is run before main() and any global variables set here
    // will be overwritten by gsinit() inside crt0.
    // Therefore, all variables used here must be located in the un-initialized
    // space (0xdf00 - 0xdfff) and initialized manually.

    // no bootstrap
    SPCR = SMODE_DISABLE;

    // fast Flash & RAM
    MB0CR = MB_0WS | MB_OE0     | MB_CS0;
    MB2CR = MB_0WS | MB_OE1_WE1 | MB_CS1;

    // run at max speed
    GCSR = GCSS = CPU_MAIN_CLOCK;
    GCDR = DOUBLER_8NS;

    calc_div_19200();

    // disable watchdog
    WDTTR = 0x51;
    WDTTR = 0x54;

    PCFR  = PC6_TXA | PC4_TXB | PC2_TXC | PC0_TXD;
    PEFR  = PEx_GPIO;
    PGDCR = PGx_STANDARD;
    PGDDR = PGx_MULTI;
    PGFR  = PG7_RXE | PG6_TXE | PG3_RXF | PG2_TXF;

    TAPR  = TAPS  = TA_CLOCK_PERIF_DIV2;
    TACR  = TACS  = TAx_CLOCK_MAIN;
    TACSR = TACSS = TA_CLOCK_ENABLE;

    tic = toc = msec_count = 0;
    ivt_intern_isr(INT_PERIOD, &isr_period);
    GCSR = (GCSS |= INT_PRIO3);

    __asm__("ipset 0");
}

////////////////////////////////////////////////////////////////////////////////
void sleep(word msec)
{
    word start = msec_count;
    word end = start + msec;

    if (end < start)
        while (msec_count >= start); // wait for msec_count to overflow
    while (msec_count < end);
}
