typedef unsigned char   undefined;

typedef unsigned char    undefined1;
typedef unsigned long    undefined3;


undefined3 DAT_mem_08fd;
undefined LAB_code_000080;
undefined LAB_code_00008c;
undefined1 UNK_mem_0000;
undefined1 UNK_mem_0001;
char UNK_mem_0011;
char UNK_mem_0012;
undefined UNK_mem_001a;
undefined SREG;
undefined UNK_mem_001c;
uint UNK_mem_001e;
undefined1 DAT_mem_011d;
undefined1 DAT_mem_011e;
byte DAT_mem_011f;
char DAT_mem_0120;
byte DAT_mem_011c;
undefined1 DAT_mem_0118;
undefined1 DAT_mem_0119;
byte DAT_mem_011a;
char DAT_mem_011b;
byte UNK_mem_0000;
char UNK_mem_001a;
char UNK_mem_001b;
undefined UNK_mem_0018;
undefined UNK_mem_0012;
undefined1 DAT_mem_0159;
undefined1 UNK_mem_0012;
undefined1 DAT_mem_015a;
undefined1 DAT_mem_015f;
undefined1 DAT_mem_0160;
byte DAT_mem_0162;
byte DAT_mem_0163;
byte *UNK_mem_001e;
undefined UNK_mem_0016;
undefined UNK_mem_0014;
undefined1 UNK_mem_001a;
undefined1 UNK_mem_001b;
undefined1 UNK_mem_001e;
undefined1 UNK_mem_001f;
byte UNK_mem_0012;
undefined UNK_mem_001e;
int UNK_mem_001a;
int UNK_mem_001c;
undefined2 *UNK_mem_0018;
int UNK_mem_0016;
undefined UNK_mem_0010;
undefined UNK_mem_000e;
undefined2 *UNK_mem_000c;
undefined UNK_mem_000a;
char UNK_mem_0001;
int UNK_mem_001e;
uint UNK_mem_0018;
bool UNK_mem_0012;
byte *UNK_mem_001a;
undefined1 *UNK_mem_001e;
undefined1 UNK_mem_0016;
char UNK_mem_0016;
byte UNK_mem_0013;
undefined1 TCCR2A;
undefined1 TCCR0A;
undefined1 TCCR1A;
byte UNK_mem_0018;
undefined2 UNK_mem_0010;
undefined OCR0A;
undefined OCR0B;
undefined OCR1AL;
undefined OCR1AH;
undefined OCR1BL;
undefined OCR1BH;
undefined OCR2A;
undefined OCR2B;
undefined1 DAT_mem_011f;
undefined1 DAT_mem_0120;
undefined1 UNK_mem_0018;
undefined1 UNK_mem_0019;
byte DAT_mem_0151;
byte DAT_mem_0152;
byte DAT_mem_0153;
char DAT_mem_0154;
byte DAT_mem_014d;
byte DAT_mem_014e;
byte DAT_mem_014f;
byte DAT_mem_0150;
byte UNK_mem_0016;
byte UNK_mem_0017;
byte UNK_mem_0001;
undefined UNK_mem_000c;
undefined2 UNK_mem_0012;
undefined2 UNK_mem_0014;
undefined2 UNK_mem_0016;
undefined1 UNK_mem_001c;
undefined EECR;
undefined1 EEDR;
undefined EEARL;
undefined EEARH;
undefined1 DAT_mem_0139;
undefined1 UNK_mem_0017;
undefined1 DAT_mem_013f;
undefined1 DAT_mem_013e;
undefined1 DAT_mem_0140;
undefined1 TIMSK0;
undefined1 DAT_mem_0155;
undefined1 DAT_mem_0156;
undefined1 DAT_mem_0157;
undefined1 ADCL;
undefined1 DAT_mem_0158;
undefined1 ADCH;
byte DAT_mem_0161;
undefined1 ADCSRA;
undefined1 DAT_mem_015d;
undefined1 DAT_mem_015e;
undefined ADMUX;
undefined1 DAT_mem_015b;
undefined1 DAT_mem_015c;
byte DAT_mem_0103;
byte DAT_mem_0102;
undefined1 TCCR1B;
char DAT_mem_0148;
char DAT_mem_0147;
char DAT_mem_013d;
char DAT_mem_013c;
byte DAT_mem_0101;
byte DAT_mem_0100;
byte DAT_mem_0143;
char DAT_mem_0144;
undefined1 DAT_mem_0145;
undefined1 DAT_mem_0146;
byte DAT_mem_0141;
undefined1 TCCR2B;
byte DAT_mem_0142;
byte DAT_mem_0140;
byte DAT_mem_013f;
byte DAT_mem_013e;
char DAT_mem_013a;
char DAT_mem_013b;
byte DAT_mem_0137;
byte DAT_mem_0138;
byte DAT_mem_0136;
byte DAT_mem_0135;
byte DAT_mem_0134;
undefined UCSR0B;
byte DAT_mem_0133;
byte DAT_mem_0132;
byte DAT_mem_0131;
byte DAT_mem_0130;
byte DAT_mem_012f;
byte DAT_mem_012e;
byte DAT_mem_012d;
byte DAT_mem_012c;
byte DAT_mem_012b;
byte DAT_mem_012a;
byte DAT_mem_0129;
undefined1 DAT_mem_0127;
undefined1 DAT_mem_0128;
undefined1 DAT_mem_0126;
undefined1 DAT_mem_0125;
undefined1 DAT_mem_0124;
undefined1 DAT_mem_0123;
undefined1 DAT_mem_0122;
undefined1 DAT_mem_0121;
undefined UNK_mem_0000;
undefined UNK_mem_0004;
undefined UNK_mem_0008;
undefined2 UNK_mem_000a;
uint UNK_mem_000c;
undefined1 TCCR0B;
undefined2 UNK_mem_0008;
undefined1 *UNK_mem_001c;
undefined1 DAT_mem_0104;
undefined1 DAT_mem_0105;
byte DAT_mem_0106;
byte DAT_mem_0107;
undefined1 DAT_mem_0106;
undefined1 DAT_mem_0107;
undefined2 UNK_mem_0018;
byte UNK_mem_0014;
char UNK_mem_0015;
byte UNK_mem_001a;
char UNK_mem_0013;
byte UNK_mem_001b;

