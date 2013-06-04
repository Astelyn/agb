#include "cpu.h"

/* 8-bit loads */
// LD r <- s
void LDbb(cpu_t* cpu) { REG_B = REG_B; }
void LDbc(cpu_t* cpu) { REG_B = REG_C; }
void LDbd(cpu_t* cpu) { REG_B = REG_D; }
void LDbe(cpu_t* cpu) { REG_B = REG_E; }
void LDbh(cpu_t* cpu) { REG_B = REG_H; }
void LDbl(cpu_t* cpu) { REG_B = REG_L; }
void LDba(cpu_t* cpu) { REG_B = REG_A; }
void LDcb(cpu_t* cpu) { REG_C = REG_B; }
void LDcc(cpu_t* cpu) { REG_C = REG_C; }
void LDcd(cpu_t* cpu) { REG_C = REG_D; }
void LDce(cpu_t* cpu) { REG_C = REG_E; }
void LDch(cpu_t* cpu) { REG_C = REG_H; }
void LDcl(cpu_t* cpu) { REG_C = REG_L; }
void LDca(cpu_t* cpu) { REG_C = REG_A; }
void LDdb(cpu_t* cpu) { REG_D = REG_B; }
void LDdc(cpu_t* cpu) { REG_D = REG_C; }
void LDdd(cpu_t* cpu) { REG_D = REG_D; }
void LDde(cpu_t* cpu) { REG_D = REG_E; }
void LDdh(cpu_t* cpu) { REG_D = REG_H; }
void LDdl(cpu_t* cpu) { REG_D = REG_L; }
void LDda(cpu_t* cpu) { REG_D = REG_A; }
void LDeb(cpu_t* cpu) {  }
void LDec(cpu_t* cpu) {  }
void LDed(cpu_t* cpu) {  }
void LDee(cpu_t* cpu) {  }
void LDeh(cpu_t* cpu) {  }
void LDel(cpu_t* cpu) {  }
void LDea(cpu_t* cpu) {  }
void LDhb(cpu_t* cpu) {  }
void LDhc(cpu_t* cpu) {  }
void LDhd(cpu_t* cpu) {  }
void LDhe(cpu_t* cpu) {  }
void LDhh(cpu_t* cpu) {  }
void LDhl(cpu_t* cpu) {  }
void LDha(cpu_t* cpu) {  }
void LDlb(cpu_t* cpu) {  }
void LDlc(cpu_t* cpu) {  }
void LDld(cpu_t* cpu) {  }
void LDle(cpu_t* cpu) {  }
void LDlh(cpu_t* cpu) {  }
void LDll(cpu_t* cpu) {  }
void LDla(cpu_t* cpu) {  }
void LDab(cpu_t* cpu) {  }
void LDac(cpu_t* cpu) {  }
void LDad(cpu_t* cpu) {  }
void LDae(cpu_t* cpu) {  }
void LDah(cpu_t* cpu) {  }
void LDal(cpu_t* cpu) {  }
void LDaa(cpu_t* cpu) {  }
void LDbn(cpu_t* cpu) {  }
void LDcn(cpu_t* cpu) {  }
void LDdn(cpu_t* cpu) {  }
void LDen(cpu_t* cpu) {  }
void LDhn(cpu_t* cpu) {  }
void LDln(cpu_t* cpu) {  }
void LDan(cpu_t* cpu) {  }
void LDbmHL(cpu_t* cpu) {  }
void LDcmHL(cpu_t* cpu) {  }
void LDdmHL(cpu_t* cpu) {  }
void LDemHL(cpu_t* cpu) {  }
void LDhmHL(cpu_t* cpu) {  }
void LDlmHL(cpu_t* cpu) {  }
// LD d <- r
void LDmHLb(cpu_t* cpu) {  }
void LDmHLc(cpu_t* cpu) {  }
void LDmHLd(cpu_t* cpu) {  }
void LDmHLe(cpu_t* cpu) {  }
void LDmHLh(cpu_t* cpu) {  }
void LDmHLl(cpu_t* cpu) {  }
// LD d <- n
void LDmHLn(cpu_t* cpu) {  }
// LD A <- (ss)
void LDamBC(cpu_t* cpu) {  }
void LDamDE(cpu_t* cpu) {  }
void LDamHL(cpu_t* cpu) {  }
void LDamnn(cpu_t* cpu) {  }
// LD (dd) <- A
void LDmBCa(cpu_t* cpu) {  }
void LDmDEa(cpu_t* cpu) {  }
void LDmHLa(cpu_t* cpu) {  }
void LDmnna(cpu_t* cpu) {  }
// LD A <- (C)
void LDamc(cpu_t* cpu) {  }
// LD (C) <- A
void LDmca(cpu_t* cpu) {  }
// LDD A <- (HL)
void LDDamHL(cpu_t* cpu) {  }
// LDD (HL) <- A
void LDDmHLa(cpu_t* cpu) {  }
// LDI A <- (HL)
void LDIamHL(cpu_t* cpu) {  }
// LDI (HL) <- A
void LDImHLa(cpu_t* cpu) {  }
// LDH (n) <- A
void LDHmna(cpu_t* cpu) {  }
// LDH A <- (n)
void LDHamn(cpu_t* cpu) {  }

