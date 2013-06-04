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

#include "cpu.h"

/* Reset all CPU registers */
void reset(cpu_t* cpu)
{
    REG_A = 0;
    REG_F = 0;
    REG_B = 0;
    REG_C = 0;
    REG_D = 0;
    REG_E = 0;
    REG_H = 0;
    REG_L = 0;

    cpu->pc = 0;
    cpu->sp = 0;

    cpu->sys_clock.m = 0;
    cpu->sys_clock.t = 0;
    cpu->ins_clock.m = 0;
    cpu->ins_clock.t = 0;

    cpu->op = 0;
}

/* Emulate one CPU cycle */
void step(cpu_t* cpu)
{
    cpu->op = read_8(cpu->mmu, pc++);

    (*ops[cpu->op])(cpu);

    cpu->pc &= 65535;

    cpu->ins_clock.m = timings_m[op];
    cpu->ins_clock.t = timings_t[op];

    cpu->sys_clock.m += cpu->ins_clock.m;
    cpu->sys_clock.t += cpu->ins_clock.t;

    if (in_bios(cpu->mmu) && cpu->pc == 0x0100)
        cpu->mmu->bios = 0;
}


/* Misc ops */
// Jump to 2-byte op table */
void CB(cpu_t* cpu)
{
    (*cb_ops[cpu->op])(cpu);
}

// No op
void NOP(cpu_t* cpu) {}

// HALT
void HALT(cpu_t* cpu)
{

}

// STOP
void STOP(cpu_t* cpu)
{

}

// Disable I
void DI(cpu_t* cpu)
{

}

// Enable I
void EI(cpu_t* cpu)
{

}

// DAA
void DAA(cpu_t* cpu)
{

}


/* Load/storage ops */
// LD B <- X
void LDbb(cpu_t* cpu) { cpu->b = cpu->b; }
void LDbc(cpu_t* cpu) { cpu->b = cpu->c; }
void LDbd(cpu_t* cpu) { cpu->b = cpu->d; }
void LDbe(cpu_t* cpu) { cpu->b = cpu->e; }
void LDbh(cpu_t* cpu) { cpu->b = cpu->h; }
void LDbl(cpu_t* cpu) { cpu->b = cpu->l; }
void LDba(cpu_t* cpu) { cpu->b = cpu->a; }

// LD C <- X
void LDcb(cpu_t* cpu) { cpu->c = cpu->b; }
void LDcc(cpu_t* cpu) { cpu->c = cpu->c; }
void LDcd(cpu_t* cpu) { cpu->c = cpu->d; }
void LDce(cpu_t* cpu) { cpu->c = cpu->e; }
void LDch(cpu_t* cpu) { cpu->c = cpu->h; }
void LDcl(cpu_t* cpu) { cpu->c = cpu->l; }
void LDca(cpu_t* cpu) { cpu->c = cpu->a; }

// LD D <- X
void LDdb(cpu_t* cpu) { cpu->d = cpu->b; }
void LDdc(cpu_t* cpu) { cpu->d = cpu->c; }
void LDdd(cpu_t* cpu) { cpu->d = cpu->d; }
void LDde(cpu_t* cpu) { cpu->d = cpu->e; }
void LDdh(cpu_t* cpu) { cpu->d = cpu->h; }
void LDdl(cpu_t* cpu) { cpu->d = cpu->l; }
void LDda(cpu_t* cpu) { cpu->d = cpu->a; }

// LD E <- X
void LDeb(cpu_t* cpu) { cpu->e = cpu->b; }
void LDec(cpu_t* cpu) { cpu->e = cpu->c; }
void LDed(cpu_t* cpu) { cpu->e = cpu->d; }
void LDee(cpu_t* cpu) { cpu->e = cpu->e; }
void LDeh(cpu_t* cpu) { cpu->e = cpu->h; }
void LDel(cpu_t* cpu) { cpu->e = cpu->l; }
void LDea(cpu_t* cpu) { cpu->e = cpu->a; }

