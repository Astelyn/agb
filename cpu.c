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

/* 8-bit loads */
// LD r <- s
void LDbb() { REG_B = REG_B; }
void LDbc() { REG_B = REG_C; }
void LDbd() { REG_B = REG_D; }
void LDbe() { REG_B = REG_E; }
void LDbh() { REG_B = REG_H; }
void LDbl() { REG_B = REG_L; }
void LDba() { REG_B = REG_A; }
void LDcb() { REG_C = REG_B; }
void LDcc() { REG_C = REG_C; }
void LDcd() { REG_C = REG_D; }
void LDce() { REG_C = REG_E; }
void LDch() { REG_C = REG_H; }
void LDcl() { REG_C = REG_L; }
void LDca() { REG_C = REG_A; }
void LDdb() { REG_D = REG_B; }
void LDdc() { REG_D = REG_C; }
void LDdd() { REG_D = REG_D; }
void LDde() { REG_D = REG_E; }
void LDdh() { REG_D = REG_H; }
void LDdl() { REG_D = REG_L; }
void LDda() { REG_D = REG_A; }
void LDeb() { REG_E = REG_B; }
void LDec() { REG_E = REG_C; }
void LDed() { REG_E = REG_D; }
void LDee() { REG_E = REG_E; }
void LDeh() { REG_E = REG_H; }
void LDel() { REG_E = REG_L; }
void LDea() { REG_E = REG_A; }
void LDhb() { REG_H = REG_B; }
void LDhc() { REG_H = REG_C; }
void LDhd() { REG_H = REG_D; }
void LDhe() { REG_H = REG_E; }
void LDhh() { REG_H = REG_H; }
void LDhl() { REG_H = REG_L; }
void LDha() { REG_H = REG_A; }
void LDlb() { REG_L = REG_B; }
void LDlc() { REG_L = REG_C; }
void LDld() { REG_L = REG_D; }
void LDle() { REG_L = REG_E; }
void LDlh() { REG_L = REG_H; }
void LDll() { REG_L = REG_L; }
void LDla() { REG_L = REG_A; }
void LDab() { REG_A = REG_B; }
void LDac() { REG_A = REG_C; }
void LDad() { REG_A = REG_D; }
void LDae() { REG_A = REG_E; }
void LDah() { REG_A = REG_H; }
void LDal() { REG_A = REG_L; }
void LDaa() { REG_A = REG_A; }
void LDbn() { REG_B = read_8(REG_PC++); }
void LDcn() { REG_C = read_8(REG_PC++); }
void LDdn() { REG_D = read_8(REG_PC++); }
void LDen() { REG_E = read_8(REG_PC++); }
void LDhn() { REG_H = read_8(REG_PC++); }
void LDln() { REG_L = read_8(REG_PC++); }
void LDan() { REG_A = read_8(REG_PC++); }
void LDbmHL() { REG_B = read_8(REG_HL); }
void LDcmHL() { REG_C = read_8(REG_HL); }
void LDdmHL() { REG_D = read_8(REG_HL); }
void LDemHL() { REG_E = read_8(REG_HL); }
void LDhmHL() { REG_H = read_8(REG_HL); }
void LDlmHL() { REG_L = read_8(REG_HL); }
// LD d <- r
void LDmHLb() { write_8(REG_HL, REG_B); }
void LDmHLc() { write_8(REG_HL, REG_C); }
void LDmHLd() { write_8(REG_HL, REG_D); }
void LDmHLe() { write_8(REG_HL, REG_E); }
void LDmHLh() { write_8(REG_HL, REG_H); }
void LDmHLl() { write_8(REG_HL, REG_L); }
// LD d <- n
void LDmHLn() { write_8(REG_HL, read_8(REG_PC++)); }
// LD A <- (ss)
void LDamBC() { REG_A = read_8(REG_BC); }
void LDamDE() { REG_A = read_8(REG_DE); }
void LDamHL() { REG_A = read_8(REG_HL); }
void LDamnn() { REG_A = read_8(read_16(REG_PC)); REG_PC += 2; }
// LD (dd) <- A
void LDmBCa() { write_8(read_16(REG_BC), REG_A); }
void LDmDEa() { write_8(read_16(REG_DE), REG_A); }
void LDmHLa() { write_8(read_16(REG_HL), REG_A); }
void LDmnna() { write_8(read_16(REG_PC), REG_A); REG_PC += 2; }
// LD A <- (C)
void LDamc() { REG_A = read_8(REG_C); }
// LD (C) <- A
void LDmca() { write_8(read8(REG_C), REG_A); }
// LDD A <- (HL)
void LDDamHL() { REG_A = read_8(REG_HL--); }
// LDD (HL) <- A
void LDDmHLa() { write_8(REG_HL--, REG_A); }
// LDI A <- (HL)
void LDIamHL() { REG_A = read_8(REG_HL++); }
// LDI (HL) <- A
void LDImHLa() { write_8(REG_HL++, REG_A); }
// LDH (n) <- A
void LDHmna() { write_8(read_8(REG_PC++) + 0xFF00, REG_A); }
// LDH A <- (n)
void LDHamn() { REG_A = read_8(0xFF00 + read_8(REG_PC++)); }