/* 16-bit loads */
// LD dd, nn
void LDBCnn(cpu_t* cpu) {  }
void LDDEnn(cpu_t* cpu) {  }
void LDHLnn(cpu_t* cpu) {  }
void LDSPnn(cpu_t* cpu) {  }
// LD (nn), SP
void LDmnnSP(cpu_t* cpu) {  }
// LD SP, HL
void LDSPHL(cpu_t* cpu) {  }
// LD HL, (SP + e)
void LSHLSPn(cpu_t* cpu) {  }
// PUSH ss
void PUSHBC(cpu_t* cpu) {  }
void PUSHDE(cpu_t* cpu) {  }
void PUSHHL(cpu_t* cpu) {  }
void PUSHAF(cpu_t* cpu) {  }
// POP dd
void POPBC(cpu_t* cpu) {  }
void POPDE(cpu_t* cpu) {  }
void POPHL(cpu_t* cpu) {  }
void POPAF(cpu_t* cpu) {  }

/* 8-bit ALU */
// ADD A, s
void ADDab(cpu_t* cpu) {  }
void ADDac(cpu_t* cpu) {  }
void ADDad(cpu_t* cpu) {  }
void ADDae(cpu_t* cpu) {  }
void ADDah(cpu_t* cpu) {  }
void ADDal(cpu_t* cpu) {  }
void ADDaa(cpu_t* cpu) {  }
void ADDan(cpu_t* cpu) {  }
void ADDamHL(cpu_t* cpu) {  }
// ADC A, s
void ADCab(cpu_t* cpu) {  }
void ADCac(cpu_t* cpu) {  }
void ADCad(cpu_t* cpu) {  }
void ADCae(cpu_t* cpu) {  }
void ADCah(cpu_t* cpu) {  }
void ADCal(cpu_t* cpu) {  }
void ADCaa(cpu_t* cpu) {  }
void ADCan(cpu_t* cpu) {  }
void ADCamHL(cpu_t* cpu) {  }
// SUB s
void SUBab(cpu_t* cpu) {  }
void SUBac(cpu_t* cpu) {  }
void SUBad(cpu_t* cpu) {  }
void SUBae(cpu_t* cpu) {  }
void SUBah(cpu_t* cpu) {  }
void SUBal(cpu_t* cpu) {  }
void SUBaa(cpu_t* cpu) {  }
void SUBan(cpu_t* cpu) {  }
void SUBamHL(cpu_t* cpu) {  }
// SBC A, s
void SBCab(cpu_t* cpu) {  }
void SBCac(cpu_t* cpu) {  }
void SBCad(cpu_t* cpu) {  }
void SBCae(cpu_t* cpu) {  }
void SBCah(cpu_t* cpu) {  }
void SBCal(cpu_t* cpu) {  }
void SBCaa(cpu_t* cpu) {  }
void SBCan(cpu_t* cpu) {  }
void SBCamHL(cpu_t* cpu) {  }
// AND s
void ANDb(cpu_t* cpu) {  }
void ANDc(cpu_t* cpu) {  }
void ANDd(cpu_t* cpu) {  }
void ANDe(cpu_t* cpu) {  }
void ANDh(cpu_t* cpu) {  }
void ANDl(cpu_t* cpu) {  }
void ANDa(cpu_t* cpu) {  }
void ANDn(cpu_t* cpu) {  }
void ANDmHL(cpu_t* cpu) {  }
// OR s
void ORb(cpu_t* cpu) {  }
void ORc(cpu_t* cpu) {  }
void ORd(cpu_t* cpu) {  }
void ORe(cpu_t* cpu) {  }
void ORh(cpu_t* cpu) {  }
void ORl(cpu_t* cpu) {  }
void ORa(cpu_t* cpu) {  }
void ORn(cpu_t* cpu) {  }
void ORmHL(cpu_t* cpu) {  }
// XOR s
void XORb(cpu_t* cpu) {  }
void XORc(cpu_t* cpu) {  }
void XORd(cpu_t* cpu) {  }
void XORe(cpu_t* cpu) {  }
void XORh(cpu_t* cpu) {  }
void XORl(cpu_t* cpu) {  }
void XORa(cpu_t* cpu) {  }
void XORn(cpu_t* cpu) {  }
void XORmHL(cpu_t* cpu) {  }
// CP s
void CPb(cpu_t* cpu) {  }
void CPc(cpu_t* cpu) {  }
void CPd(cpu_t* cpu) {  }
void CPe(cpu_t* cpu) {  }
void CPh(cpu_t* cpu) {  }
void CPl(cpu_t* cpu) {  }
void CPa(cpu_t* cpu) {  }
void CPn(cpu_t* cpu) {  }
void CPmHL(cpu_t* cpu) {  }
// INC s
void INCb(cpu_t* cpu) {  }
void INCc(cpu_t* cpu) {  }
void INCd(cpu_t* cpu) {  }
void INCe(cpu_t* cpu) {  }
void INCh(cpu_t* cpu) {  }
void INCl(cpu_t* cpu) {  }
void INCa(cpu_t* cpu) {  }
void INCmHL(cpu_t* cpu) {  }
// DEC s
void DECb(cpu_t* cpu) {  }
void DECc(cpu_t* cpu) {  }
void DECd(cpu_t* cpu) {  }
void DECe(cpu_t* cpu) {  }
void DECh(cpu_t* cpu) {  }
void DECl(cpu_t* cpu) {  }
void DECa(cpu_t* cpu) {  }
void DECmHL(cpu_t* cpu) {  }