// LD H <- X
void LDhb(cpu_t* cpu) { cpu->h = cpu->b; }
void LDhc(cpu_t* cpu) { cpu->h = cpu->c; }
void LDhd(cpu_t* cpu) { cpu->h = cpu->d; }
void LDhe(cpu_t* cpu) { cpu->h = cpu->e; }
void LDhh(cpu_t* cpu) { cpu->h = cpu->h; }
void LDhl(cpu_t* cpu) { cpu->h = cpu->l; }
void LDha(cpu_t* cpu) { cpu->h = cpu->a; }

// LD L <- X
void LDlb(cpu_t* cpu) { cpu->l = cpu->b; }
void LDlc(cpu_t* cpu) { cpu->l = cpu->c; }
void LDld(cpu_t* cpu) { cpu->l = cpu->d; }
void LDle(cpu_t* cpu) { cpu->l = cpu->e; }
void LDlh(cpu_t* cpu) { cpu->l = cpu->h; }
void LDll(cpu_t* cpu) { cpu->l = cpu->l; }
void LDla(cpu_t* cpu) { cpu->l = cpu->a; }

// LD A <- X
void LDab(cpu_t* cpu) { cpu->a = cpu->b; }
void LDac(cpu_t* cpu) { cpu->a = cpu->c; }
void LDad(cpu_t* cpu) { cpu->a = cpu->d; }
void LDae(cpu_t* cpu) { cpu->a = cpu->e; }
void LDah(cpu_t* cpu) { cpu->a = cpu->h; }
void LDal(cpu_t* cpu) { cpu->a = cpu->l; }
void LDaa(cpu_t* cpu) { cpu->a = cpu->a; }

// LD X <- (HL)
void LDbmHL(cpu_t* cpu) { cpu->b = read_8(cpu->mmu, cpu->h << 8) + cpu->l; }
void LDcmHL(cpu_t* cpu) { cpu->c = read_8(cpu->mmu, cpu->h << 8) + cpu->l; }
void LDdmHL(cpu_t* cpu) { cpu->d = read_8(cpu->mmu, cpu->h << 8) + cpu->l; }
void LDemHL(cpu_t* cpu) { cpu->e = read_8(cpu->mmu, cpu->h << 8) + cpu->l; }
void LDhmHL(cpu_t* cpu) { cpu->h = read_8(cpu->mmu, cpu->h << 8) + cpu->l; }
void LDlmHL(cpu_t* cpu) { cpu->l = read_8(cpu->mmu, cpu->h << 8) + cpu->l; }
void LDamHL(cpu_t* cpu) { cpu->a = read_8(cpu->mmu, cpu->h << 8) + cpu->l; }

// LD (HL) <- X
void LDmHLb(cpu_t* cpu) { write_8(cpu->mmu, (cpu->h << 8) + cpu->l, cpu->b); }
void LDmHLc(cpu_t* cpu) { write_8(cpu->mmu, (cpu->h << 8) + cpu->l, cpu->c); }
void LDmHLd(cpu_t* cpu) { write_8(cpu->mmu, (cpu->h << 8) + cpu->l, cpu->d); }
void LDmHLe(cpu_t* cpu) { write_8(cpu->mmu, (cpu->h << 8) + cpu->l, cpu->e); }
void LDmHLh(cpu_t* cpu) { write_8(cpu->mmu, (cpu->h << 8) + cpu->l, cpu->h); }
void LDmHLl(cpu_t* cpu) { write_8(cpu->mmu, (cpu->h << 8) + cpu->l, cpu->l); }
void LDmHLa(cpu_t* cpu) { write_8(cpu->mmu, (cpu->h << 8) + cpu->l, cpu->a); }

// LD X <- n
void LDbn(cpu_t* cpu) { cpu->b = read_8(cpu->mmu, cpu->pc); cpu->pc++; }
void LDcn(cpu_t* cpu) { cpu->c = read_8(cpu->mmu, cpu->pc); cpu->pc++; }
void LDdn(cpu_t* cpu) { cpu->d = read_8(cpu->mmu, cpu->pc); cpu->pc++; }
void LDen(cpu_t* cpu) { cpu->e = read_8(cpu->mmu, cpu->pc); cpu->pc++; }
void LDhn(cpu_t* cpu) { cpu->h = read_8(cpu->mmu, cpu->pc); cpu->pc++; }
void LDln(cpu_t* cpu) { cpu->l = read_8(cpu->mmu, cpu->pc); cpu->pc++; }
void LDan(cpu_t* cpu) { cpu->a = read_8(cpu->mmu, cpu->pc); cpu->pc++; }

