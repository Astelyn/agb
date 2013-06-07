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

/* Global CPU structure */
cpu_t cpu;

/* Save states */
uint8_t curr_save_slot;
cpu_t save_states[10];

/* Helper functions */
void push(uint16_t val)
{
    REG_SP -= 2;
    write_16(cpu.mmu, REG_SP, val);
}

uint16_t pop(void)
{
    REG_SP += 2;
    return read_16(cpu.mmu, REG_SP - 2);
}

uint8_t add_8_8(uint8_t a, uint8_t b)
{
    uint8_t ret = a + b;

    FLAG_C = (0xFF - a < b);
    FLAG_H = (0x0F - (a & 0x0F) < (b & 0x0F));
    FLAG_Z = (ret == 0);
    FLAG_N = 0;

    return ret;
}

uint16_t add_16_16(uint16_t a, uint16_t b)
{
    uint16_t ret = a + b;

    FLAG_C = (0xFFFF - a < b);
    FLAG_H = (0x0FFF - (a & 0x0FFF) < (b & 0x0FFF));
    FLAG_N = 0;

    return ret;
}

uint16_t add_16_8(uint16_t a, uint8_t b)
{
    uint16_t ret = a + (uint16_t)b;

    FLAG_C = (0xFF - (a & 0x00ff) < b);
    FLAG_H = (0x0F - (a & 0x0F) < (b & 0x0F));
    FLAG_Z = 0;
    FLAG_N = 0;

    return ret;
}

uint8_t adc(uint8_t a, uint8_t b)
{
    uint8_t tmp, ret;
    uint8_t h = 0;
    uint8_t c = 0;

    tmp = a + FLAG_C;

    if (0x0F - (a & 0x0F) < (FLAG_C & 0x0F))
        h = 1;

    if (0xFF - a < FLAG_C)
        c = 1;

    ret = tmp + b;

    if (0x0F - (tmp & 0x0F) < (b & 0x0F))
        h = 1;

    if (0xFF - tmp < b)
        c = 1;

    FLAG_H = h;
    FLAG_C = c;
    FLAG_Z = (ret == 0);
    FLAG_N = 0;

    return ret;
}

uint8_t sub(uint8_t a, uint8_t b)
{
    uint8_t ret;

    FLAG_C = (a < b);
    FLAG_H = ((a & 0x0F) < (b & 0x0F));

    ret = a - b;

    FLAG_Z = (ret == 0);
    FLAG_N = 1;

    return ret;
}

uint8_t sbc(uint8_t a, uint8_t b)
{
    uint8_t tmp, ret;
    uint8_t h = 0;
    uint8_t c = 0;

    tmp = a - FLAG_C;

    if (a < FLAG_C || tmp < b)
        c = 1;

    if ((a & 0x0F) < (FLAG_C & 0x0F) || (tmp & 0x0F) < (b & 0x0F))
        h = 1;

    ret = tmp - b;

    FLAG_H = h;
    FLAG_C = c;
    FLAG_Z = (ret == 0);
    FLAG_N = 1;

    return ret;
}

uint8_t inc_8(uint8_t a)
{
    a++;

    FLAG_H = ((a & 0x0F) == 0);
    FLAG_Z = (a == 0);
    FLAG_N = 0;

    return a;
}

uint16_t inc_16(uint16_t a)
{
    return a + 1;
}

uint8_t dec_8(uint8_t a)
{
    a--;

    FLAG_H = ((a & 0x0F) == 0x0F);
    FLAG_Z = (a == 0);
    FLAG_N = 1;

    return a;
}

uint16_t dec_16(uint16_t a)
{
    return a - 1;
}

uint8_t and(uint8_t a, uint8_t b)
{
    uint8_t ret = a & b;

    FLAG_N = 0;
    FLAG_H = 1;
    FLAG_C = 0;
    FLAG_Z = (ret == 0);

    return ret;
}

uint8_t or(uint8_t a, uint8_t b)
{
    uint8_t ret = a | b;

    FLAG_N = 0;
    FLAG_H = 0;
    FLAG_C = 0;
    FLAG_Z = (ret == 0);

    return ret;
}

uint8_t xor(uint8_t a, uint8_t b)
{
    uint8_t ret = a ^ b;

    FLAG_N = 0;
    FLAG_H = 0;
    FLAG_C = 0;
    FLAG_Z = (ret == 0);

    return ret;
}

uint8_t swap(uint8_t a)
{
    uint8_t tmp = a & 0x0F;

    a >>= 4;
    a |= (tmp << 4);

    FLAG_Z = (a == 0);
    FLAG_N = 0;
    FLAG_H = 0;
    FLAG_C = 0;

    return a;
}

uint8_t rlc(uint8_t a)
{
    FLAG_C = (a & 0x80) >> 7;
    a = (a << 1) + FLAG_C;

    FLAG_Z = (a == 0);
    FLAG_N = 0;
    FLAG_H = 0;

    return a;
}

uint8_t rl(uint8_t a)
{
    uint8_t tmp = FLAG_C;
    FLAG_C = (a & 0x80) >> 7;
    a = (a << 1) + tmp;

    FLAG_Z = (a == 0);
    FLAG_N = 0;
    FLAG_H = 0;

    return a;
}

uint8_t rrc(uint8_t a)
{
    FLAG_C = a & 0x01;
    a = (a >> 1) + (FLAG_C << 7);

    FLAG_Z = (a == 0);
    FLAG_N = 0;
    FLAG_N = 0;

    return a;
}

uint8_t rr(uint8_t a)
{
    uint8_t tmp = FLAG_C;
    FLAG_C = a & 0x01;
    a = (a >> 1) + (tmp << 7);

    FLAG_Z = (a == 0);
    FLAG_N = 0;
    FLAG_H = 0;

    return a;
}

uint8_t sla(uint8_t a)
{
    FLAG_C = (a & 0x80) >> 7;
    a <<= 1;

    FLAG_Z = (a == 0);
    FLAG_N = 0;
    FLAG_H = 0;

    return a;
}

uint8_t sra(uint8_t a)
{
    FLAG_C = a & 0x01;
    a >>= 1;
    a |= ((a & 0x40) << 1);

    FLAG_Z = (a == 0);
    FLAG_N = 0;
    FLAG_H = 0;

    return a;
}

uint8_t srl(uint8_t a)
{
    FLAG_C = a & 0x01;
    a >>= 1;

    FLAG_Z = (a == 0);
    FLAG_N = 0;
    FLAG_H = 0;

    return a;
}

void bit(uint8_t reg, uint8_t b)
{
    FLAG_Z = ((reg & (0x01 << b)) == 0);
    FLAG_N = 0;
    FLAG_H = 1;
}

uint8_t set(uint8_t reg, uint8_t b)
{
    return reg |= (1 << b);
}

uint8_t res(uint8_t reg, uint8_t b)
{
    return reg &= ~(1 << b);
}

void jr(uint8_t a)
{
    if ((a & 0x80) == 0x80) {
        a--;
        a = ~a;
        REG_PC -= a;
    } else {
        REG_PC += a;
    }
}

void call(uint16_t addr)
{
    push(REG_PC + 2);
    REG_PC = addr;
}

void rst(uint8_t a)
{
    push(REG_PC);
    REG_PC = 0x0000 + a;
}

void ret(void)
{
    REG_PC = pop();
}

