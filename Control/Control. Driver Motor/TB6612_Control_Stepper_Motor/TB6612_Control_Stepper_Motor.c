//// This code demo run 1 Stepper Motor, with can setup and display on LCD
//// Push button SET, to begin run motor with specific had been setup
//// Push button UP/DOWN to increase/decrease speed of motor, unit (RPM)
//// Push button RIGHT/LEFT to set numbers rotate for motor, when number(-) it rotate CCW, number(+) it rotate CW
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
#define PWMB                PIN_C1  // Enable channel B
#define BIN1                PIN_B2  // Coils B Stepper Motor
#define BIN2                PIN_B1  //
#define PWMA                PIN_C2  // Enable channel A
#define AIN1                PIN_B3  // Coils B Stepper Motor
#define AIN2                PIN_B4  // 
#include "Stepper.h"

/*
 * 3. Connect wire for 3 pin controller LCD
 * RD3 (Pin D3) <-> D3 <-> ENA (Enable)
 * RD2 (Pin D2) <-> D2 <-> R/W (Read/Write)
 * RD1 (Pin D1) <-> D1 <-> RS  (Register Select)
 * 
 * 4. Connect jumper for 4 pin data-line
 * RD7 (Pin D7) <-> D7
 * RD6 (Pin D6) <-> D6
 * RD5 (Pin D5) <-> D5
 * RD4 (Pin D4) <-> D4
 */
#define LCD_RS_PIN          PIN_D1  // Register Select
#define LCD_RW_PIN          PIN_D2  // Read/Write
#define LCD_ENABLE_PIN      PIN_D3  // Enable
#define LCD_DATA4           PIN_D4  // Data 4
#define LCD_DATA5           PIN_D5  // Data 5
#define LCD_DATA6           PIN_D6  // Data 6
#define LCD_DATA7           PIN_D7  // Data 7
#include <LCD.c>

/*
 * 5. Connect jumper for Button
 * RA1 (Pin A1) <-> SW5 (Button SET)
 * RA2 (Pin A2) <-> SW4 (Button DOWN)
 * RA3 (Pin A3) <-> SW3 (Button UP)
 * RA4 (Pin A4) <-> SW2 (Button RIGHT)
 * RA5 (Pin A5) <-> SW1 (Button LEFT)
 *
 * 6. Connect jumper for Led
 * RD0 (Pin D0) <-> L8  (Led 8)
 */
#define BUTTON_RUN          PIN_A1  // Button SET
#define BUTTON_DOWN_RPM     PIN_A2  // Button DOWN
#define BUTTON_UP_RPM       PIN_A3  // Button UP
#define BUTTON_UP_PULSE     PIN_A4  // Button RIGHT
#define BUTTON_DOWN_PULSES  PIN_A5  // Button LEFT
#define LED                 PIN_D0  //

// -------------------------------------------------------------------------------------------------- //

#define STEPS 200               // Change this to the number of steps on your motor (200 Pulses/Rotation)

void main()
{
  signed int16 stepMotor = 200; // Default: 200 pulses, equivalent 1 rotate
  unsigned int16 speedRpm = 60; // Default: speed is 60 RPM, mean 1 rotate in 1 second

  /* Initialization for stepper motor type "4-wire", and setup speed */
  stepper(STEPS, AIN1, AIN2, BIN1, BIN2);
  setSpeed(speedRpm);

  /* Turn on Motor A+B = Turn on Motor Step*/
  output_high(PWMA);
  output_high(PWMB);

  /* Turn off Led, it only on when motor running */
  output_high(LED);

  /* Initializes the LCD module with above defined connections */
  lcd_init();
  lcd_gotoxy(1, 1); lcd_putc("Speed: "); printf(lcd_putc,"%Lu Rpm    ", speedRpm);
  lcd_gotoxy(1, 2); lcd_putc("Pulses: "); printf(lcd_putc,"%Ld       ", stepMotor);

  delay_ms(10);
  while(true)
  {
    if(!input_state(BUTTON_RUN))              {output_low(LED); setSpeed(speedRpm); step(stepMotor); output_high(LED);}
    else if(!input_state(BUTTON_DOWN_RPM))    {speedRpm--; lcd_gotoxy(8, 1); printf(lcd_putc,"%Lu Rpm    ", speedRpm); delay_ms(50);}
    else if(!input_state(BUTTON_UP_RPM))      {speedRpm++; lcd_gotoxy(8, 1); printf(lcd_putc,"%Lu Rpm    ", speedRpm); delay_ms(50);}
    else if(!input_state(BUTTON_UP_PULSE))    {stepMotor++; lcd_gotoxy(9, 2); printf(lcd_putc,"%Ld       ", stepMotor); delay_ms(5);}
    else if(!input_state(BUTTON_DOWN_PULSES)) {stepMotor--; lcd_gotoxy(9, 2); printf(lcd_putc,"%Ld       ", stepMotor); delay_ms(5);}
  }
}