// LD XX <- nn
void LDBCnn(cpu_t* cpu)
{
    cpu->c = read_8(cpu->mmu, cpu->pc);
    cpu->b = read_8(cpu->mmu, cpu->pc + 1);
    cpu->pc += 2;
}

void LDDEnn(cpu_t* cpu)
{
    cpu->e = read_8(cpu->mmu, cpu->pc);
    cpu->d = read_8(cpu->mmu, cpu->pc + 1);
    cpu->pc += 2;
}

void LDHLnn(cpu_t* cpu)
{
    cpu->l = read_8(cpu->mmu, cpu->pc);
    cpu->h = read_8(cpu->mmu, cpu->pc + 1);
    cpu->pc += 2;
}

void LDSPnn(cpu_t* cpu)
{
    cpu->p = read_8(cpu->mmu, cpu->pc);
    cpu->s = read_8(cpu->mmu, cpu->pc + 1);
    cpu->pc += 2;
}

// LD (HL) <- n
void LDmHLn(cpu_t* cpu)
{

}


// LD (XX) <- a
void LDmBCa(cpu_t* cpu)
{

}

void LDmDEa(cpu_t* cpu)
{

}


// LD a <- (XX)
void LDamBC(cpu_t* cpu)
{

}

void LDamDE(cpu_t* cpu)
{

}


// LDI (HL) <- a
void LDImHLa(cpu_t* cpu)
{

}


// LDI a <- (HL)
void LDIamHL(cpu_t* cpu)
{

}


// LDD (HL) <- a
void LDDmHLa(cpu_t* cpu)
{

}


//LDD a <- (HL)
void LDDamHL(cpu_t* cpu)
{

}

// LD (nn) <- SP
void LDmnnSP(cpu_t* cpu)
{

}


// LD (nn) <- a
void LDmnna(cpu_t* cpu)
{

}


// LD a <- (nn)
void LDamnn(cpu_t* cpu)
{

}


// LDH (n) <- a
void LDHmna(cpu_t* cpu)
{

}


// LDH a <- (n)
void LDHamn(cpu_t* cpu)
{

}


// LDH (c) <- a
void LDHmca(cpu_t* cpu)
{

}


// LD SP <- HL
void LDSPHL(cpu_t* cpu)
{

}


// LDHL SP <- n
void LDHLSPn(cpu_t* cpu)
{

}


// SWAP X
void SWAPb(cpu_t* cpu)
{

}

void SWAPc(cpu_t* cpu)
{

}

void SWAPd(cpu_t* cpu)
{

}

void SWAPe(cpu_t* cpu)
{

}

void SWAPh(cpu_t* cpu)
{

}

void SWAPl(cpu_t* cpu)
{

}

void SWAPmHL(cpu_t* cpu)
{

}

void SWAPa(cpu_t* cpu)
{

}


/* Arithmetic/logic ops */
// ADD a, X
void ADDab(cpu_t* cpu)
{

}

void ADDac(cpu_t* cpu)
{

}

void ADDad(cpu_t* cpu)
{

}

void ADDae(cpu_t* cpu)
{

}

void ADDah(cpu_t* cpu)
{

}

void ADDal(cpu_t* cpu)
{

}

void ADDamHL(cpu_t* cpu)
{

}

void ADDaa(cpu_t* cpu)
{

}

void ADDan(cpu_t* cpu)
{

}


// ADD HL, XX
void ADDHLBC(cpu_t* cpu)
{

}

void ADDHLDE(cpu_t* cpu)
{

}

void ADDHLHL(cpu_t* cpu)
{

}

void ADDHLSP(cpu_t* cpu)
{

}


// ADD SP, n
void ADDSPn(cpu_t* cpu)
{

}


// ADC a, X
void ADCab(cpu_t* cpu)
{

}

void ADCac(cpu_t* cpu)
{

}

void ADCad(cpu_t* cpu)
{

}

void ADCae(cpu_t* cpu)
{

}

void ADCah(cpu_t* cpu)
{

}

void ADCal(cpu_t* cpu)
{

}

void ADCamHL(cpu_t* cpu)
{

}

