//// This code display on Led Seg 4 digit, it will display number from 0000 to 9999 by method ScanLed + ShiftRegister
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
 * To use with KIT DEV PIC (Led Seg Common Anode, and 74HC595 Serial Shift Register)
 * 
 * 1. Connect jumper for 3 pin
 * ST_CP (RCLK) <-> E2 <-> RE2 (Pin E2)
 * SH_CP (SCLK) <-> E1 <-> RE1 (Pin E1)
 * DS (DIO)     <-> E0 <-> RE0 (Pin E0)
 * 
 * Because this LedSeg Common Anode, therefore we logic HIGH to turn on digit
 * And logic LOW to turn on led
 * 
 * This is sequence of sending data, the first bit come to Q0 of (U1) and after send 16bit it pushed to Q7 of (U2)
 *    |--------------- U2 ---------------|     |--------------- U1 ---------------|
 * -> Q7 > Q6 > Q5 > Q4 > Q3 > Q2 > Q1 > Q0 -> Q7 > Q6 > Q5 > Q4 > Q3 > Q2 > Q1 > Q0
 *    |                                                                           |
 *    First bit transferred                                                       Last bit transferred
 *    |
 *    H  > G  > F  > E  > D  > C  > B  > A  -> 0  > 0  > 0  > 0  > ONE (digit 4)
 *    |                                        |                      > TEN (digit 3)
 *    |                                        |                           > HUNDRED (digit 2)
 *    |                                        |                                > THOUSAND (digit 1)
 *    8 LED                                    4 DIGIT
 */
#define DIGIT_ONE       0b00001000
#define DIGIT_TEN       0b00000100
#define DIGIT_HUNDRED   0b00000010
#define DIGIT_THOUSAND  0b00000001
#define TURN_OFF_DIGIT  0b00000000
#define TURN_OFF_LED    0b11111111

/* Setup all pins below is input 74HC595, is output PIC */
#define PIN_CLOCK PIN_E2  // Pin ST_CP (RCLK)
#define PIN_LATCH PIN_E1  // Pin SH_CP (SCLK)
#define PIN_DATA  PIN_E0  // Pin DS (DIO)

// -------------------------------------------------------------------------------------------------- //

/* With 1 pulse over, logic value at PIN_DATA went to the "Shift Register" */
void clock_signal()
{
  output_high(PIN_CLOCK);
  output_low(PIN_CLOCK);
}

/* With 1 pulse over, logic value of the "Shift Register" transferred to the "Storage Register" */
void latch_enable()
{
  output_high(PIN_LATCH);
  output_low(PIN_LATCH);
}

/* Take every bit of data, then transfer to the "Shift Register", and then transfer to the "Storage Register" */
void send_data(int8 data_led, int8 data_digit)
{
  signed int8 i;
  for(i=7; i>=0; i--)
  { // Transfer first bit from MSB to LSB of data Led
    output_bit(PIN_DATA, bit_test(data_led, i));
    clock_signal(); // Allows bit-by-bit transfer
  }
  for(i=7; i>=0; i--)
  { // Transfer first bit from MSB to LSB of data Digit
    output_bit(PIN_DATA, bit_test(data_digit, i));
    clock_signal(); // Allows bit-by-bit transfer
  }
  latch_enable();   // Data finally submitted
}

// -------------------------------------------------------------------------------------------------- //

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

// -------------------------------------------------------------------------------------------------- //

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

  send_data(TURN_OFF_LED , TURN_OFF_DIGIT); // Turn off all digit
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
      check = 1;
      while(check)
      { // Wait until 0.1s
        int16 temp = number;
        send_data(seg_code[temp%10], DIGIT_ONE);      temp = temp/10; delay_ms(1);
        send_data(seg_code[temp%10], DIGIT_TEN);      temp = temp/10; delay_ms(1);
        send_data(seg_code[temp%10], DIGIT_HUNDRED);  temp = temp/10; delay_ms(1);
        send_data(seg_code[temp], DIGIT_THOUSAND);                    delay_ms(1);
      }
    }
  }
}