/* 8-bit loads */
/* LD r <- s */
void LDbb(void) { REG_B = REG_B; }
void LDbc(void) { REG_B = REG_C; }
void LDbd(void) { REG_B = REG_D; }
void LDbe(void) { REG_B = REG_E; }
void LDbh(void) { REG_B = REG_H; }
void LDbl(void) { REG_B = REG_L; }
void LDba(void) { REG_B = REG_A; }
void LDcb(void) { REG_C = REG_B; }
void LDcc(void) { REG_C = REG_C; }
void LDcd(void) { REG_C = REG_D; }
void LDce(void) { REG_C = REG_E; }
void LDch(void) { REG_C = REG_H; }
void LDcl(void) { REG_C = REG_L; }
void LDca(void) { REG_C = REG_A; }
void LDdb(void) { REG_D = REG_B; }
void LDdc(void) { REG_D = REG_C; }
void LDdd(void) { REG_D = REG_D; }
void LDde(void) { REG_D = REG_E; }
void LDdh(void) { REG_D = REG_H; }
void LDdl(void) { REG_D = REG_L; }
void LDda(void) { REG_D = REG_A; }
void LDeb(void) { REG_E = REG_B; }
void LDec(void) { REG_E = REG_C; }
void LDed(void) { REG_E = REG_D; }
void LDee(void) { REG_E = REG_E; }
void LDeh(void) { REG_E = REG_H; }
void LDel(void) { REG_E = REG_L; }
void LDea(void) { REG_E = REG_A; }
void LDhb(void) { REG_H = REG_B; }
void LDhc(void) { REG_H = REG_C; }
void LDhd(void) { REG_H = REG_D; }
void LDhe(void) { REG_H = REG_E; }
void LDhh(void) { REG_H = REG_H; }
void LDhl(void) { REG_H = REG_L; }
void LDha(void) { REG_H = REG_A; }
void LDlb(void) { REG_L = REG_B; }
void LDlc(void) { REG_L = REG_C; }
void LDld(void) { REG_L = REG_D; }
void LDle(void) { REG_L = REG_E; }
void LDlh(void) { REG_L = REG_H; }
void LDll(void) { REG_L = REG_L; }
void LDla(void) { REG_L = REG_A; }
void LDab(void) { REG_A = REG_B; }
void LDac(void) { REG_A = REG_C; }
void LDad(void) { REG_A = REG_D; }
void LDae(void) { REG_A = REG_E; }
void LDah(void) { REG_A = REG_H; }
void LDal(void) { REG_A = REG_L; }
void LDaa(void) { REG_A = REG_A; }
void LDbn(void) { REG_B = read_8(cpu.mmu, REG_PC++); }
void LDcn(void) { REG_C = read_8(cpu.mmu, REG_PC++); }
void LDdn(void) { REG_D = read_8(cpu.mmu, REG_PC++); }
void LDen(void) { REG_E = read_8(cpu.mmu, REG_PC++); }
void LDhn(void) { REG_H = read_8(cpu.mmu, REG_PC++); }
void LDln(void) { REG_L = read_8(cpu.mmu, REG_PC++); }
void LDan(void) { REG_A = read_8(cpu.mmu, REG_PC++); }
void LDbmHL(void) { REG_B = read_8(cpu.mmu, REG_HL); }
void LDcmHL(void) { REG_C = read_8(cpu.mmu, REG_HL); }
void LDdmHL(void) { REG_D = read_8(cpu.mmu, REG_HL); }
void LDemHL(void) { REG_E = read_8(cpu.mmu, REG_HL); }
void LDhmHL(void) { REG_H = read_8(cpu.mmu, REG_HL); }
void LDlmHL(void) { REG_L = read_8(cpu.mmu, REG_HL); }
/* LD d <- r */
void LDmHLb(void) { write_8(cpu.mmu, REG_HL, REG_B); }
void LDmHLc(void) { write_8(cpu.mmu, REG_HL, REG_C); }
void LDmHLd(void) { write_8(cpu.mmu, REG_HL, REG_D); }
void LDmHLe(void) { write_8(cpu.mmu, REG_HL, REG_E); }
void LDmHLh(void) { write_8(cpu.mmu, REG_HL, REG_H); }
void LDmHLl(void) { write_8(cpu.mmu, REG_HL, REG_L); }
/* LD d <- n */
void LDmHLn(void) { write_8(cpu.mmu, REG_HL, read_8(cpu.mmu, REG_PC++)); }
/* LD A <- (ss) */
void LDamBC(void) { REG_A = read_8(cpu.mmu, REG_BC); }
void LDamDE(void) { REG_A = read_8(cpu.mmu, REG_DE); }
void LDamHL(void) { REG_A = read_8(cpu.mmu, REG_HL); }
void LDamnn(void) { REG_A = read_8(cpu.mmu, read_16(cpu.mmu, REG_PC)); REG_PC += 2; }
/* LD (dd) <- A */
void LDmBCa(void) { write_8(cpu.mmu, read_16(cpu.mmu, REG_BC), REG_A); }
void LDmDEa(void) { write_8(cpu.mmu, read_16(cpu.mmu, REG_DE), REG_A); }
void LDmHLa(void) { write_8(cpu.mmu, read_16(cpu.mmu, REG_HL), REG_A); }
void LDmnna(void) { write_8(cpu.mmu, read_16(cpu.mmu, REG_PC), REG_A); REG_PC += 2; }
/* LD A <- (C) */
void LDamc(void) { REG_A = read_8(cpu.mmu, REG_C); }
/* LD (C) <- A */
void LDmca(void) { write_8(cpu.mmu, read_8(cpu.mmu, REG_C), REG_A); }
/* LDD A <- (HL) */
void LDDamHL(void) { REG_A = read_8(cpu.mmu, REG_HL--); }
/* LDD (HL) <- A */
void LDDmHLa(void) { write_8(cpu.mmu, REG_HL--, REG_A); }
/* LDI A <- (HL) */
void LDIamHL(void) { REG_A = read_8(cpu.mmu, REG_HL++); }
/* LDI (HL) <- A */
void LDImHLa(void) { write_8(cpu.mmu, REG_HL++, REG_A); }
/* LDH (n) <- A */
void LDHmna(void) { write_8(cpu.mmu, read_8(cpu.mmu, REG_PC++) + 0xFF00, REG_A); }
/* LDH A <- (n) */
void LDHamn(void) { REG_A = read_8(cpu.mmu, 0xFF00 + read_8(cpu.mmu, REG_PC++)); }

/* 16-bit loads */
/* LD dd, nn */
void LDBCnn(void) { REG_BC = read_16(cpu.mmu, REG_PC); REG_PC += 2; }
void LDDEnn(void) { REG_DE = read_16(cpu.mmu, REG_PC); REG_PC += 2; }
void LDHLnn(void) { REG_HL = read_16(cpu.mmu, REG_PC); REG_PC += 2; }
void LDSPnn(void) { REG_SP = read_16(cpu.mmu, REG_PC); REG_PC += 2; }
/* LD (nn), SP */
void LDmnnSP(void) { write_16(cpu.mmu, read_16(cpu.mmu, REG_PC), REG_SP); REG_PC += 2; }
/* LD SP, HL */
void LDSPHL(void) { REG_SP = REG_HL; }
/* LD HL, (SP + e) */
void LDHLSPn(void) { REG_HL = add_16_8(REG_SP, read_8(cpu.mmu, REG_PC++)); }
/* PUSH ss */
void PUSHBC(void) { push(REG_BC); }
void PUSHDE(void) { push(REG_DE); }
void PUSHHL(void) { push(REG_HL); }
void PUSHAF(void) { push(REG_AF); }
/* POP dd */
void POPBC(void) { REG_BC = pop(); }
void POPDE(void) { REG_DE = pop(); }
void POPHL(void) { REG_HL = pop(); }
void POPAF(void) { REG_AF = pop(); }

