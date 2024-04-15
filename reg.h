////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef REG_H
#define REG_H

#include "types.h"

_reg _at (0x0000) GCSR;
_reg _at (0x0002) RTC0R;
_reg _at (0x0003) RTC1R;
_reg _at (0x0004) RTC2R;
_reg _at (0x0005) RTC3R;
_reg _at (0x0006) RTC4R;
_reg _at (0x0007) RTC5R;
_reg _at (0x0008) WDTCR;
_reg _at (0x0009) WDTTR;
_reg _at (0x000f) GCDR;
_reg _at (0x0014) MB0CR;
_reg _at (0x0015) MB1CR;
_reg _at (0x0016) MB2CR;
_reg _at (0x0017) MB3CR;
_reg _at (0x0024) SPCR;

_reg _at (0x0040) PBDR;
_reg _at (0x0047) PBDDR;
_reg _at (0x0050) PCDR;
_reg _at (0x0055) PCFR;

_reg _at (0x00a0) TACSR;
_reg _at (0x00a1) TAPR;
_reg _at (0x00a3) TAT1R;
_reg _at (0x00a4) TACR;
_reg _at (0x00a5) TAT2R;
_reg _at (0x00a6) TAT8R;
_reg _at (0x00a7) TAT3R;
_reg _at (0x00a8) TAT9R;
_reg _at (0x00a9) TAT4R;
_reg _at (0x00aa) TAT10R;
_reg _at (0x00ab) TAT5R;
_reg _at (0x00ad) TAT6R;
_reg _at (0x00af) TAT7R;

_reg _at (0x00c0) SADR;
_reg _at (0x00c1) SAAR;
_reg _at (0x00c2) SALR;
_reg _at (0x00c3) SASR;
_reg _at (0x00c4) SACR;
_reg _at (0x00c5) SAER;

_reg _at (0x00d0) SBDR;
_reg _at (0x00d1) SBAR;
_reg _at (0x00d2) SBLR;
_reg _at (0x00d3) SBSR;
_reg _at (0x00d4) SBCR;
_reg _at (0x00d5) SBER;

_reg _at (0x00e0) SCDR;
_reg _at (0x00e1) SCAR;
_reg _at (0x00e2) SCLR;
_reg _at (0x00e3) SCSR;
_reg _at (0x00e4) SCCR;
_reg _at (0x00e5) SCER;

_reg _at (0x00f0) SDDR;
_reg _at (0x00f1) SDAR;
_reg _at (0x00f2) SDLR;
_reg _at (0x00f3) SDSR;
_reg _at (0x00f4) SDCR;
_reg _at (0x00f5) SDER;

void reg_write(word n, byte d) _sdcccall;
byte reg_read(word n) _sdcccall;

////////////////////////////////////////////////////////////////////////////////
enum // GCSR
{
    PERIODIC_INT_DISABLE = 0x00,
    PERIODIC_INT_PRIO1 = 0x01,
    PERIODIC_INT_PRIO2 = 0x02,
    PERIODIC_INT_PRIO3 = 0x03,

    SPEED_MAIN_CLOCK = 0x08,
    SPEED_MAIN_CLOCK_DIV2 = 0x0c,
    SPEED_MAIN_CLOCK_DIV4 = 0x18,
    SPEED_MAIN_CLOCK_DIV6 = 0x0c,
    SPEED_MAIN_CLOCK_DIV8 = 0x00,
};

enum // GCDR
{
    DOUBLER_DISABLE = 0x00, // >52.8384 MHz
    DOUBLER_8NS     = 0x03, //  20.2752 - 52.8384 MHz
    DOUBLER_11NS    = 0x06, //  16.5888 - 20.2752 MHz
    DOUBLER_14NS    = 0x09, //  11.0592 - 16.5888 MHz
    DOUBLER_16NS    = 0x0b, //   7.3728 - 11.0592 MHz
    DOUBLER_20NS    = 0x0f, // <=7.3728 MHz
};

enum // MBxCR
{
    MEM_BANK_CS0 = 0x00,
    MEM_BANK_CS1 = 0x01,
    MEM_BANK_CS2 = 0x02,

    MEM_BANK_OE0_WE0 = 0x00,
    MEM_BANK_OE1_WE1 = 0x04,
    MEM_BANK_OE0 = 0x08,
    MEM_BANK_OE1 = 0x0c,

    MEM_BANK_4WS = 0x00,
    MEM_BANK_2WS = 0x40,
    MEM_BANK_1WS = 0x80,
    MEM_BANK_0WS = 0xc0,
};

enum // SPCR
{
    SMODE_DISABLE = 0x80,
    SMODE_ENABLE = 0x00,
};

enum // PBDDR
{
    PORTB_PIN0_7_INPUT = 0x00,
    PORTB_PIN0_7_OUTPUT = 0xff,
};

enum // PCFR
{
    PORTC_PIN0_TXD = 0x01,
    PORTC_PIN2_TXC = 0x04,
    PORTC_PIN4_TXB = 0x10,
    PORTC_PIN6_TXA = 0x40,
};

enum // TACSR
{
    TIMERA_CLOCK_ENABLE = 0x01,

    TIMERA1_INT_DISABLE = 0x00,
    TIMERA1_INT_ENABLE = 0x02,

    TIMERA2_7_INT_DISABLE = 0x00,
    TIMERA2_INT_ENABLE = 0x04,
    TIMERA3_INT_ENABLE = 0x08,
    TIMERA4_INT_ENABLE = 0x10,
    TIMERA5_INT_ENABLE = 0x20,
    TIMERA6_INT_ENABLE = 0x40,
    TIMERA7_INT_ENABLE = 0x80,
};

enum // TAPR
{
    TIMERA_CLOCK_PERIF = 0x00,
    TIMERA_CLOCK_PERIF_DIV2 = 0x01,
};

enum // TACR
{
    TIMERA_INT_DISABLE = 0x00,
    TIMERA_INT_PRIO1 = 0x01,
    TIMERA_INT_PRIO2 = 0x02,
    TIMERA_INT_PRIO3 = 0x03,

    TIMERA2_7_CLOCK_MAIN = 0x00,
    TIMERA2_CLOCK_A1 = 0x04,
    TIMERA3_CLOCK_A1 = 0x08,
    TIMERA4_CLOCK_A1 = 0x10,
    TIMERA5_CLOCK_A1 = 0x20,
    TIMERA6_CLOCK_A1 = 0x40,
    TIMERA7_CLOCK_A1 = 0x80,
};

enum // SACR
{
    SERIALA_INT_DISABLE = 0x00,
    SERIALA_INT_PRIO1 = 0x01,
    SERIALA_INT_PRIO2 = 0x02,
    SERIALA_INT_PRIO3 = 0x03,

    SERIALA_MODE_8BIT = 0x00,

    SERIALA_PORTC_IN = 0x00,
    SERIALA_PORTD_IN = 0x01,
    SERIALA_DISABLE_IN = 0x10,
};

enum // SBCR
{
    SERIALB_INT_DISABLE = 0x00,
    SERIALB_INT_PRIO1 = 0x01,
    SERIALB_INT_PRIO2 = 0x02,
    SERIALB_INT_PRIO3 = 0x03,

    SERIALB_MODE_8BIT = 0x00,

    SERIALB_PORTC_IN = 0x00,
    SERIALB_PORTD_IN = 0x01,
    SERIALB_DISABLE_IN = 0x10,
};

////////////////////////////////////////////////////////////////////////////////
#endif
