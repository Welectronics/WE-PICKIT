//// This code make all Led blink 1s together
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
  while(true)
  {
    output_d(0xFF); // Turn off led
    delay_ms(1000);
    output_d(0x00); // Turn on led
    delay_ms(1000);
  }
}