/* 8-bit ALU */
/* ADD A, s */
void ADDab(void) { REG_A = add_8_8(REG_A, REG_B); }
void ADDac(void) { REG_A = add_8_8(REG_A, REG_C); }
void ADDad(void) { REG_A = add_8_8(REG_A, REG_D); }
void ADDae(void) { REG_A = add_8_8(REG_A, REG_E); }
void ADDah(void) { REG_A = add_8_8(REG_A, REG_H); }
void ADDal(void) { REG_A = add_8_8(REG_A, REG_L); }
void ADDaa(void) { REG_A = add_8_8(REG_A, REG_A); }
void ADDan(void) { REG_A = add_8_8(REG_A, read_8(cpu.mmu, REG_PC++)); }
void ADDamHL(void) { REG_A = add_8_8(REG_A, read_8(cpu.mmu, REG_HL)); }
/* ADC A, s */
void ADCab(void) { REG_A = adc(REG_A, REG_B); }
void ADCac(void) { REG_A = adc(REG_A, REG_C); }
void ADCad(void) { REG_A = adc(REG_A, REG_D); }
void ADCae(void) { REG_A = adc(REG_A, REG_E); }
void ADCah(void) { REG_A = adc(REG_A, REG_H); }
void ADCal(void) { REG_A = adc(REG_A, REG_L); }
void ADCaa(void) { REG_A = adc(REG_A, REG_A); }
void ADCan(void) { REG_A = adc(REG_A, read_8(cpu.mmu, REG_PC++)); }
void ADCamHL(void) { REG_A = adc(REG_A, read_8(cpu.mmu, REG_HL)); }
/* SUB s */
void SUBab(void) { REG_A = sub(REG_A, REG_B); }
void SUBac(void) { REG_A = sub(REG_A, REG_C); }
void SUBad(void) { REG_A = sub(REG_A, REG_D); }
void SUBae(void) { REG_A = sub(REG_A, REG_E); }
void SUBah(void) { REG_A = sub(REG_A, REG_H); }
void SUBal(void) { REG_A = sub(REG_A, REG_L); }
void SUBaa(void) { REG_A = sub(REG_A, REG_A); }
void SUBan(void) { REG_A = sub(REG_A, read_8(cpu.mmu, REG_PC++)); }
void SUBamHL(void) { REG_A = sub(REG_A, read_8(cpu.mmu, REG_HL)); }
/* SBC A, s */
void SBCab(void) { REG_A = sbc(REG_A, REG_B); }
void SBCac(void) { REG_A = sbc(REG_A, REG_C); }
void SBCad(void) { REG_A = sbc(REG_A, REG_D); }
void SBCae(void) { REG_A = sbc(REG_A, REG_E); }
void SBCah(void) { REG_A = sbc(REG_A, REG_H); }
void SBCal(void) { REG_A = sbc(REG_A, REG_L); }
void SBCaa(void) { REG_A = sbc(REG_A, REG_A); }
void SBCan(void) { REG_A = sbc(REG_A, read_8(cpu.mmu, REG_PC++)); }
void SBCamHL(void) { REG_A = sbc(REG_A, read_8(cpu.mmu, REG_HL)); }
/* AND s */
void ANDb(void) { REG_A = and(REG_A, REG_B); }
void ANDc(void) { REG_A = and(REG_A, REG_C); }
void ANDd(void) { REG_A = and(REG_A, REG_D); }
void ANDe(void) { REG_A = and(REG_A, REG_E); }
void ANDh(void) { REG_A = and(REG_A, REG_H); }
void ANDl(void) { REG_A = and(REG_A, REG_L); }
void ANDa(void) { REG_A = and(REG_A, REG_A); }
void ANDn(void) { REG_A = and(REG_A, read_8(cpu.mmu, REG_PC++)); }
void ANDmHL(void) { REG_A = and(REG_A, read_8(cpu.mmu, REG_HL)); }
/* OR s */
void ORb(void) { REG_A = or(REG_A, REG_B); }
void ORc(void) { REG_A = or(REG_A, REG_C); }
void ORd(void) { REG_A = or(REG_A, REG_D); }
void ORe(void) { REG_A = or(REG_A, REG_E); }
void ORh(void) { REG_A = or(REG_A, REG_H); }
void ORl(void) { REG_A = or(REG_A, REG_L); }
void ORa(void) { REG_A = or(REG_A, REG_A); }
void ORn(void) { REG_A = or(REG_A, read_8(cpu.mmu, REG_PC++)); }
void ORmHL(void) { REG_A = or(REG_A, read_8(cpu.mmu, REG_HL)); }
/* XOR s */
void XORb(void) { REG_A = xor(REG_A, REG_B); }
void XORc(void) { REG_A = xor(REG_A, REG_C); }
void XORd(void) { REG_A = xor(REG_A, REG_D); }
void XORe(void) { REG_A = xor(REG_A, REG_E); }
void XORh(void) { REG_A = xor(REG_A, REG_H); }
void XORl(void) { REG_A = xor(REG_A, REG_L); }
void XORa(void) { REG_A = xor(REG_A, REG_A); }
void XORn(void) { REG_A = xor(REG_A, read_8(cpu.mmu, REG_PC++)); }
void XORmHL(void) { REG_A = xor(REG_A, read_8(cpu.mmu, REG_HL)); }
/* CP s */
void CPb(void) { sub(REG_A, REG_B); }
void CPc(void) { sub(REG_A, REG_C); }
void CPd(void) { sub(REG_A, REG_D); }
void CPe(void) { sub(REG_A, REG_E); }
void CPh(void) { sub(REG_A, REG_H); }
void CPl(void) { sub(REG_A, REG_L); }
void CPa(void) { sub(REG_A, REG_A); }
void CPn(void) { sub(REG_A, read_8(cpu.mmu, REG_PC++)); }
void CPmHL(void) { sub(REG_A, read_8(cpu.mmu, REG_HL)); }
/* INC s */
void INCb(void) { REG_B = inc_8(REG_B); }
void INCc(void) { REG_C = inc_8(REG_C); }
void INCd(void) { REG_D = inc_8(REG_D); }
void INCe(void) { REG_E = inc_8(REG_E); }
void INCh(void) { REG_H = inc_8(REG_H); }
void INCl(void) { REG_L = inc_8(REG_L); }
void INCa(void) { REG_A = inc_8(REG_A); }
void INCmHL(void) { write_8(cpu.mmu, REG_HL, inc_8(read_8(cpu.mmu, REG_HL))); }
/* DEC s */
void DECb(void) { REG_B = dec_8(REG_B); }
void DECc(void) { REG_C = dec_8(REG_C); }
void DECd(void) { REG_D = dec_8(REG_D); }
void DECe(void) { REG_E = dec_8(REG_E); }
void DECh(void) { REG_H = dec_8(REG_H); }
void DECl(void) { REG_L = dec_8(REG_L); }
void DECa(void) { REG_A = dec_8(REG_A); }
void DECmHL(void) { write_8(cpu.mmu, REG_HL, dec_8(read_8(cpu.mmu, REG_HL))); }

