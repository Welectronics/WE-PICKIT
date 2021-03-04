//// This code run 2 motor by TB6612, it make 2 motor rotate opposite together
//// When you push-button SET, it makes the motor "Stop"
//// When you push-button DOWN, it makes the motor "Short brake"
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
 * _ Button - It will receive LOW when the button pressed
 * 
 * 1. Connect jumper for PWM (not use) of Driver
 * CCP1 of RC2 (pin C2) <-> PWMA
 * CCP2 of RC1 (pin C1) <-> PWMB
 *
 * 2. Connect jumper for Driver Motor
 * RB1 (pin B1) : B1 <-> IN4 (BIN2 = IN2 Motor B)
 * RB2 (pin B2) : B2 <-> IN3 (BIN1 = IN1 Motor B)
 * RB3 (pin B3) : B3 <-> IN2 (AIN1 = IN1 Motor A)
 * RB4 (pin B4) : B4 <-> IN1 (AIN2 = IN2 Motor A)
 *
 * 3. Connect jumper for Button
 * RA1 (Pin A1) <-> SW5 (Button SET)
 * RA2 (Pin A2) <-> SW4 (Button DOWN)
 */
#define BIN2                PIN_B1  // Motor B
#define BIN1                PIN_B2
#define AIN1                PIN_B3  // Motor A
#define AIN2                PIN_B4

#define PWM_B               PIN_C1
#define PWM_A               PIN_C2

#define BUTTON_STOP         PIN_A1  // Button SET
#define BUTTON_SHORT_BRAKE  PIN_A2  // Button DOWN

void main()
{
  /* Motor A rotate CW and motor B rotate CCW */
  output_high(AIN1); output_low(AIN2); output_high(PWM_A);
  output_high(BIN2); output_low(BIN1); output_high(PWM_B);
  int1 stateStop=1, stateShortBrake=1, stateRotate=0;
  while(true)
  {
    if(!input_state(BUTTON_STOP) && stateStop)
    {
      output_low(AIN1); output_low(AIN2);
      output_low(BIN1); output_low(BIN2);
      stateStop = 0; stateShortBrake = 1; stateRotate = 1;
    }
    else if (!input_state(BUTTON_SHORT_BRAKE) && stateShortBrake)
    {
      output_low(PWM_A);
      output_low(PWM_B);
      stateStop = 1; stateShortBrake = 0; stateRotate = 1;
    }
    else if (stateRotate)
    {
      output_high(AIN1); output_low(AIN2); output_high(PWM_A);
      output_high(BIN2); output_low(BIN1); output_high(PWM_B);
      stateStop = 1; stateShortBrake = 1; stateRotate = 0;
    }
  }
}