void ADCaa(cpu_t* cpu)
{

}

void ADCan(cpu_t* cpu)
{

}


// SUB a, X
void SUBab(cpu_t* cpu)
{

}

void SUBac(cpu_t* cpu)
{

}

void SUBad(cpu_t* cpu)
{

}

void SUBae(cpu_t* cpu)
{

}

void SUBah(cpu_t* cpu)
{

}

void SUBal(cpu_t* cpu)
{

}

void SUBamHL(cpu_t* cpu)
{

}

void SUBaa(cpu_t* cpu)
{

}

void SUBan(cpu_t* cpu)
{

}


// SBC a, X
void SBCab(cpu_t* cpu)
{

}

void SBCac(cpu_t* cpu)
{

}

void SBCad(cpu_t* cpu)
{

}

void SBCae(cpu_t* cpu)
{

}

void SBCah(cpu_t* cpu)
{

}

void SBCal(cpu_t* cpu)
{

}

void SBCamHL(cpu_t* cpu)
{

}

void SBCaa(cpu_t* cpu)
{

}

void SBCan(cpu_t* cpu)
{

}


// CP X
void CPb(cpu_t* cpu)
{

}

void CPc(cpu_t* cpu)
{

}

void CPd(cpu_t* cpu)
{

}

void CPe(cpu_t* cpu)
{

}

void CPh(cpu_t* cpu)
{

}

void CPl(cpu_t* cpu)
{

}

void CPmHL(cpu_t* cpu)
{

}

void CPa(cpu_t* cpu)
{

}

void CPn(cpu_t* cpu)
{

}


// AND X
void ANDb(cpu_t* cpu)
{

}

void ANDc(cpu_t* cpu)
{

}

void ANDd(cpu_t* cpu)
{

}

void ANDe(cpu_t* cpu)
{

}

void ANDh(cpu_t* cpu)
{

}

void ANDl(cpu_t* cpu)
{

}

void ANDmHL(cpu_t* cpu)
{

}

void ANDa(cpu_t* cpu)
{

}

void ANDn(cpu_t* cpu)
{

}


// OR X
void ORb(cpu_t* cpu)
{

}

void ORc(cpu_t* cpu)
{

}

void ORd(cpu_t* cpu)
{

}

void ORe(cpu_t* cpu)
{

}

void ORh(cpu_t* cpu)
{

}

void ORl(cpu_t* cpu)
{

}

void ORmHL(cpu_t* cpu)
{

}

void ORa(cpu_t* cpu)
{

}

void ORn(cpu_t* cpu)
{

}


// XOR X
void XORb(cpu_t* cpu)
{

}

void XORc(cpu_t* cpu)
{

}

void XORd(cpu_t* cpu)
{

}

void XORe(cpu_t* cpu)
{

}

void XORh(cpu_t* cpu)
{

}

void XORl(cpu_t* cpu)
{

}

void XORmHL(cpu_t* cpu)
{

}

void XORa(cpu_t* cpu)
{

}

void XORn(cpu_t* cpu)
{

}


// INC X
void INCb(cpu_t* cpu)
{

}

void INCc(cpu_t* cpu)
{

}

void INCd(cpu_t* cpu)
{

}

void INCe(cpu_t* cpu)
{

}

void INCh(cpu_t* cpu)
{

}

void INCl(cpu_t* cpu)
{

}

void INCmHL(cpu_t* cpu)
{

}

void INCa(cpu_t* cpu)
{

}

void INCn(cpu_t* cpu)
{

}


// INC XX
void INCBC(cpu_t* cpu)
{

}

void INCDE(cpu_t* cpu)
{

}

void INCHL(cpu_t* cpu)
{

}

void INCSP(cpu_t* cpu)
{

}


// DEC X
void DECb(cpu_t* cpu)
{

}

void DECc(cpu_t* cpu)
{

}

void DECd(cpu_t* cpu)
{

}

void DECe(cpu_t* cpu)
{

}

void DECh(cpu_t* cpu)
{

}

void DECl(cpu_t* cpu)
{

}

void DECmHL(cpu_t* cpu)
{

}

void DECa(cpu_t* cpu)
{

}


// DEC XX
void DECBC(cpu_t* cpu)
{

}

