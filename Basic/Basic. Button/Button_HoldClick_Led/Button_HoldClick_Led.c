// This code control Led by Button, when button push it will make Led turn on
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
 * To use with KIT DEV PIC (Button)
 * _ Button - A1/A2/A3/A4/A5 of Port A for Button SET/DOWN/UP/RIGHT/UP, it will receive LOW when the button pressed
 * _ Led - D1/D2/D3/D4/D5 of Port D for Led_7/Led_6/Led_5/Led_4/Led_3, it need output LOW to turn on the led
 * 
 * Connect jumper for Led
 * RD1 (Pin D1) <-> L7
 * RD2 (Pin D2) <-> L6
 * RD3 (Pin D3) <-> L5
 * RD4 (Pin D4) <-> L4
 * RD5 (Pin D5) <-> L3
 * 
 * Connect jumper for Button
 * RA1 (Pin A1) <-> SW5 (SET)
 * RA2 (Pin A2) <-> SW4 (DOWN)
 * RA3 (Pin A3) <-> SW3 (UP)
 * RA4 (Pin A4) <-> SW2 (RIGHT)
 * RA5 (Pin A5) <-> SW1 (LEFT)
 */

void main()
{
  while(true)
  {
    /* Therefore when you preseed button, the led will turn on */
    output_d(input_a());
  }
}