/* 16-bit loads */
// LD dd, nn
void LDBCnn() {  }
void LDDEnn() {  }
void LDHLnn() {  }
void LDSPnn() {  }
// LD (nn), SP
void LDmnnSP() {  }
// LD SP, HL
void LDSPHL() {  }
// LD HL, (SP + e)
void LSHLSPn() {  }
// PUSH ss
void PUSHBC() {  }
void PUSHDE() {  }
void PUSHHL() {  }
void PUSHAF() {  }
// POP dd
void POPBC() {  }
void POPDE() {  }
void POPHL() {  }
void POPAF() {  }

/* 8-bit ALU */
// ADD A, s
void ADDab() {  }
void ADDac() {  }
void ADDad() {  }
void ADDae() {  }
void ADDah() {  }
void ADDal() {  }
void ADDaa() {  }
void ADDan() {  }
void ADDamHL() {  }
// ADC A, s
void ADCab() {  }
void ADCac() {  }
void ADCad() {  }
void ADCae() {  }
void ADCah() {  }
void ADCal() {  }
void ADCaa() {  }
void ADCan() {  }
void ADCamHL() {  }
// SUB s
void SUBab() {  }
void SUBac() {  }
void SUBad() {  }
void SUBae() {  }
void SUBah() {  }
void SUBal() {  }
void SUBaa() {  }
void SUBan() {  }
void SUBamHL() {  }
// SBC A, s
void SBCab() {  }
void SBCac() {  }
void SBCad() {  }
void SBCae() {  }
void SBCah() {  }
void SBCal() {  }
void SBCaa() {  }
void SBCan() {  }
void SBCamHL() {  }
// AND s
void ANDb() {  }
void ANDc() {  }
void ANDd() {  }
void ANDe() {  }
void ANDh() {  }
void ANDl() {  }
void ANDa() {  }
void ANDn() {  }
void ANDmHL() {  }
// OR s
void ORb() {  }
void ORc() {  }
void ORd() {  }
void ORe() {  }
void ORh() {  }
void ORl() {  }
void ORa() {  }
void ORn() {  }
void ORmHL() {  }
// XOR s
void XORb() {  }
void XORc() {  }
void XORd() {  }
void XORe() {  }
void XORh() {  }
void XORl() {  }
void XORa() {  }
void XORn() {  }
void XORmHL() {  }
// CP s
void CPb() {  }
void CPc() {  }
void CPd() {  }
void CPe() {  }
void CPh() {  }
void CPl() {  }
void CPa() {  }
void CPn() {  }
void CPmHL() {  }
// INC s
void INCb() {  }
void INCc() {  }
void INCd() {  }
void INCe() {  }
void INCh() {  }
void INCl() {  }
void INCa() {  }
void INCmHL() {  }
// DEC s
void DECb() {  }
void DECc() {  }
void DECd() {  }
void DECe() {  }
void DECh() {  }
void DECl() {  }
void DECa() {  }
void DECmHL() {  }

/* 16-bit arithmetic */
// ADD HL, ss
void ADDHLBC() {  }
void ADDHLDE() {  }
void ADDHLHL() {  }
void ADDHLSP() {  }
// ADD SP, e
void ADDSPn() {  }
// INC ss
void INCBC() {  }
void INCDE() {  }
void INCHL() {  }
void INCSP() {  }
// DEC ss
void DECBC() {  }
void DECDE() {  }
void DECHL() {  }
void DECSP() {  }

