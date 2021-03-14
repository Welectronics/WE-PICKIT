//// This code demo run 1 Stepper Motor
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
#define PWMB PIN_C1 // Enable channel B
#define BIN1 PIN_B2 // Coils B Stepper Motor
#define BIN2 PIN_B1 //
#define PWMA PIN_C2 // Enable channel A
#define AIN1 PIN_B3 // Coils B Stepper Motor
#define AIN2 PIN_B4 // 
#include "Stepper.h"

/* Change this to the number of steps on your motor (200 Pulses/Rotation) */
#define STEPS 200

void main()
{
  stepper(STEPS, AIN1, AIN2, BIN1, BIN2); // Specifying the number of steps of the motor and the pins it's attached to
  setSpeed(60);                           // Speed is 60 RPM, mean 1 rotate in 1 second

  /* Turn on Motor A and Motor B */
  output_high(PWMA);
  output_high(PWMB);
  delay_ms(10);

  while(true)
  {
    /* Rotate CW 1 rotate*/
    step(STEPS);
    delay_ms(1000);

    /* Rotate CCW 1 rotate*/
    step(-STEPS);
    delay_ms(1000);
  }
}
