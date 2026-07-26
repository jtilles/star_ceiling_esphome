typedef unsigned char   undefined;

typedef unsigned char    undefined1;
typedef unsigned long    undefined3;


undefined3 DAT_mem_08fd;
undefined LAB_code_000080;
undefined1 UNK_mem_0001;
char UNK_mem_0012;
undefined UNK_mem_001a;
undefined1 UNK_mem_001c;
undefined1 UNK_mem_001d;
undefined SREG;
undefined1 DAT_mem_0101;
undefined1 DAT_mem_0102;
byte DAT_mem_0103;
char DAT_mem_0104;
byte DAT_mem_0100;
undefined1 DAT_mem_0105;
undefined1 DAT_mem_0106;
byte DAT_mem_0107;
char DAT_mem_0108;
byte UNK_mem_0000;
char UNK_mem_001a;
char UNK_mem_001b;
undefined UNK_mem_0018;
undefined UNK_mem_0012;
byte UNK_mem_0018;
undefined1 TCCR2A;
undefined1 TCCR0A;
undefined1 TCCR1A;
byte UNK_mem_0001;
byte UNK_mem_001c;
byte UNK_mem_001d;
undefined UNK_mem_001e;
byte UNK_mem_0016;
uint UNK_mem_0012;
undefined2 UNK_mem_0010;
char UNK_mem_0001;
byte UNK_mem_0012;
char UNK_mem_0016;
undefined UNK_mem_001c;
char DAT_mem_0107;
undefined1 DAT_mem_0108;
byte UNK_mem_0013;
char UNK_mem_0014;
undefined UNK_mem_0016;
undefined2 UNK_mem_001a;
undefined1 TCNT0;
undefined TIFR0;
byte DAT_mem_010a;
byte DAT_mem_0109;
undefined1 TIMSK0;
byte DAT_codebyte_0000a8;
undefined UNK_mem_0004;
undefined UNK_mem_0006;
uint UNK_mem_0008;
undefined1 ADCSRA;
byte UNK_mem_000c;
byte UNK_mem_000d;
byte UNK_mem_000e;
byte UNK_mem_000f;
uint UNK_mem_0010;
undefined1 TCCR1B;
byte UNK_mem_0017;
char UNK_mem_0019;
undefined1 TCCR2B;
undefined UCSR0B;
undefined1 TCCR0B;
undefined EECR;
undefined1 EEDR;
undefined EEARL;
undefined EEARH;
undefined1 UNK_mem_0016;
undefined1 UNK_mem_0012;