/* Misc */
// SWAP s
void SWAPb() {  }
void SWAPc() {  }
void SWAPd() {  }
void SWAPe() {  }
void SWAPh() {  }
void SWAPl() {  }
void SWAPa() {  }
void SWAPmHL() {  }
// DAA
void DAA() {  }
// CPL
void CPL() {  }
// CCF
void CCF() {  }
// SCF
void SCF() {  }
// NOP
void NOP() {  }
// HALT
void HALT() {  }
// STOP
void STOP() {  }
// DI
void DI() {  }
// EI
void EI() {  }

/* Rotates and shifts */
// RLCA
void RLCA() {  }
// RLA
void RLA() {  }
// RRCA
void RRCA() {  }
// RRA
void RRA() {  }
// RLC s
void RLCb() {  }
void RLCc() {  }
void RLCd() {  }
void RLCe() {  }
void RLCh() {  }
void RLCl() {  }
void RLCa() {  }
void RLCmHL() {  }
// RL s
void RLb() {  }
void RLc() {  }
void RLd() {  }
void RLe() {  }
void RLh() {  }
void RLl() {  }
void RLa() {  }
void RLmHL() {  }
// RRC s
void RRCb() {  }
void RRCc() {  }
void RRCd() {  }
void RRCe() {  }
void RRCh() {  }
void RRCl() {  }
void RRCa() {  }
void RRCmHL() {  }
// RR s
void RRb() {  }
void RRc() {  }
void RRd() {  }
void RRe() {  }
void RRh() {  }
void RRl() {  }
void RRa() {  }
void RRmHL() {  }
// SLA s
void SLAb() {  }
void SLAc() {  }
void SLAd() {  }
void SLAe() {  }
void SLAh() {  }
void SLAl() {  }
void SLAa() {  }
void SLAmHL() {  }
// SRA s
void SRAb() {  }
void SRAc() {  }
void SRAd() {  }
void SRAe() {  }
void SRAh() {  }
void SRAl() {  }
void SRAa() {  }
void SRAmHL() {  }
// SRL s
void SRLb() {  }
void SRLc() {  }
void SRLd() {  }
void SRLe() {  }
void SRLh() {  }
void SRLl() {  }
void SRLa() {  }
void SRLmHL() {  }

