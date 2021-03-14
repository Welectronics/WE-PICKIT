//// This code run 2 motor by TB6612, control speed PWM
//// It make to 2 motor DC rotation CW with speed up, and then rotation CCW with speed down, and versa again
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
 * CCP2 of RC1 (pin C1) <-> PWMB
 *
 * 2. Connect jumper for Driver Motor
 * RB1 (pin B1) : B1 <-> IN4 (BIN2 = IN2 Motor B)
 * RB2 (pin B2) : B2 <-> IN3 (BIN1 = IN1 Motor B)
 * RB3 (pin B3) : B3 <-> IN2 (AIN1 = IN1 Motor A)
 * RB4 (pin B4) : B4 <-> IN1 (AIN2 = IN2 Motor A)
 */
#define BIN2 PIN_B1
#define BIN1 PIN_B2
#define AIN1 PIN_B3
#define AIN2 PIN_B4

long count = 0;
void main()
{
  setup_ccp1(CCP_PWM);  // Configure CCP1 as a PWM, use PIN_RC2 for PWMA
  setup_ccp2(CCP_PWM);  // Configure CCP2 as a PWM, use PIN_RC1 for PWMB
  /* This config will make PWM frequency 1250Hz, and 100% duty cycle have value is 1000 */
  setup_timer_2(T2_DIV_BY_16, 249, 1);

  while(true)
  {
    /* Rotate CW with SpeedUp */
    output_high(AIN1); output_low(AIN2);
    output_high(BIN1); output_low(BIN2);
    for(count=0; count<=1000; count++)
    {
      set_pwm1_duty(count);
      set_pwm2_duty(count);
      delay_ms(7);
    }
    /* Rotate CCW with SpeedDown */
    output_high(AIN2); output_low(AIN1);
    output_high(BIN2); output_low(BIN1);
    for(count=0; count<=1000; count++)
    {
      set_pwm1_duty(1000-count);
      set_pwm2_duty(1000-count);
      delay_ms(7);
    }
  }
}
