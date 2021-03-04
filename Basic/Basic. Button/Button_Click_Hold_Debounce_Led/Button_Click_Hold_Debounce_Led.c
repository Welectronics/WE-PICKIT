//// This code tests of process Debounce of button, it will blink Led 1s when button holding
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
 * 
 * Connect jumper
 * RA1 (Pin A1) <-> SW5 (SET)
 * RD5 (Pin D7) <-> L1 (Led 1)
 */
#define BUTTON_SET  PIN_A1
#define LED_1       PIN_D7

void main()
{
  while(true)
  {
    if(!input(BUTTON_SET))    // If the button is pressed
    {
      delay_ms(100);          // Button Debounce
      if(!input(BUTTON_SET))  // If the button is still pressed
      {
        output_high(LED_1);   // Led on
        delay_ms(1000);
        output_low(LED_1);    // Led off
      }
    }
  }
}
