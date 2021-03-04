//// This code to demo function Interrupt of Timer1
//// It make blink Led 1s without use delay
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
 * To use with KIT DEV PIC (TIMER1)
 * 
 * Connect jumper for Led
 * RD0 (Pin D0) <-> L8 (Led 8)
 */

unsigned int8 n;                // To do 10 times Interrupt of Timer1
/*
 * This directives specify the following function is an interrupt function
 * Interrupt fires when timer1 overflows
 */
#INT_TIMER1                     // Timer 1 overflow
void Timer1_isr(void)           // Do this function, when Interrupt happens
{
  n++;
  if(n > 9)                     // Do 10 times
  {
    n = 0;
    output_toggle(PIN_D0);      // To blink Led 1s
  }
  set_timer1(3036);             // Timer1 preload value, initializes the timer1 clock/counter, value 16 bit. All timers count up
  clear_interrupt(INT_TIMER1);  // Clear Timer1 overflow bit (interrupt flag of timer1)
}

void main()
{
  /*
   * PIC16F887 Microcontroller Timer1 module interrupt example CCS C code
   * Internal oscillator used @ 20MHz
   * Timer1 period = 0.1s
   * LED ON time = OFF time = 0.1s * 10 = 1s
   *
   * Timer1_freq = MCU_freq / {4 * Prescaler * (65536 - TMR1)}
   * 1/0.1       = 20,000,000 / (4 * 8 * (65536 - 3036))
   */
  clear_interrupt(INT_TIMER1);              // Clear Timer1 overflow bit (interrupt flag of timer1)
  enable_interrupts(INT_TIMER1);            // Enable Timer1 interrupt
  enable_interrupts(GLOBAL);                // Enable global interrupts
  setup_timer_1(T1_INTERNAL | T1_DIV_BY_8); // Timer1 configuration: internal clock source + 8 prescaler
  set_timer1(3036);                         // Timer1 preload value, initializes the timer1 clock/counter, value 16 bit. All timers count up

  while(true);                              // Endless loop
}
