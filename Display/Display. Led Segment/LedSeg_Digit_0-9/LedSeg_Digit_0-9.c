//// This code display on Led Seg 4 digit, it will display number from 0 to 9, for all 4 digit, but not use ScanLed or ShiftRegister
//// Display 1 number in 1s, use Delay
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
 * To use with KIT DEV PIC (Led Seg Common Anode)
 * 
 * 1. Connect wire port "LED A" for led 'a' to 'g'
 * 
 * 2. Connect jumper for 4 digit
 * Jumer LED1 <-> RB4 (Pin B4)
 * Jumer LED2 <-> RB5 (Pin B5)
 * Jumer LED3 <-> RB6 (Pin B6)
 * Jumer LED4 <-> RB7 (Pin B7)
 * 
 * The led 'a' to 'g' need logic LOW
 * And the digit need Vcc, in the kit we use PNP therefore we need logic LOW to turn on digit
 */
#define DIGIT_1 PIN_B4
#define DIGIT_2 PIN_B5
#define DIGIT_3 PIN_B6
#define DIGIT_4 PIN_B7

void main()
{
  char seg_code[] = {
    0xC0, // 0
    0xF9, // 1
    0xA4, // 2
    0xB0, // 3
    0x99, // 4
    0x92, // 5
    0x82, // 6
    0xF8, // 7
    0x80, // 8
    0x90  // 9
  };

  output_low(DIGIT_1);  // On Digit 1
  output_low(DIGIT_2);  // On Digit 2
  output_low(DIGIT_3);  // On Digit 3
  output_low(DIGIT_4);  // On Digit 4

  while(true) 
  {
    /* Note: because we for light all led, therefore the light is low */
    for (int8 i=0; i<=9; i++)
    { // Display digit form 0-9 for all Digit
      output_d(seg_code[i]);
      delay_ms(1000);
    }
  }
}