void DECDE(cpu_t* cpu)
{

}

void DECHL(cpu_t* cpu)
{

}

void DECSP(cpu_t* cpu)
{

}


/* Bit manipulation */
// BIT 0, X
void BIT0b(cpu_t* cpu)
{

}

void BIT0c(cpu_t* cpu)
{

}

void BIT0d(cpu_t* cpu)
{

}

void BIT0e(cpu_t* cpu)
{

}

void BIT0h(cpu_t* cpu)
{

}

void BIT0l(cpu_t* cpu)
{

}

void BIT0mHL(cpu_t* cpu)
{

}

void BIT0a(cpu_t* cpu)
{

}


// BIT 1, X
void BIT1b(cpu_t* cpu)
{

}

void BIT1c(cpu_t* cpu)
{

}

void BIT1d(cpu_t* cpu)
{

}

void BIT1e(cpu_t* cpu)
{

}

void BIT1h(cpu_t* cpu)
{

}

void BIT1l(cpu_t* cpu)
{

}

void BIT1mHL(cpu_t* cpu)
{

}

void BIT1a(cpu_t* cpu)
{

}


// BIT 2, X
void BIT2b(cpu_t* cpu)
{

}

void BIT2c(cpu_t* cpu)
{

}

void BIT2d(cpu_t* cpu)
{

}

void BIT2e(cpu_t* cpu)
{

}

void BIT2h(cpu_t* cpu)
{

}

void BIT2l(cpu_t* cpu)
{

}

void BIT2mHL(cpu_t* cpu)
{

}

void BIT2a(cpu_t* cpu)
{

}


// BIT 3, X
void BIT3b(cpu_t* cpu)
{

}

void BIT3c(cpu_t* cpu)
{

}

void BIT3d(cpu_t* cpu)
{

}

void BIT3e(cpu_t* cpu)
{

}

void BIT3h(cpu_t* cpu)
{

}

void BIT3l(cpu_t* cpu)
{

}

void BIT3mHL(cpu_t* cpu)
{

}

void BIT3a(cpu_t* cpu)
{

}


// BIT 4, X
void BIT4b(cpu_t* cpu)
{

}

void BIT4c(cpu_t* cpu)
{

}

void BIT4d(cpu_t* cpu)
{

}

void BIT4e(cpu_t* cpu)
{

}

void BIT4h(cpu_t* cpu)
{

}

void BIT4l(cpu_t* cpu)
{

}

void BIT4mHL(cpu_t* cpu)
{

}

void BIT4a(cpu_t* cpu)
{

}


// BIT 5, X
void BIT5b(cpu_t* cpu)
{

}

void BIT5c(cpu_t* cpu)
{

}

void BIT5d(cpu_t* cpu)
{

}

void BIT5e(cpu_t* cpu)
{

}

void BIT5h(cpu_t* cpu)
{

}

void BIT5l(cpu_t* cpu)
{

}

void BIT5mHL(cpu_t* cpu)
{

}

void BIT5a(cpu_t* cpu)
{

}


// BIT 6, X
void BIT6b(cpu_t* cpu)
{

}

void BIT6c(cpu_t* cpu)
{

}

void BIT6d(cpu_t* cpu)
{

}

void BIT6e(cpu_t* cpu)
{

}

void BIT6h(cpu_t* cpu)
{

}

void BIT6l(cpu_t* cpu)
{

}

void BIT6mHL(cpu_t* cpu)
{

}

void BIT6a(cpu_t* cpu)
{

}


// BIT 7, X
void BIT7b(cpu_t* cpu)
{

}

void BIT7c(cpu_t* cpu)
{

}

void BIT7d(cpu_t* cpu)
{

}

void BIT7e(cpu_t* cpu)
{

}

void BIT7h(cpu_t* cpu)
{

}

void BIT7l(cpu_t* cpu)
{

}

void BIT7mHL(cpu_t* cpu)
{

}

void BIT7a(cpu_t* cpu)
{

}


// RES 0, X
void RES0b(cpu_t* cpu)
{

}

void RES0c(cpu_t* cpu)
{

}

void RES0d(cpu_t* cpu)
{

}

void RES0e(cpu_t* cpu)
{

}

void RES0h(cpu_t* cpu)
{

}

