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
word msec_count = 0;

// divisor for 19200 baud
byte _at (0xdf00) div_19200;

////////////////////////////////////////////////////////////////////////////////
static byte tic = 0, toc = 0;

static void isr_periodic() _naked
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
void board_init() _sdcccall
{
    // no bootstrap
    SPCR = SMODE_DISABLE;

    // fast Flash & RAM
    MB0CR = MEM_BANK_0WS | MEM_BANK_OE0     | MEM_BANK_CS0;
    MB2CR = MEM_BANK_0WS | MEM_BANK_OE1_WE1 | MEM_BANK_CS1;

    // run at max speed
    GCSR = SPEED_MAIN_CLOCK;
    GCDR = DOUBLER_8NS;

    calc_div_19200();

    // disable watchdog
    WDTTR = 0x51;
    WDTTR = 0x54;

    PCFR = PORTC_PIN6_TXA | PORTC_PIN4_TXB | PORTC_PIN2_TXC | PORTC_PIN0_TXD;

    TAPR  = TAPS  = TIMERA_CLOCK_PERIF_DIV2;
    TACR  = TACS  = TIMERAx_CLOCK_MAIN;
    TACSR = TACSS = TIMERA_CLOCK_ENABLE;

    ivt_intern_isr(INT_PERIODIC, &isr_periodic);
    GCSR = SPEED_MAIN_CLOCK | INT_PRIO3;

    __asm__("ipset 0");
}

void sleep(word msec) _sdcccall
{
    word start = msec_count;
    word end = start + msec;

    if (end < start)
        while (msec_count >= start); // wait for msec_count to overflow
    while (msec_count < end);
}
