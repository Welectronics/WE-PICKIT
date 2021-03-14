//// This code to demo function Interrupt of Timer0
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
 * To use with KIT DEV PIC (TIMER0)
 * 
 * Connect jumper for Led
 * RD0 (Pin D0) <-> L8 (Led 8)
 */

unsigned int8 n;                // To do 100 times Interrupt of Timer0
/*
 * This directives specify the following function is an interrupt function
 * Interrupt fires when timer0 overflows
 */
#INT_TIMER0                     // Timer 0 (RTCC) overflow, 
void Timer0_isr(void)           // Do this function, when Interrupt happens
{
  n++;
  if(n > 99)                    // Do 100 times
  {
    n = 0;
    output_toggle(PIN_D0);      // To blink Led in 1s
  }
  set_timer0(61);               // Timer0 preload value, initializes the timer0 clock/counter, value 8 bit. All timers count up
  clear_interrupt(INT_TIMER0);  // Clear Timer0 overflow bit (interrupt flag of timer0)
}

void main()
{
  /*
   * PIC16F887 Microcontroller Timer0 module interrupt example CCS C code
   * Internal oscillator used @ 20MHz
   * Timer0 period = 9.984 ms
   * LED ON time = OFF time = 9.984 ms * 100 = 0.9984s ~ 1s
   *
   * Timer0_freq = MCU_freq / {4 * Prescaler * (256 - TMR0)}
   * 1/0.009984  = 20,000,000 / (4 * 256 * (256 - 61))
   */
  clear_interrupt(INT_TIMER0);              // Clear Timer0 overflow bit (interrupt flag of timer0)
  enable_interrupts(INT_TIMER0);            // Enable Timer0 interrupt
  enable_interrupts(GLOBAL);                // Enable global interrupts
  setup_timer_0(T0_INTERNAL | T0_DIV_256);  // Timer0 configuration: internal clock source + 256 prescaler
  set_timer0(61);                           // Timer0 preload value, initializes the timer0 clock/counter, value 8 bit. All timers count up

  while(true);                              // Endless loop
}