/* 16-bit arithmetic */
// ADD HL, ss
void ADDHLBC(cpu_t* cpu) {  }
void ADDHLDE(cpu_t* cpu) {  }
void ADDHLHL(cpu_t* cpu) {  }
void ADDHLSP(cpu_t* cpu) {  }
// ADD SP, e
void ADDSPn(cpu_t* cpu) {  }
// INC ss
void INCBC(cpu_t* cpu) {  }
void INCDE(cpu_t* cpu) {  }
void INCHL(cpu_t* cpu) {  }
void INCSP(cpu_t* cpu) {  }
// DEC ss
void DECBC(cpu_t* cpu) {  }
void DECDE(cpu_t* cpu) {  }
void DECHL(cpu_t* cpu) {  }
void DECSP(cpu_t* cpu) {  }

/* Misc */
// SWAP s
void SWAPb(cpu_t* cpu) {  }
void SWAPc(cpu_t* cpu) {  }
void SWAPd(cpu_t* cpu) {  }
void SWAPe(cpu_t* cpu) {  }
void SWAPh(cpu_t* cpu) {  }
void SWAPl(cpu_t* cpu) {  }
void SWAPa(cpu_t* cpu) {  }
void SWAPmHL(cpu_t* cpu) {  }
// DAA
void DAA(cpu_t* cpu) {  }
// CPL
void CPL(cpu_t* cpu) {  }
// CCF
void CCF(cpu_t* cpu) {  }
// SCF
void SCF(cpu_t* cpu) {  }
// NOP
void NOP(cpu_t* cpu) {  }
// HALT
void HALT(cpu_t* cpu) {  }
// STOP
void STOP(cpu_t* cpu) {  }
// DI
void DI(cpu_t* cpu) {  }
// EI
void EI(cpu_t* cpu) {  }

