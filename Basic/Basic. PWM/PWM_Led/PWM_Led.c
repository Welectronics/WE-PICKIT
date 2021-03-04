//// This code demo function of PWM to increase/decrease the light gradually of LED
//// Connect PIN_C1 and PIN_C2 with any LED you want. Here I connect like below:
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
 * To use with KIT DEV PIC (PWM)
 * _ Led - It need output LOW to turn on the led
 * 
 * Connect wire for Led
 * RC1 (Pin C1) <-> L1 (Led 1)
 * RC2 (Pin C2) <-> L2 (Led 2)
 */

void main()
{
  setup_ccp1(CCP_PWM);                  // Configure CCP1 as a PWM, use PIN_RC2
  setup_ccp2(CCP_PWM);                  // Configure CCP2 as a PWM, use PIN_RC1
  delay_ms(100);

  /*
   *                                         1
   * Pwm_Frequency = -------------------------------------------------
   *                                         1
   *                 [(PR2) + 1] x 4 x ------------- x (TMR2 Prescale)
   *                                   MCU_Frequency
   *
   * PR2 = 249
   * TMR2 Prescale = 16
   * MCU_Frequency = 20x10^6
   */
  setup_timer_2(T2_DIV_BY_16, 249, 1);  // Set PWM frequency to 1250Hz

  long count = 0;
  while(true)
  {
    /*
     * Calculate for command "set_pwm..._duty (value)"
     *
     *                     value
     * Duty_Cycle (%) = ----------- x 100
     *                  4 * (PR2+1)
     *
     * 100% : value = 1000
     * 0%   : value = 0
     */
    for(count=0; count<=1000; count++)
    {
      set_pwm1_duty(count);
      set_pwm2_duty(1000-count);
      delay_ms(5);
    }
    for(count=0; count<=1000; count++)
    {
      set_pwm1_duty(1000-count);
      set_pwm2_duty(count);
      delay_ms(5);
    }
  }
}