/* 16-bit arithmetic */
/* ADD HL, ss */
void ADDHLBC(void) { REG_HL = add_16_16(REG_HL, REG_BC); }
void ADDHLDE(void) { REG_HL = add_16_16(REG_HL, REG_DE); }
void ADDHLHL(void) { REG_HL = add_16_16(REG_HL, REG_HL); }
void ADDHLSP(void) { REG_HL = add_16_16(REG_HL, REG_SP); }
/* ADD SP, e */
void ADDSPn(void) { REG_SP = add_16_8(REG_SP, read_8(cpu.mmu, REG_PC++)); }
/* INC ss */
void INCBC(void) { REG_BC = inc_16(REG_BC); }
void INCDE(void) { REG_DE = inc_16(REG_DE); }
void INCHL(void) { REG_HL = inc_16(REG_HL); }
void INCSP(void) { REG_SP = inc_16(REG_SP); }
/* DEC ss */
void DECBC(void) { REG_BC = dec_16(REG_BC); }
void DECDE(void) { REG_DE = dec_16(REG_DE); }
void DECHL(void) { REG_HL = dec_16(REG_HL); }
void DECSP(void) { REG_SP = dec_16(REG_SP); }

/* Misc */
/* SWAP s */
void SWAPb(void) { REG_B = swap(REG_B); }
void SWAPc(void) { REG_C = swap(REG_C); }
void SWAPd(void) { REG_D = swap(REG_D); }
void SWAPe(void) { REG_E = swap(REG_E); }
void SWAPh(void) { REG_H = swap(REG_H); }
void SWAPl(void) { REG_L = swap(REG_L); }
void SWAPa(void) { REG_A = swap(REG_A); }
void SWAPmHL(void) { write_8(cpu.mmu, REG_HL, swap(read_8(cpu.mmu, REG_HL))); }
/* DAA */
void DAA(void)
{
    uint8_t tmp = REG_A;
    if (!FLAG_N) {
        if (FLAG_H || ((tmp & 0x0F) > 9))
            tmp += 6;
        if (FLAG_C || (tmp > 0x9F))
            tmp += 0x60;
    } else {
        if (FLAG_H)
            tmp = (tmp - 6) & 0xFF;
        if (FLAG_C)
            tmp -= 0x60;
    }

    if (tmp & 0x100)
        FLAG_C = 1;

    FLAG_H = 0;

    tmp &= 0xFF;

    FLAG_Z = (tmp == 0);

    REG_A = tmp;
}
/* CPL */
void CPL(void) { REG_A = ~REG_A; FLAG_N = 1; FLAG_H = 1; }
/* CCF */
void CCF(void) { FLAG_C = (FLAG_C == 0); FLAG_N = 0; FLAG_H = 0; }
/* SCF */
void SCF(void) { FLAG_C = 1; FLAG_N = 0; FLAG_H = 0; }
/* NOP */
void NOP(void) { /* No operation */ }
/* HALT */
void HALT(void) { cpu.halt = 1; }
/* STOP */
void STOP(void) { REG_PC++; cpu.stop = 1; }
/* DI */
void DI(void) { cpu.ime = 0; }
/* EI */
void EI(void) { cpu.ime = 1; }

/* Rotates and shifts */
/* RLCA */
void RLCA(void) { REG_A = rlc(REG_A); FLAG_Z = 0; }
/* RLA */
void RLA(void) { REG_A = rl(REG_A); FLAG_Z = 0; }
/* RRCA */
void RRCA(void) { REG_A = rrc(REG_A); FLAG_Z = 0; }
/* RRA */
void RRA(void) { REG_A = rr(REG_A); FLAG_Z = 0; }
/* RLC s */
void RLCb(void) { REG_B = rlc(REG_B); }
void RLCc(void) { REG_C = rlc(REG_C); }
void RLCd(void) { REG_D = rlc(REG_D); }
void RLCe(void) { REG_E = rlc(REG_E); }
void RLCh(void) { REG_H = rlc(REG_H); }
void RLCl(void) { REG_L = rlc(REG_L); }
void RLCa(void) { REG_A = rlc(REG_A); }
void RLCmHL(void) { write_8(cpu.mmu, REG_HL, rlc(read_8(cpu.mmu, REG_HL))); }
/* RL s */
void RLb(void) { REG_B = rl(REG_B); }
void RLc(void) { REG_C = rl(REG_C); }
void RLd(void) { REG_D = rl(REG_D); }
void RLe(void) { REG_E = rl(REG_E); }
void RLh(void) { REG_H = rl(REG_H); }
void RLl(void) { REG_L = rl(REG_L); }
void RLa(void) { REG_A = rl(REG_A); }
void RLmHL(void) { write_8(cpu.mmu, REG_HL, rl(read_8(cpu.mmu, REG_HL))); }
/* RRC s */
void RRCb(void) { REG_B = rrc(REG_B); }
void RRCc(void) { REG_C = rrc(REG_C); }
void RRCd(void) { REG_D = rrc(REG_D); }
void RRCe(void) { REG_E = rrc(REG_E); }
void RRCh(void) { REG_H = rrc(REG_H); }
void RRCl(void) { REG_L = rrc(REG_L); }
void RRCa(void) { REG_A = rrc(REG_A); }
void RRCmHL(void) { write_8(cpu.mmu, REG_HL, rrc(read_8(cpu.mmu, REG_HL))); }
/* RR s */
void RRb(void) { REG_B = rr(REG_B); }
void RRc(void) { REG_C = rr(REG_C); }
void RRd(void) { REG_D = rr(REG_D); }
void RRe(void) { REG_E = rr(REG_E); }
void RRh(void) { REG_H = rr(REG_H); }
void RRl(void) { REG_L = rr(REG_L); }
void RRa(void) { REG_A = rr(REG_A); }
void RRmHL(void) { write_8(cpu.mmu, REG_HL, rr(read_8(cpu.mmu, REG_HL))); }
/* SLA s */
void SLAb(void) { REG_B = sla(REG_B); }
void SLAc(void) { REG_C = sla(REG_C); }
void SLAd(void) { REG_D = sla(REG_D); }
void SLAe(void) { REG_E = sla(REG_E); }
void SLAh(void) { REG_H = sla(REG_H); }
void SLAl(void) { REG_L = sla(REG_L); }
void SLAa(void) { REG_A = sla(REG_A); }
void SLAmHL(void) { write_8(cpu.mmu, REG_HL, sla(read_8(cpu.mmu, REG_HL))); }
/* SRA s */
void SRAb(void) { REG_B = sra(REG_B); }
void SRAc(void) { REG_C = sra(REG_C); }
void SRAd(void) { REG_D = sra(REG_D); }
void SRAe(void) { REG_E = sra(REG_E); }
void SRAh(void) { REG_H = sra(REG_H); }
void SRAl(void) { REG_L = sra(REG_L); }
void SRAa(void) { REG_A = sra(REG_A); }
void SRAmHL(void) { write_8(cpu.mmu, REG_HL, sra(read_8(cpu.mmu, REG_HL))); }
/* SRL s */
void SRLb(void) { REG_B = srl(REG_B); }
void SRLc(void) { REG_C = srl(REG_C); }
void SRLd(void) { REG_D = srl(REG_D); }
void SRLe(void) { REG_E = srl(REG_E); }
void SRLh(void) { REG_H = srl(REG_H); }
void SRLl(void) { REG_L = srl(REG_L); }
void SRLa(void) { REG_A = srl(REG_A); }
void SRLmHL(void) { write_8(cpu.mmu, REG_HL, srl(read_8(cpu.mmu, REG_HL))); }

