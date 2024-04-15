////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef REG_H
#define REG_H

#include "types.h"

// I/O registers
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
_reg _at (0x0024) SPCR ;

_reg _at (0x0040) PBDR;
_reg _at (0x0047) PBDDR;
_reg _at (0x0050) PCDR;
_reg _at (0x0055) PCFR;
_reg _at (0x0070) PEDR;
_reg _at (0x0074) PECR;
_reg _at (0x0075) PEFR;
_reg _at (0x0077) PEDDR;
_reg _at (0x0078) PEB0R;
_reg _at (0x0079) PEB1R;
_reg _at (0x007a) PEB2R;
_reg _at (0x007b) PEB3R;
_reg _at (0x007c) PEB4R;
_reg _at (0x007d) PEB5R;
_reg _at (0x007e) PEB6R;
_reg _at (0x007f) PEB7R;

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
    CPU_MAIN_CLOCK = 0x08,
    CPU_MAIN_CLOCK_DIV2 = 0x0c,
    CPU_MAIN_CLOCK_DIV4 = 0x18,
    CPU_MAIN_CLOCK_DIV6 = 0x0c,
    CPU_MAIN_CLOCK_DIV8 = 0x00,
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
    MB_CS0 = 0x00,
    MB_CS1 = 0x01,
    MB_CS2 = 0x02,

    MB_OE0_WE0 = 0x00,
    MB_OE1_WE1 = 0x04,
    MB_OE0 = 0x08,
    MB_OE1 = 0x0c,

    MB_4WS = 0x00,
    MB_2WS = 0x40,
    MB_1WS = 0x80,
    MB_0WS = 0xc0,
};

enum // SPCR
{
    SMODE_DISABLE = 0x80,
    SMODE_ENABLE = 0x00,
};

enum // PBDDR
{
    PBx_INPUT = 0x00,
    PBx_OUTPUT = 0xff,
};

enum // PCFR
{
    PC0_TXD = 0x01,
    PC2_TXC = 0x04,
    PC4_TXB = 0x10,
    PC6_TXA = 0x40,
};

enum // PECR
{
    PE_CLOCK_PERIF_DIV2 = 0x00,
    PE_CLOCK_TA1 = 0x11,
    PE_CLOCK_TB1 = 0x22,
    PE_CLOCK_TB2 = 0x33,
};

enum // PEFR
{
    PEx_GPIO = 0x00,
    PEx_IO_STROBE = 0xff,
};

enum // PEDDR
{
    PEx_INPUT = 0x00,
    PEx_OUTPUT = 0xff,
};

enum // TACSR
{
    TA_CLOCK_DISABLE = 0x00,
    TA_CLOCK_ENABLE = 0x01,
};

enum // TAPR
{
    TA_CLOCK_PERIF = 0x00,
    TA_CLOCK_PERIF_DIV2 = 0x01,
};

enum // TACR
{
    TAx_CLOCK_MAIN = 0x00,

    TA2_CLOCK_A1 = 0x04,
    TA3_CLOCK_A1 = 0x08,
    TA4_CLOCK_A1 = 0x10,
    TA5_CLOCK_A1 = 0x20,
    TA6_CLOCK_A1 = 0x40,
    TA7_CLOCK_A1 = 0x80,
};

enum // SACR
{
    SA_8BIT = 0x00,

    SA_PC_IN = 0x00,
    SA_PD_IN = 0x01,
    SA_DISABLE_IN = 0x10,
};

enum // SBCR
{
    SB_8BIT = 0x00,

    SB_PC_IN = 0x00,
    SB_PD_IN = 0x01,
    SB_DISABLE_IN = 0x10,
};

////////////////////////////////////////////////////////////////////////////////
#endif
