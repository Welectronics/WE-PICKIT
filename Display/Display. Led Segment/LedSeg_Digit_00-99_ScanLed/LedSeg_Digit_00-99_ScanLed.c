//// This code display on Led Seg 4 digit, it will display number from 0000 to 9999 by method ScanLed
//// Display 1 number in 0.1s, use Timer (Interrupt)
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
#define DIGIT_ONE       output_b(0b01110000)
#define DIGIT_TEN       output_b(0b10110000)
#define DIGIT_HUNDRED   output_b(0b11010000)
#define DIGIT_THOUSAND  output_b(0b11100000)
#define TURN_OFF        output_b(0b11110000)

/*
 * This directives specify the following function is an interrupt function
 * Interrupt fires when timer1 overflows
 */
int1 check;                     // To check when 0.1s passed
#INT_TIMER1                     // Timer 1 overflow
void Timer1_isr(void)           // Do this function, when Interrupt happens
{
  check = 0;
  set_timer1(3036);             // Timer1 preload value, initializes the timer1 clock/counter, value 16 bit. All timers count up
  clear_interrupt(INT_TIMER1);  // Clear Timer1 overflow bit (interrupt flag of timer1)
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
    0x90  // 9
  };

  TURN_OFF;                                 // Turn off all digit
  /*
   * Internal oscillator used @ 20MHz
   * Timer1 period = 0.1s
   *
   * Timer1_freq = MCU_freq / {4 * Prescaler * (65536 - TMR1)}
   * 1/0.1       = 20,000,000 / (4 * 8 * (65536 - 3036))
   *
   * Tolerance: 0
   */
  clear_interrupt(INT_TIMER1);              // Clear Timer1 overflow bit (interrupt flag of timer1)
  enable_interrupts(INT_TIMER1);            // Enable Timer1 interrupt
  enable_interrupts(GLOBAL);                // Enable global interrupts
  setup_timer_1(T1_INTERNAL | T1_DIV_BY_8); // Timer1 configuration: internal clock source + 8 prescaler
  set_timer1(3036);                         // Timer1 preload value, initializes the timer1 clock/counter, value 16 bit. All timers count up

  while(true)
  {
    for(int16 number=0; number<=9999; number++)
    {
      int16 temp = number;
      int16 unitThousand = temp / 1000; temp = temp - unitThousand*1000;
      int16 unitHundred = temp / 100; temp = temp - unitHundred*100;
      int16 unitTen = temp / 10;
      int16 unitOne = temp % 10;
      check = 1;
      while(check)
      { // Wait until 0.1s
        output_d(seg_code[unitOne]);
        DIGIT_ONE; delay_ms(1);
        output_d(seg_code[unitTen]);
        DIGIT_TEN; delay_ms(1);
        output_d(seg_code[unitHundred]);
        DIGIT_HUNDRED; delay_ms(1);
        output_d(seg_code[unitThousand]);
        DIGIT_THOUSAND; delay_ms(1);
      }
    }
  }
}