/* Bit manipulation */
/* BIT b, s */
void BIT0b(void) { bit(REG_B, 0); }
void BIT0c(void) { bit(REG_C, 0); }
void BIT0d(void) { bit(REG_D, 0); }
void BIT0e(void) { bit(REG_E, 0); }
void BIT0h(void) { bit(REG_H, 0); }
void BIT0l(void) { bit(REG_L, 0); }
void BIT0a(void) { bit(REG_A, 0); }
void BIT0mHL(void) { bit(read_8(cpu.mmu, REG_HL), 0); }
void BIT1b(void) { bit(REG_B, 1); }
void BIT1c(void) { bit(REG_C, 1); }
void BIT1d(void) { bit(REG_D, 1); }
void BIT1e(void) { bit(REG_E, 1); }
void BIT1h(void) { bit(REG_H, 1); }
void BIT1l(void) { bit(REG_L, 1); }
void BIT1a(void) { bit(REG_A, 1); }
void BIT1mHL(void) { bit(read_8(cpu.mmu, REG_HL), 1); }
void BIT2b(void) { bit(REG_B, 2); }
void BIT2c(void) { bit(REG_C, 2); }
void BIT2d(void) { bit(REG_D, 2); }
void BIT2e(void) { bit(REG_E, 2); }
void BIT2h(void) { bit(REG_H, 2); }
void BIT2l(void) { bit(REG_L, 2); }
void BIT2a(void) { bit(REG_A, 2); }
void BIT2mHL(void) { bit(read_8(cpu.mmu, REG_HL), 2); }
void BIT3b(void) { bit(REG_B, 3); }
void BIT3c(void) { bit(REG_C, 3); }
void BIT3d(void) { bit(REG_D, 3); }
void BIT3e(void) { bit(REG_E, 3); }
void BIT3h(void) { bit(REG_H, 3); }
void BIT3l(void) { bit(REG_L, 3); }
void BIT3a(void) { bit(REG_A, 3); }
void BIT3mHL(void) { bit(read_8(cpu.mmu, REG_HL), 3); }
void BIT4b(void) { bit(REG_B, 4); }
void BIT4c(void) { bit(REG_C, 4); }
void BIT4d(void) { bit(REG_D, 4); }
void BIT4e(void) { bit(REG_E, 4); }
void BIT4h(void) { bit(REG_H, 4); }
void BIT4l(void) { bit(REG_L, 4); }
void BIT4a(void) { bit(REG_A, 4); }
void BIT4mHL(void) { bit(read_8(cpu.mmu, REG_HL), 4); }
void BIT5b(void) { bit(REG_B, 5); }
void BIT5c(void) { bit(REG_C, 5); }
void BIT5d(void) { bit(REG_D, 5); }
void BIT5e(void) { bit(REG_E, 5); }
void BIT5h(void) { bit(REG_H, 5); }
void BIT5l(void) { bit(REG_L, 5); }
void BIT5a(void) { bit(REG_A, 5); }
void BIT5mHL(void) { bit(read_8(cpu.mmu, REG_HL), 5); }
void BIT6b(void) { bit(REG_B, 6); }
void BIT6c(void) { bit(REG_C, 6); }
void BIT6d(void) { bit(REG_D, 6); }
void BIT6e(void) { bit(REG_E, 6); }
void BIT6h(void) { bit(REG_H, 6); }
void BIT6l(void) { bit(REG_L, 6); }
void BIT6a(void) { bit(REG_A, 6); }
void BIT6mHL(void) { bit(read_8(cpu.mmu, REG_HL), 6); }
void BIT7b(void) { bit(REG_B, 7); }
void BIT7c(void) { bit(REG_C, 7); }
void BIT7d(void) { bit(REG_D, 7); }
void BIT7e(void) { bit(REG_E, 7); }
void BIT7h(void) { bit(REG_H, 7); }
void BIT7l(void) { bit(REG_L, 7); }
void BIT7a(void) { bit(REG_A, 7); }
void BIT7mHL(void) { bit(read_8(cpu.mmu, REG_HL), 7); }
/* SET b, s */
void SET0b(void) { set(REG_B, 0); }
void SET0c(void) { set(REG_C, 0); }
void SET0d(void) { set(REG_D, 0); }
void SET0e(void) { set(REG_E, 0); }
void SET0h(void) { set(REG_H, 0); }
void SET0l(void) { set(REG_L, 0); }
void SET0a(void) { set(REG_A, 0); }
void SET0mHL(void) { set(read_8(cpu.mmu, REG_HL), 0); }
void SET1b(void) { set(REG_B, 1); }
void SET1c(void) { set(REG_C, 1); }
void SET1d(void) { set(REG_D, 1); }
void SET1e(void) { set(REG_E, 1); }
void SET1h(void) { set(REG_H, 1); }
void SET1l(void) { set(REG_L, 1); }
void SET1a(void) { set(REG_A, 1); }
void SET1mHL(void) { set(read_8(cpu.mmu, REG_HL), 1); }
void SET2b(void) { set(REG_B, 2); }
void SET2c(void) { set(REG_C, 2); }
void SET2d(void) { set(REG_D, 2); }
void SET2e(void) { set(REG_E, 2); }
void SET2h(void) { set(REG_H, 2); }
void SET2l(void) { set(REG_L, 2); }
void SET2a(void) { set(REG_A, 2); }
void SET2mHL(void) { set(read_8(cpu.mmu, REG_HL), 2); }
void SET3b(void) { set(REG_B, 3); }
void SET3c(void) { set(REG_C, 3); }
void SET3d(void) { set(REG_D, 3); }
void SET3e(void) { set(REG_E, 3); }
void SET3h(void) { set(REG_H, 3); }
void SET3l(void) { set(REG_L, 3); }
void SET3a(void) { set(REG_A, 3); }
void SET3mHL(void) { set(read_8(cpu.mmu, REG_HL), 3); }
void SET4b(void) { set(REG_B, 4); }
void SET4c(void) { set(REG_C, 4); }
void SET4d(void) { set(REG_D, 4); }
void SET4e(void) { set(REG_E, 4); }
void SET4h(void) { set(REG_H, 4); }
void SET4l(void) { set(REG_L, 4); }
void SET4a(void) { set(REG_A, 4); }
void SET4mHL(void) { set(read_8(cpu.mmu, REG_HL), 4); }
void SET5b(void) { set(REG_B, 5); }
void SET5c(void) { set(REG_C, 5); }
void SET5d(void) { set(REG_D, 5); }
void SET5e(void) { set(REG_E, 5); }
void SET5h(void) { set(REG_H, 5); }
void SET5l(void) { set(REG_L, 5); }
void SET5a(void) { set(REG_A, 5); }
void SET5mHL(void) { set(read_8(cpu.mmu, REG_HL), 5); }
void SET6b(void) { set(REG_B, 6); }
void SET6c(void) { set(REG_C, 6); }
void SET6d(void) { set(REG_D, 6); }
void SET6e(void) { set(REG_E, 6); }
void SET6h(void) { set(REG_H, 6); }
void SET6l(void) { set(REG_L, 6); }
void SET6a(void) { set(REG_A, 6); }
void SET6mHL(void) { set(read_8(cpu.mmu, REG_HL), 6); }
void SET7b(void) { set(REG_B, 7); }
void SET7c(void) { set(REG_C, 7); }
void SET7d(void) { set(REG_D, 7); }
void SET7e(void) { set(REG_E, 7); }
void SET7h(void) { set(REG_H, 7); }
void SET7l(void) { set(REG_L, 7); }
void SET7a(void) { set(REG_A, 7); }
void SET7mHL(void) { set(read_8(cpu.mmu, REG_HL), 7); }
/* RES b, s */
void RES0b(void) { res(REG_B, 0); }
void RES0c(void) { res(REG_C, 0); }
void RES0d(void) { res(REG_D, 0); }
void RES0e(void) { res(REG_E, 0); }
void RES0h(void) { res(REG_H, 0); }
void RES0l(void) { res(REG_L, 0); }
void RES0a(void) { res(REG_A, 0); }
void RES0mHL(void) { res(read_8(cpu.mmu, REG_HL), 0); }
void RES1b(void) { res(REG_B, 1); }
void RES1c(void) { res(REG_C, 1); }
void RES1d(void) { res(REG_D, 1); }
void RES1e(void) { res(REG_E, 1); }
void RES1h(void) { res(REG_H, 1); }
void RES1l(void) { res(REG_L, 1); }
void RES1a(void) { res(REG_A, 1); }
void RES1mHL(void) { res(read_8(cpu.mmu, REG_HL), 1); }
void RES2b(void) { res(REG_B, 2); }
void RES2c(void) { res(REG_C, 2); }
void RES2d(void) { res(REG_D, 2); }
void RES2e(void) { res(REG_E, 2); }
void RES2h(void) { res(REG_H, 2); }
void RES2l(void) { res(REG_L, 2); }
void RES2a(void) { res(REG_A, 2); }
void RES2mHL(void) { res(read_8(cpu.mmu, REG_HL), 2); }
void RES3b(void) { res(REG_B, 3); }
void RES3c(void) { res(REG_C, 3); }
void RES3d(void) { res(REG_D, 3); }
void RES3e(void) { res(REG_E, 3); }
void RES3h(void) { res(REG_H, 3); }
void RES3l(void) { res(REG_L, 3); }
void RES3a(void) { res(REG_A, 3); }
void RES3mHL(void) { res(read_8(cpu.mmu, REG_HL), 3); }
void RES4b(void) { res(REG_B, 4); }
void RES4c(void) { res(REG_C, 4); }
void RES4d(void) { res(REG_D, 4); }
void RES4e(void) { res(REG_E, 4); }
void RES4h(void) { res(REG_H, 4); }
void RES4l(void) { res(REG_L, 4); }
void RES4a(void) { res(REG_A, 4); }
void RES4mHL(void) { res(read_8(cpu.mmu, REG_HL), 4); }
void RES5b(void) { res(REG_B, 5); }
void RES5c(void) { res(REG_C, 5); }
void RES5d(void) { res(REG_D, 5); }
void RES5e(void) { res(REG_E, 5); }
void RES5h(void) { res(REG_H, 5); }
void RES5l(void) { res(REG_L, 5); }
void RES5a(void) { res(REG_A, 5); }
void RES5mHL(void) { res(read_8(cpu.mmu, REG_HL), 5); }
void RES6b(void) { res(REG_B, 6); }
void RES6c(void) { res(REG_C, 6); }
void RES6d(void) { res(REG_D, 6); }
void RES6e(void) { res(REG_E, 6); }
void RES6h(void) { res(REG_H, 6); }
void RES6l(void) { res(REG_L, 6); }
void RES6a(void) { res(REG_A, 6); }
void RES6mHL(void) { res(read_8(cpu.mmu, REG_HL), 6); }
void RES7b(void) { res(REG_B, 7); }
void RES7c(void) { res(REG_C, 7); }
void RES7d(void) { res(REG_D, 7); }
void RES7e(void) { res(REG_E, 7); }
void RES7h(void) { res(REG_H, 7); }
void RES7l(void) { res(REG_L, 7); }
void RES7a(void) { res(REG_A, 7); }
void RES7mHL(void) { res(read_8(cpu.mmu, REG_HL), 7); }

