/*

Copyright 2013 Allie Saia <allie@fcraft.net>
    https://github.com/astelyn

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef _CPU_H
#define _CPU_H

#include <stdint.h>
#include "mmu.h"

#define REG_AF cpu.af.w
#define REG_BC cpu.bc.w
#define REG_DE cpu.de.w
#define REG_HL cpu.hl.w
#define REG_PC cpu.pc
#define REG_SP cpu.sp

#define FLAG_Z cpu.z
#define FLAG_N cpu.n
#define FLAG_H cpu.h
#define FLAG_C cpu.c

#ifdef _BIG_ENDIAN
#define REG_A cpu.af.b.h
#define REG_F cpu.af.b.l
#define REG_B cpu.bc.b.h
#define REG_C cpu.bc.b.l
#define REG_D cpu.bc.b.h
#define REG_E cpu.bc.b.l
#define REG_H cpu.bc.b.h
#define REG_L cpu.bc.b.l
#else
#define REG_A cpu.af.b.l
#define REG_F cpu.af.b.h
#define REG_B cpu.bc.b.l
#define REG_C cpu.bc.b.h
#define REG_D cpu.bc.b.l
#define REG_E cpu.bc.b.h
#define REG_H cpu.bc.b.l
#define REG_L cpu.bc.b.h
#endif

/* Double 8-bit registers */
typedef union {
    uint16_t w; /* 16-bit register */
    struct {
        uint8_t h; /* High byte */
        uint8_t l; /* Low byte */
    } b;
} cpureg_t;

typedef struct {
    uint32_t m; /* Machine cycles */
    uint32_t t; /* Clock periods */
} cpuclock_t;

typedef struct {
    mmu_t* mmu;              /* Memory */
    cpuclock_t sys_clock;    /* Master clock */
    cpuclock_t ins_clock;    /* Last instruction clock */
    cpureg_t af, bc, de, hl; /* 8-bit registers */
    uint16_t pc, sp;         /* 16-bit registers */
    uint8_t z, n, h, c;      /* Status flags */
    uint8_t op;              /* Current opcode */
} cpu_t;

/* Executive functions */
void cpu_reset(void);
int cpu_run(int cycles);

#endif