void RES0l(cpu_t* cpu)
{

}

void RES0mHL(cpu_t* cpu)
{

}

void RES0a(cpu_t* cpu)
{

}


// RES 1, X
void RES1b(cpu_t* cpu)
{

}

void RES1c(cpu_t* cpu)
{

}

void RES1d(cpu_t* cpu)
{

}

void RES1e(cpu_t* cpu)
{

}

void RES1h(cpu_t* cpu)
{

}

void RES1l(cpu_t* cpu)
{

}

void RES1mHL(cpu_t* cpu)
{

}

void RES1a(cpu_t* cpu)
{

}


// RES 2, X
void RES2b(cpu_t* cpu)
{

}

void RES2c(cpu_t* cpu)
{

}

void RES2d(cpu_t* cpu)
{

}

void RES2e(cpu_t* cpu)
{

}

void RES2h(cpu_t* cpu)
{

}

void RES2l(cpu_t* cpu)
{

}

void RES2mHL(cpu_t* cpu)
{

}

void RES2a(cpu_t* cpu)
{

}


// RES 3, X
void RES3b(cpu_t* cpu)
{

}

void RES3c(cpu_t* cpu)
{

}

void RES3d(cpu_t* cpu)
{

}

void RES3e(cpu_t* cpu)
{

}

void RES3h(cpu_t* cpu)
{

}

void RES3l(cpu_t* cpu)
{

}

void RES3mHL(cpu_t* cpu)
{

}

void RES3a(cpu_t* cpu)
{

}


// RES 4, X
void RES4b(cpu_t* cpu)
{

}

void RES4c(cpu_t* cpu)
{

}

void RES4d(cpu_t* cpu)
{

}

void RES4e(cpu_t* cpu)
{

}

void RES4h(cpu_t* cpu)
{

}

void RES4l(cpu_t* cpu)
{

}

void RES4mHL(cpu_t* cpu)
{

}

void RES4a(cpu_t* cpu)
{

}


// RES 5, X
void RES5b(cpu_t* cpu)
{

}

void RES5c(cpu_t* cpu)
{

}

void RES5d(cpu_t* cpu)
{

}

void RES5e(cpu_t* cpu)
{

}

void RES5h(cpu_t* cpu)
{

}

void RES5l(cpu_t* cpu)
{

}

void RES5mHL(cpu_t* cpu)
{

}

void RES5a(cpu_t* cpu)
{

}


// RES 6, X
void RES6b(cpu_t* cpu)
{

}

void RES6c(cpu_t* cpu)
{

}

void RES6d(cpu_t* cpu)
{

}

void RES6e(cpu_t* cpu)
{

}

void RES6h(cpu_t* cpu)
{

}

void RES6l(cpu_t* cpu)
{

}

void RES6mHL(cpu_t* cpu)
{

}

void RES6a(cpu_t* cpu)
{

}


// RES 7, X
void RES7b(cpu_t* cpu)
{

}

void RES7c(cpu_t* cpu)
{

}

void RES7d(cpu_t* cpu)
{

}

void RES7e(cpu_t* cpu)
{

}

void RES7h(cpu_t* cpu)
{

}

void RES7l(cpu_t* cpu)
{

}

void RES7mHL(cpu_t* cpu)
{

}

void RES7a(cpu_t* cpu)
{

}


// SET 0, X
void SET0b(cpu_t* cpu)
{

}

void SET0c(cpu_t* cpu)
{

}

void SET0d(cpu_t* cpu)
{

}

void SET0e(cpu_t* cpu)
{

}

void SET0h(cpu_t* cpu)
{

}

void SET0l(cpu_t* cpu)
{

}

void SET0mHL(cpu_t* cpu)
{

}

void SET0a(cpu_t* cpu)
{

}


// SET 1, X
void SET1b(cpu_t* cpu)
{

}

void SET1c(cpu_t* cpu)
{

}

void SET1d(cpu_t* cpu)
{

}

void SET1e(cpu_t* cpu)
{

}

void SET1h(cpu_t* cpu)
{

}

void SET1l(cpu_t* cpu)
{

}

void SET1mHL(cpu_t* cpu)
{

}

void SET1a(cpu_t* cpu)
{

}


