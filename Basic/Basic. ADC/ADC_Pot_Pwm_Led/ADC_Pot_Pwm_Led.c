//// This code demo read analog from pot (ADC) to adj the light of LED
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

/*
 * To use with KIT DEV PIC (ADC, PWM)
 *
 * Connect jumper for ADC
 * RA0 (Pin A0) <-> A0 <-> ADC0 (Pot)
 * 
 * Connect wire for PWM, you can connect PIN_C1 with any LED you want, here i chosen
 * RC1 (Pin C1) <-> L1 (Led 1)
 */
#device ADC=10          // Set ADC resolution to 10-bit
#use delay(clock=20MHZ) //

/*
 * Store value read by analog, with resolution 10bit
 * And calculate mapping for unit direct on variable
 * Therefore we need the variable have large space
 */
long long value;
void main()
{
  /* With this config, 100% duty cycle is value 1000 */
  setup_ccp2(CCP_PWM);                  // Configure CCP2 as a PWM, use PIN_RC1
  setup_timer_2(T2_DIV_BY_16, 249, 1);  // Set PWM frequency to 1250Hz

  setup_adc(ADC_CLOCK_DIV_32);  // Set ADC conversion time to 32Tosc
  setup_adc_ports(AN0);         // Configure AN0 as analog
  set_adc_channel(0);           // Select channel 0 input

  delay_ms(100);                  // Wait 100ms, before begin
  while(true)
  {
    value = read_adc();           // The value is between 0 - 1023
    value = value * 1000 / 1023;  // Convert scale ADC to scale PWM
    set_pwm2_duty(value);
    delay_ms(10);                 // Wait 10ms
  }
}