/* Jumps */
/* JP nn */
void JPnn(void) { REG_PC = read_16(cpu.mmu, REG_PC); }
/* JP cc, nn */
void JPZnn(void)
{
    if (FLAG_Z) {
        REG_PC = read_16(cpu.mmu, REG_PC);
        cpu.ins_clock.m = 4;
        return;
    }

    REG_PC += 2;
}
void JPCnn(void)
{
    if (FLAG_C) {
        REG_PC = read_16(cpu.mmu, REG_PC);
        cpu.ins_clock.m = 4;
        return;
    }

    REG_PC += 2;
}
void JPNZnn(void)
{
    if (!FLAG_Z) {
        REG_PC = read_16(cpu.mmu, REG_PC);
        cpu.ins_clock.m = 4;
        return;
    }

    REG_PC += 2;
}
void JPNCnn(void)
{
    if (!FLAG_C) {
        REG_PC = read_16(cpu.mmu, REG_PC);
        cpu.ins_clock.m = 4;
        return;
    }

    REG_PC += 2;
}
/* JP (HL) */
void JPmHL(void)
{
    /* Why is this called JP (HL) and not JP HL? */
    REG_PC = REG_HL;
}
/* JR e */
void JRn(void) { jr(read_8(cpu.mmu, REG_PC)); REG_PC++; }
/* JR cc, e */
void JRZn(void)
{
    if (FLAG_Z) {
        jr(read_8(cpu.mmu, REG_PC));
        REG_PC++;
        cpu.ins_clock.m = 4;
        return;
    }

    REG_PC++;
}
void JRCn(void)
{
    if (FLAG_C) {
        jr(read_8(cpu.mmu, REG_PC));
        REG_PC++;
        cpu.ins_clock.m = 4;
        return;
    }

    REG_PC++;
}
void JRNZn(void)
{
    if (!FLAG_Z) {
        jr(read_8(cpu.mmu, REG_PC));
        REG_PC++;
        cpu.ins_clock.m = 4;
        return;
    }

    REG_PC++;
}
void JRNCn(void)
{
    if (!FLAG_C) {
        jr(read_8(cpu.mmu, REG_PC));
        REG_PC++;
        cpu.ins_clock.m = 4;
        return;
    }

    REG_PC++;
}

/* Calls */
/* CALL nn */
void CALLnn(void) { call(read_16(cpu.mmu, REG_PC)); }
/* CALL cc, nn */
void CALLZnn(void)
{
    if (FLAG_Z) {
        call(read_8(cpu.mmu, REG_PC));
        cpu.ins_clock.m = 12;
        return;
    }

    REG_PC += 2;
}
void CALLCnn(void)
{
    if (FLAG_C) {
        call(read_8(cpu.mmu, REG_PC));
        cpu.ins_clock.m = 12;
        return;
    }

    REG_PC += 2;
}
void CALLNZnn(void)
{
    if (!FLAG_Z) {
        call(read_8(cpu.mmu, REG_PC));
        cpu.ins_clock.m = 12;
        return;
    }

    REG_PC += 2;
}
void CALLNCnn(void)
{
    if (!FLAG_C) {
        call(read_8(cpu.mmu, REG_PC));
        cpu.ins_clock.m = 12;
        return;
    }

    REG_PC += 2;
}

/* Restarts */
/* RST f */
void RST0(void) { rst(0x00); }
void RST8(void) { rst(0x08); }
void RST10(void) { rst(0x10); }
void RST18(void) { rst(0x18); }
void RST20(void) { rst(0x20); }
void RST28(void) { rst(0x28); }
void RST30(void) { rst(0x30); }
void RST38(void) { rst(0x38); }

