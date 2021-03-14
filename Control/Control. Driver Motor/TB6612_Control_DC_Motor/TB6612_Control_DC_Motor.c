//// This code run 1 motor by TB6612, control speed PWM by Button UP/DOWN, from 0-100 %
//// You can change rotate's motor by push button LEFT (CCW) and RIGHT (CW)
//// And Stop motor when push button SET
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
 * To use with KIT DEV PIC (DRIVER)
 * 
 * 1. Connect jumper for PWM of Driver
 * CCP1 of RC2 (pin C2) <-> PWMA
 *
 * 2. Connect jumper for Driver Motor
 * RB3 (pin B3) : B3 <-> IN2 (AIN1 = IN1 Motor A)
 * RB4 (pin B4) : B4 <-> IN1 (AIN2 = IN2 Motor A)
 */
#define AIN1 PIN_B3
#define AIN2 PIN_B4

/* 
 * 3. Connect jumper for Button
 * RA1 (Pin A1) <-> SW5 (Button SET)
 * RA2 (Pin A2) <-> SW4 (Button DOWN)
 * RA3 (Pin A3) <-> SW3 (Button UP)
 * RA4 (Pin A4) <-> SW2 (Button RIGHT)
 * RA5 (Pin A5) <-> SW1 (Button LEFT)
 */
#define BUTTON_STOP       PIN_A1  // Button SET
#define BUTTON_DOWN_SPEED PIN_A2  // Button DOWN
#define BUTTON_UP_SPEED   PIN_A3  // Button UP
#define BUTTON_CW         PIN_A4  // Button RIGHT
#define BUTTON_CCW        PIN_A5  // Button LEFT

/*
 * 4. Connect jumper for LegSeg (74HC595)
 * ST_CP (RCLK) <-> E2 <-> RE2 (Pin E2)
 * SH_CP (SCLK) <-> E1 <-> RE1 (Pin E1)
 * DS (DIO)     <-> E0 <-> RE0 (Pin E0)
 * 
 * Setup all pins below is input 74HC595, is output PIC
 */
#define DIGIT_ONE       0b00001000
#define DIGIT_TEN       0b00000100
#define DIGIT_HUNDRED   0b00000010
#define TURN_OFF_DIGIT  0b00000000
#define TURN_OFF_LED    0b11111111

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
  for(signed int8 i=7; i>=0; i--)
  { // Transfer first bit from MSB to LSB of data Led
    output_bit(PIN_DATA, bit_test(data_led, i));
    clock_signal(); // Allows bit-by-bit transfer
  }
  for(signed int8 i=7; i>=0; i--)
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

/* Make motor Stop */
void stop_motor()
{
  output_low(AIN1); output_low(AIN2);
}

/* Make motor run with specifications of "speed" and "direction" */
void run_motor(int16 speed, int1 direction)
{
  set_pwm1_duty(speed*10);                              // Converter value percent (0-100%) to value for PWM (0-1000)
  if(direction) {output_high(AIN1); output_low(AIN2);}  // Rotate CW
  else          {output_low(AIN1); output_high(AIN2);}  // Rotate CCW
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

  setup_ccp1(CCP_PWM);  // Configure CCP1 as a PWM, use PIN_RC2 for PWMA
  /* This config will make PWM frequency 1250Hz, and 100% duty cycle have value is 1000 */
  setup_timer_2(T2_DIV_BY_16, 249, 1);

  int16 speed = 0;      // Have value range from 0-100, unit (%)
  int1 direction = 1;   // (Default begin) HIGH mean rotate CW, and LOW mean rotate CCW

  while(true)
  {
    if(!input_state(BUTTON_STOP))             {stop_motor(); delay_ms(5);}
    else if(!input_state(BUTTON_DOWN_SPEED))  {if(speed>0) speed--;   run_motor(speed, direction); delay_ms(5);}
    else if(!input_state(BUTTON_UP_SPEED))    {if(speed<100) speed++; run_motor(speed, direction); delay_ms(5);}
    else if(!input_state(BUTTON_CW))          {direction=1;           run_motor(speed, direction); delay_ms(5);}
    else if(!input_state(BUTTON_CCW))         {direction=0;           run_motor(speed, direction); delay_ms(5);}

    check = 1;
    while(check)
    { // Wait until 0.1s
      int16 temp = speed;
      send_data(seg_code[temp%10], DIGIT_ONE); temp = temp/10;  delay_ms(1);
      send_data(seg_code[temp%10], DIGIT_TEN); temp = temp/10;  delay_ms(1);
      send_data(seg_code[temp], DIGIT_HUNDRED);                 delay_ms(1);
    }
  }
}