// SET 2, X
void SET2b(cpu_t* cpu)
{

}

void SET2c(cpu_t* cpu)
{

}

void SET2d(cpu_t* cpu)
{

}

void SET2e(cpu_t* cpu)
{

}

void SET2h(cpu_t* cpu)
{

}

void SET2l(cpu_t* cpu)
{

}

void SET2mHL(cpu_t* cpu)
{

}

void SET2a(cpu_t* cpu)
{

}


// SET 3, X
void SET3b(cpu_t* cpu)
{

}

void SET3c(cpu_t* cpu)
{

}

void SET3d(cpu_t* cpu)
{

}

void SET3e(cpu_t* cpu)
{

}

void SET3h(cpu_t* cpu)
{

}

void SET3l(cpu_t* cpu)
{

}

void SET3mHL(cpu_t* cpu)
{

}

void SET3a(cpu_t* cpu)
{

}


// SET 4, X
void SET4b(cpu_t* cpu)
{

}

void SET4c(cpu_t* cpu)
{

}

void SET4d(cpu_t* cpu)
{

}

void SET4e(cpu_t* cpu)
{

}

void SET4h(cpu_t* cpu)
{

}

void SET4l(cpu_t* cpu)
{

}

void SET4mHL(cpu_t* cpu)
{

}

void SET4a(cpu_t* cpu)
{

}


// SET 5, X
void SET5b(cpu_t* cpu)
{

}

void SET5c(cpu_t* cpu)
{

}

void SET5d(cpu_t* cpu)
{

}

void SET5e(cpu_t* cpu)
{

}

void SET5h(cpu_t* cpu)
{

}

void SET5l(cpu_t* cpu)
{

}

void SET5mHL(cpu_t* cpu)
{

}

void SET5a(cpu_t* cpu)
{

}


// SET 6, X
void SET6b(cpu_t* cpu)
{

}

void SET6c(cpu_t* cpu)
{

}

void SET6d(cpu_t* cpu)
{

}

void SET6e(cpu_t* cpu)
{

}

void SET6h(cpu_t* cpu)
{

}

void SET6l(cpu_t* cpu)
{

}

void SET6mHL(cpu_t* cpu)
{

}

void SET6a(cpu_t* cpu)
{

}


// SET 7, X
void SET7b(cpu_t* cpu)
{

}

void SET7c(cpu_t* cpu)
{

}

void SET7d(cpu_t* cpu)
{

}

void SET7e(cpu_t* cpu)
{

}

void SET7h(cpu_t* cpu)
{

}

void SET7l(cpu_t* cpu)
{

}

void SET7mHL(cpu_t* cpu)
{

}

void SET7a(cpu_t* cpu)
{

}


// RL X
void RLb(cpu_t* cpu)
{

}

void RLc(cpu_t* cpu)
{

}

void RLd(cpu_t* cpu)
{

}

void RLe(cpu_t* cpu)
{

}

void RLh(cpu_t* cpu)
{

}

void RLl(cpu_t* cpu)
{

}

void RLmHL(cpu_t* cpu)
{

}

void RLa(cpu_t* cpu)
{

}


// RR X
void RRb(cpu_t* cpu)
{

}

void RRc(cpu_t* cpu)
{

}

void RRd(cpu_t* cpu)
{

}

void RRe(cpu_t* cpu)
{

}

void RRh(cpu_t* cpu)
{

}

void RRl(cpu_t* cpu)
{

}

void RRmHL(cpu_t* cpu)
{

}

void RRa(cpu_t* cpu)
{

}


// RLC X
void RLCb(cpu_t* cpu)
{

}

void RLCc(cpu_t* cpu)
{

}

void RLCd(cpu_t* cpu)
{

}

void RLCe(cpu_t* cpu)
{

}

void RLCh(cpu_t* cpu)
{

}

void RLCl(cpu_t* cpu)
{

}

void RLCmHL(cpu_t* cpu)
{

}

void RLCa(cpu_t* cpu)
{

}


// RRC X
void RRCb(cpu_t* cpu)
{

}

void RRCc(cpu_t* cpu)
{

}

void RRCd(cpu_t* cpu)
{

}

void RRCe(cpu_t* cpu)
{

}

void RRCh(cpu_t* cpu)
{

}