/* Returns */
/* RET */
void RET(void) { ret(); }
/* RET cc */
void RETZ(void)
{
    if (FLAG_Z) {
        ret();
        cpu.ins_clock.m = 12;
    }
}
void RETC(void)
{
    if (FLAG_C) {
        ret();
        cpu.ins_clock.m = 12;
    }
}
void RETNZ(void)
{
    if (!FLAG_Z) {
        ret();
        cpu.ins_clock.m = 12;
    }
}
void RETNC(void)
{
    if (!FLAG_C) {
        ret();
        cpu.ins_clock.m = 12;
    }
}
/* RETI */
void RETI(void) { ret(); cpu.ime = 1; }

/* Lookup table for two-byte opcodes */
void (*cb_ops[256])() = {
/*   |   0   |   1  |   2  |   3  |   4  |   5  |    6   |   7  |   8  |   9  |   A  |   B  |   C  |   D  |    E   |   F  | */
/* 0 */ RLCb , RLCc , RLCd , RLCe , RLCh , RLCl , RLCmHL , RLCa , RRCb , RRCc , RRCd , RRCe , RRCh , RRCl , RRCmHL , RRCa ,
/* 1 */ RLb  , RLc  , RLd  , RLe  , RLh  , RLl  , RLmHL  , RLa  , RRb  , RRc  , RRd  , RRe  , RRh  , RRl  , RRmHL  , RRa  ,
/* 2 */ SLAb , SLAc , SLAd , SLAe , SLAh , SLAl , SLAmHL , SLAa , SRAb , SRAc , SRAd , SRAe , SRAh , SRAl , SRAmHL , SRAa ,
/* 3 */ SWAPb, SWAPc, SWAPd, SWAPe, SWAPh, SWAPl, SWAPmHL, SWAPa, SRLb , SRLc , SRLd , SRLe , SRLh , SRLl , SRLmHL , SRLa ,
/* 4 */ BIT0b, BIT0c, BIT0d, BIT0e, BIT0h, BIT0l, BIT0mHL, BIT0a, BIT1b, BIT1c, BIT1d, BIT1e, BIT1h, BIT1l, BIT1mHL, BIT1a,
/* 5 */ BIT2b, BIT2c, BIT2d, BIT2e, BIT2h, BIT2l, BIT2mHL, BIT2a, BIT3b, BIT3c, BIT3d, BIT3e, BIT3h, BIT3l, BIT3mHL, BIT3a,
/* 6 */ BIT4b, BIT4c, BIT4d, BIT4e, BIT4h, BIT4l, BIT4mHL, BIT4a, BIT5b, BIT5c, BIT5d, BIT5e, BIT5h, BIT5l, BIT5mHL, BIT5a,
/* 7 */ BIT6b, BIT6c, BIT6d, BIT6e, BIT6h, BIT6l, BIT6mHL, BIT6a, BIT7b, BIT7c, BIT7d, BIT7e, BIT7h, BIT7l, BIT7mHL, BIT7a,
/* 8 */ RES0b, RES0c, RES0d, RES0e, RES0h, RES0l, RES0mHL, RES0a, RES1b, RES1c, RES1d, RES1e, RES1h, RES1l, RES1mHL, RES1a,
/* 9 */ RES2b, RES2c, RES2d, RES2e, RES2h, RES2l, RES2mHL, RES2a, RES3b, RES3c, RES3d, RES3e, RES3h, RES3l, RES3mHL, RES3a,
/* A */ RES4b, RES4c, RES4d, RES4e, RES4h, RES4l, RES4mHL, RES4a, RES5b, RES5c, RES5d, RES5e, RES5h, RES5l, RES5mHL, RES5a,
/* B */ RES6b, RES6c, RES6d, RES6e, RES6h, RES6l, RES6mHL, RES6a, RES7b, RES7c, RES7d, RES7e, RES7h, RES7l, RES7mHL, RES7a,
/* 8 */ SET0b, SET0c, SET0d, SET0e, SET0h, SET0l, SET0mHL, SET0a, SET1b, SET1c, SET1d, SET1e, SET1h, SET1l, SET1mHL, SET1a,
/* 9 */ SET2b, SET2c, SET2d, SET2e, SET2h, SET2l, SET2mHL, SET2a, SET3b, SET3c, SET3d, SET3e, SET3h, SET3l, SET3mHL, SET3a,
/* A */ SET4b, SET4c, SET4d, SET4e, SET4h, SET4l, SET4mHL, SET4a, SET5b, SET5c, SET5d, SET5e, SET5h, SET5l, SET5mHL, SET5a,
/* B */ SET6b, SET6c, SET6d, SET6e, SET6h, SET6l, SET6mHL, SET6a, SET7b, SET7c, SET7d, SET7e, SET7h, SET7l, SET7mHL, SET7a
};

/* Jump to 2-byte opcodes */
void CB(void)
{
    cpu.cb_op = read_8(cpu.mmu, REG_PC++);
    (*cb_ops[cpu.cb_op])();
}

/* Table of function pointers indexed by opcode */
void (*ops[256])() = {
/*    |    0   |   1   |    2   |   3   |    4    |   5   |    6   |   7   |    8   |    9   |    A   |   B  |    C   |   D   |    E   |   F  | */
/* 0 */ NOP    , LDBCnn, LDmBCa , INCBC , INCb    , DECb  , LDbn   , RLCA  , LDmnnSP, ADDHLBC, LDamBC , DECBC, INCc   , DECc  , LDcn   , RRCA ,
/* 1 */ STOP   , LDDEnn, LDmDEa , INCDE , INCd    , DECd  , LDdn   , RLA   , JRn    , ADDHLDE, LDamDE , DECDE, INCe   , DECe  , LDen   , RRA  ,
/* 2 */ JRNZn  , LDHLnn, LDImHLa, INCHL , INCh    , DECh  , LDhn   , DAA   , JRZn   , ADDHLHL, LDIamHL, DECHL, INCl   , DECl  , LDln   , CPL  ,
/* 3 */ JRNCn  , LDSPnn, LDDmHLa, INCSP , INCmHL  , DECmHL, LDmHLn , SCF   , JRCn   , ADDHLSP, LDDamHL, DECSP, INCa   , DECa  , LDan   , CCF  ,
/* 4 */ LDbb   , LDbc  , LDbd   , LDbe  , LDbh    , LDbl  , LDbmHL , LDba  , LDcb   , LDcc   , LDcd   , LDce , LDch   , LDcl  , LDcmHL , LDca ,
/* 5 */ LDdb   , LDdc  , LDdd   , LDde  , LDdh    , LDdl  , LDdmHL , LDda  , LDeb   , LDec   , LDed   , LDee , LDeh   , LDel  , LDemHL , LDea ,
/* 6 */ LDhb   , LDhc  , LDhd   , LDhe  , LDhh    , LDhl  , LDhmHL , LDha  , LDlb   , LDlc   , LDld   , LDle , LDlh   , LDll  , LDlmHL , LDla ,
/* 7 */ LDmHLb , LDmHLc, LDmHLd , LDmHLe, LDmHLh  , LDmHLl, HALT   , LDmHLa, LDab   , LDac   , LDad   , LDae , LDah   , LDal  , LDamHL , LDaa ,
/* 8 */ ADDab  , ADDac , ADDad  , ADDae , ADDah   , ADDal , ADDamHL, ADDaa , ADCab  , ADCac  , ADCad  , ADCae, ADCah  , ADCal , ADCamHL, ADCaa,
/* 9 */ SUBab  , SUBac , SUBad  , SUBae , SUBah   , SUBal , SUBamHL, SUBaa , SBCab  , SBCac  , SBCad  , SBCae, SBCah  , SBCal , SBCamHL, SBCaa,
/* A */ ANDb   , ANDc  , ANDd   , ANDe  , ANDh    , ANDl  , ANDmHL , ANDa  , XORb   , XORc   , XORd   , XORe , XORh   , XORl  , XORmHL , XORa ,
/* B */ ORb    , ORc   , ORd    , ORe   , ORh     , ORl   , ORmHL  , ORa   , CPb    , CPc    , CPd    , CPe  , CPh    , CPl   , CPmHL  , CPa  ,
/* C */ RETNZ  , POPBC , JPNZnn , JPnn  , CALLNZnn, PUSHBC, ADDan  , RST0  , RETZ   , RET    , JPZnn  , CB   , CALLZnn, CALLnn, ADCan  , RST8 ,
/* D */ RETNC  , POPDE , JPNCnn , NOP   , CALLNCnn, PUSHDE, SUBan  , RST10 , RETC   , RETI   , JPCnn  , NOP  , CALLCnn, NOP   , SBCan  , RST18,
/* E */ LDHmna , POPHL , LDmca  , NOP   , NOP     , PUSHHL, ANDn   , RST20 , ADDSPn , JPmHL  , LDmnna , NOP  , NOP    , NOP   , XORn   , RST28,
/* F */ LDHamn , POPAF , LDamc  , DI    , NOP     , PUSHAF, ORn    , RST30 , LDHLSPn, LDSPHL , LDamnn , EI   , NOP    , NOP   , CPn    , RST38
};