void RESET(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void INT0(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void INT1(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void PCINT0(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void PCINT1(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void PCINT2(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void WDT(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void TIMER2_COMPA(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void TIMER2_COMPB(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void TIMER2_OVF(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void TIMER1_CAPT(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void TIMER1_COMPA(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void TIMER1_COMPB(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void TIMER1_OVF(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void TIMER0_COMPA(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void TIMER0_COMPB(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
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
  R25R24 = CONCAT11(DAT_mem_011e,DAT_mem_011d);
  uVar4 = R25R24;
  R19R18._0_1_ = DAT_mem_011c + 3;
  if ((byte)R19R18 < 0x7d) {
    R25R24 = R25R24 + 1;
    bVar7 = 0xfffe < uVar4;
  }
  else {
    R19R18._0_1_ = DAT_mem_011c + 0x86;
    R25R24 = R25R24 + 2;
    bVar7 = 0xfffd < uVar4;
  }
  Xhi = DAT_mem_0120 + CARRY1(DAT_mem_011f,bVar7);
  Xlo = DAT_mem_011f + bVar7;
  DAT_mem_011c = (byte)R19R18;
  DAT_mem_011d = (undefined1)R25R24;
  DAT_mem_011e = R25R24._1_1_;
  DAT_mem_011f = Xlo;
  DAT_mem_0120 = Xhi;
  R25R24 = CONCAT11(DAT_mem_0119,DAT_mem_0118);
  uVar4 = R25R24;
  R25R24 = R25R24 + 1;
  bVar7 = CARRY1(DAT_mem_011a,0xfffe < uVar4);
  DAT_mem_0118 = (undefined1)R25R24;
  DAT_mem_0119 = R25R24._1_1_;
  DAT_mem_011a = DAT_mem_011a + (0xfffe < uVar4);
  DAT_mem_011b = DAT_mem_011b + bVar7;
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
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



undefined2 USART_RX(undefined2 R25R24,undefined2 param_2,undefined2 param_3,undefined1 R18)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined2 uVar3;
  undefined1 uVar4;
  undefined2 uVar5;
  char in_Cflg;
  char in_Zflg;
  char in_Nflg;
  char in_Vflg;
  char in_Sflg;
  char in_Hflg;
  char in_Tflg;
  char in_Iflg;
  
  uVar5 = Z;
  uVar3 = R25R24;
  uVar1 = R0;
  uVar4 = R25R24._1_1_;
  uVar2 = (byte)R25R24;
  R0 = in_Cflg == '\x01' | (in_Zflg == '\x01') << 1 | (in_Nflg == '\x01') << 2 |
       (in_Vflg == '\x01') << 3 | (in_Sflg == '\x01') << 4 | (in_Hflg == '\x01') << 5 |
       (in_Tflg == '\x01') << 6 | (in_Iflg == '\x01') << 7;
  SREG = R0;
  Z = (byte *)CONCAT11(DAT_mem_015a,DAT_mem_0159);
  R25R24._0_1_ = *Z;
  Z = (byte *)CONCAT11(DAT_mem_0160,DAT_mem_015f);
  if (((byte)R25R24 & 4) == 0) {
    R25R24._1_1_ = *Z;
    R25R24._0_1_ = DAT_mem_0162 + 1 & 0x3f;
    if ((byte)R25R24 != DAT_mem_0163) {
      Z = (byte *)CONCAT11(-((DAT_mem_0162 < 0xb7) + -2),DAT_mem_0162 + 0x49);
      *(undefined1 *)((int)Z + 0x1d) = R25R24._1_1_;
      DAT_mem_0162 = (byte)R25R24;
    }
  }
  Z = (byte *)uVar5;
  R25R24._1_1_ = uVar4;
  R25R24._0_1_ = uVar2;
  SREG = R0;
  R0 = uVar1;
  R25R24 = uVar3;
  return R25R24;
}



undefined8 USART_UDRE(undefined2 R25R24,undefined2 R23R22,undefined2 R21R20,undefined2 R19R18)

{
  undefined1 uVar1;
  char in_Cflg;
  char in_Zflg;
  char in_Nflg;
  char in_Vflg;
  char in_Sflg;
  char in_Hflg;
  char in_Tflg;
  char in_Iflg;
  
  uVar1 = R0;
  R0 = in_Cflg == '\x01' | (in_Zflg == '\x01') << 1 | (in_Nflg == '\x01') << 2 |
       (in_Vflg == '\x01') << 3 | (in_Sflg == '\x01') << 4 | (in_Hflg == '\x01') << 5 |
       (in_Tflg == '\x01') << 6 | (in_Iflg == '\x01') << 7;
  SREG = R0;
  FUN_code_000115(0x49);
  SREG = R0;
  R0 = uVar1;
  _R19R18 = CONCAT17(R25R24._1_1_,
                     CONCAT16((undefined1)R25R24,
                              CONCAT15(R23R22._1_1_,
                                       CONCAT14((undefined1)R23R22,
                                                CONCAT13(R21R20._1_1_,
                                                         CONCAT12((undefined1)R21R20,R19R18))))));
  return _R19R18;
}



void USART_TX(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void ADC(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void EE_READY(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void ANALOG_COMP(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void TWI(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void SPM_READY(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void FUN_code_00005d(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



void RESET(void)

{
  undefined1 *puVar1;
  
  R1 = 0;
  SREG = 0;
  R17 = '\x01';
  X = &DAT_mem_0100;
  Z = 0x12e6;
  while (puVar1 = X, (byte)X != 0x18 || X._1_1_ != (char)(R17 + ((byte)X < 0x18))) {
    R0 = *(undefined1 *)(uint3)Z;
    Z = Z + 1;
    X = X + 1;
    *puVar1 = R0;
  }
  R18 = '\x01';
  X = &DAT_mem_0118;
  while (puVar1 = X, (byte)X != 0xe6 || X._1_1_ != (char)(R18 + ((byte)X < 0xe6))) {
    X = X + 1;
    *puVar1 = R1;
  }
  R17 = 0;
  Y = 0x5d;
  while ((byte)Y != 0x5c || (bool)Y._1_1_ != (byte)Y < 0x5c) {
    Y = Y - 1;
    Z = Y;
    DAT_mem_08fd = 0x7e;
    FUN_code_00096b();
  }
  DAT_mem_08fd = 0x83;
  FUN_code_0003c1();
  FUN_code_000971();
  return;
}



uint FUN_code_000087(undefined2 R25R24)

{
  char cVar1;
  undefined1 *puVar2;
  byte bVar3;
  bool bVar4;
  bool bVar5;
  bool bVar6;
  char in_Hflg;
  char in_Tflg;
  char in_Iflg;
  
  Z = CONCAT11(-(((byte)R25R24 < 0x5c) + -1),(byte)R25R24 + 0xa4);
  R18 = *(byte *)(uint3)Z;
  cVar1 = -(((byte)R25R24 < 0x70) + -1);
  R25R24 = CONCAT11(cVar1,(byte)R25R24 + 0x90);
  Z = R25R24;
  R25R24._0_1_ = *(byte *)(uint3)R25R24;
  R25R24 = CONCAT11(cVar1,(byte)R25R24);
  if ((byte)R25R24 != 0) {
    bVar3 = (byte)R25R24 * '\x02';
    bVar4 = CARRY1((byte)R25R24,(byte)R25R24);
    Z = CONCAT11(bVar4 - ((bVar3 < 0x7a) + -1),bVar3 + 0x86);
    puVar2 = (undefined1 *)(uint3)Z;
    X._0_1_ = *puVar2;
    Z = Z + 1;
    X._1_1_ = *(undefined1 *)(uint3)Z;
    bVar5 = bVar3 < 0x84;
    cVar1 = bVar4 - (bVar5 + -1);
    bVar6 = SBORROW1(bVar4,-1) != SBORROW1(bVar4 + 1U,bVar5);
    Z = CONCAT11(cVar1,bVar3 + 0x7c);
    Z = Z + 1;
    R25R24._1_1_ = *(undefined1 *)(uint3)Z;
    R25R24._0_1_ = (bVar4 != true || (byte)(bVar4 + 1U) < bVar5) |
                   ((byte)(bVar3 + 0x7c) == '\0' && cVar1 == '\0') << 1 | (cVar1 < '\0') << 2 |
                   bVar6 << 3 | (cVar1 < '\0' != bVar6) << 4 | (in_Hflg == '\x01') << 5 |
                   (in_Tflg == '\x01') << 6 | (in_Iflg == '\x01') << 7;
    SREG = (byte)R25R24;
    Z._0_1_ = *X;
    Z._0_1_ = (byte)Z | R18;
    *X = (byte)Z;
    SREG = (byte)R25R24;
  }
  return R25R24;
}



undefined2 *
FUN_code_0000a6(undefined2 *R25R24,int R23R22,undefined2 R21R20,undefined2 param_4,undefined2 R17R16
               ,int R15R14,undefined2 *R13R12,undefined2 R11R10)

{
  byte bVar1;
  undefined2 uVar2;
  char cVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  undefined4 uVar8;
  
  uVar7 = Y;
  uVar6 = R17R16;
  uVar5 = R15R14;
  uVar4 = R13R12;
  uVar2 = R11R10;
  R13R12 = R25R24;
  R15R14 = R23R22;
  R17R16._0_1_ = (byte)R23R22;
  R17R16._1_1_ = (char)((uint)R23R22 >> 8);
  R17R16._1_1_ = R17R16._1_1_ + R21R20._1_1_ + CARRY1((byte)R17R16,(byte)R21R20);
  Y = R23R22;
  R17R16._0_1_ = (byte)R17R16 + (byte)R21R20;
  do {
    R11R10._0_1_ = (byte)Y;
    bVar1 = (byte)R11R10;
    R11R10._1_1_ = (char)((uint)Y >> 8);
    cVar3 = R11R10._1_1_;
    R11R10 = CONCAT11(R11R10._1_1_ - (R15R14._1_1_ + ((byte)R11R10 < (byte)R15R14)),
                      (byte)R11R10 - (byte)R15R14);
    R25R24 = (undefined2 *)R11R10;
    if (bVar1 == (byte)R17R16 && cVar3 == (char)(R17R16._1_1_ + (bVar1 < (byte)R17R16))) {
      R11R10 = uVar2;
      R13R12 = (undefined2 *)uVar4;
      R15R14 = uVar5;
      R17R16 = uVar6;
      Y = uVar7;
      return R25R24;
    }
    Y = Y + 1;
    X = (int)R13R12 + 1;
    Z = *(uint *)*R13R12;
    uVar8 = (*(code *)(uint3)Z)(R13R12);
    R23R22 = (int)uVar8;
    R25R24._1_1_ = (char)((ulong)uVar8 >> 0x18);
    R25R24._0_1_ = (char)((ulong)uVar8 >> 0x10);
  } while ((char)R25R24 != '\0' || R25R24._1_1_ != '\0');
  R25R24 = (undefined2 *)R11R10;
  Y = uVar7;
  R17R16 = uVar6;
  R15R14 = uVar5;
  R13R12 = (undefined2 *)uVar4;
  R11R10 = uVar2;
  return R25R24;
}



uint FUN_code_0000d3(uint R25R24)

{
  int iVar1;
  
  iVar1 = R25R24;
  Z = R25R24;
  R25R24._1_1_ = *(char *)(R25R24 + 0x19);
  R25R24._0_1_ = *(char *)(iVar1 + 0x1a);
  if (R25R24._1_1_ != (char)R25R24) {
    X._0_1_ = *(byte *)(iVar1 + 0x1a);
    Z._0_1_ = (byte)iVar1;
    Z._1_1_ = (char)((uint)iVar1 >> 8);
    X = (undefined1 *)CONCAT11(Z._1_1_ + R1 + CARRY1((byte)X,(byte)Z),(byte)X + (byte)Z);
    X = (undefined1 *)((int)X + 0x1d);
    R25R24._0_1_ = *X;
    R25R24._1_1_ = *(char *)(iVar1 + 0x1a);
    R25R24 = CONCAT11(R25R24._1_1_ + '\x01',(char)R25R24) & 0x3fff;
    Z = iVar1;
    *(char *)(iVar1 + 0x1a) = R25R24._1_1_;
    R25R24 = R25R24 & 0xff;
    return R25R24;
  }
  R25R24 = 0xffff;
  return R25R24;
}



uint FUN_code_0000e7(uint R25R24)

{
  int iVar1;
  
  iVar1 = R25R24;
  Z = R25R24;
  R25R24._1_1_ = *(char *)(R25R24 + 0x19);
  R25R24._0_1_ = *(char *)(iVar1 + 0x1a);
  if (R25R24._1_1_ != (byte)R25R24) {
    R25R24._0_1_ = *(byte *)(iVar1 + 0x1a);
    Z._0_1_ = (byte)iVar1;
    Z._1_1_ = (char)((uint)iVar1 >> 8);
    Z = CONCAT11(Z._1_1_ + R1 + CARRY1((byte)Z,(byte)R25R24),(byte)Z + (byte)R25R24);
    R25R24._0_1_ = *(byte *)(Z + 0x1d);
    R25R24 = (uint)(byte)R25R24;
    return R25R24;
  }
  R25R24 = 0xffff;
  return R25R24;
}



uint FUN_code_0000f5(uint R25R24)

{
  int iVar1;
  
  iVar1 = R25R24;
  Z = R25R24;
  R25R24._1_1_ = *(char *)(R25R24 + 0x19);
  R18 = *(char *)(iVar1 + 0x1a);
  R25R24 = (uint)((R25R24._1_1_ + '@') - R18 & 0x3f);
  return R25R24;
}



undefined2 FUN_code_000101(void)

{
  FUN_code_0000f5(0x49);
  R18 = (char)R25R24 != '\0' || R25R24._1_1_ != '\0';
  R25R24._0_1_ = R18;
  R25R24 = CONCAT11(R25R24._1_1_,R18);
  return R25R24;
}



undefined1 * FUN_code_000115(undefined1 *R25R24)

{
  undefined1 *puVar1;
  
  puVar1 = R25R24;
  Z = R25R24;
  X._0_1_ = R25R24[0x1c];
  X = (byte *)CONCAT11((R25R24._1_1_ + R1 + CARRY1((byte)X,(byte)R25R24)) -
                       (((byte)((byte)X + (byte)R25R24) < 0xa3) + -1),(byte)X + (byte)R25R24 + 0x5d)
  ;
  R18 = *X;
  R25R24._0_1_ = R25R24[0x1c];
  R25R24._0_1_ = (byte)R25R24 + 1 & 0x3f;
  puVar1[0x1c] = (byte)R25R24;
  **(undefined1 **)(Z + 0x16) = R18;
  X = *(byte **)(Z + 0x10);
  R25R24._0_1_ = *X;
  R25R24._0_1_ = (byte)R25R24 | 0x40;
  *X = (byte)R25R24;
  R25R24._1_1_ = Z[0x1b];
  R25R24._0_1_ = Z[0x1c];
  if (R25R24._1_1_ == (byte)R25R24) {
    R0 = Z[0x12];
    Z = *(undefined1 **)(Z + 0x12);
    R25R24._0_1_ = *Z;
    R25R24 = (undefined1 *)(CONCAT11(R25R24._1_1_,(byte)R25R24) & 0xffdf);
    *Z = (byte)R25R24;
  }
  return R25R24;
}



int FUN_code_000136(int R25R24,undefined1 R22,undefined2 param_3,undefined2 param_4,
                   undefined2 R17R16,undefined2 R15R14)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined2 uVar3;
  char cVar4;
  char cVar5;
  char cVar6;
  char cVar7;
  char cVar8;
  char in_Hflg;
  char in_Tflg;
  char in_Iflg;
  
  uVar3 = Y;
  uVar1 = (byte)R17R16;
  Y = R25R24;
  *(undefined1 *)(R25R24 + 0x18) = 1;
  uVar2 = R22;
  R25R24._1_1_ = *(char *)(Y + 0x1b);
  R25R24._0_1_ = *(char *)(Y + 0x1c);
  if ((R25R24._1_1_ == (byte)R25R24) &&
     (R25R24._0_1_ = **(byte **)(Y + 0x10), ((byte)R25R24 & 0x20) != 0)) {
    **(undefined1 **)(Y + 0x16) = R22;
    Z = *(byte **)(Y + 0x10);
    R25R24._0_1_ = *Z;
    R25R24._0_1_ = (byte)R25R24 | 0x40;
  }
  else {
    R17R16._0_1_ = *(char *)(Y + 0x1b);
    cVar4 = '\x01';
    R17R16._0_1_ = (byte)R17R16 + 1 & 0x3f;
    cVar7 = '\0';
    cVar6 = '\0';
    cVar5 = '\x01';
    cVar8 = cVar6;
    while (R25R24._0_1_ = *(byte *)(Y + 0x1c), (byte)R25R24 == (byte)R17R16) {
      R0 = cVar4 == '\x01' | (cVar5 == '\x01') << 1 | (cVar6 == '\x01') << 2 |
           (cVar7 == '\x01') << 3 | (cVar8 == '\x01') << 4 | (in_Hflg == '\x01') << 5 |
           (in_Tflg == '\x01') << 6 | (in_Iflg == '\x01') << 7;
      SREG = R0;
      if (in_Iflg != '\x01') {
        Z = *(byte **)(Y + 0x10);
        R25R24._0_1_ = *Z;
        if (((byte)R25R24 & 0x20) != 0) {
          FUN_code_000115(Y);
        }
      }
    }
    Z._0_1_ = *(byte *)(Y + 0x1b);
    Z = (byte *)CONCAT11((Y._1_1_ + R1 + CARRY1((byte)Z,(byte)Y)) -
                         (((byte)((byte)Z + (byte)Y) < 0xa3) + -1),(byte)Z + (byte)Y + 0x5d);
    *Z = uVar2;
    *(byte *)(Y + 0x1b) = (byte)R17R16;
    Z = *(byte **)(Y + 0x12);
    R25R24._0_1_ = *Z;
    R25R24._0_1_ = (byte)R25R24 | 0x20;
  }
  *Z = (byte)R25R24;
  R25R24 = 1;
  Y = uVar3;
  R17R16._0_1_ = uVar1;
  return R25R24;
}



uint FUN_code_00019d(undefined2 R25R24,char R22)

{
  char cVar1;
  char cVar2;
  undefined1 *puVar3;
  byte bVar4;
  bool bVar5;
  bool bVar6;
  bool bVar7;
  char in_Hflg;
  char in_Tflg;
  char in_Iflg;
  
  bVar4 = (byte)R25R24;
  R25R24 = (uint)(byte)R25R24;
  Z = CONCAT11(-((bVar4 < 0x98) + -1),bVar4 + 0x68);
  R18 = *(byte *)(uint3)Z;
  Z = CONCAT11(-((bVar4 < 0x5c) + -1),bVar4 + 0xa4);
  R19 = *(byte *)(uint3)Z;
  cVar1 = -((bVar4 < 0x70) + -1);
  Z = CONCAT11(cVar1,bVar4 + 0x90);
  Z._0_1_ = *(byte *)(uint3)Z;
  Z = CONCAT11(cVar1,(byte)Z);
  if ((byte)Z == 0) {
    return R25R24;
  }
  if (R18 == 0) goto LAB_code_0001b5;
  if (R18 == 3) {
    bVar4 = TCCR1A;
    bVar4 = bVar4 & 0x7f;
LAB_code_0001d3:
    TCCR1A = bVar4;
  }
  else {
    if (R18 < 3) {
      if (R18 == 1) {
        bVar4 = TCCR0A;
        bVar4 = bVar4 & 0x7f;
      }
      else {
        if (R18 != 2) goto LAB_code_0001b5;
        bVar4 = TCCR0A;
        bVar4 = bVar4 & 0xdf;
      }
      TCCR0A = bVar4;
      goto LAB_code_0001b5;
    }
    if (R18 == 7) {
      bVar4 = TCCR2A;
      bVar4 = bVar4 & 0x7f;
    }
    else {
      if (R18 != 8) {
        if (R18 != 4) goto LAB_code_0001b5;
        bVar4 = TCCR1A;
        bVar4 = bVar4 & 0xdf;
        goto LAB_code_0001d3;
      }
      bVar4 = TCCR2A;
      bVar4 = bVar4 & 0xdf;
    }
    TCCR2A = bVar4;
  }
LAB_code_0001b5:
  bVar5 = CARRY1((byte)Z,(byte)Z);
  cVar1 = (byte)Z * '\x02' + 0x7c;
  bVar6 = (byte)((byte)Z * '\x02') < 0x84;
  cVar2 = bVar5 - (bVar6 + -1);
  bVar7 = SBORROW1(bVar5,-1) != SBORROW1(bVar5 + 1U,bVar6);
  Z = CONCAT11(cVar2,cVar1);
  puVar3 = (undefined1 *)(uint3)Z;
  X._0_1_ = *puVar3;
  Z = Z + 1;
  X._1_1_ = *(undefined1 *)(uint3)Z;
  R25R24._0_1_ = (bVar5 != true || (byte)(bVar5 + 1U) < bVar6) |
                 (cVar1 == '\0' && cVar2 == '\0') << 1 | (cVar2 < '\0') << 2 | bVar7 << 3 |
                 (cVar2 < '\0' != bVar7) << 4 | (in_Hflg == '\x01') << 5 | (in_Tflg == '\x01') << 6
                 | (in_Iflg == '\x01') << 7;
  bVar4 = (byte)R25R24;
  R25R24 = (uint)(byte)R25R24;
  SREG = bVar4;
  Z._0_1_ = *X;
  if (R22 == R1) {
    R19 = ~R19;
    R19 = R19 & (byte)Z;
  }
  else {
    R19 = R19 | (byte)Z;
  }
  *X = R19;
  SREG = (byte)R25R24;
  return R25R24;
}



byte FUN_code_0001e9(byte R24,int R23R22,undefined2 param_3,undefined2 param_4,undefined2 R17R16)

{
  char cVar1;
  int iVar2;
  undefined2 uVar3;
  
  uVar3 = Y;
  iVar2 = R23R22;
  FUN_code_000087();
  if (iVar2 != 0) {
    Y._0_1_ = (char)iVar2;
    Y._1_1_ = (char)((uint)iVar2 >> 8);
    if ((char)Y != -1 || Y._1_1_ != (char)(R1 + ((char)Y != -1))) {
      cVar1 = -((R24 < 0x98) + -1);
      Z = CONCAT11(cVar1,R24 + 0x68);
      Z._0_1_ = *(byte *)(uint3)Z;
      Z = CONCAT11(cVar1,(byte)Z);
      if ((byte)Z == 3) {
        R24 = TCCR1A;
        R24 = R24 | 0x80;
        TCCR1A = R24;
        OCR1AH = Y._1_1_;
        OCR1AL = (char)Y;
      }
      else if ((byte)Z < 3) {
        if ((byte)Z == 1) {
          R24 = TCCR0A;
          R24 = R24 | 0x80;
          TCCR0A = R24;
          OCR0A = (char)Y;
        }
        else {
          if ((byte)Z != 2) goto LAB_code_0001f3;
          R24 = TCCR0A;
          R24 = R24 | 0x20;
          TCCR0A = R24;
          OCR0B = (char)Y;
        }
      }
      else if ((byte)Z == 7) {
        R24 = TCCR2A;
        R24 = R24 | 0x80;
        TCCR2A = R24;
        OCR2A = (char)Y;
      }
      else if ((byte)Z == 8) {
        R24 = TCCR2A;
        R24 = R24 | 0x20;
        TCCR2A = R24;
        OCR2B = (char)Y;
      }
      else {
        if ((byte)Z != 4) goto LAB_code_0001f3;
        R24 = TCCR1A;
        R24 = R24 | 0x20;
        TCCR1A = R24;
        OCR1BH = Y._1_1_;
        OCR1BL = (char)Y;
      }
      Y = uVar3;
      return R24;
    }
  }
LAB_code_0001f3:
  Y = uVar3;
  FUN_code_00019d(R24);
  return R24;
}



undefined4 FUN_code_000245(void)

{
  char in_Cflg;
  char in_Zflg;
  char in_Nflg;
  char in_Vflg;
  char in_Sflg;
  char in_Hflg;
  char in_Tflg;
  char in_Iflg;
  
  R18 = in_Cflg == '\x01' | (in_Zflg == '\x01') << 1 | (in_Nflg == '\x01') << 2 |
        (in_Vflg == '\x01') << 3 | (in_Sflg == '\x01') << 4 | (in_Hflg == '\x01') << 5 |
        (in_Tflg == '\x01') << 6 | (in_Iflg == '\x01') << 7;
  SREG = R18;
  R23R22._0_1_ = DAT_mem_011d;
  R23R22._1_1_ = DAT_mem_011e;
  R24 = DAT_mem_011f;
  R25 = DAT_mem_0120;
  SREG = R18;
  _R23R22 = CONCAT13(DAT_mem_0120,CONCAT12(DAT_mem_011f,CONCAT11(DAT_mem_011e,DAT_mem_011d)));
  return _R23R22;
}



undefined2 FUN_code_000251(void)

{
  byte bVar1;
  byte bVar2;
  bool bVar3;
  
  FUN_code_000245();
  DAT_mem_0151 = R22;
  DAT_mem_0152 = R23;
  DAT_mem_0153 = (byte)R25R24;
  DAT_mem_0154 = R25R24._1_1_;
  do {
    FUN_code_0000e7(0x49);
    if ((R25R24._1_1_ & 0x80) == 0) goto LAB_code_000280;
    FUN_code_000245();
    bVar1 = R23 - (DAT_mem_0152 + (R22 < DAT_mem_0151));
    bVar3 = R23 < DAT_mem_0152 || (byte)(R23 - DAT_mem_0152) < (R22 < DAT_mem_0151);
    bVar2 = (byte)R25R24 - (DAT_mem_0153 + bVar3);
    R25R24._1_1_ = R25R24._1_1_ -
                   (DAT_mem_0154 +
                   ((byte)R25R24 < DAT_mem_0153 || (byte)((byte)R25R24 - DAT_mem_0153) < bVar3));
    R18 = DAT_mem_014f;
    R19 = DAT_mem_0150;
  } while (R25R24._1_1_ < DAT_mem_0150 ||
           (byte)(R25R24._1_1_ - DAT_mem_0150) <
           (bVar2 < DAT_mem_014f ||
           (byte)(bVar2 - DAT_mem_014f) <
           (bVar1 < DAT_mem_014e ||
           (byte)(bVar1 - DAT_mem_014e) < ((byte)(R22 - DAT_mem_0151) < DAT_mem_014d))));
  R25R24._0_1_ = 0xff;
  R25R24._1_1_ = 0xff;
  R22 = R22 - DAT_mem_0151;
  R23 = bVar1;
LAB_code_000280:
  R25R24 = CONCAT11(R25R24._1_1_,(byte)R25R24);
  return R25R24;
}



undefined4 FUN_code_000283(void)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  uint uVar9;
  bool bVar10;
  bool bVar11;
  bool bVar12;
  bool bVar13;
  undefined4 uVar14;
  
  uVar7 = R15R14;
  uVar5 = R13R12;
  while( true ) {
    FUN_code_000251();
    Z = R25R24;
    if ((R25R24 & 0x8000) != 0) {
      R13R12 = CONCAT11(R1,R1);
      uVar6 = R13R12;
      R25R24 = R13R12;
      R23R22 = R13R12;
      R15R14 = uVar7;
      R13R12 = uVar5;
      return CONCAT22(uVar6,uVar6);
    }
    if (((byte)R25R24 == 0x2d && R25R24._1_1_ == (byte)(R1 + ((byte)R25R24 < 0x2d))) ||
       (R25R24 = R25R24 - 0x30, R25R24 < 10)) break;
    FUN_code_0000d3();
  }
  R13R12 = CONCAT11(R1,R1);
  R15R14 = R13R12;
  bVar13 = false;
  do {
    do {
      if ((byte)Z != 1 || Z._1_1_ != (byte)(R1 + ((byte)Z == 0))) {
        if ((byte)Z == 0x2d && Z._1_1_ == (byte)(R1 + ((byte)Z < 0x2d))) {
          bVar13 = true;
        }
        else {
          R25R24 = Z - 0x30;
          uVar9 = R25R24;
          R25R24 = Z - 0x3a;
          if (uVar9 < 10) {
            uVar14 = FUN_code_0008ff(R25R24,R23R22,R15R14,R13R12);
            bVar2 = Z._1_1_;
            bVar10 = CARRY1(Z._1_1_,Z._1_1_);
            Z = CONCAT11(Z._1_1_ * '\x02',(byte)Z);
            bVar4 = -bVar10;
            R23R22._0_1_ = (byte)uVar14;
            bVar3 = (byte)Z + (byte)R23R22;
            R23R22._1_1_ = (byte)((ulong)uVar14 >> 8);
            bVar1 = bVar2 + R23R22._1_1_ + CARRY1((byte)Z,(byte)R23R22);
            bVar11 = CARRY1(bVar2,R23R22._1_1_) ||
                     CARRY1(bVar2 + R23R22._1_1_,CARRY1((byte)Z,(byte)R23R22));
            R25R24._0_1_ = (byte)((ulong)uVar14 >> 0x10);
            bVar2 = bVar4 + (byte)R25R24 + bVar11;
            R25R24._1_1_ = (char)((ulong)uVar14 >> 0x18);
            bVar12 = bVar1 < R1 || (byte)(bVar1 - R1) < (bVar3 < 0x30);
            R13R12 = CONCAT11(bVar1 - (R1 + (bVar3 < 0x30)),bVar3 - 0x30);
            R15R14 = CONCAT11(((R25R24._1_1_ - bVar10) +
                              (CARRY1(bVar4,(byte)R25R24) || CARRY1(bVar4 + (byte)R25R24,bVar11))) -
                              (R1 + (bVar2 < R1 || (byte)(bVar2 - R1) < bVar12)),
                              bVar2 - (R1 + bVar12));
          }
        }
      }
      FUN_code_0000d3();
      FUN_code_000251();
      uVar9 = R25R24;
      uVar8 = R15R14;
      uVar6 = R13R12;
      Z = R25R24;
      R25R24 = R25R24 - 0x30;
    } while (R25R24 < 10);
    Z._0_1_ = (byte)uVar9;
    bVar10 = (byte)Z == '\x01';
    Z._1_1_ = (byte)(uVar9 >> 8);
    bVar11 = Z._1_1_ == (byte)(R1 + ((byte)Z == '\0'));
  } while (bVar10 && bVar11);
  if (!bVar13) {
    R13R12 = uVar5;
    R15R14 = uVar7;
    R23R22 = uVar6;
    R25R24 = uVar8;
    Z = uVar9;
    return CONCAT22(uVar8,uVar6);
  }
  R15R14._1_1_ = ~R15R14._1_1_;
  R15R14._0_1_ = ~(byte)R15R14;
  R13R12._1_1_ = ~R13R12._1_1_;
  R13R12._0_1_ = ~(byte)R13R12;
  bVar13 = CARRY1((byte)R13R12,R1) || 0xfe < (byte)((byte)R13R12 + R1);
  bVar10 = CARRY1(R13R12._1_1_,R1) || CARRY1(R13R12._1_1_ + R1,bVar13);
  R13R12 = CONCAT11(R13R12._1_1_ + R1 + bVar13,(byte)R13R12 + R1 + '\x01');
  R23R22 = R13R12;
  R15R14 = CONCAT11(R15R14._1_1_ + R1 +
                    (CARRY1((byte)R15R14,R1) || CARRY1((byte)R15R14 + R1,bVar10)),
                    (byte)R15R14 + R1 + bVar10);
  R25R24 = R15R14;
  R13R12 = uVar5;
  R15R14 = uVar7;
  Z = uVar9;
  return CONCAT22(R25R24,R23R22);
}



int thunk_FUN_code_0008d2(int R25R24,undefined1 R22)

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



undefined2 FUN_code_0002e1(undefined2 R25R24)

{
  FUN_code_0001e9(9,(undefined1)R25R24);
  DAT_mem_0139 = (undefined1)R25R24;
  thunk_FUN_code_0008d2(0x39,(undefined1)R25R24);
  R25R24 = CONCAT11(R25R24._1_1_,(undefined1)R25R24);
  return R25R24;
}



undefined2 FUN_code_0002f0(undefined2 R25R24)

{
  FUN_code_0001e9(6,(undefined1)R25R24);
  DAT_mem_013f = (undefined1)R25R24;
  thunk_FUN_code_0008d2(0x38,(undefined1)R25R24);
  R25R24 = CONCAT11(R25R24._1_1_,(undefined1)R25R24);
  return R25R24;
}



undefined2 FUN_code_0002ff(undefined2 R25R24)

{
  FUN_code_0001e9(5,(undefined1)R25R24);
  DAT_mem_013e = (undefined1)R25R24;
  thunk_FUN_code_0008d2(0x37,(undefined1)R25R24);
  R25R24 = CONCAT11(R25R24._1_1_,(undefined1)R25R24);
  return R25R24;
}



undefined2 FUN_code_00030e(undefined2 R25R24)

{
  FUN_code_0001e9(3,(undefined1)R25R24);
  DAT_mem_0140 = (undefined1)R25R24;
  thunk_FUN_code_0008d2(0x36,(undefined1)R25R24);
  R25R24 = CONCAT11(R25R24._1_1_,(undefined1)R25R24);
  return R25R24;
}



undefined1 thunk_FUN_code_0008ca(undefined2 R25R24)

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



undefined8 FUN_code_00031f(undefined2 R25R24,undefined2 R23R22,undefined2 R21R20,undefined2 R19R18)

{
  undefined1 uVar1;
  char in_Cflg;
  char in_Zflg;
  char in_Nflg;
  char in_Vflg;
  char in_Sflg;
  char in_Hflg;
  char in_Tflg;
  char in_Iflg;
  
  uVar1 = R0;
  R0 = in_Cflg == '\x01' | (in_Zflg == '\x01') << 1 | (in_Nflg == '\x01') << 2 |
       (in_Vflg == '\x01') << 3 | (in_Sflg == '\x01') << 4 | (in_Hflg == '\x01') << 5 |
       (in_Tflg == '\x01') << 6 | (in_Iflg == '\x01') << 7;
  SREG = R0;
  FUN_code_000115(0x49);
  SREG = R0;
  R0 = uVar1;
  _R19R18 = CONCAT17(R25R24._1_1_,
                     CONCAT16((undefined1)R25R24,
                              CONCAT15(R23R22._1_1_,
                                       CONCAT14((undefined1)R23R22,
                                                CONCAT13(R21R20._1_1_,
                                                         CONCAT12((undefined1)R21R20,R19R18))))));
  return _R19R18;
}



undefined2 FUN_code_000345(undefined2 R25R24,undefined2 param_2,undefined2 param_3,undefined1 R18)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined2 uVar3;
  undefined1 uVar4;
  undefined2 uVar5;
  char in_Cflg;
  char in_Zflg;
  char in_Nflg;
  char in_Vflg;
  char in_Sflg;
  char in_Hflg;
  char in_Tflg;
  char in_Iflg;
  
  uVar5 = Z;
  uVar3 = R25R24;
  uVar1 = R0;
  uVar4 = R25R24._1_1_;
  uVar2 = (byte)R25R24;
  R0 = in_Cflg == '\x01' | (in_Zflg == '\x01') << 1 | (in_Nflg == '\x01') << 2 |
       (in_Vflg == '\x01') << 3 | (in_Sflg == '\x01') << 4 | (in_Hflg == '\x01') << 5 |
       (in_Tflg == '\x01') << 6 | (in_Iflg == '\x01') << 7;
  SREG = R0;
  Z = (byte *)CONCAT11(DAT_mem_015a,DAT_mem_0159);
  R25R24._0_1_ = *Z;
  Z = (byte *)CONCAT11(DAT_mem_0160,DAT_mem_015f);
  if (((byte)R25R24 & 4) == 0) {
    R25R24._1_1_ = *Z;
    R25R24._0_1_ = DAT_mem_0162 + 1 & 0x3f;
    if ((byte)R25R24 != DAT_mem_0163) {
      Z = (byte *)CONCAT11(-((DAT_mem_0162 < 0xb7) + -2),DAT_mem_0162 + 0x49);
      *(undefined1 *)((int)Z + 0x1d) = R25R24._1_1_;
      DAT_mem_0162 = (byte)R25R24;
    }
  }
  Z = (byte *)uVar5;
  R25R24._1_1_ = uVar4;
  R25R24._0_1_ = uVar2;
  SREG = R0;
  R0 = uVar1;
  R25R24 = uVar3;
  return R25R24;
}



uint FUN_code_000377(uint R25R24,undefined2 param_2,undefined2 param_3,undefined2 R19R18)

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
  R25R24 = CONCAT11(DAT_mem_011e,DAT_mem_011d);
  uVar4 = R25R24;
  R19R18._0_1_ = DAT_mem_011c + 3;
  if ((byte)R19R18 < 0x7d) {
    R25R24 = R25R24 + 1;
    bVar7 = 0xfffe < uVar4;
  }
  else {
    R19R18._0_1_ = DAT_mem_011c + 0x86;
    R25R24 = R25R24 + 2;
    bVar7 = 0xfffd < uVar4;
  }
  Xhi = DAT_mem_0120 + CARRY1(DAT_mem_011f,bVar7);
  Xlo = DAT_mem_011f + bVar7;
  DAT_mem_011c = (byte)R19R18;
  DAT_mem_011d = (undefined1)R25R24;
  DAT_mem_011e = R25R24._1_1_;
  DAT_mem_011f = Xlo;
  DAT_mem_0120 = Xhi;
  R25R24 = CONCAT11(DAT_mem_0119,DAT_mem_0118);
  uVar4 = R25R24;
  R25R24 = R25R24 + 1;
  bVar7 = CARRY1(DAT_mem_011a,0xfffe < uVar4);
  DAT_mem_0118 = (undefined1)R25R24;
  DAT_mem_0119 = R25R24._1_1_;
  DAT_mem_011a = DAT_mem_011a + (0xfffe < uVar4);
  DAT_mem_011b = DAT_mem_011b + bVar7;
  Xhi = uVar6;
  Xlo = uVar5;
  R25R24 = uVar3;
  R19R18._0_1_ = uVar2;
  SREG = R0;
  R0 = uVar1;
  return R25R24;
}



void FUN_code_0003c1(void)

{
  byte bVar1;
  char cVar2;
  byte bVar3;
  byte bVar4;
  int iVar5;
  uint uVar6;
  bool bVar7;
  undefined4 uVar8;
  
  R25R24._0_1_ = TCCR0A;
  R25R24._0_1_ = (byte)R25R24 | 2;
  TCCR0A = (byte)R25R24;
  R25R24._0_1_ = TCCR0A;
  R25R24._0_1_ = (byte)R25R24 | 1;
  TCCR0A = (byte)R25R24;
  R25R24._0_1_ = TCCR0B;
  R25R24._0_1_ = (byte)R25R24 | 2;
  TCCR0B = (byte)R25R24;
  R25R24._0_1_ = TCCR0B;
  R25R24._0_1_ = (byte)R25R24 | 1;
  TCCR0B = (byte)R25R24;
  R25R24._0_1_ = TIMSK0;
  R25R24._0_1_ = (byte)R25R24 | 1;
  TIMSK0 = (byte)R25R24;
  TCCR1B = R1R0._1_1_;
  R25R24._0_1_ = TCCR1B;
  R25R24._0_1_ = (byte)R25R24 | 2;
  TCCR1B = (byte)R25R24;
  R25R24._0_1_ = TCCR1B;
  R25R24._0_1_ = (byte)R25R24 | 1;
  TCCR1B = (byte)R25R24;
  R25R24._0_1_ = TCCR1A;
  R25R24._0_1_ = (byte)R25R24 | 1;
  TCCR1A = (byte)R25R24;
  R25R24._0_1_ = TCCR2B;
  R25R24._0_1_ = (byte)R25R24 | 4;
  TCCR2B = (byte)R25R24;
  R25R24._0_1_ = TCCR2A;
  R25R24._0_1_ = (byte)R25R24 | 1;
  TCCR2A = (byte)R25R24;
  R25R24._0_1_ = ADCSRA;
  R25R24._0_1_ = (byte)R25R24 | 4;
  ADCSRA = (byte)R25R24;
  R25R24._0_1_ = ADCSRA;
  R25R24._0_1_ = (byte)R25R24 | 2;
  ADCSRA = (byte)R25R24;
  R25R24._0_1_ = ADCSRA;
  R25R24._0_1_ = (byte)R25R24 | 1;
  ADCSRA = (byte)R25R24;
  R25R24._0_1_ = ADCSRA;
  R25R24._0_1_ = (byte)R25R24 | 0x80;
  ADCSRA = (byte)R25R24;
  UCSR0B = R1R0._1_1_;
  Z = (byte *)CONCAT11(DAT_mem_015a,DAT_mem_0159);
  *Z = 2;
  Z = (byte *)CONCAT11(DAT_mem_0156,DAT_mem_0155);
  *Z = R1R0._1_1_;
  Z = (byte *)CONCAT11(DAT_mem_0158,DAT_mem_0157);
  *Z = 0xcf;
  DAT_mem_0161 = R1R0._1_1_;
  Z = (byte *)CONCAT11(DAT_mem_015e,DAT_mem_015d);
  *Z = 6;
  Z = (byte *)CONCAT11(DAT_mem_015c,DAT_mem_015b);
  R25R24._0_1_ = *Z;
  R25R24._0_1_ = (byte)R25R24 | 0x10;
  *Z = (byte)R25R24;
  Z = (byte *)CONCAT11(DAT_mem_015c,DAT_mem_015b);
  R25R24._0_1_ = *Z;
  R25R24._0_1_ = (byte)R25R24 | 8;
  *Z = (byte)R25R24;
  Z = (byte *)CONCAT11(DAT_mem_015c,DAT_mem_015b);
  R25R24._0_1_ = *Z;
  R25R24._0_1_ = (byte)R25R24 | 0x80;
  *Z = (byte)R25R24;
  Z = (byte *)CONCAT11(DAT_mem_015c,DAT_mem_015b);
  R25R24._0_1_ = *Z;
  R25R24._0_1_ = (byte)R25R24 & 0xdf;
  *Z = (byte)R25R24;
  ADMUX = 0x40;
  R25R24._0_1_ = ADCSRA;
  R25R24._0_1_ = (byte)R25R24 | 0x40;
  ADCSRA = (byte)R25R24;
  do {
    bVar1 = ADCSRA;
  } while ((bVar1 & 0x40) != 0);
  bVar1 = ADCL;
  bVar3 = ADCH;
  R1R0 = CONCAT11(R1R0._1_1_,bVar3 * '\x02');
  bVar4 = -CARRY1(bVar3,bVar3);
  bVar7 = bVar3 < R1R0._1_1_ || (byte)(bVar3 - R1R0._1_1_) < (bVar1 < R1R0._1_1_);
  if (((bVar1 != R1R0._1_1_ || bVar3 != (byte)(R1R0._1_1_ + (bVar1 < R1R0._1_1_))) ||
      bVar4 != (byte)(R1R0._1_1_ + bVar7)) ||
      (byte)-CARRY1(bVar3,bVar3) !=
      (byte)(R1R0._1_1_ + (bVar4 < R1R0._1_1_ || (byte)(bVar4 - R1R0._1_1_) < bVar7))) {
    FUN_code_0008c1();
  }
  FUN_code_000087();
  FUN_code_000087();
  FUN_code_000087();
  FUN_code_000087();
  thunk_FUN_code_0008ca();
  DAT_mem_0103 = 0;
  DAT_mem_0102 = (byte)R25R24;
  thunk_FUN_code_0008ca();
  cVar2 = (byte)R25R24;
  DAT_mem_0148 = '\0';
  DAT_mem_0147 = (byte)R25R24;
  thunk_FUN_code_0008ca();
  DAT_mem_013d = '\0';
  DAT_mem_013c = (byte)R25R24;
  thunk_FUN_code_0008ca();
  DAT_mem_0101 = 0;
  DAT_mem_0100 = (byte)R25R24;
  if (cVar2 == '\0') {
    thunk_FUN_code_0008ca();
    FUN_code_00030e();
    thunk_FUN_code_0008ca();
    FUN_code_0002ff();
    thunk_FUN_code_0008ca();
    FUN_code_0002f0();
    thunk_FUN_code_0008ca();
    FUN_code_0002e1();
  }
  if (DAT_mem_013c == '\0' && DAT_mem_013d == '\0') {
    thunk_FUN_code_0008ca();
    FUN_code_00030e();
    thunk_FUN_code_0008ca();
    FUN_code_0002ff();
    thunk_FUN_code_0008ca();
    FUN_code_0002f0();
    thunk_FUN_code_0008ca();
    FUN_code_0002e1();
  }
  uVar8 = FUN_code_000245();
  R23R22._0_1_ = (byte)uVar8;
  DAT_mem_0143 = (byte)R23R22;
  R23R22._1_1_ = (byte)((ulong)uVar8 >> 8);
  DAT_mem_0144 = R23R22._1_1_;
  R25R24._0_1_ = (byte)((ulong)uVar8 >> 0x10);
  DAT_mem_0145 = (byte)R25R24;
  R25R24._1_1_ = (char)((ulong)uVar8 >> 0x18);
  DAT_mem_0146 = R25R24._1_1_;
  R5R4._1_1_ = '\x01';
  R5R4._0_1_ = R1R0._1_1_;
  R9R8 = CONCAT11(R1R0._1_1_,R1R0._1_1_);
  R11R10 = R9R8;
  R9R8._1_1_ = R1R0._1_1_ + 1;
  R9R8._0_1_ = R1R0._1_1_;
LAB_code_0004da:
  do {
    while( true ) {
      R25R24 = FUN_code_0000f5();
      if ((byte)R25R24 != '\0' || R25R24._1_1_ != '\0') break;
      if (DAT_mem_0147 != '\0' || DAT_mem_0148 != '\0') {
        uVar8 = FUN_code_000245();
        cVar2 = DAT_mem_0144;
        bVar3 = DAT_mem_0143;
        R23R22._0_1_ = (byte)uVar8;
        DAT_mem_0143 = (byte)R23R22;
        R23R22._1_1_ = (byte)((ulong)uVar8 >> 8);
        DAT_mem_0144 = R23R22._1_1_;
        R25R24._0_1_ = (byte)((ulong)uVar8 >> 0x10);
        DAT_mem_0145 = (byte)R25R24;
        R25R24._1_1_ = (char)((ulong)uVar8 >> 0x18);
        DAT_mem_0146 = R25R24._1_1_;
        bVar1 = DAT_mem_0141 - bVar3;
        cVar2 = DAT_mem_0142 - (cVar2 + (DAT_mem_0141 < bVar3));
        Z = (byte *)CONCAT11(cVar2,bVar1);
        DAT_mem_0141 = bVar1 + (byte)R23R22;
        DAT_mem_0142 = cVar2 + R23R22._1_1_ + CARRY1(bVar1,(byte)R23R22);
        if (DAT_mem_0103 <= DAT_mem_0142 &&
            (DAT_mem_0141 < DAT_mem_0102) <= (byte)(DAT_mem_0142 - DAT_mem_0103)) {
          DAT_mem_0142 = R1R0._1_1_;
          DAT_mem_0141 = R1R0._1_1_;
          if (DAT_mem_0140 == R1R0._1_1_) {
            if (DAT_mem_013f == 0xff) {
              if (DAT_mem_013e != 0xff) {
LAB_code_00067c:
                FUN_code_0002ff();
                goto LAB_code_00067f;
              }
            }
            else {
              if (DAT_mem_013e != 0xff) goto LAB_code_000725;
              if (DAT_mem_013f == 0) goto LAB_code_000710;
            }
LAB_code_000722:
            FUN_code_0002f0();
          }
          else {
            if (DAT_mem_013e == 0xff) {
              if (DAT_mem_013f != R1R0._1_1_) {
                if (DAT_mem_0140 != 0xff) goto LAB_code_0007ef;
LAB_code_000731:
                if (DAT_mem_013e != 0) {
                  FUN_code_0002ff();
                }
                if (DAT_mem_013f == 0) goto LAB_code_00067f;
                goto LAB_code_000722;
              }
              if (DAT_mem_0140 == 0xff) goto LAB_code_00067c;
            }
            else {
              if (DAT_mem_0140 == 0xff) {
                if (DAT_mem_013f == R1R0._1_1_) {
                  if (DAT_mem_013e != 0) goto LAB_code_00067c;
                }
                else {
                  if (DAT_mem_013e != R1R0._1_1_) goto LAB_code_000731;
                  if (DAT_mem_013f == 0xff) goto LAB_code_000710;
                }
                goto LAB_code_000722;
              }
LAB_code_0007ef:
              if (DAT_mem_013f == 0xff) {
                if (DAT_mem_013e == R1R0._1_1_) {
                  if (DAT_mem_0140 != 0) goto LAB_code_000710;
                }
                else if (DAT_mem_013e <= DAT_mem_0140) goto LAB_code_000728;
LAB_code_0007b0:
                if (DAT_mem_013e != 0) {
                  FUN_code_0002ff();
                }
              }
              else {
LAB_code_000725:
                if (DAT_mem_013e <= DAT_mem_0140) {
LAB_code_000728:
                  if (DAT_mem_013f <= DAT_mem_0140) {
                    if (DAT_mem_0140 != 0xff) {
                      FUN_code_00030e();
                    }
                    goto LAB_code_000731;
                  }
                  bVar1 = DAT_mem_0140;
                  if (DAT_mem_013f < DAT_mem_013e) {
joined_c0x00073f:
                    if (DAT_mem_013e < bVar1) goto LAB_code_00067f;
                    goto LAB_code_000741;
                  }
                  if (DAT_mem_013f != 0xff) goto LAB_code_0007c4;
                  goto LAB_code_0007b0;
                }
                if (DAT_mem_013e <= DAT_mem_013f) {
                  bVar1 = DAT_mem_013f;
                  if (DAT_mem_013f < DAT_mem_0140) goto joined_c0x00073f;
LAB_code_0007c4:
                  FUN_code_0002f0();
                  goto LAB_code_0007b0;
                }
LAB_code_000741:
                if (DAT_mem_013e != 0xff) {
                  FUN_code_0002ff();
                }
                if (DAT_mem_013f != 0) {
                  FUN_code_0002f0();
                }
              }
              if (DAT_mem_0140 == 0) goto LAB_code_00067f;
            }
LAB_code_000710:
            FUN_code_00030e();
          }
        }
      }
LAB_code_00067f:
      if (DAT_mem_013c != '\0' || DAT_mem_013d != '\0') {
        uVar8 = FUN_code_000245();
        cVar2 = DAT_mem_0144;
        bVar3 = DAT_mem_0143;
        R23R22._0_1_ = (byte)uVar8;
        DAT_mem_0143 = (byte)R23R22;
        R23R22._1_1_ = (byte)((ulong)uVar8 >> 8);
        DAT_mem_0144 = R23R22._1_1_;
        R25R24._0_1_ = (byte)((ulong)uVar8 >> 0x10);
        DAT_mem_0145 = (byte)R25R24;
        R25R24._1_1_ = (char)((ulong)uVar8 >> 0x18);
        DAT_mem_0146 = R25R24._1_1_;
        bVar1 = DAT_mem_0141 - bVar3;
        cVar2 = DAT_mem_0142 - (cVar2 + (DAT_mem_0141 < bVar3));
        Z = (byte *)CONCAT11(cVar2,bVar1);
        DAT_mem_0141 = bVar1 + (byte)R23R22;
        DAT_mem_0142 = cVar2 + R23R22._1_1_ + CARRY1(bVar1,(byte)R23R22);
        if (DAT_mem_0101 <= DAT_mem_0142 &&
            (DAT_mem_0141 < DAT_mem_0100) <= (byte)(DAT_mem_0142 - DAT_mem_0101)) {
          DAT_mem_0142 = R1R0._1_1_;
          DAT_mem_0141 = R1R0._1_1_;
          if (DAT_mem_0140 == R1R0._1_1_) {
            if (DAT_mem_013f == 0xff) {
              if (DAT_mem_013e != 0xff) {
LAB_code_0006ba:
                FUN_code_0002ff();
                goto LAB_code_0006bc;
              }
            }
            else {
              if (DAT_mem_013e != 0xff) goto LAB_code_000779;
              if (DAT_mem_013f == 0) goto LAB_code_00075f;
            }
LAB_code_000755:
            FUN_code_0002f0();
          }
          else {
            if (DAT_mem_013e == 0xff) {
              if (DAT_mem_013f != R1R0._1_1_) goto LAB_code_000770;
              if (DAT_mem_0140 == 0xff) goto LAB_code_0006ba;
            }
            else {
              if (DAT_mem_0140 == 0xff) {
                if (DAT_mem_013f == R1R0._1_1_) {
                  if (DAT_mem_013e != 0) goto LAB_code_0006ba;
                }
                else {
                  if (DAT_mem_013e != R1R0._1_1_) goto LAB_code_000770;
                  if (DAT_mem_013f == 0xff) goto LAB_code_00075f;
                }
                goto LAB_code_000755;
              }
LAB_code_000770:
              if (DAT_mem_013f == 0xff) {
                if (DAT_mem_013e == R1R0._1_1_) {
                  if (DAT_mem_0140 != 0) goto LAB_code_00075f;
                }
                else if (DAT_mem_013e <= DAT_mem_0140) goto LAB_code_00077c;
LAB_code_0007cc:
                if (DAT_mem_013e != 0) {
                  FUN_code_0002ff();
                }
              }
              else {
LAB_code_000779:
                if (DAT_mem_013e <= DAT_mem_0140) {
LAB_code_00077c:
                  if (DAT_mem_0140 < DAT_mem_013f) {
                    if (DAT_mem_013e <= DAT_mem_013f) {
                      if (DAT_mem_013f != 0xff) goto LAB_code_0007e0;
                      goto LAB_code_0007cc;
                    }
                    if (DAT_mem_0140 <= DAT_mem_013e) goto LAB_code_000796;
                  }
                  else {
                    if (DAT_mem_0140 != 0xff) {
                      FUN_code_00030e();
                    }
                    if (DAT_mem_013e != 0) {
                      FUN_code_0002ff();
                    }
                    if (DAT_mem_013f != 0) goto LAB_code_000755;
                  }
                  goto LAB_code_0006bc;
                }
                if (DAT_mem_013e <= DAT_mem_013f) {
                  if (DAT_mem_0140 <= DAT_mem_013f) {
LAB_code_0007e0:
                    FUN_code_0002f0();
                    goto LAB_code_0007cc;
                  }
                  if (DAT_mem_013e < DAT_mem_013f) goto LAB_code_0006bc;
                }
LAB_code_000796:
                if (DAT_mem_013e != 0xff) {
                  FUN_code_0002ff();
                }
                if (DAT_mem_013f != 0) {
                  FUN_code_0002f0();
                }
              }
              if (DAT_mem_0140 == 0) goto LAB_code_0006bc;
            }
LAB_code_00075f:
            FUN_code_00030e();
          }
        }
      }
LAB_code_0006bc:
      if (DAT_mem_013a != '\0' || DAT_mem_013b != '\0') {
        DAT_mem_013b = R1R0._1_1_;
        DAT_mem_013a = R1R0._1_1_;
        thunk_FUN_code_0008d2();
        thunk_FUN_code_0008d2();
        thunk_FUN_code_0008d2();
        thunk_FUN_code_0008d2();
        thunk_FUN_code_0008d2();
        thunk_FUN_code_0008d2();
        thunk_FUN_code_0008d2();
        thunk_FUN_code_0008d2();
      }
      if ((R1R0._1_1_ != '\0' || (byte)(R1R0._1_1_ + (R1R0._1_1_ != '\0')) != '\0') &&
         (FUN_code_000101(), (byte)R25R24 != '\0')) {
        RESET();
      }
    }
    bVar1 = FUN_code_0000d3();
    cVar2 = R1R0._1_1_ + (bVar1 < 0x65);
    if (bVar1 == 0x65 && cVar2 == '\0') {
      FUN_code_00030e();
      FUN_code_0002ff();
      FUN_code_0002f0();
      FUN_code_0002e1();
      DAT_mem_0148 = '\0';
      DAT_mem_0147 = '\x01';
      DAT_mem_013d = R1R0._1_1_;
      DAT_mem_013c = R1R0._1_1_;
      DAT_mem_013b = '\0';
      DAT_mem_013a = '\x01';
      FUN_code_000283();
      DAT_mem_0102 = (byte)R5R4 - (byte)R23R22;
      DAT_mem_0103 = R5R4._1_1_ - (R23R22._1_1_ + ((byte)R5R4 < (byte)R23R22));
      R19R18 = CONCAT11(DAT_mem_0103,DAT_mem_0102);
      thunk_FUN_code_0008d2(0x3a,R19R18);
      goto LAB_code_00056d;
    }
    if (-cVar2 < '\0' == (SBORROW1('\0',R1R0._1_1_) != SBORROW1(-R1R0._1_1_,bVar1 < 0x65))) {
      if (bVar1 == 0x6b && (byte)(R1R0._1_1_ + (bVar1 < 0x6b)) == '\0') {
        FUN_code_00030e();
        FUN_code_0002ff();
        FUN_code_0002f0();
        FUN_code_0002e1();
        DAT_mem_0148 = R1R0._1_1_;
        DAT_mem_0147 = R1R0._1_1_;
        DAT_mem_013d = '\0';
        DAT_mem_013c = '\x01';
        DAT_mem_013b = '\0';
        DAT_mem_013a = '\x01';
        FUN_code_000283();
        cVar2 = R23R22._1_1_ + ((byte)R9R8 < (byte)R23R22);
        R1R0 = (uint)(byte)((byte)R9R8 - (byte)R23R22) * 0xe8;
        iVar5 = R1R0;
        R1R0._0_1_ = ((byte)R9R8 - (byte)R23R22) * '\x03';
        R23R22._1_1_ = (byte)((uint)iVar5 >> 8);
        R23R22._0_1_ = (byte)iVar5;
        bVar1 = (R9R8._1_1_ - cVar2) * -0x18;
        DAT_mem_0101 = R23R22._1_1_ + (char)R1R0 + bVar1;
        R1R0 = (uint)bVar1;
        DAT_mem_0100 = (byte)R23R22;
        thunk_FUN_code_0008d2();
        goto LAB_code_00056d;
      }
      if (bVar1 == 0x6e && (byte)(R1R0._1_1_ + (bVar1 < 0x6e)) == '\0') {
        DAT_mem_0148 = R1R0._1_1_;
        DAT_mem_0147 = R1R0._1_1_;
        DAT_mem_013d = R1R0._1_1_;
        DAT_mem_013c = R1R0._1_1_;
        DAT_mem_013b = '\0';
        DAT_mem_013a = '\x01';
      }
    }
    else {
      if (bVar1 != 0x41 || (byte)(R1R0._1_1_ + (bVar1 < 0x41)) != '\0') break;
      DAT_mem_0148 = R1R0._1_1_;
      DAT_mem_0147 = R1R0._1_1_;
      DAT_mem_013d = R1R0._1_1_;
      DAT_mem_013c = R1R0._1_1_;
      DAT_mem_013b = '\0';
      DAT_mem_013a = '\x01';
      FUN_code_000283();
      DAT_mem_0137 = (byte)R23R22;
      DAT_mem_0138 = R23R22._1_1_;
      FUN_code_000283();
      DAT_mem_0136 = R23R22._1_1_;
      DAT_mem_0135 = (byte)R23R22;
      FUN_code_000283();
      DAT_mem_0134 = R23R22._1_1_;
      DAT_mem_0133 = (byte)R23R22;
      FUN_code_000283();
      DAT_mem_0132 = R23R22._1_1_;
      DAT_mem_0131 = (byte)R23R22;
      R25R24 = FUN_code_0000d3();
      if (R25R24 == 0x2a) {
        FUN_code_00030e();
        FUN_code_0002ff();
        FUN_code_0002f0();
        FUN_code_0002e1();
        DAT_mem_0130 = DAT_mem_0138;
        DAT_mem_012f = DAT_mem_0137;
        DAT_mem_012e = DAT_mem_0136;
        DAT_mem_012d = DAT_mem_0135;
        DAT_mem_012c = DAT_mem_0134;
        DAT_mem_012b = DAT_mem_0133;
        DAT_mem_012a = DAT_mem_0132;
        DAT_mem_0129 = DAT_mem_0131;
      }
    }
  } while( true );
  if (bVar1 == 100 && (byte)(R1R0._1_1_ + (bVar1 < 100)) == '\0') {
    DAT_mem_0148 = R1R0._1_1_;
    DAT_mem_0147 = R1R0._1_1_;
    DAT_mem_013d = R1R0._1_1_;
    DAT_mem_013c = R1R0._1_1_;
    FUN_code_000283();
    R13R12 = R23R22;
    uVar6 = R13R12;
    R1R0 = (uint)DAT_mem_012f * (R23R22 & 0xff);
    iVar5 = R1R0;
    R13R12._1_1_ = (char)(R23R22 >> 8);
    R1R0._0_1_ = DAT_mem_012f * R13R12._1_1_;
    R25R24._1_1_ = (char)((uint)iVar5 >> 8);
    R25R24._1_1_ = R25R24._1_1_ + (char)R1R0;
    R25R24._0_1_ = (byte)iVar5;
    R1R0 = (uint)DAT_mem_0130 * (R23R22 & 0xff);
    R25R24 = CONCAT11(R25R24._1_1_ + (char)R1R0,(byte)R25R24);
    R1R0 = R1R0 & 0xff;
    R13R12 = uVar6;
    FUN_code_000943(R25R24,bVar1);
    DAT_mem_0127 = (byte)R23R22;
    DAT_mem_0128 = R23R22._1_1_;
    R1R0 = (uint)DAT_mem_012d * (R13R12 & 0xff);
    iVar5 = R1R0;
    R1R0._0_1_ = DAT_mem_012d * R13R12._1_1_;
    R25R24._1_1_ = (char)((uint)iVar5 >> 8);
    R25R24._1_1_ = R25R24._1_1_ + (char)R1R0;
    R25R24._0_1_ = (byte)iVar5;
    R1R0 = (uint)DAT_mem_012e * (R13R12 & 0xff);
    R25R24 = CONCAT11(R25R24._1_1_ + (char)R1R0,(byte)R25R24);
    R1R0 = R1R0 & 0xff;
    FUN_code_000943(R25R24,bVar1);
    DAT_mem_0126 = R23R22._1_1_;
    DAT_mem_0125 = (byte)R23R22;
    R1R0 = (uint)DAT_mem_012b * (R13R12 & 0xff);
    iVar5 = R1R0;
    R1R0._0_1_ = DAT_mem_012b * R13R12._1_1_;
    R25R24._1_1_ = (char)((uint)iVar5 >> 8);
    R25R24._1_1_ = R25R24._1_1_ + (char)R1R0;
    R25R24._0_1_ = (byte)iVar5;
    R1R0 = (uint)DAT_mem_012c * (R13R12 & 0xff);
    R25R24 = CONCAT11(R25R24._1_1_ + (char)R1R0,(byte)R25R24);
    R1R0 = R1R0 & 0xff;
    FUN_code_000943(R25R24,bVar1);
    DAT_mem_0124 = R23R22._1_1_;
    DAT_mem_0123 = (byte)R23R22;
    R1R0 = (uint)DAT_mem_0129 * (R13R12 & 0xff);
    iVar5 = R1R0;
    R1R0._0_1_ = DAT_mem_0129 * R13R12._1_1_;
    R25R24._1_1_ = (char)((uint)iVar5 >> 8);
    R25R24._1_1_ = R25R24._1_1_ + (char)R1R0;
    R25R24._0_1_ = (byte)iVar5;
    R1R0 = (uint)DAT_mem_012a * (R13R12 & 0xff);
    R25R24 = CONCAT11(R25R24._1_1_ + (char)R1R0,(byte)R25R24);
    R1R0 = R1R0 & 0xff;
    FUN_code_000943(R25R24,bVar1);
    DAT_mem_0122 = R23R22._1_1_;
    DAT_mem_0121 = (byte)R23R22;
    FUN_code_00030e();
    FUN_code_0002ff();
    FUN_code_0002f0();
    FUN_code_0002e1();
    thunk_FUN_code_0008d2();
    thunk_FUN_code_0008d2();
    thunk_FUN_code_0008d2();
LAB_code_00056d:
    thunk_FUN_code_0008d2();
  }
  goto LAB_code_0004da;
}



ulong FUN_code_000826(undefined1 *R25R24)

{
  byte bVar1;
  byte bVar2;
  char cVar3;
  uint uVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  undefined4 uVar9;
  undefined2 uVar10;
  undefined1 *puVar11;
  undefined2 uVar12;
  bool bVar13;
  
  uVar12 = Y;
  puVar11 = R25R24;
  uVar10 = R15R14;
  uVar7 = R13R12;
  uVar6 = R11R10;
  uVar5 = R9R8;
  Y = R25R24;
  FUN_code_0008e0();
  R9R8 = R19R18;
  R11R10 = R21R20;
  _R13R12 = FUN_code_0008ff(R25R24,R23R22,R25R24,R23R22);
  R25R24 = (undefined1 *)((ulong)_R13R12 >> 0x10);
  R23R22 = (undefined2)_R13R12;
  FUN_code_00090d(R25R24,R23R22,R11R10,R9R8);
  uVar9 = _R13R12;
  cVar3 = (byte)R13R12 + (byte)R23R22;
  bVar1 = R13R12._1_1_ + R23R22._1_1_ + CARRY1((byte)R13R12,(byte)R23R22);
  bVar13 = CARRY1(R13R12._1_1_,R23R22._1_1_) ||
           CARRY1(R13R12._1_1_ + R23R22._1_1_,CARRY1((byte)R13R12,(byte)R23R22));
  R13R12 = CONCAT11(bVar1,cVar3);
  R15R14._0_1_ = SUB41(uVar9,2);
  bVar2 = (byte)R15R14 + (byte)R25R24 + bVar13;
  R15R14._1_1_ = SUB41(uVar9,3);
  R15R14._1_1_ = R15R14._1_1_ + R25R24._1_1_ +
                 (CARRY1((byte)R15R14,(byte)R25R24) || CARRY1((byte)R15R14 + (byte)R25R24,bVar13));
  R15R14._0_1_ = bVar2;
  if ((R15R14._1_1_ & 0x80) != 0) {
    bVar13 = bVar1 < R1 || (byte)(bVar1 - R1) < (cVar3 == '\0');
    R13R12 = CONCAT11(bVar1 - (R1 + (cVar3 == '\0')),cVar3 + -1);
    R15R14._1_1_ = R15R14._1_1_ - ((bVar2 < R1 || (byte)(bVar2 - R1) < bVar13) + -0x80);
    R15R14._0_1_ = bVar2 - (R1 + bVar13);
  }
  *puVar11 = (byte)R13R12;
  Y[1] = R13R12._1_1_;
  Y[2] = (byte)R15R14;
  Y[3] = R15R14._1_1_;
  uVar8 = R13R12;
  R23R22 = R13R12;
  uVar4 = CONCAT11(R15R14._1_1_,(byte)R15R14);
  R25R24 = (undefined1 *)(uVar4 & 0x7fff);
  Y = (undefined1 *)uVar12;
  R15R14 = uVar10;
  R13R12 = uVar7;
  R11R10 = uVar6;
  R9R8 = uVar5;
  return CONCAT22(uVar4,uVar8) & 0x7fffffff;
}



ulong FUN_code_000872(void)

{
  byte bVar1;
  byte bVar2;
  char cVar3;
  uint uVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  undefined4 uVar9;
  undefined2 uVar10;
  bool bVar11;
  
  uVar10 = R15R14;
  uVar7 = R13R12;
  uVar6 = R11R10;
  uVar5 = R9R8;
  FUN_code_0008e0();
  R9R8 = R19R18;
  R11R10 = R21R20;
  _R13R12 = FUN_code_0008ff(R25R24,R23R22,R25R24,R23R22);
  R25R24 = (uint)((ulong)_R13R12 >> 0x10);
  R23R22 = (undefined2)_R13R12;
  FUN_code_00090d(R25R24,R23R22,R11R10,R9R8);
  uVar9 = _R13R12;
  cVar3 = (byte)R13R12 + (byte)R23R22;
  bVar1 = R13R12._1_1_ + R23R22._1_1_ + CARRY1((byte)R13R12,(byte)R23R22);
  bVar11 = CARRY1(R13R12._1_1_,R23R22._1_1_) ||
           CARRY1(R13R12._1_1_ + R23R22._1_1_,CARRY1((byte)R13R12,(byte)R23R22));
  R13R12 = CONCAT11(bVar1,cVar3);
  R15R14._0_1_ = SUB41(uVar9,2);
  bVar2 = (byte)R15R14 + (byte)R25R24 + bVar11;
  R15R14._1_1_ = SUB41(uVar9,3);
  R15R14._1_1_ = R15R14._1_1_ + R25R24._1_1_ +
                 (CARRY1((byte)R15R14,(byte)R25R24) || CARRY1((byte)R15R14 + (byte)R25R24,bVar11));
  R15R14._0_1_ = bVar2;
  if ((R15R14._1_1_ & 0x80) != 0) {
    bVar11 = bVar1 < R1 || (byte)(bVar1 - R1) < (cVar3 == '\0');
    R13R12 = CONCAT11(bVar1 - (R1 + (cVar3 == '\0')),cVar3 + -1);
    R15R14._1_1_ = R15R14._1_1_ - ((bVar2 < R1 || (byte)(bVar2 - R1) < bVar11) + -0x80);
    R15R14._0_1_ = bVar2 - (R1 + bVar11);
  }
  uVar8 = R13R12;
  DAT_mem_0104 = (byte)R13R12;
  DAT_mem_0105 = R13R12._1_1_;
  DAT_mem_0106 = (byte)R15R14;
  DAT_mem_0107 = R15R14._1_1_;
  R23R22 = R13R12;
  uVar4 = CONCAT11(R15R14._1_1_,(byte)R15R14);
  R25R24 = uVar4 & 0x7fff;
  R15R14 = uVar10;
  R13R12 = uVar7;
  R11R10 = uVar6;
  R9R8 = uVar5;
  return CONCAT22(uVar4,uVar8) & 0x7fffffff;
}



undefined4 FUN_code_0008c1(undefined2 R25R24,undefined2 R23R22)

{
  DAT_mem_0104 = (undefined1)R23R22;
  DAT_mem_0105 = R23R22._1_1_;
  DAT_mem_0106 = (undefined1)R25R24;
  DAT_mem_0107 = R25R24._1_1_;
  _R23R22 = CONCAT13(R25R24._1_1_,CONCAT12((undefined1)R25R24,R23R22));
  return _R23R22;
}



undefined1 FUN_code_0008ca(undefined2 R25R24)

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



int FUN_code_0008d2(int R25R24,undefined1 R22)

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



void FUN_code_0008e0(undefined2 R25R24,undefined2 param_2,undefined2 R21R20)

{
  undefined1 uVar1;
  
  uVar1 = R25R24._1_1_ >> 7;
  if ((bool)uVar1) {
    FUN_code_0008f7();
  }
  if ((R21R20._1_1_ & 0x80) != 0) {
    FUN_code_0008ef();
  }
  FUN_code_000912();
  if ((R0 & 0x80) != 0) {
    FUN_code_0008ef();
  }
  if ((bool)uVar1) {
    FUN_code_0008f7();
    return;
  }
  return;
}



void FUN_code_0008ef(undefined2 param_1,undefined2 param_2,undefined2 R21R20,undefined2 R19R18)

{
  byte bVar1;
  bool bVar2;
  bool bVar3;
  
  R21R20._1_1_ = ~R21R20._1_1_;
  R21R20._0_1_ = ~(byte)R21R20;
  R19R18._1_1_ = ~R19R18._1_1_;
  R19R18._0_1_ = -(char)R19R18;
  bVar3 = R19R18._1_1_ != 0xff || (byte)(R19R18._1_1_ + 1) < ((char)R19R18 != '\0');
  R19R18._1_1_ = R19R18._1_1_ - (((char)R19R18 != '\0') + -1);
  bVar1 = (byte)R21R20 + 1;
  bVar2 = (byte)R21R20 != 0xff;
  R21R20._0_1_ = (byte)R21R20 - (bVar3 + -1);
  R21R20._1_1_ = R21R20._1_1_ - ((bVar2 || bVar1 < bVar3) + -1);
  return;
}



undefined4 FUN_code_0008f7(undefined2 R25R24,undefined2 R23R22)

{
  byte bVar1;
  char cVar2;
  char cVar3;
  bool bVar4;
  bool bVar5;
  
  R25R24._1_1_ = ~R25R24._1_1_;
  R25R24._0_1_ = ~(byte)R25R24;
  R23R22._1_1_ = ~R23R22._1_1_;
  R23R22._0_1_ = -(char)R23R22;
  cVar2 = R23R22._1_1_ - (((char)R23R22 != '\0') + -1);
  bVar5 = R23R22._1_1_ != 0xff || (byte)(R23R22._1_1_ + 1) < ((char)R23R22 != '\0');
  R23R22._1_1_ = cVar2;
  bVar1 = (byte)R25R24 + 1;
  cVar3 = (byte)R25R24 - (bVar5 + -1);
  bVar4 = (byte)R25R24 != 0xff;
  R25R24._0_1_ = cVar3;
  R25R24._1_1_ = R25R24._1_1_ - ((bVar4 || bVar1 < bVar5) + -1);
  R23R22 = CONCAT11(cVar2,(char)R23R22);
  _R23R22 = CONCAT12(cVar3,R23R22);
  return _R23R22;
}



undefined2 FUN_code_0008ff(void)

{
  char cVar1;
  bool bVar2;
  
  FUN_code_000934();
  R1R0 = (uint)Xlo * (uint)R20;
  cVar1 = (byte)R25R24 + (byte)R1R0;
  bVar2 = CARRY1((byte)R25R24,(byte)R1R0);
  R25R24._0_1_ = cVar1;
  R25R24._1_1_ = R25R24._1_1_ + Xlo * R21 + Xhi * R20 + R1R0._1_1_ + bVar2;
  R1R0 = R1R0 & 0xff;
  R25R24 = CONCAT11(R25R24._1_1_,cVar1);
  return R25R24;
}



undefined2 FUN_code_00090d(void)

{
  char cVar1;
  bool bVar2;
  
  FUN_code_0008ff();
  cVar1 = (byte)R25R24 - R18;
  bVar2 = (byte)R25R24 < R18;
  R25R24._0_1_ = cVar1;
  R25R24._1_1_ = R25R24._1_1_ - (R19 + bVar2);
  R25R24 = CONCAT11(R25R24._1_1_,cVar1);
  return R25R24;
}



undefined8 FUN_code_000912(undefined2 R25R24,undefined2 R23R22,undefined2 R21R20,undefined2 R19R18)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  bool bVar7;
  bool bVar8;
  bool bVar9;
  
  R1 = '!';
  bVar7 = false;
  X = 0;
  Z = 0;
  while( true ) {
    bVar1 = (byte)R23R22 * '\x02' + bVar7;
    bVar7 = CARRY1((byte)R23R22,(byte)R23R22) || CARRY1((byte)R23R22 * '\x02',bVar7);
    bVar2 = R23R22._1_1_ * '\x02' + bVar7;
    bVar7 = CARRY1(R23R22._1_1_,R23R22._1_1_) || CARRY1(R23R22._1_1_ * '\x02',bVar7);
    R23R22 = CONCAT11(bVar2,bVar1);
    bVar3 = (byte)R25R24 * '\x02' + bVar7;
    bVar7 = CARRY1((byte)R25R24,(byte)R25R24) || CARRY1((byte)R25R24 * '\x02',bVar7);
    bVar4 = R25R24._1_1_ * '\x02' + bVar7;
    bVar7 = CARRY1(R25R24._1_1_,R25R24._1_1_) || CARRY1(R25R24._1_1_ * '\x02',bVar7);
    R25R24 = CONCAT11(bVar4,bVar3);
    R1 = R1 + -1;
    if (R1 == '\0') break;
    bVar1 = (byte)X * '\x02' + bVar7;
    bVar7 = CARRY1((byte)X,(byte)X) || CARRY1((byte)X * '\x02',bVar7);
    bVar2 = X._1_1_ * '\x02' + bVar7;
    bVar7 = CARRY1(X._1_1_,X._1_1_) || CARRY1(X._1_1_ * '\x02',bVar7);
    X = CONCAT11(bVar2,bVar1);
    bVar3 = (byte)Z * '\x02' + bVar7;
    bVar4 = Z._1_1_ * '\x02' + (CARRY1((byte)Z,(byte)Z) || CARRY1((byte)Z * '\x02',bVar7));
    Z = CONCAT11(bVar4,bVar3);
    bVar8 = (byte)(bVar4 - R21R20._1_1_) <
            (bVar3 < (byte)R21R20 ||
            (byte)(bVar3 - (byte)R21R20) <
            (bVar2 < R19R18._1_1_ || (byte)(bVar2 - R19R18._1_1_) < (bVar1 < (byte)R19R18)));
    bVar7 = bVar4 < R21R20._1_1_ || bVar8;
    if (bVar4 >= R21R20._1_1_ && !bVar8) {
      bVar8 = bVar2 < R19R18._1_1_ || (byte)(bVar2 - R19R18._1_1_) < (bVar1 < (byte)R19R18);
      X = CONCAT11(bVar2 - (R19R18._1_1_ + (bVar1 < (byte)R19R18)),bVar1 - (byte)R19R18);
      bVar9 = bVar3 < (byte)R21R20 || (byte)(bVar3 - (byte)R21R20) < bVar8;
      bVar7 = bVar4 < R21R20._1_1_ || (byte)(bVar4 - R21R20._1_1_) < bVar9;
      Z = CONCAT11(bVar4 - (R21R20._1_1_ + bVar9),bVar3 - ((byte)R21R20 + bVar8));
    }
  }
  R23R22._0_1_ = ~bVar1;
  R23R22._1_1_ = ~bVar2;
  uVar5 = R23R22;
  R25R24._0_1_ = ~bVar3;
  R25R24._1_1_ = ~bVar4;
  uVar6 = R25R24;
  R19R18 = R23R22;
  R21R20 = R25R24;
  R23R22 = X;
  R25R24 = Z;
  _R19R18 = CONCAT26(Z,CONCAT24(X,CONCAT22(uVar6,uVar5)));
  return _R19R18;
}



undefined4 FUN_code_000934(void)

{
  char cVar1;
  byte bVar2;
  bool bVar3;
  bool bVar4;
  byte bVar5;
  int iVar6;
  int iVar7;
  bool bVar8;
  bool bVar9;
  
  R1R0 = (uint)Xlo * (R19R18 & 0xff);
  iVar6 = R1R0;
  R1R0 = (uint)Xhi * (uint)R19R18._1_1_;
  iVar7 = R1R0;
  R1R0 = (uint)Xlo * (uint)R19R18._1_1_;
  R23R22._1_1_ = (byte)((uint)iVar6 >> 8);
  bVar5 = R23R22._1_1_ + (byte)R1R0;
  bVar8 = CARRY1(R23R22._1_1_,(byte)R1R0);
  R25R24._0_1_ = (byte)iVar7;
  bVar2 = (byte)R25R24 + R1R0._1_1_ + bVar8;
  bVar3 = CARRY1((byte)R25R24,R1R0._1_1_);
  R25R24._0_1_ = (byte)R25R24 + R1R0._1_1_;
  R25R24._1_1_ = (char)((uint)iVar7 >> 8);
  R1R0 = (uint)Xhi * (R19R18 & 0xff);
  bVar9 = CARRY1(bVar5,(byte)R1R0);
  R23R22 = CONCAT11(bVar5 + (byte)R1R0,(char)iVar6);
  cVar1 = bVar2 + R1R0._1_1_;
  bVar4 = CARRY1(bVar2,R1R0._1_1_);
  bVar2 = bVar2 + R1R0._1_1_;
  R1R0 = R1R0 & 0xff;
  R25R24 = CONCAT11(R25R24._1_1_ + (bVar3 || CARRY1((byte)R25R24,bVar8)) +
                    (bVar4 || CARRY1(bVar2,bVar9)),cVar1 + bVar9);
  return CONCAT22(R25R24,R23R22);
}



undefined2 FUN_code_000943(undefined2 R25R24,undefined2 R23R22)

{
  undefined1 uVar1;
  
  uVar1 = R25R24._1_1_ >> 7;
  if ((bool)uVar1) {
    FUN_code_00094f();
  }
  if ((R23R22._1_1_ & 0x80) != 0) {
    FUN_code_000953();
  }
  FUN_code_000957();
  if ((R0 & 0x80) != 0) {
    FUN_code_000953();
  }
  if ((bool)uVar1) {
    R25R24._1_1_ = ~R25R24._1_1_;
    R25R24._0_1_ = -(char)R25R24;
    R25R24._1_1_ = R25R24._1_1_ - (((char)R25R24 != '\0') + -1);
    R25R24 = CONCAT11(R25R24._1_1_,(char)R25R24);
    return R25R24;
  }
  R25R24 = CONCAT11(R25R24._1_1_,(char)R25R24);
  return R25R24;
}



undefined2 FUN_code_00094f(undefined2 R25R24)

{
  R25R24._1_1_ = ~R25R24._1_1_;
  R25R24._0_1_ = -(char)R25R24;
  R25R24._1_1_ = R25R24._1_1_ - (((char)R25R24 != '\0') + -1);
  R25R24 = CONCAT11(R25R24._1_1_,(char)R25R24);
  return R25R24;
}



void FUN_code_000953(undefined2 param_1,undefined2 R23R22)

{
  R23R22._1_1_ = ~R23R22._1_1_;
  R23R22._0_1_ = -(char)R23R22;
  R23R22._1_1_ = R23R22._1_1_ - (((char)R23R22 != '\0') + -1);
  return;
}



undefined4 FUN_code_000957(undefined2 R25R24,undefined2 R23R22)

{
  bool bVar1;
  byte bVar2;
  byte bVar3;
  undefined2 uVar4;
  bool bVar5;
  
  bVar5 = false;
  X = 0;
  R21 = '\x11';
  while( true ) {
    bVar2 = (byte)R25R24 * '\x02' + bVar5;
    bVar5 = CARRY1((byte)R25R24,(byte)R25R24) || CARRY1((byte)R25R24 * '\x02',bVar5);
    bVar3 = R25R24._1_1_ * '\x02' + bVar5;
    bVar5 = CARRY1(R25R24._1_1_,R25R24._1_1_) || CARRY1(R25R24._1_1_ * '\x02',bVar5);
    R25R24 = CONCAT11(bVar3,bVar2);
    R21 = R21 + -1;
    if (R21 == '\0') break;
    bVar2 = (byte)X * '\x02' + bVar5;
    bVar3 = X._1_1_ * '\x02' + (CARRY1((byte)X,(byte)X) || CARRY1((byte)X * '\x02',bVar5));
    X = CONCAT11(bVar3,bVar2);
    bVar1 = (byte)(bVar3 - R23R22._1_1_) < (bVar2 < (byte)R23R22);
    bVar5 = bVar3 < R23R22._1_1_ || bVar1;
    if (bVar3 >= R23R22._1_1_ && !bVar1) {
      bVar5 = bVar3 < R23R22._1_1_ || (byte)(bVar3 - R23R22._1_1_) < (bVar2 < (byte)R23R22);
      X = CONCAT11(bVar3 - (R23R22._1_1_ + (bVar2 < (byte)R23R22)),bVar2 - (byte)R23R22);
    }
  }
  R25R24._0_1_ = ~bVar2;
  R25R24._1_1_ = ~bVar3;
  uVar4 = R25R24;
  R23R22 = R25R24;
  R25R24 = X;
  R21 = 0;
  return CONCAT22(X,uVar4);
}



void FUN_code_00096b(void)

{
  Z = CONCAT11(Z._1_1_ * '\x02' + CARRY1((byte)Z,(byte)Z),(byte)Z * '\x02');
  R0 = *(undefined1 *)(uint3)Z;
  Z = Z + 1;
  Z._1_1_ = *(undefined1 *)(uint3)Z;
  Z = CONCAT11(Z._1_1_,R0);
                    // WARNING: Could not recover jumptable at 0x000970. Too many branches
                    // WARNING: Treating indirect jump as call
  (*(code *)(uint3)Z)();
  return;
}



void FUN_code_000971(void)

{
  do {
                    // WARNING: Do nothing block with infinite loop
  } while( true );
}


