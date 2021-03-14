//// This code display on Led Seg 4 digit, it will display number from 0 to 9 and A to F, for all 4 digit by method ScanLed
//// Display 1 number in 1s, use Counter (tick)
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
#use timer(timer=1, tick=1ms, bits=16, noisr)

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
#define DIGIT_1   output_b(0b11100000)
#define DIGIT_2   output_b(0b11010000)
#define DIGIT_3   output_b(0b10110000)
#define DIGIT_4   output_b(0b01110000)
#define TURN_OFF  output_b(0b11110000)

// Use to know time past, unit (ms)
unsigned int16 tick_difference(unsigned int16 current, unsigned int16 previous)
{
  return(current - previous);
}

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
    0x90, // 9
    0x88, // A
    0x83, // B
    0xC6, // C
    0xA1, // D
    0x86, // E
    0x8E  // F
  };

  unsigned int16 current_tick, previous_tick;
  current_tick = previous_tick = get_ticks();

  TURN_OFF; // Turn off all digit

  while(true)
  {
    for(int8 number=0x00; number<=0x0F; number++)
    {
      current_tick = get_ticks();
      while(tick_difference(current_tick, previous_tick) <= 1000)
      { // Running continuously for 1s
        output_d(seg_code[number]);
        DIGIT_1; delay_ms(1);
        DIGIT_2; delay_ms(1);
        DIGIT_3; delay_ms(1);
        DIGIT_4; delay_ms(1);
        current_tick = get_ticks();
      }
      previous_tick = current_tick;
    }
  }
}