/* M clock values */
uint8_t timings_m[256] = {
/*   |  0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B| C | D | E| F | */
/* 0 */  4, 12,  8,  8,  4,  4,  8,  4, 20,  8,  8, 8,  4,  4, 8,  4,
/* 1 */  4, 12,  8,  8,  4,  4,  8,  4, 12,  8,  8, 8,  4,  4, 8,  4,
/* 2 */  8, 12,  8,  8,  4,  4,  8,  4,  8,  8,  8, 8,  4,  4, 8,  4,
/* 3 */  8, 12,  8,  8, 12, 12, 12,  4,  8,  8,  8, 8,  4,  4, 8,  4,
/* 4 */  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4, 4,  4,  4, 8,  4,
/* 5 */  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4, 4,  4,  4, 8,  4,
/* 6 */  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4, 4,  4,  4, 8,  4,
/* 7 */  8,  8,  8,  8,  8,  8,  4,  8,  4,  4,  4, 4,  4,  4, 8,  4,
/* 8 */  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4, 4,  4,  4, 8,  4,
/* 9 */  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4, 4,  4,  4, 8,  4,
/* A */  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4, 4,  4,  4, 8,  4,
/* B */  4,  4,  4,  4,  4,  4,  8,  4,  4,  4,  4, 4,  4,  4, 8,  4,
/* C */  8, 12, 12, 16, 12, 16,  8, 16,  8, 16, 12, 4, 12, 24, 8, 16,
/* D */  8, 12, 12,  4, 12, 16,  8, 16,  8, 16, 12, 4, 12,  4, 8, 16,
/* E */ 12, 12,  8,  4,  4, 16,  8, 16, 16,  4, 16, 4,  4,  4, 8, 16,
/* F */ 12, 12,  8,  4,  4, 16,  8, 16, 12,  8, 16, 4,  4,  4, 8, 16
};

/* M clock values (0xCB prefix) */
uint8_t cb_timings_m[256] = {
/*   | 0 | 1| 2| 3| 4| 5| 6 | 7| 8| 9| A| B| C| D| E | F| */
/* 0 */ 8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
/* 1 */ 8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
/* 2 */ 8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
/* 3 */ 8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
/* 4 */ 8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
/* 5 */ 8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
/* 6 */ 8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
/* 7 */ 8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
/* 8 */ 8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
/* 9 */ 8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
/* A */ 8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
/* B */ 8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
/* C */ 8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
/* D */ 8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
/* E */ 8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
/* F */ 8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8
};


/* T clock values */
uint8_t timings_t[/*256*/] = {
    0 /* Stuff */
};

/* Executive functions */
void cpu_reset(void)
{
    FLAG_Z = 1;
    FLAG_N = 0;
    FLAG_H = 1;
    FLAG_C = 1;

    REG_BC = 0x0013;
    REG_DE = 0x00D8;
    REG_HL = 0x014D;

    REG_PC = 0x0100;
    REG_SP = 0xFFFE;

    REG_A = 0x01;

    cpu.op = 0;

    cpu.sys_clock.m = 0;
    cpu.sys_clock.t = 0;
    cpu.ins_clock.m = 0;
    cpu.ins_clock.t = 0;
}

int cpu_run(uint32_t cycles)
{
    uint32_t total = 0;

    while (total < cycles) {
        cpu.ins_clock.m = 0;

        /* TODO: Interrupts */

        cpu.op = read_8(cpu.mmu, REG_PC++);

        (*ops[cpu.op])();

        if (cpu.op != 0xCB) {
            cpu.ins_clock.m += timings_m[cpu.op];
            cpu.ins_clock.t = timings_t[cpu.op];
        } else {
            cpu.ins_clock.m += cb_timings_m[cpu.cb_op];
        }
        /* TODO: ^ replace with:
            cpu.ins_clock.t = CEIL(cpu.ins_clock.m / 4.0)
        */
        cpu.sys_clock.m += cpu.ins_clock.m;
        cpu.sys_clock.t += cpu.ins_clock.t;

        total += cpu.ins_clock.m;
    }

    return 0;
}

/* Save states */
void save_state(void)
{
    /* TODO */
    /* Save CPU state */
    /* Write to disk */
}

void load_state(void)
{
    cpu = save_states[curr_save_slot];
}

/* Debug */
void print_cpu(void)
{
    printf("AF     :  $%x\n", REG_AF);
    printf("BC     :  $%x\n", REG_BC);
    printf("DE     :  $%x\n", REG_DE);
    printf("HL     :  $%x\n", REG_HL);
    printf("A      :  $%hx\n", REG_A);
    printf("B      :  $%hx\n", REG_B);
    printf("C      :  $%hx\n", REG_C);
    printf("D      :  $%hx\n", REG_D);
    printf("E      :  $%hx\n", REG_E);
    printf("H      :  $%hx\n", REG_H);
    printf("L      :  $%hx\n", REG_L);
    printf("PC     :  $%x\n", REG_PC);
    printf("SP     :  $%x\n", REG_SP);
    printf("OP     :  $%hx\n", cpu.op);
    printf("CBOP   :  $%hx\n", cpu.cb_op);
    printf("FLAGZ  :  $%hx\n", FLAG_Z);
    printf("FLAGN  :  $%hx\n", FLAG_N);
    printf("FLAGH  :  $%hx\n", FLAG_H);
    printf("FLAGC  :  $%hx\n", FLAG_C);
    printf("IME    :  $%hx\n", cpu.ime);
    printf("HALT   :  $%hx\n", cpu.halt);
    printf("STOP   :  $%hx\n", cpu.stop);
    printf("SCLOCKM:  $%hx\n", cpu.sys_clock.m);
    printf("SCLOCKT:  $%hx\n", cpu.sys_clock.t);
    printf("ICLOCKM:  $%hx\n", cpu.ins_clock.m);
    printf("ICLOCKT:  $%hx\n", cpu.ins_clock.t);
}

int main(void)
{
#ifdef _DEBUG

    uint8_t prg[5] = {
        0x3E, 0x01, 0x0E, 0x01, 0x81
    };

    mmu_t mem;
    mem.ram = prg;

    cpu.mmu = &mem;

    cpu_run(20);
    print_cpu();

#endif
    return 0;
}

