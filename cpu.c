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
int curr_save_slot;
cpu_t save_states[10];

/* Helper functions */
static void push(uint16_t val)
{
    REG_SP -= 2;
    write_16(cpu.mmu, REG_SP, val);
}

static uint16_t pop(void)
{
    REG_SP += 2;
    return read_16(cpu.mmu, REG_SP - 2);
}

static uint8_t add_8_8(uint8_t a, uint8_t b)
{
    uint8_t ret = a + b;

    FLAG_C = (0xFF - a < b);
    FLAG_H = (0x0F - (a & 0x0F) < (b & 0x0F));
    FLAG_Z = (ret == 0);
    FLAG_N = 0;

    return ret;
}

static uint16_t add_16_16(uint16_t a, uint16_t b)
{
    uint16_t ret = a + b;

    FLAG_C = (0xFFFF - a < b);
    FLAG_H = (0x0FFF - (a & 0x0FFF) < (b & 0x0FFF));
    FLAG_N = 0;

    return ret;
}

static uint16_t add_16_8(uint16_t a, uint8_t b)
{
    uint16_t ret = a + (uint16_t)b;

    FLAG_C = (0xFF - (a & 0x00ff) < b);
    FLAG_H = (0x0F - (a & 0x0F) < (b & 0x0F));
    FLAG_Z = 0;
    FLAG_N = 0;

    return ret;
}