void RRCl(cpu_t* cpu)
{

}

void RRCmHL(cpu_t* cpu)
{

}

void RRCa(cpu_t* cpu)
{

}


// RLA
void RLA(cpu_t* cpu)
{

}


// RRA
void RRA(cpu_t* cpu)
{

}


// RLCA
void RLCA(cpu_t* cpu)
{

}


// RRCA
void RRCA(cpu_t* cpu)
{

}


// SLA X
void SLAb(cpu_t* cpu)
{

}

void SLAc(cpu_t* cpu)
{

}

void SLAd(cpu_t* cpu)
{

}

void SLAe(cpu_t* cpu)
{

}

void SLAh(cpu_t* cpu)
{

}

void SLAl(cpu_t* cpu)
{

}

void SLAmHL(cpu_t* cpu)
{

}

void SLAa(cpu_t* cpu)
{

}


// SRA X
void SRAb(cpu_t* cpu)
{

}

void SRAc(cpu_t* cpu)
{

}

void SRAd(cpu_t* cpu)
{

}

void SRAe(cpu_t* cpu)
{

}

void SRAh(cpu_t* cpu)
{

}

void SRAl(cpu_t* cpu)
{

}

void SRAmHL(cpu_t* cpu)
{

}

void SRAa(cpu_t* cpu)
{

}


// SRL X
void SRLb(cpu_t* cpu)
{

}

void SRLc(cpu_t* cpu)
{

}

void SRLd(cpu_t* cpu)
{

}

void SRLe(cpu_t* cpu)
{

}

void SRLh(cpu_t* cpu)
{

}

void SRLl(cpu_t* cpu)
{

}

void SRLmHL(cpu_t* cpu)
{

}

void SRLa(cpu_t* cpu)
{

}


// CPL
void CPL(cpu_t* cpu)
{

}


// NEG
void NEG(cpu_t* cpu)
{

}


// CCF
void CCF(cpu_t* cpu)
{

}


// SCF
void SCF(cpu_t* cpu)
{

}


/* Stack ops */
// PUSH XX
void PUSHBC(cpu_t* cpu)
{

}

void PUSHDE(cpu_t* cpu)
{

}

void PUSHHL(cpu_t* cpu)
{

}

void PUSHAF(cpu_t* cpu)
{

}


// POP XX
void POPBC(cpu_t* cpu)
{

}

void POPDE(cpu_t* cpu)
{

}

void POPHL(cpu_t* cpu)
{

}

void POPAF(cpu_t* cpu)
{

}


/* Jump ops */
// JP
void JPnn(cpu_t* cpu)
{

}

void JPZnn(cpu_t* cpu)
{

}

void JPCnn(cpu_t* cpu)
{

}

void JPmHL(cpu_t* cpu)
{

}

void JPNZnn(cpu_t* cpu)
{

}

void JPNCnn(cpu_t* cpu)
{

}


// JR
void JRn(cpu_t* cpu)
{

}

void JRZn(cpu_t* cpu)
{

}

void JRCn(cpu_t* cpu)
{

}

void JRNZn(cpu_t* cpu)
{

}

void JRNCn(cpu_t* cpu)
{

}


// CALL
void CALLnn(cpu_t* cpu)
{

}

void CALLZnn(cpu_t* cpu)
{

}

void CALLCnn(cpu_t* cpu)
{

}

void CALLNZnn(cpu_t* cpu)
{

}

void CALLNCnn(cpu_t* cpu)
{

}


// RET
void RET(cpu_t* cpu)
{

}

void RETI(cpu_t* cpu)
{

}

void RETZ(cpu_t* cpu)
{

}

void RETC(cpu_t* cpu)
{

}

void RETNZ(cpu_t* cpu)
{

}

void RETNC(cpu_t* cpu)
{

}


// RST
void RST0(cpu_t* cpu)
{

}

void RST8(cpu_t* cpu)
{

}

void RST10(cpu_t* cpu)
{

}

void RST18(cpu_t* cpu)
{

}

void RST20(cpu_t* cpu)
{

}

void RST28(cpu_t* cpu)
{

}

void RST30(cpu_t* cpu)
{

}

void RST38(cpu_t* cpu)
{

}

int main(void)
{
    return 0;
}

