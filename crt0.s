;-------------------------------------------------------------------------------
; crt0.s for Rabbit 3000A
;
; Copyright (c) 2000 Michael Hope
; Copyright (c) 2020 Philipp Klaus Krause
; Copyright (c) 2023 Dimitry Ishenko <dimitry.ishenko (at) (gee) mail (dot) com>
;
; Distributed under the GNU GPL v3 license. See the LICENSE.md file for details.
;-------------------------------------------------------------------------------
    .module crt0
    .globl _main
    .globl ___sdcc_external_startup

GCSR     .equ 0x0000
STACKSEG .equ 0x0011
DATASEG  .equ 0x0012
SEGSIZE  .equ 0x0013
MB0CR    .equ 0x0014
MB2CR    .equ 0x0016

DATA     .equ 0x8000
STACK    .equ 0xa000
INT_IVT  .equ 0xdc00
EXT_IVT  .equ 0xde00

; RABBIT 3000A                            physical         1MB
; MEMORY MAP                              0xfffff +---------------+
;                                                 |               |
;                                                 |               |
;                                                 |               |
;                                                        ...
;                                                 |               |
;     logical                                     |               |
;     0xffff +---------------+                    | quadrant #3   |
;            |               |            0xc0000 +---------------+
;            | XPC segment   |                    |               |
;            |               |                    |               |
;     0xe000 +---------------+                    |               |          +---------------+
;            | unused        |                           ...                 | RAM           |
;     0xdf00 +---------------+                    |               |          | /CS1          |
;            | external IVT  |                    |               |          | /OE1          |
;     0xde00 +---------------+                    | quadrant #2   |          | /WE1          |
;            |               |       +--> 0x80000 +---------------+ (5) ---> +---------------+
;            | internal IVT  |       |            |               |
;     0xdc00 +---------------+       |            |               |
;            | stack         |       |            |               |
;            |               |       |                   ... 
;            |               |       |            |               |
;            |               |       |            |               |
;            | global data   |       |            | quadrant #1   |
; (1) 0xa000 +---------------+ (3) --+    0x40000 +---------------+
;            |               |                    |               |
;            |               |                    |               |
;            |               |                    |               |          +---------------+
; (1) 0x8000 |               | (2)                       ...                 | Flash         |
;            |               |                    |               |          | /CS0          |
;            | code and      |                    |               |          | /OE0          |
;            | const data    |                    | quadrant #0   |          | /WE0          |
;     0x0000 +---------------+ ---------> 0x00000 +---------------+ (4) ---> +---------------+
;
; notes:
; (1) SEGSIZE
; (2) DATASEG
; (3) STACKSEG
; (4) MB0CR
; (5) MB2CR

    .area _HEADER (ABS)
    .org 0

; map memory quadrants #0 and #2
    ld a, #0x08             ; /CS0, /OE1, 4ws
    ioi
    ld (MB0CR), a           ; read-only Flash @ physical address 0x00000

    ld a, #0x05             ; /CS1, /OE1, /WE1, 4ws
    ioi
    ld (MB2CR), a           ; read/write RAM @ physical address 0x80000

; set stack and data boundaries
    ld a, #((STACK >> 8) | (DATA >> 12))
    ioi
    ld (SEGSIZE), a

; map stack segment to the physical address 0x80000 (start of RAM)
; this segment is used for the interrupt vector tables (IVT), stack and global data
    ld a, #((0x80000 - STACK) >> 12)
    ioi
    ld (STACKSEG), a
    ld sp, #INT_IVT         ; set stack pointer below the internal IVT

; leave data segment mapped to 0x00000 since it is not being used
    ld a, #0x00
    ioi
    ld (DATASEG), a

; map internal and external IVT
    ld a, #(INT_IVT >> 8)
    ld iir, a
    ld a, #(EXT_IVT >> 8)
    ld eir, a

    call ___sdcc_external_startup
    ld a, l
    or a, a
    jr nz, dont_gsinit

    call gsinit

dont_gsinit:
    call _main
    jp _exit

    .area _HOME
    .area _CODE
    .area _INITIALIZER
    .area _GSINIT
    .area _GSFINAL

    .area _DATA
    .area _INITIALIZED
    .area _BSEG
    .area _BSS
    .area _HEAP

    .area _CODE
_exit::
    ld a, #0
    rst #0x28
1$: jr 1$

    .area _GSINIT
gsinit::
fill_zero:
    ld bc, #l__DATA
    ld a, b
    or a, c
    jr z, fill_data
    ld hl, #s__DATA
    ld (hl), #0x00
    dec bc
    ld a, b
    or a, c
    jr z, fill_data
    ld e, l
    ld d, h
    inc de
    ldir

fill_data:
    ld bc, #l__INITIALIZER
    ld a, b
    or a, c
    jr z, fill_done
    ld de, #s__INITIALIZED
    ld hl, #s__INITIALIZER
    ldir

fill_done:
    .area _GSFINAL
    ret
