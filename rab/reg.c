////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "reg.h"

// shadow registers
byte _at (0xdf00) GCSS;

byte _at (0xdfa0) TACSS;
byte _at (0xdfa1) TAPS;
byte _at (0xdfa4) TACS;