static uint8_t adc(uint8_t a, uint8_t b)
{
    uint8_t tmp, ret;
    int c = 0;
    int h = 0;

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

/* 8-bit loads */
/* LD r <- s */
static void LDbb() { REG_B = REG_B; }
static void LDbc() { REG_B = REG_C; }
static void LDbd() { REG_B = REG_D; }
static void LDbe() { REG_B = REG_E; }
static void LDbh() { REG_B = REG_H; }
static void LDbl() { REG_B = REG_L; }
static void LDba() { REG_B = REG_A; }
static void LDcb() { REG_C = REG_B; }
static void LDcc() { REG_C = REG_C; }
static void LDcd() { REG_C = REG_D; }
static void LDce() { REG_C = REG_E; }
static void LDch() { REG_C = REG_H; }
static void LDcl() { REG_C = REG_L; }
static void LDca() { REG_C = REG_A; }
static void LDdb() { REG_D = REG_B; }
static void LDdc() { REG_D = REG_C; }
static void LDdd() { REG_D = REG_D; }
static void LDde() { REG_D = REG_E; }
static void LDdh() { REG_D = REG_H; }
static void LDdl() { REG_D = REG_L; }
static void LDda() { REG_D = REG_A; }
static void LDeb() { REG_E = REG_B; }
static void LDec() { REG_E = REG_C; }
static void LDed() { REG_E = REG_D; }
static void LDee() { REG_E = REG_E; }
static void LDeh() { REG_E = REG_H; }
static void LDel() { REG_E = REG_L; }
static void LDea() { REG_E = REG_A; }
static void LDhb() { REG_H = REG_B; }
static void LDhc() { REG_H = REG_C; }
static void LDhd() { REG_H = REG_D; }
static void LDhe() { REG_H = REG_E; }
static void LDhh() { REG_H = REG_H; }
static void LDhl() { REG_H = REG_L; }
static void LDha() { REG_H = REG_A; }
static void LDlb() { REG_L = REG_B; }
static void LDlc() { REG_L = REG_C; }
static void LDld() { REG_L = REG_D; }
static void LDle() { REG_L = REG_E; }
static void LDlh() { REG_L = REG_H; }
static void LDll() { REG_L = REG_L; }
static void LDla() { REG_L = REG_A; }
static void LDab() { REG_A = REG_B; }
static void LDac() { REG_A = REG_C; }
static void LDad() { REG_A = REG_D; }
static void LDae() { REG_A = REG_E; }
static void LDah() { REG_A = REG_H; }
static void LDal() { REG_A = REG_L; }
static void LDaa() { REG_A = REG_A; }
static void LDbn() { REG_B = read_8(cpu.mmu, REG_PC++); }
static void LDcn() { REG_C = read_8(cpu.mmu, REG_PC++); }
static void LDdn() { REG_D = read_8(cpu.mmu, REG_PC++); }
static void LDen() { REG_E = read_8(cpu.mmu, REG_PC++); }
static void LDhn() { REG_H = read_8(cpu.mmu, REG_PC++); }
static void LDln() { REG_L = read_8(cpu.mmu, REG_PC++); }
static void LDan() { REG_A = read_8(cpu.mmu, REG_PC++); }
static void LDbmHL() { REG_B = read_8(cpu.mmu, REG_HL); }
static void LDcmHL() { REG_C = read_8(cpu.mmu, REG_HL); }
static void LDdmHL() { REG_D = read_8(cpu.mmu, REG_HL); }
static void LDemHL() { REG_E = read_8(cpu.mmu, REG_HL); }
static void LDhmHL() { REG_H = read_8(cpu.mmu, REG_HL); }
static void LDlmHL() { REG_L = read_8(cpu.mmu, REG_HL); }
/* LD d <- r */
static void LDmHLb() { write_8(cpu.mmu, REG_HL, REG_B); }
static void LDmHLc() { write_8(cpu.mmu, REG_HL, REG_C); }
static void LDmHLd() { write_8(cpu.mmu, REG_HL, REG_D); }
static void LDmHLe() { write_8(cpu.mmu, REG_HL, REG_E); }
static void LDmHLh() { write_8(cpu.mmu, REG_HL, REG_H); }
static void LDmHLl() { write_8(cpu.mmu, REG_HL, REG_L); }
/* LD d <- n */
static void LDmHLn() { write_8(cpu.mmu, REG_HL, read_8(cpu.mmu, REG_PC++)); }
/* LD A <- (ss) */
static void LDamBC() { REG_A = read_8(cpu.mmu, REG_BC); }
static void LDamDE() { REG_A = read_8(cpu.mmu, REG_DE); }
static void LDamHL() { REG_A = read_8(cpu.mmu, REG_HL); }
static void LDamnn() { REG_A = read_8(cpu.mmu, read_16(cpu.mmu, REG_PC)); REG_PC += 2; }
/* LD (dd) <- A */
static void LDmBCa() { write_8(cpu.mmu, read_16(cpu.mmu, REG_BC), REG_A); }
static void LDmDEa() { write_8(cpu.mmu, read_16(cpu.mmu, REG_DE), REG_A); }
static void LDmHLa() { write_8(cpu.mmu, read_16(cpu.mmu, REG_HL), REG_A); }
static void LDmnna() { write_8(cpu.mmu, read_16(cpu.mmu, REG_PC), REG_A); REG_PC += 2; }
/* LD A <- (C) */
static void LDamc() { REG_A = read_8(cpu.mmu, REG_C); }
/* LD (C) <- A */
static void LDmca() { write_8(cpu.mmu, read8(REG_C), REG_A); }
/* LDD A <- (HL) */
static void LDDamHL() { REG_A = read_8(cpu.mmu, REG_HL--); }
/* LDD (HL) <- A */
static void LDDmHLa() { write_8(cpu.mmu, REG_HL--, REG_A); }
/* LDI A <- (HL) */
static void LDIamHL() { REG_A = read_8(cpu.mmu, REG_HL++); }
/* LDI (HL) <- A */
static void LDImHLa() { write_8(cpu.mmu, REG_HL++, REG_A); }
/* LDH (n) <- A */
static void LDHmna() { write_8(cpu.mmu, read_8(cpu.mmu, REG_PC++) + 0xFF00, REG_A); }
/* LDH A <- (n) */
static void LDHamn() { REG_A = read_8(cpu.mmu, 0xFF00 + read_8(cpu.mmu, REG_PC++)); }

/* 16-bit loads */
/* LD dd, nn */
static void LDBCnn() { REG_BC = read_16(cpu.mmu, REG_PC); REG_PC += 2; }
static void LDDEnn() { REG_DE = read_16(cpu.mmu, REG_PC); REG_PC += 2; }
static void LDHLnn() { REG_HL = read_16(cpu.mmu, REG_PC); REG_PC += 2; }
static void LDSPnn() { REG_SP = read_16(cpu.mmu, REG_PC); REG_PC += 2; }
/* LD (nn), SP */
static void LDmnnSP() { write_16(cpu.mmu, read_16(cpu.mmu, REG_PC), REG_SP); REG_PC += 2; }
/* LD SP, HL */
static void LDSPHL() { REG_SP = REG_HL; }
/* LD HL, (SP + e) */
static void LDHLSPn() {  }
/* PUSH ss */
static void PUSHBC() { push(REG_BC); }
static void PUSHDE() { push(REG_DE); }
static void PUSHHL() { push(REG_HL); }
static void PUSHAF() { push(REG_AF); }
/* POP dd */
static void POPBC() { REG_BC = pop(); }
static void POPDE() { REG_DE = pop(); }
static void POPHL() { REG_HL = pop(); }
static void POPAF() { REG_AF = pop(); }

/* 8-bit ALU */
/* ADD A, s */
static void ADDab() { REG_A = add_8_8(REG_A, REG_B); }
static void ADDac() { REG_A = add_8_8(REG_A, REG_C); }
static void ADDad() { REG_A = add_8_8(REG_A, REG_D); }
static void ADDae() { REG_A = add_8_8(REG_A, REG_E); }
static void ADDah() { REG_A = add_8_8(REG_A, REG_H); }
static void ADDal() { REG_A = add_8_8(REG_A, REG_L); }
static void ADDaa() { REG_A = add_8_8(REG_A, REG_A); }
static void ADDan() { REG_A = add_8_8(REG_A, read8(REG_PC++)); }
static void ADDamHL() { REG_A = add_8_8(REG_A, read8(REG_HL)); }
/* ADC A, s */
static void ADCab() { REG_A = adc(REG_A, REG_B); }
static void ADCac() { REG_A = adc(REG_A, REG_C); }
static void ADCad() { REG_A = adc(REG_A, REG_D); }
static void ADCae() { REG_A = adc(REG_A, REG_E); }
static void ADCah() { REG_A = adc(REG_A, REG_H); }
static void ADCal() { REG_A = adc(REG_A, REG_L); }
static void ADCaa() { REG_A = adc(REG_A, REG_A); }
static void ADCan() { REG_A = adc(REG_A, read_8(REG_PC++)); }
static void ADCamHL() { REG_A = adc(REG_A, read_8(REG_HL)); }
/* SUB s */
static void SUBab() {  }
static void SUBac() {  }
static void SUBad() {  }
static void SUBae() {  }
static void SUBah() {  }
static void SUBal() {  }
static void SUBaa() {  }
static void SUBan() {  }
static void SUBamHL() {  }
/* SBC A, s */
static void SBCab() {  }
static void SBCac() {  }
static void SBCad() {  }
static void SBCae() {  }
static void SBCah() {  }
static void SBCal() {  }
static void SBCaa() {  }
static void SBCan() {  }
static void SBCamHL() {  }
/* AND s */
static void ANDb() {  }
static void ANDc() {  }
static void ANDd() {  }
static void ANDe() {  }
static void ANDh() {  }
static void ANDl() {  }
static void ANDa() {  }
static void ANDn() {  }
static void ANDmHL() {  }
/* OR s */
static void ORb() {  }
static void ORc() {  }
static void ORd() {  }
static void ORe() {  }
static void ORh() {  }
static void ORl() {  }
static void ORa() {  }
static void ORn() {  }
static void ORmHL() {  }
/* XOR s */
static void XORb() {  }
static void XORc() {  }
static void XORd() {  }
static void XORe() {  }
static void XORh() {  }
static void XORl() {  }
static void XORa() {  }
static void XORn() {  }
static void XORmHL() {  }
/* CP s */
static void CPb() {  }
static void CPc() {  }
static void CPd() {  }
static void CPe() {  }
static void CPh() {  }
static void CPl() {  }
static void CPa() {  }
static void CPn() {  }
static void CPmHL() {  }
/* INC s */
static void INCb() {  }
static void INCc() {  }
static void INCd() {  }
static void INCe() {  }
static void INCh() {  }
static void INCl() {  }
static void INCa() {  }
static void INCmHL() {  }
/* DEC s */
static void DECb() {  }
static void DECc() {  }
static void DECd() {  }
static void DECe() {  }
static void DECh() {  }
static void DECl() {  }
static void DECa() {  }
static void DECmHL() {  }

/* 16-bit arithmetic */
/* ADD HL, ss */
static void ADDHLBC() {  }
static void ADDHLDE() {  }
static void ADDHLHL() {  }
static void ADDHLSP() {  }
/* ADD SP, e */
static void ADDSPn() {  }
/* INC ss */
static void INCBC() {  }
static void INCDE() {  }
static void INCHL() {  }
static void INCSP() {  }
/* DEC ss */
static void DECBC() {  }
static void DECDE() {  }
static void DECHL() {  }
static void DECSP() {  }

/* Misc */
/* SWAP s */
static void SWAPb() {  }
static void SWAPc() {  }
static void SWAPd() {  }
static void SWAPe() {  }
static void SWAPh() {  }
static void SWAPl() {  }
static void SWAPa() {  }
static void SWAPmHL() {  }
/* DAA */
static void DAA() {  }
/* CPL */
static void CPL() {  }
/* CCF */
static void CCF() {  }
/* SCF */
static void SCF() {  }
/* NOP */
static void NOP() {  }
/* HALT */
static void HALT() {  }
/* STOP */
static void STOP() {  }
/* DI */
static void DI() {  }
/* EI */
static void EI() {  }

/* Rotates and shifts */
/* RLCA */
static void RLCA() {  }
/* RLA */
static void RLA() {  }
/* RRCA */
static void RRCA() {  }
/* RRA */
static void RRA() {  }
/* RLC s */
static void RLCb() {  }
static void RLCc() {  }
static void RLCd() {  }
static void RLCe() {  }
static void RLCh() {  }
static void RLCl() {  }
static void RLCa() {  }
static void RLCmHL() {  }
/* RL s */
static void RLb() {  }
static void RLc() {  }
static void RLd() {  }
static void RLe() {  }
static void RLh() {  }
static void RLl() {  }
static void RLa() {  }
static void RLmHL() {  }
/* RRC s */
static void RRCb() {  }
static void RRCc() {  }
static void RRCd() {  }
static void RRCe() {  }
static void RRCh() {  }
static void RRCl() {  }
static void RRCa() {  }
static void RRCmHL() {  }
/* RR s */
static void RRb() {  }
static void RRc() {  }
static void RRd() {  }
static void RRe() {  }
static void RRh() {  }
static void RRl() {  }
static void RRa() {  }
static void RRmHL() {  }
/* SLA s */
static void SLAb() {  }
static void SLAc() {  }
static void SLAd() {  }
static void SLAe() {  }
static void SLAh() {  }
static void SLAl() {  }
static void SLAa() {  }
static void SLAmHL() {  }
/* SRA s */
static void SRAb() {  }
static void SRAc() {  }
static void SRAd() {  }
static void SRAe() {  }
static void SRAh() {  }
static void SRAl() {  }
static void SRAa() {  }
static void SRAmHL() {  }
/* SRL s */
static void SRLb() {  }
static void SRLc() {  }
static void SRLd() {  }
static void SRLe() {  }
static void SRLh() {  }
static void SRLl() {  }
static void SRLa() {  }
static void SRLmHL() {  }

/* Bit manipulation */
/* BIT b, s */
static void BIT0b() {  }
static void BIT0c() {  }
static void BIT0d() {  }
static void BIT0e() {  }
static void BIT0h() {  }
static void BIT0l() {  }
static void BIT0a() {  }
static void BIT0mHL() {  }
static void BIT1b() {  }
static void BIT1c() {  }
static void BIT1d() {  }
static void BIT1e() {  }
static void BIT1h() {  }
static void BIT1l() {  }
static void BIT1a() {  }
static void BIT1mHL() {  }
static void BIT2b() {  }
static void BIT2c() {  }
static void BIT2d() {  }
static void BIT2e() {  }
static void BIT2h() {  }
static void BIT2l() {  }
static void BIT2a() {  }
static void BIT2mHL() {  }
static void BIT3b() {  }
static void BIT3c() {  }
static void BIT3d() {  }
static void BIT3e() {  }
static void BIT3h() {  }
static void BIT3l() {  }
static void BIT3a() {  }
static void BIT3mHL() {  }
static void BIT4b() {  }
static void BIT4c() {  }
static void BIT4d() {  }
static void BIT4e() {  }
static void BIT4h() {  }
static void BIT4l() {  }
static void BIT4a() {  }
static void BIT4mHL() {  }
static void BIT5b() {  }
static void BIT5c() {  }
static void BIT5d() {  }
static void BIT5e() {  }
static void BIT5h() {  }
static void BIT5l() {  }
static void BIT5a() {  }
static void BIT5mHL() {  }
static void BIT6b() {  }
static void BIT6c() {  }
static void BIT6d() {  }
static void BIT6e() {  }
static void BIT6h() {  }
static void BIT6l() {  }
static void BIT6a() {  }
static void BIT6mHL() {  }
static void BIT7b() {  }
static void BIT7c() {  }
static void BIT7d() {  }
static void BIT7e() {  }
static void BIT7h() {  }
static void BIT7l() {  }
static void BIT7a() {  }
static void BIT7mHL() {  }
/* SET b, s */
static void SET0b() {  }
static void SET0c() {  }
static void SET0d() {  }
static void SET0e() {  }
static void SET0h() {  }
static void SET0l() {  }
static void SET0a() {  }
static void SET0mHL() {  }
static void SET1b() {  }
static void SET1c() {  }
static void SET1d() {  }
static void SET1e() {  }
static void SET1h() {  }
static void SET1l() {  }
static void SET1a() {  }
static void SET1mHL() {  }
static void SET2b() {  }
static void SET2c() {  }
static void SET2d() {  }
static void SET2e() {  }
static void SET2h() {  }
static void SET2l() {  }
static void SET2a() {  }
static void SET2mHL() {  }
static void SET3b() {  }
static void SET3c() {  }
static void SET3d() {  }
static void SET3e() {  }
static void SET3h() {  }
static void SET3l() {  }
static void SET3a() {  }
static void SET3mHL() {  }
static void SET4b() {  }
static void SET4c() {  }
static void SET4d() {  }
static void SET4e() {  }
static void SET4h() {  }
static void SET4l() {  }
static void SET4a() {  }
static void SET4mHL() {  }
static void SET5b() {  }
static void SET5c() {  }
static void SET5d() {  }
static void SET5e() {  }
static void SET5h() {  }
static void SET5l() {  }
static void SET5a() {  }
static void SET5mHL() {  }
static void SET6b() {  }
static void SET6c() {  }
static void SET6d() {  }
static void SET6e() {  }
static void SET6h() {  }
static void SET6l() {  }
static void SET6a() {  }
static void SET6mHL() {  }
static void SET7b() {  }
static void SET7c() {  }
static void SET7d() {  }
static void SET7e() {  }
static void SET7h() {  }
static void SET7l() {  }
static void SET7a() {  }
static void SET7mHL() {  }
/* RES b, s */
static void RES0b() {  }
static void RES0c() {  }
static void RES0d() {  }
static void RES0e() {  }
static void RES0h() {  }
static void RES0l() {  }
static void RES0a() {  }
static void RES0mHL() {  }
static void RES1b() {  }
static void RES1c() {  }
static void RES1d() {  }
static void RES1e() {  }
static void RES1h() {  }
static void RES1l() {  }
static void RES1a() {  }
static void RES1mHL() {  }
static void RES2b() {  }
static void RES2c() {  }
static void RES2d() {  }
static void RES2e() {  }
static void RES2h() {  }
static void RES2l() {  }
static void RES2a() {  }
static void RES2mHL() {  }
static void RES3b() {  }
static void RES3c() {  }
static void RES3d() {  }
static void RES3e() {  }
static void RES3h() {  }
static void RES3l() {  }
static void RES3a() {  }
static void RES3mHL() {  }
static void RES4b() {  }
static void RES4c() {  }
static void RES4d() {  }
static void RES4e() {  }
static void RES4h() {  }
static void RES4l() {  }
static void RES4a() {  }
static void RES4mHL() {  }
static void RES5b() {  }
static void RES5c() {  }
static void RES5d() {  }
static void RES5e() {  }
static void RES5h() {  }
static void RES5l() {  }
static void RES5a() {  }
static void RES5mHL() {  }
static void RES6b() {  }
static void RES6c() {  }
static void RES6d() {  }
static void RES6e() {  }
static void RES6h() {  }
static void RES6l() {  }
static void RES6a() {  }
static void RES6mHL() {  }
static void RES7b() {  }
static void RES7c() {  }
static void RES7d() {  }
static void RES7e() {  }
static void RES7h() {  }
static void RES7l() {  }
static void RES7a() {  }
static void RES7mHL() {  }

/* Jumps */
/* JP nn */
static void JPnn() {  }
/* JP cc, nn */
static void JPZnn() {  }
static void JPCnn() {  }
static void JPNZnn() {  }
static void JPNCnn() {  }
/* JP (HL) */
static void JPmHL() {  }
/* JR e */
static void JRn() {  }
/* JR cc, e */
static void JRZn() {  }
static void JRCn() {  }
static void JRNZn() {  }
static void JRNCn() {  }

/* Calls */
/* CALL nn */
static void CALLnn() {  }
/* CALL cc, nn */
static void CALLZnn() {  }
static void CALLCnn() {  }
static void CALLNZnn() {  }
static void CALLNCnn() {  }

/* Restarts */
/* RST f */
static void RST0() {  }
static void RST8() {  }
static void RST10() {  }
static void RST18() {  }
static void RST20() {  }
static void RST28() {  }
static void RST30() {  }
static void RST38() {  }

/* Returns */
/* RET */
static void RET() {  }
/* RET cc */
static void RETZ() {  }
static void RETC() {  }
static void RETNZ() {  }
static void RETNC() {  }
/* RETI */
static void RETI() {  }

/* Lookup table for two-byte opcodes */
static void (*cb_ops[256])() = {
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
static void CB() { (*cb_ops[read_8(cpu.mmu, REG_PC++)])(); }

/* Table of function pointers indexed by opcode */
static void (*ops[256])() = {
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
uint8_t timings_m[/*256*/] = {
    0 /* Stuff */
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

int cpu_run(int cycles)
{
    uint32_t total = 0;

    while (total < cycles) {
        /* TODO: Interrupts */

        cpu.op = read_8(cpu.mmu, REG_PC++);

        (*ops[cpu.op])();

        cpu.ins_clock.m = timings_m[cpu.op];
        cpu.ins_clock.t = timings_t[cpu.op];
        cpu.sys_clock.m += cpu.ins_clock.m;
        cpu.sys_clock.t += cpu.ins_clock.t;

        total += cpu.ins_clock.m;
    }

    return 0;
}

/* Save states */
void save_state(void)
{
    /* Save CPU state */
    /* Write to disk */
}

void load_state(void)
{
    cpu = save_states[curr_save_slot];
}