/* Bit manipulation */
// BIT b, s
void BIT0b() {  }
void BIT0c() {  }
void BIT0d() {  }
void BIT0e() {  }
void BIT0h() {  }
void BIT0l() {  }
void BIT0a() {  }
void BIT0mHL() {  }
void BIT1b() {  }
void BIT1c() {  }
void BIT1d() {  }
void BIT1e() {  }
void BIT1h() {  }
void BIT1l() {  }
void BIT1a() {  }
void BIT1mHL() {  }
void BIT2b() {  }
void BIT2c() {  }
void BIT2d() {  }
void BIT2e() {  }
void BIT2h() {  }
void BIT2l() {  }
void BIT2a() {  }
void BIT2mHL() {  }
void BIT3b() {  }
void BIT3c() {  }
void BIT3d() {  }
void BIT3e() {  }
void BIT3h() {  }
void BIT3l() {  }
void BIT3a() {  }
void BIT3mHL() {  }
void BIT4b() {  }
void BIT4c() {  }
void BIT4d() {  }
void BIT4e() {  }
void BIT4h() {  }
void BIT4l() {  }
void BIT4a() {  }
void BIT4mHL() {  }
void BIT5b() {  }
void BIT5c() {  }
void BIT5d() {  }
void BIT5e() {  }
void BIT5h() {  }
void BIT5l() {  }
void BIT5a() {  }
void BIT5mHL() {  }
void BIT6b() {  }
void BIT6c() {  }
void BIT6d() {  }
void BIT6e() {  }
void BIT6h() {  }
void BIT6l() {  }
void BIT6a() {  }
void BIT6mHL() {  }
void BIT7b() {  }
void BIT7c() {  }
void BIT7d() {  }
void BIT7e() {  }
void BIT7h() {  }
void BIT7l() {  }
void BIT7a() {  }
void BIT7mHL() {  }
// SET b, s
void SET0b() {  }
void SET0c() {  }
void SET0d() {  }
void SET0e() {  }
void SET0h() {  }
void SET0l() {  }
void SET0a() {  }
void SET0mHL() {  }
void SET1b() {  }
void SET1c() {  }
void SET1d() {  }
void SET1e() {  }
void SET1h() {  }
void SET1l() {  }
void SET1a() {  }
void SET1mHL() {  }
void SET2b() {  }
void SET2c() {  }
void SET2d() {  }
void SET2e() {  }
void SET2h() {  }
void SET2l() {  }
void SET2a() {  }
void SET2mHL() {  }
void SET3b() {  }
void SET3c() {  }
void SET3d() {  }
void SET3e() {  }
void SET3h() {  }
void SET3l() {  }
void SET3a() {  }
void SET3mHL() {  }
void SET4b() {  }
void SET4c() {  }
void SET4d() {  }
void SET4e() {  }
void SET4h() {  }
void SET4l() {  }
void SET4a() {  }
void SET4mHL() {  }
void SET5b() {  }
void SET5c() {  }
void SET5d() {  }
void SET5e() {  }
void SET5h() {  }
void SET5l() {  }
void SET5a() {  }
void SET5mHL() {  }
void SET6b() {  }
void SET6c() {  }
void SET6d() {  }
void SET6e() {  }
void SET6h() {  }
void SET6l() {  }
void SET6a() {  }
void SET6mHL() {  }
void SET7b() {  }
void SET7c() {  }
void SET7d() {  }
void SET7e() {  }
void SET7h() {  }
void SET7l() {  }
void SET7a() {  }
void SET7mHL() {  }
// RES b, s
void RES0b() {  }
void RES0c() {  }
void RES0d() {  }
void RES0e() {  }
void RES0h() {  }
void RES0l() {  }
void RES0a() {  }
void RES0mHL() {  }
void RES1b() {  }
void RES1c() {  }
void RES1d() {  }
void RES1e() {  }
void RES1h() {  }
void RES1l() {  }
void RES1a() {  }
void RES1mHL() {  }
void RES2b() {  }
void RES2c() {  }
void RES2d() {  }
void RES2e() {  }
void RES2h() {  }
void RES2l() {  }
void RES2a() {  }
void RES2mHL() {  }
void RES3b() {  }
void RES3c() {  }
void RES3d() {  }
void RES3e() {  }
void RES3h() {  }
void RES3l() {  }
void RES3a() {  }
void RES3mHL() {  }
void RES4b() {  }
void RES4c() {  }
void RES4d() {  }
void RES4e() {  }
void RES4h() {  }
void RES4l() {  }
void RES4a() {  }
void RES4mHL() {  }
void RES5b() {  }
void RES5c() {  }
void RES5d() {  }
void RES5e() {  }
void RES5h() {  }
void RES5l() {  }
void RES5a() {  }
void RES5mHL() {  }
void RES6b() {  }
void RES6c() {  }
void RES6d() {  }
void RES6e() {  }
void RES6h() {  }
void RES6l() {  }
void RES6a() {  }
void RES6mHL() {  }
void RES7b() {  }
void RES7c() {  }
void RES7d() {  }
void RES7e() {  }
void RES7h() {  }
void RES7l() {  }
void RES7a() {  }
void RES7mHL() {  }

/* Jumps */
// JP nn
void JPnn() {  }
// JP cc, nn
void JPZnn() {  }
void JPCnn() {  }
void JPNZnn() {  }
void JPNCnn() {  }
// JP (HL)
void JPmHL() {  }
// JR e
void JRn() {  }
// JR cc, e
void JRZn() {  }
void JRCn() {  }
void JRNZn() {  }
void JRNCn() {  }

/* Calls */
// CALL nn
void CALLnn() {  }
// CALL cc, nn
void CALLZnn() {  }
void CALLCnn() {  }
void CALLNZnn() {  }
void CALLNCnn() {  }

/* Restarts */
// RST f
void RST0() {  }
void RST8() {  }
void RST10() {  }
void RST18() {  }
void RST20() {  }
void RST28() {  }
void RST30() {  }
void RST38() {  }

/* Returns */
// RET
void RET() {  }
// RET cc
void RETZ() {  }
void RETC() {  }
void RETNZ() {  }
void RETNC() {  }
// RETI
void RETI() {  }