void RESET(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void INT0(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void INT1(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void PCINT0(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void PCINT1(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void PCINT2(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void WDT(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void TIMER2_COMPA(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void TIMER2_COMPB(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void TIMER2_OVF(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void TIMER1_CAPT(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void TIMER1_COMPA(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void TIMER1_COMPB(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void TIMER1_OVF(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void TIMER0_COMPA(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void TIMER0_COMPB(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



uint TIMER0_OVF(uint R25R24,undefined2 param_2,undefined2 param_3,undefined2 R19R18)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined2 uVar3;
  uint uVar4;
  undefined1 uVar5;
  undefined1 uVar6;
  char in_Cflg;
  bool bVar7;
  char in_Zflg;
  char in_Nflg;
  char in_Vflg;
  char in_Sflg;
  char in_Hflg;
  char in_Tflg;
  char in_Iflg;
  
  uVar6 = Xhi;
  uVar5 = Xlo;
  uVar3 = R25R24;
  uVar1 = R0;
  uVar2 = (byte)R19R18;
  R0 = in_Cflg == '\x01' | (in_Zflg == '\x01') << 1 | (in_Nflg == '\x01') << 2 |
       (in_Vflg == '\x01') << 3 | (in_Sflg == '\x01') << 4 | (in_Hflg == '\x01') << 5 |
       (in_Tflg == '\x01') << 6 | (in_Iflg == '\x01') << 7;
  SREG = R0;
  R25R24 = CONCAT11(DAT_mem_0102,DAT_mem_0101);
  uVar4 = R25R24;
  R19R18._0_1_ = DAT_mem_0100 + 3;
  if ((byte)R19R18 < 0x7d) {
    R25R24 = R25R24 + 1;
    bVar7 = 0xfffe < uVar4;
  }
  else {
    R19R18._0_1_ = DAT_mem_0100 + 0x86;
    R25R24 = R25R24 + 2;
    bVar7 = 0xfffd < uVar4;
  }
  Xhi = DAT_mem_0104 + CARRY1(DAT_mem_0103,bVar7);
  Xlo = DAT_mem_0103 + bVar7;
  DAT_mem_0100 = (byte)R19R18;
  DAT_mem_0101 = (undefined1)R25R24;
  DAT_mem_0102 = R25R24._1_1_;
  DAT_mem_0103 = Xlo;
  DAT_mem_0104 = Xhi;
  R25R24 = CONCAT11(DAT_mem_0106,DAT_mem_0105);
  uVar4 = R25R24;
  R25R24 = R25R24 + 1;
  bVar7 = CARRY1(DAT_mem_0107,0xfffe < uVar4);
  DAT_mem_0105 = (undefined1)R25R24;
  DAT_mem_0106 = R25R24._1_1_;
  DAT_mem_0107 = DAT_mem_0107 + (0xfffe < uVar4);
  DAT_mem_0108 = DAT_mem_0108 + bVar7;
  Xhi = uVar6;
  Xlo = uVar5;
  R25R24 = uVar3;
  R19R18._0_1_ = uVar2;
  SREG = R0;
  R0 = uVar1;
  return R25R24;
}



void SPI_STC(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void USART_RX(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void USART_UDRE(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void USART_TX(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void ADC(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void EE_READY(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void ANALOG_COMP(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void TWI(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void SPM_READY(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void FUN_code_000061(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



void RESET(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  Ylo = 0xff;
  Yhi = 8;
  R18 = '\x01';
  X = &DAT_mem_0100;
  while (puVar1 = X, (byte)X != 0xb || X._1_1_ != (char)(R18 + ((byte)X < 0xb))) {
    X = X + 1;
    *puVar1 = R1;
  }
  DAT_mem_08fd = 0x71;
  FUN_code_00016c();
  FUN_code_000263();
  return;
}



byte FUN_code_000075(byte R24)

{
  if (R24 == 3) {
    R24 = TCCR1A;
    R24 = R24 & 0x7f;
LAB_code_00008a:
    TCCR1A = R24;
    return R24;
  }
  if (R24 < 3) {
    if (R24 == 1) {
      R24 = TCCR0A;
      R24 = R24 & 0x7f;
    }
    else {
      if (R24 != 2) {
        return R24;
      }
      R24 = TCCR0A;
      R24 = R24 & 0xdf;
    }
    TCCR0A = R24;
    return R24;
  }
  if (R24 == 7) {
    R24 = TCCR2A;
    R24 = R24 & 0x7f;
  }
  else {
    if (R24 != 8) {
      if (R24 != 4) {
        return R24;
      }
      R24 = TCCR1A;
      R24 = R24 & 0xdf;
      goto LAB_code_00008a;
    }
    R24 = TCCR2A;
    R24 = R24 & 0xdf;
  }
  TCCR2A = R24;
  return R24;
}



byte FUN_code_00009e(byte R24,byte R22,undefined2 param_3,undefined2 R19R18,undefined2 R17R16)

{
  char cVar1;
  char cVar2;
  undefined1 *puVar3;
  byte bVar4;
  byte bVar5;
  undefined1 uVar6;
  undefined1 uVar7;
  bool bVar8;
  bool bVar9;
  bool bVar10;
  char in_Hflg;
  char in_Tflg;
  char in_Iflg;
  
  uVar7 = Yhi;
  uVar6 = Ylo;
  bVar5 = R24;
  bVar4 = R22;
  R19R18 = (uint)R24;
  Z = CONCAT11(-((R24 < 0x52) + -1),R24 + 0xae);
  R24 = *(byte *)(uint3)Z;
  Z = CONCAT11(-((bVar5 < 0x66) + -1),bVar5 + 0x9a);
  Yhi = *(byte *)(uint3)Z;
  Z = CONCAT11(-((bVar5 < 0x7a) + -1),bVar5 + 0x86);
  Ylo = *(byte *)(uint3)Z;
  if (Ylo != 0) {
    if (R24 != R1) {
      FUN_code_000075();
    }
    bVar8 = CARRY1(Ylo,Ylo);
    cVar1 = Ylo * '\x02' + 0x72;
    bVar9 = (byte)(Ylo * '\x02') < 0x8e;
    cVar2 = bVar8 - (bVar9 + -1);
    bVar10 = SBORROW1(bVar8,-1) != SBORROW1(bVar8 + 1U,bVar9);
    Z = CONCAT11(cVar2,cVar1);
    puVar3 = (undefined1 *)(uint3)Z;
    X._0_1_ = *puVar3;
    Z = Z + 1;
    X._1_1_ = *(undefined1 *)(uint3)Z;
    R24 = (bVar8 != true || (byte)(bVar8 + 1U) < bVar9) | (cVar1 == '\0' && cVar2 == '\0') << 1 |
          (cVar2 < '\0') << 2 | bVar10 << 3 | (cVar2 < '\0' != bVar10) << 4 |
          (in_Hflg == '\x01') << 5 | (in_Tflg == '\x01') << 6 | (in_Iflg == '\x01') << 7;
    SREG = R24;
    Z._0_1_ = *X;
    if (bVar4 == R1) {
      Yhi = ~Yhi;
      Yhi = Yhi & (byte)Z;
    }
    else {
      Yhi = Yhi | (byte)Z;
    }
    *X = Yhi;
    SREG = R24;
  }
  Yhi = uVar7;
  Ylo = uVar6;
  return R24;
}



uint FUN_code_0000cc(undefined2 R25R24,char R22)

{
  char cVar1;
  undefined1 *puVar2;
  byte bVar3;
  undefined2 uVar4;
  bool bVar5;
  bool bVar6;
  bool bVar7;
  bool bVar8;
  bool bVar9;
  char in_Hflg;
  char in_Tflg;
  char in_Iflg;
  
  uVar4 = Y;
  Z = CONCAT11(-(((byte)R25R24 < 0x66) + -1),(byte)R25R24 + 0x9a);
  R18 = *(byte *)(uint3)Z;
  cVar1 = -(((byte)R25R24 < 0x7a) + -1);
  R25R24 = CONCAT11(cVar1,(byte)R25R24 + 0x86);
  Z = R25R24;
  R25R24._0_1_ = *(byte *)(uint3)R25R24;
  R25R24 = CONCAT11(cVar1,(byte)R25R24);
  if ((byte)R25R24 != 0) {
    bVar3 = (byte)R25R24 * '\x02';
    bVar5 = CARRY1((byte)R25R24,(byte)R25R24);
    Z = CONCAT11(bVar5 - ((bVar3 < 0x98) + -1),bVar3 + 0x68);
    puVar2 = (undefined1 *)(uint3)Z;
    X._0_1_ = *puVar2;
    Z = Z + 1;
    X._1_1_ = *(undefined1 *)(uint3)Z;
    bVar6 = bVar3 < 0x8e;
    cVar1 = bVar5 - (bVar6 + -1);
    bVar9 = SBORROW1(bVar5,-1) != SBORROW1(bVar5 + 1U,bVar6);
    bVar6 = bVar5 != true || (byte)(bVar5 + 1U) < bVar6;
    bVar8 = cVar1 < '\0';
    bVar7 = (byte)(bVar3 + 0x72) == '\0' && cVar1 == '\0';
    Z = CONCAT11(cVar1,bVar3 + 0x72);
    puVar2 = (undefined1 *)(uint3)Z;
    Y._0_1_ = *puVar2;
    Z = Z + 1;
    Y._1_1_ = *(undefined1 *)(uint3)Z;
    if (R22 == R1) {
      R25R24._1_1_ = bVar6 | bVar7 << 1 | bVar8 << 2 | bVar9 << 3 | (bVar8 != bVar9) << 4 |
                     (in_Hflg == '\x01') << 5 | (in_Tflg == '\x01') << 6 | (in_Iflg == '\x01') << 7;
      SREG = R25R24._1_1_;
      R25R24._0_1_ = *X;
      Z._0_1_ = ~R18;
      R25R24._0_1_ = (byte)R25R24 & (byte)Z;
      *X = (byte)R25R24;
      R18 = *Y;
      Z._0_1_ = (byte)Z & R18;
      *Y = (byte)Z;
      SREG = R25R24._1_1_;
    }
    else {
      R25R24._0_1_ = bVar6 | bVar7 << 1 | bVar8 << 2 | bVar9 << 3 | (bVar8 != bVar9) << 4 |
                     (in_Hflg == '\x01') << 5 | (in_Tflg == '\x01') << 6 | (in_Iflg == '\x01') << 7;
      R25R24 = CONCAT11(bVar5,(byte)R25R24);
      SREG = (byte)R25R24;
      Z._0_1_ = *X;
      Z._0_1_ = (byte)Z | R18;
      *X = (byte)Z;
      SREG = (byte)R25R24;
    }
  }
  Y = (byte *)uVar4;
  return R25R24;
}



undefined4 FUN_code_0000fd(void)

{
  byte bVar1;
  char cVar2;
  uint uVar3;
  char in_Cflg;
  bool bVar4;
  char in_Zflg;
  char in_Nflg;
  char in_Vflg;
  char in_Sflg;
  char in_Hflg;
  char in_Tflg;
  char in_Iflg;
  
  R19 = in_Cflg == '\x01' | (in_Zflg == '\x01') << 1 | (in_Nflg == '\x01') << 2 |
        (in_Vflg == '\x01') << 3 | (in_Sflg == '\x01') << 4 | (in_Hflg == '\x01') << 5 |
        (in_Tflg == '\x01') << 6 | (in_Iflg == '\x01') << 7;
  SREG = R19;
  R25R24 = CONCAT11(DAT_mem_0106,DAT_mem_0105);
  uVar3 = R25R24;
  R18 = TCNT0;
  bVar1 = TIFR0;
  cVar2 = DAT_mem_0107;
  if (((bVar1 & 1) != 0) && (R18 != -1)) {
    R25R24 = R25R24 + 1;
    cVar2 = DAT_mem_0107 + R1 + (0xfffe < uVar3);
  }
  SREG = R19;
  X = CONCAT11(cVar2,R25R24._1_1_);
  R23R22 = CONCAT11((byte)R25R24 + R1,R18);
  R25R24 = CONCAT11(cVar2 + R1 +
                    (CARRY1(R25R24._1_1_,R1) || CARRY1(R25R24._1_1_ + R1,CARRY1((byte)R25R24,R1))),
                    R25R24._1_1_ + R1 + CARRY1((byte)R25R24,R1));
  R20 = '\x02';
  do {
    bVar4 = CARRY1(R23R22._1_1_,R23R22._1_1_) ||
            CARRY1(R23R22._1_1_ * '\x02',CARRY1((byte)R23R22,(byte)R23R22));
    R23R22 = CONCAT11(R23R22._1_1_ * '\x02' + CARRY1((byte)R23R22,(byte)R23R22),
                      (byte)R23R22 * '\x02');
    R25R24 = CONCAT11(R25R24._1_1_ * '\x02' +
                      (CARRY1((byte)R25R24,(byte)R25R24) || CARRY1((byte)R25R24 * '\x02',bVar4)),
                      (byte)R25R24 * '\x02' + bVar4);
    R20 = R20 + -1;
  } while (R20 != '\0');
  R20 = 0;
  return CONCAT22(R25R24,R23R22);
}



uint FUN_code_000122(uint R25R24,undefined2 param_2,undefined2 param_3,undefined2 R19R18)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined2 uVar3;
  uint uVar4;
  undefined1 uVar5;
  undefined1 uVar6;
  char in_Cflg;
  bool bVar7;
  char in_Zflg;
  char in_Nflg;
  char in_Vflg;
  char in_Sflg;
  char in_Hflg;
  char in_Tflg;
  char in_Iflg;
  
  uVar6 = Xhi;
  uVar5 = Xlo;
  uVar3 = R25R24;
  uVar1 = R0;
  uVar2 = (byte)R19R18;
  R0 = in_Cflg == '\x01' | (in_Zflg == '\x01') << 1 | (in_Nflg == '\x01') << 2 |
       (in_Vflg == '\x01') << 3 | (in_Sflg == '\x01') << 4 | (in_Hflg == '\x01') << 5 |
       (in_Tflg == '\x01') << 6 | (in_Iflg == '\x01') << 7;
  SREG = R0;
  R25R24 = CONCAT11(DAT_mem_0102,DAT_mem_0101);
  uVar4 = R25R24;
  R19R18._0_1_ = DAT_mem_0100 + 3;
  if ((byte)R19R18 < 0x7d) {
    R25R24 = R25R24 + 1;
    bVar7 = 0xfffe < uVar4;
  }
  else {
    R19R18._0_1_ = DAT_mem_0100 + 0x86;
    R25R24 = R25R24 + 2;
    bVar7 = 0xfffd < uVar4;
  }
  Xhi = DAT_mem_0104 + CARRY1(DAT_mem_0103,bVar7);
  Xlo = DAT_mem_0103 + bVar7;
  DAT_mem_0100 = (byte)R19R18;
  DAT_mem_0101 = (undefined1)R25R24;
  DAT_mem_0102 = R25R24._1_1_;
  DAT_mem_0103 = Xlo;
  DAT_mem_0104 = Xhi;
  R25R24 = CONCAT11(DAT_mem_0106,DAT_mem_0105);
  uVar4 = R25R24;
  R25R24 = R25R24 + 1;
  bVar7 = CARRY1(DAT_mem_0107,0xfffe < uVar4);
  DAT_mem_0105 = (undefined1)R25R24;
  DAT_mem_0106 = R25R24._1_1_;
  DAT_mem_0107 = DAT_mem_0107 + (0xfffe < uVar4);
  DAT_mem_0108 = DAT_mem_0108 + bVar7;
  Xhi = uVar6;
  Xlo = uVar5;
  R25R24 = uVar3;
  R19R18._0_1_ = uVar2;
  SREG = R0;
  R0 = uVar1;
  return R25R24;
}



void FUN_code_00016c(void)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  undefined1 *puVar4;
  bool bVar5;
  undefined4 uVar6;
  
  R24 = TCCR0A;
  R24 = R24 | 2;
  TCCR0A = R24;
  R24 = TCCR0A;
  R24 = R24 | 1;
  TCCR0A = R24;
  R24 = TCCR0B;
  R24 = R24 | 2;
  TCCR0B = R24;
  R24 = TCCR0B;
  R24 = R24 | 1;
  TCCR0B = R24;
  R24 = TIMSK0;
  R24 = R24 | 1;
  TIMSK0 = R24;
  TCCR1B = R1;
  R24 = TCCR1B;
  R24 = R24 | 2;
  TCCR1B = R24;
  R24 = TCCR1B;
  R24 = R24 | 1;
  TCCR1B = R24;
  R24 = TCCR1A;
  R24 = R24 | 1;
  TCCR1A = R24;
  R24 = TCCR2B;
  R24 = R24 | 4;
  TCCR2B = R24;
  R24 = TCCR2A;
  R24 = R24 | 1;
  TCCR2A = R24;
  R24 = ADCSRA;
  R24 = R24 | 4;
  ADCSRA = R24;
  R24 = ADCSRA;
  R24 = R24 | 2;
  ADCSRA = R24;
  R24 = ADCSRA;
  R24 = R24 | 1;
  ADCSRA = R24;
  R24 = ADCSRA;
  R24 = R24 | 0x80;
  ADCSRA = R24;
  UCSR0B = R1;
  FUN_code_0000cc();
  FUN_code_0000cc();
  FUN_code_0000cc();
  FUN_code_00024d();
  DAT_mem_010a = 0;
  DAT_mem_0109 = R24;
  if (R24 == 0) {
    FUN_code_00009e();
  }
  else {
    if (R24 != 1) goto LAB_code_0001cf;
    FUN_code_00009e();
  }
  FUN_code_00009e();
LAB_code_0001cf:
  R9R8 = 0xbc;
  R17R16 = 0x94;
  do {
    R14 = DAT_codebyte_0000a8;
    R24 = *(byte *)(uint3)R9R8;
    Z = R17R16;
    R15 = *(byte *)(uint3)R17R16;
    if (R15 != 0) {
      if (R24 != R1) {
        FUN_code_000075();
      }
      Z = CONCAT11(CARRY1(R15,R15) - (((byte)(R15 * '\x02') < 0x84) + -1),R15 * '\x02' + 0x7c);
      puVar4 = (undefined1 *)(uint3)Z;
      X._0_1_ = *puVar4;
      Z = Z + 1;
      X._1_1_ = *(undefined1 *)(uint3)Z;
      R24 = *X;
      R14 = R14 & R24;
      if (R14 != 0) {
        if (DAT_mem_0109 == 0 && DAT_mem_010a == 0) {
          DAT_mem_010a = 0;
          DAT_mem_0109 = 1;
          FUN_code_00009e();
        }
        else {
          DAT_mem_010a = R1;
          DAT_mem_0109 = R1;
          FUN_code_00009e();
        }
        FUN_code_00009e();
        _R5R4 = FUN_code_0000fd();
        R12 = 0xe8;
        R13 = 3;
        R14 = R1;
        R15 = R1;
        do {
          do {
            uVar6 = FUN_code_0000fd();
            R22 = (byte)uVar6;
            R23 = (byte)((ulong)uVar6 >> 8);
            bVar1 = R23 - (R5R4._1_1_ + (R22 < (byte)R5R4));
            bVar5 = R23 < R5R4._1_1_ || (byte)(R23 - R5R4._1_1_) < (R22 < (byte)R5R4);
            R24 = (byte)((ulong)uVar6 >> 0x10);
            bVar2 = R24 - ((byte)R7R6 + bVar5);
            R25 = (char)((ulong)uVar6 >> 0x18);
            bVar3 = R25 - (R7R6._1_1_ + (R24 < (byte)R7R6 || (byte)(R24 - (byte)R7R6) < bVar5));
          } while (bVar3 < R1 ||
                   (byte)(bVar3 - R1) <
                   (bVar2 < R1 ||
                   (byte)(bVar2 - R1) <
                   (bVar1 < 3 || (byte)(bVar1 - 3) < ((byte)(R22 - (byte)R5R4) < 0xe8))));
          Z = CONCAT11(1,(undefined1)Z);
          bVar1 = R12 - 1;
          bVar2 = R13 - (R1 + (R12 == 0));
          bVar5 = R13 < R1 || (byte)(R13 - R1) < (R12 == 0);
          bVar3 = R14 - (R1 + bVar5);
          R15 = R15 - (R1 + (R14 < R1 || (byte)(R14 - R1) < bVar5));
          bVar5 = 0xfc < R5R4._1_1_ || CARRY1(R5R4._1_1_ + 3,0x17 < (byte)R5R4);
          R5R4 = CONCAT11(R5R4._1_1_ + 3 + (0x17 < (byte)R5R4),(byte)R5R4 - 0x18);
          R7R6 = CONCAT11(R7R6._1_1_ + R1 + (CARRY1((byte)R7R6,R1) || CARRY1((byte)R7R6 + R1,bVar5))
                          ,(byte)R7R6 + R1 + bVar5);
          bVar5 = bVar2 < R1 || (byte)(bVar2 - R1) < (bVar1 < R1);
          R12 = bVar1;
          R13 = bVar2;
          R14 = bVar3;
        } while (((bVar1 != R1 || bVar2 != (byte)(R1 + (bVar1 < R1))) || bVar3 != (byte)(R1 + bVar5)
                 ) || R15 != (byte)(R1 + (bVar3 < R1 || (byte)(bVar3 - R1) < bVar5)));
        FUN_code_000255();
      }
    }
    if (R1 != 0 || (byte)(R1 + (R1 != 0)) != '\0') {
      RESET();
    }
  } while( true );
}



undefined1 FUN_code_00024d(undefined2 R25R24)

{
  byte bVar1;
  
  do {
    bVar1 = EECR;
  } while ((bVar1 & 2) != 0);
  EEARH = R25R24._1_1_;
  EEARL = (undefined1)R25R24;
  bVar1 = EECR;
  EECR = bVar1 | 1;
  R25R24._1_1_ = 0;
  R25R24._0_1_ = EEDR;
  return (undefined1)R25R24;
}



int FUN_code_000255(int R25R24,undefined1 R22)

{
  byte bVar1;
  char in_Cflg;
  char in_Zflg;
  char in_Nflg;
  char in_Vflg;
  char in_Sflg;
  char in_Hflg;
  char in_Tflg;
  char in_Iflg;
  
  R18 = R22;
  do {
    bVar1 = EECR;
  } while ((bVar1 & 2) != 0);
  EECR = R1;
  EEARH = R25R24._1_1_;
  EEARL = (undefined1)R25R24;
  EEDR = R22;
  R0 = in_Cflg == '\x01' | (in_Zflg == '\x01') << 1 | (in_Nflg == '\x01') << 2 |
       (in_Vflg == '\x01') << 3 | (in_Sflg == '\x01') << 4 | (in_Hflg == '\x01') << 5 |
       (in_Tflg == '\x01') << 6 | (in_Iflg == '\x01') << 7;
  SREG = R0;
  bVar1 = EECR;
  EECR = bVar1 | 4;
  bVar1 = EECR;
  EECR = bVar1 | 2;
  SREG = R0;
  R25R24 = R25R24 + 1;
  return R25R24;
}



void FUN_code_000263(void)

{
  do {
                    // WARNING: Do nothing block with infinite loop
  } while( true );
}