/* Rotates and shifts */
// RLCA
void RLCA(cpu_t* cpu) {  }
// RLA
void RLA(cpu_t* cpu) {  }
// RRCA
void RRCA(cpu_t* cpu) {  }
// RRA
void RRA(cpu_t* cpu) {  }
// RLC s
void RLCb(cpu_t* cpu) {  }
void RLCc(cpu_t* cpu) {  }
void RLCd(cpu_t* cpu) {  }
void RLCe(cpu_t* cpu) {  }
void RLCh(cpu_t* cpu) {  }
void RLCl(cpu_t* cpu) {  }
void RLCa(cpu_t* cpu) {  }
void RLCmHL(cpu_t* cpu) {  }
// RL s
void RLb(cpu_t* cpu) {  }
void RLc(cpu_t* cpu) {  }
void RLd(cpu_t* cpu) {  }
void RLe(cpu_t* cpu) {  }
void RLh(cpu_t* cpu) {  }
void RLl(cpu_t* cpu) {  }
void RLa(cpu_t* cpu) {  }
void RLmHL(cpu_t* cpu) {  }
// RRC s
void RRCb(cpu_t* cpu) {  }
void RRCc(cpu_t* cpu) {  }
void RRCd(cpu_t* cpu) {  }
void RRCe(cpu_t* cpu) {  }
void RRCh(cpu_t* cpu) {  }
void RRCl(cpu_t* cpu) {  }
void RRCa(cpu_t* cpu) {  }
void RRCmHL(cpu_t* cpu) {  }
// RR s
void RRb(cpu_t* cpu) {  }
void RRc(cpu_t* cpu) {  }
void RRd(cpu_t* cpu) {  }
void RRe(cpu_t* cpu) {  }
void RRh(cpu_t* cpu) {  }
void RRl(cpu_t* cpu) {  }
void RRa(cpu_t* cpu) {  }
void RRmHL(cpu_t* cpu) {  }
// SLA s
void SLAb(cpu_t* cpu) {  }
void SLAc(cpu_t* cpu) {  }
void SLAd(cpu_t* cpu) {  }
void SLAe(cpu_t* cpu) {  }
void SLAh(cpu_t* cpu) {  }
void SLAl(cpu_t* cpu) {  }
void SLAa(cpu_t* cpu) {  }
void SLAmHL(cpu_t* cpu) {  }
// SRA s
void SRAb(cpu_t* cpu) {  }
void SRAc(cpu_t* cpu) {  }
void SRAd(cpu_t* cpu) {  }
void SRAe(cpu_t* cpu) {  }
void SRAh(cpu_t* cpu) {  }
void SRAl(cpu_t* cpu) {  }
void SRAa(cpu_t* cpu) {  }
void SRAmHL(cpu_t* cpu) {  }
// SRL s
void SRLb(cpu_t* cpu) {  }
void SRLc(cpu_t* cpu) {  }
void SRLd(cpu_t* cpu) {  }
void SRLe(cpu_t* cpu) {  }
void SRLh(cpu_t* cpu) {  }
void SRLl(cpu_t* cpu) {  }
void SRLa(cpu_t* cpu) {  }
void SRLmHL(cpu_t* cpu) {  }

