//// This code makes effect snake run by Led
#include <16F877A.h>

//// Fuses: LP, XT, HS, RC
//// NOWDT, WDT
//// PUT, NOPUT
//// NOBROWNOUT, BROWNOUT
//// NOLVP, LVP
//// CPD, NOCPD
//// WRT_1000, WRT_800, WRT_100, NOWRT
//// DEBUG, NODEBUG
//// PROTECT, NOPROTECT
#fuses HS, NOWDT, NOPROTECT, NOBROWNOUT, NOLVP
#use delay(clock=20MHZ)

/*
 * To use with KIT DEV PIC (Led)
 *
 * Connect jumper for all pin of Port D, we need logic LOW to turn on Led, and vice versa
 * RD0 (Pin D0) <-> L8 (Led 8)
 * RD0 (Pin D1) <-> L7 (Led 7)
 * RD0 (Pin D2) <-> L6 (Led 6)
 * RD0 (Pin D3) <-> L5 (Led 5)
 * RD0 (Pin D4) <-> L4 (Led 4)
 * RD0 (Pin D5) <-> L3 (Led 3)
 * RD0 (Pin D6) <-> L2 (Led 2)
 * RD0 (Pin D7) <-> L1 (Led 1)
 */

void main()
{  
  byte snake = 0b00000111;
  
  while(true)
  {
    if(bit_test(snake,7)) // The last bit is High, mean the snake had come there
    {
      snake<<=1;          //
      snake |= 0x01;      // After the snake go overflow, we need display the head in the first bit
    }
    else
    {
      snake<<=1;          //
    }
    output_d(~snake);     // To display on led, we need reverse the bits
    delay_ms(500);
  }
}
