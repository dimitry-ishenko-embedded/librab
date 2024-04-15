////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef REG_H
#define REG_H

#include "types.h"

#define reg(addr, name) enum { _ ## name = addr }; _reg _at (_ ## name) name;

// I/O registers
reg(0x0000, GCSR )
reg(0x0002, RTC0R)
reg(0x0003, RTC1R)
reg(0x0004, RTC2R)
reg(0x0005, RTC3R)
reg(0x0006, RTC4R)
reg(0x0007, RTC5R)
reg(0x0008, WDTCR)
reg(0x0009, WDTTR)
reg(0x000f, GCDR )
reg(0x0014, MB0CR)
reg(0x0015, MB1CR)
reg(0x0016, MB2CR)
reg(0x0017, MB3CR)
reg(0x0024, SPCR )

reg(0x0040, PBDR )
reg(0x0047, PBDDR)
reg(0x0050, PCDR )
reg(0x0055, PCFR )
reg(0x0070, PEDR )
reg(0x0074, PECR )
reg(0x0075, PEFR )
reg(0x0077, PEDDR)
reg(0x0078, PEB0R)
reg(0x0079, PEB1R)
reg(0x007a, PEB2R)
reg(0x007b, PEB3R)
reg(0x007c, PEB4R)
reg(0x007d, PEB5R)
reg(0x007e, PEB6R)
reg(0x007f, PEB7R)

reg(0x00a0, TACSR)
reg(0x00a1, TAPR )
reg(0x00a3, TAT1R)
reg(0x00a4, TACR )
reg(0x00a5, TAT2R)
reg(0x00a6, TAT8R)
reg(0x00a7, TAT3R)
reg(0x00a8, TAT9R)
reg(0x00a9, TAT4R)
reg(0x00aa, TAT10R)
reg(0x00ab, TAT5R)
reg(0x00ad, TAT6R)
reg(0x00af, TAT7R)

reg(0x00c0, SADR )
reg(0x00c1, SAAR )
reg(0x00c2, SALR )
reg(0x00c3, SASR )
reg(0x00c4, SACR )
reg(0x00c5, SAER )

reg(0x00d0, SBDR )
reg(0x00d1, SBAR )
reg(0x00d2, SBLR )
reg(0x00d3, SBSR )
reg(0x00d4, SBCR )
reg(0x00d5, SBER )

reg(0x00e0, SCDR )
reg(0x00e1, SCAR )
reg(0x00e2, SCLR )
reg(0x00e3, SCSR )
reg(0x00e4, SCCR )
reg(0x00e5, SCER )

reg(0x00f0, SDDR )
reg(0x00f1, SDAR )
reg(0x00f2, SDLR )
reg(0x00f3, SDSR )
reg(0x00f4, SDCR )
reg(0x00f5, SDER )

// shadow registers
extern byte _at (0xdf00) GCSS;

extern byte _at (0xdfa0) TACSS;
extern byte _at (0xdfa1) TAPS;
extern byte _at (0xdfa4) TACS;

////////////////////////////////////////////////////////////////////////////////
enum // common interrupt priorities
{
    INT_NMASK = 0xfc,
    INT_PRIO1 = 0x01,
    INT_PRIO2 = 0x02,
    INT_PRIO3 = 0x03,
};

enum // GCSR
{
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
    PORTB_PINx_INPUT = 0x00,
    PORTB_PINx_OUTPUT = 0xff,
};

enum // PCFR
{
    PORTC_PIN0_TXD = 0x01,
    PORTC_PIN2_TXC = 0x04,
    PORTC_PIN4_TXB = 0x10,
    PORTC_PIN6_TXA = 0x40,
};

enum // PECR
{
    PORTE_CLOCK_PERIF_DIV2 = 0x00,
    PORTE_CLOCK_TIMERA1 = 0x11,
    PORTE_CLOCK_TIMERB1 = 0x22,
    PORTE_CLOCK_TIMERB2 = 0x33,
};

enum // PEFR
{
    PORTE_PINx_GPIO = 0x00,
    PORTE_PINx_IO_STROBE = 0xff,
};

enum // PEDDR
{
    PORTE_PINx_INPUT = 0x00,
    PORTE_PINx_OUTPUT = 0xff,
};

enum // TACSR
{
    TIMERA_CLOCK_DISABLE = 0x00,
    TIMERA_CLOCK_ENABLE = 0x01,
};

enum // TAPR
{
    TIMERA_CLOCK_PERIF = 0x00,
    TIMERA_CLOCK_PERIF_DIV2 = 0x01,
};

enum // TACR
{
    TIMERAx_CLOCK_MAIN = 0x00,

    TIMERA2_CLOCK_A1 = 0x04,
    TIMERA3_CLOCK_A1 = 0x08,
    TIMERA4_CLOCK_A1 = 0x10,
    TIMERA5_CLOCK_A1 = 0x20,
    TIMERA6_CLOCK_A1 = 0x40,
    TIMERA7_CLOCK_A1 = 0x80,
};

enum // SACR
{
    SERIALA_MODE_8BIT = 0x00,

    SERIALA_PORTC_IN = 0x00,
    SERIALA_PORTD_IN = 0x01,
    SERIALA_DISABLE_IN = 0x10,
};

enum // SBCR
{
    SERIALB_MODE_8BIT = 0x00,

    SERIALB_PORTC_IN = 0x00,
    SERIALB_PORTD_IN = 0x01,
    SERIALB_DISABLE_IN = 0x10,
};

////////////////////////////////////////////////////////////////////////////////
#endif