/* Bit manipulation */
// BIT b, s
void BIT0b(cpu_t* cpu) {  }
void BIT0c(cpu_t* cpu) {  }
void BIT0d(cpu_t* cpu) {  }
void BIT0e(cpu_t* cpu) {  }
void BIT0h(cpu_t* cpu) {  }
void BIT0l(cpu_t* cpu) {  }
void BIT0a(cpu_t* cpu) {  }
void BIT0mHL(cpu_t* cpu) {  }
void BIT1b(cpu_t* cpu) {  }
void BIT1c(cpu_t* cpu) {  }
void BIT1d(cpu_t* cpu) {  }
void BIT1e(cpu_t* cpu) {  }
void BIT1h(cpu_t* cpu) {  }
void BIT1l(cpu_t* cpu) {  }
void BIT1a(cpu_t* cpu) {  }
void BIT1mHL(cpu_t* cpu) {  }
void BIT2b(cpu_t* cpu) {  }
void BIT2c(cpu_t* cpu) {  }
void BIT2d(cpu_t* cpu) {  }
void BIT2e(cpu_t* cpu) {  }
void BIT2h(cpu_t* cpu) {  }
void BIT2l(cpu_t* cpu) {  }
void BIT2a(cpu_t* cpu) {  }
void BIT2mHL(cpu_t* cpu) {  }
void BIT3b(cpu_t* cpu) {  }
void BIT3c(cpu_t* cpu) {  }
void BIT3d(cpu_t* cpu) {  }
void BIT3e(cpu_t* cpu) {  }
void BIT3h(cpu_t* cpu) {  }
void BIT3l(cpu_t* cpu) {  }
void BIT3a(cpu_t* cpu) {  }
void BIT3mHL(cpu_t* cpu) {  }
void BIT4b(cpu_t* cpu) {  }
void BIT4c(cpu_t* cpu) {  }
void BIT4d(cpu_t* cpu) {  }
void BIT4e(cpu_t* cpu) {  }
void BIT4h(cpu_t* cpu) {  }
void BIT4l(cpu_t* cpu) {  }
void BIT4a(cpu_t* cpu) {  }
void BIT4mHL(cpu_t* cpu) {  }
void BIT5b(cpu_t* cpu) {  }
void BIT5c(cpu_t* cpu) {  }
void BIT5d(cpu_t* cpu) {  }
void BIT5e(cpu_t* cpu) {  }
void BIT5h(cpu_t* cpu) {  }
void BIT5l(cpu_t* cpu) {  }
void BIT5a(cpu_t* cpu) {  }
void BIT5mHL(cpu_t* cpu) {  }
void BIT6b(cpu_t* cpu) {  }
void BIT6c(cpu_t* cpu) {  }
void BIT6d(cpu_t* cpu) {  }
void BIT6e(cpu_t* cpu) {  }
void BIT6h(cpu_t* cpu) {  }
void BIT6l(cpu_t* cpu) {  }
void BIT6a(cpu_t* cpu) {  }
void BIT6mHL(cpu_t* cpu) {  }
void BIT7b(cpu_t* cpu) {  }
void BIT7c(cpu_t* cpu) {  }
void BIT7d(cpu_t* cpu) {  }
void BIT7e(cpu_t* cpu) {  }
void BIT7h(cpu_t* cpu) {  }
void BIT7l(cpu_t* cpu) {  }
void BIT7a(cpu_t* cpu) {  }
void BIT7mHL(cpu_t* cpu) {  }
// SET b, s
void SET0b(cpu_t* cpu) {  }
void SET0c(cpu_t* cpu) {  }
void SET0d(cpu_t* cpu) {  }
void SET0e(cpu_t* cpu) {  }
void SET0h(cpu_t* cpu) {  }
void SET0l(cpu_t* cpu) {  }
void SET0a(cpu_t* cpu) {  }
void SET0mHL(cpu_t* cpu) {  }
void SET1b(cpu_t* cpu) {  }
void SET1c(cpu_t* cpu) {  }
void SET1d(cpu_t* cpu) {  }
void SET1e(cpu_t* cpu) {  }
void SET1h(cpu_t* cpu) {  }
void SET1l(cpu_t* cpu) {  }
void SET1a(cpu_t* cpu) {  }
void SET1mHL(cpu_t* cpu) {  }
void SET2b(cpu_t* cpu) {  }
void SET2c(cpu_t* cpu) {  }
void SET2d(cpu_t* cpu) {  }
void SET2e(cpu_t* cpu) {  }
void SET2h(cpu_t* cpu) {  }
void SET2l(cpu_t* cpu) {  }
void SET2a(cpu_t* cpu) {  }
void SET2mHL(cpu_t* cpu) {  }
void SET3b(cpu_t* cpu) {  }
void SET3c(cpu_t* cpu) {  }
void SET3d(cpu_t* cpu) {  }
void SET3e(cpu_t* cpu) {  }
void SET3h(cpu_t* cpu) {  }
void SET3l(cpu_t* cpu) {  }
void SET3a(cpu_t* cpu) {  }
void SET3mHL(cpu_t* cpu) {  }
void SET4b(cpu_t* cpu) {  }
void SET4c(cpu_t* cpu) {  }
void SET4d(cpu_t* cpu) {  }
void SET4e(cpu_t* cpu) {  }
void SET4h(cpu_t* cpu) {  }
void SET4l(cpu_t* cpu) {  }
void SET4a(cpu_t* cpu) {  }
void SET4mHL(cpu_t* cpu) {  }
void SET5b(cpu_t* cpu) {  }
void SET5c(cpu_t* cpu) {  }
void SET5d(cpu_t* cpu) {  }
void SET5e(cpu_t* cpu) {  }
void SET5h(cpu_t* cpu) {  }
void SET5l(cpu_t* cpu) {  }
void SET5a(cpu_t* cpu) {  }
void SET5mHL(cpu_t* cpu) {  }
void SET6b(cpu_t* cpu) {  }
void SET6c(cpu_t* cpu) {  }
void SET6d(cpu_t* cpu) {  }
void SET6e(cpu_t* cpu) {  }
void SET6h(cpu_t* cpu) {  }
void SET6l(cpu_t* cpu) {  }
void SET6a(cpu_t* cpu) {  }
void SET6mHL(cpu_t* cpu) {  }
void SET7b(cpu_t* cpu) {  }
void SET7c(cpu_t* cpu) {  }
void SET7d(cpu_t* cpu) {  }
void SET7e(cpu_t* cpu) {  }
void SET7h(cpu_t* cpu) {  }
void SET7l(cpu_t* cpu) {  }
void SET7a(cpu_t* cpu) {  }
void SET7mHL(cpu_t* cpu) {  }
// RES b, s
void RES0b(cpu_t* cpu) {  }
void RES0c(cpu_t* cpu) {  }
void RES0d(cpu_t* cpu) {  }
void RES0e(cpu_t* cpu) {  }
void RES0h(cpu_t* cpu) {  }
void RES0l(cpu_t* cpu) {  }
void RES0a(cpu_t* cpu) {  }
void RES0mHL(cpu_t* cpu) {  }
void RES1b(cpu_t* cpu) {  }
void RES1c(cpu_t* cpu) {  }
void RES1d(cpu_t* cpu) {  }
void RES1e(cpu_t* cpu) {  }
void RES1h(cpu_t* cpu) {  }
void RES1l(cpu_t* cpu) {  }
void RES1a(cpu_t* cpu) {  }
void RES1mHL(cpu_t* cpu) {  }
void RES2b(cpu_t* cpu) {  }
void RES2c(cpu_t* cpu) {  }
void RES2d(cpu_t* cpu) {  }
void RES2e(cpu_t* cpu) {  }
void RES2h(cpu_t* cpu) {  }
void RES2l(cpu_t* cpu) {  }
void RES2a(cpu_t* cpu) {  }
void RES2mHL(cpu_t* cpu) {  }
void RES3b(cpu_t* cpu) {  }
void RES3c(cpu_t* cpu) {  }
void RES3d(cpu_t* cpu) {  }
void RES3e(cpu_t* cpu) {  }
void RES3h(cpu_t* cpu) {  }
void RES3l(cpu_t* cpu) {  }
void RES3a(cpu_t* cpu) {  }
void RES3mHL(cpu_t* cpu) {  }
void RES4b(cpu_t* cpu) {  }
void RES4c(cpu_t* cpu) {  }
void RES4d(cpu_t* cpu) {  }
void RES4e(cpu_t* cpu) {  }
void RES4h(cpu_t* cpu) {  }
void RES4l(cpu_t* cpu) {  }
void RES4a(cpu_t* cpu) {  }
void RES4mHL(cpu_t* cpu) {  }
void RES5b(cpu_t* cpu) {  }
void RES5c(cpu_t* cpu) {  }
void RES5d(cpu_t* cpu) {  }
void RES5e(cpu_t* cpu) {  }
void RES5h(cpu_t* cpu) {  }
void RES5l(cpu_t* cpu) {  }
void RES5a(cpu_t* cpu) {  }
void RES5mHL(cpu_t* cpu) {  }
void RES6b(cpu_t* cpu) {  }
void RES6c(cpu_t* cpu) {  }
void RES6d(cpu_t* cpu) {  }
void RES6e(cpu_t* cpu) {  }
void RES6h(cpu_t* cpu) {  }
void RES6l(cpu_t* cpu) {  }
void RES6a(cpu_t* cpu) {  }
void RES6mHL(cpu_t* cpu) {  }
void RES7b(cpu_t* cpu) {  }
void RES7c(cpu_t* cpu) {  }
void RES7d(cpu_t* cpu) {  }
void RES7e(cpu_t* cpu) {  }
void RES7h(cpu_t* cpu) {  }
void RES7l(cpu_t* cpu) {  }
void RES7a(cpu_t* cpu) {  }
void RES7mHL(cpu_t* cpu) {  }

/* Jumps */
// JP nn
void JPnn(cpu_t* cpu) {  }
// JP cc, nn
void JPZnn(cpu_t* cpu) {  }
void JPCnn(cpu_t* cpu) {  }
void JPNZnn(cpu_t* cpu) {  }
void JPNCnn(cpu_t* cpu) {  }
// JP (HL)
void JPmHL(cpu_t* cpu) {  }
// JR e
void JRn(cpu_t* cpu) {  }
// JR cc, e
void JRZn(cpu_t* cpu) {  }
void JRCn(cpu_t* cpu) {  }
void JRNZn(cpu_t* cpu) {  }
void JRNCn(cpu_t* cpu) {  }

/* Calls */
// CALL nn
void CALLnn(cpu_t* cpu) {  }
// CALL cc, nn
void CALLZnn(cpu_t* cpu) {  }
void CALLCnn(cpu_t* cpu) {  }
void CALLNZnn(cpu_t* cpu) {  }
void CALLNCnn(cpu_t* cpu) {  }

/* Restarts */
// RST f
void RST0(cpu_t* cpu) {  }
void RST8(cpu_t* cpu) {  }
void RST10(cpu_t* cpu) {  }
void RST18(cpu_t* cpu) {  }
void RST20(cpu_t* cpu) {  }
void RST28(cpu_t* cpu) {  }
void RST30(cpu_t* cpu) {  }
void RST38(cpu_t* cpu) {  }

/* Returns */
// RET
void RET(cpu_t* cpu) {  }
// RET cc
void RETZ(cpu_t* cpu) {  }
void RETC(cpu_t* cpu) {  }
void RETNZ(cpu_t* cpu) {  }
void RETNC(cpu_t* cpu) {  }
// RETI
void RETI(cpu_t* cpu) {  }

