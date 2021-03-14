//// This code to turn on/off Buzzer by button SET
//// And it can be increase/decrease volume intensity by button UP/DOWN
//// Display of volume intensity on LCD
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
 * To use with KIT DEV PIC (LCD)
 * _ 4-bit mode of LCD
 * _ Button - A1/A2/A3 of Port A for Button SET/DOWN/UP, it will receive LOW when the button pressed
 * 
 * 1. Connect jumper for 3 pin controller
 * RB1 (Pin B1) <-> B1 <-> ENA (Enable)
 * RB2 (Pin B2) <-> B2 <-> R/W (Read/Write)
 * RB3 (Pin B3) <-> B3 <-> RS  (Register Select)
 * 
 * 2. Connect jumper for 4 pin data-line
 * RD7 (Pin D7) <-> D7
 * RD6 (Pin D6) <-> D6
 * RD5 (Pin D5) <-> D5
 * RD4 (Pin D4) <-> D4
 * 
 * 3. Connect jumper for Button
 * RA1 (Pin A1) <-> SW5 (Button SET)
 * RA2 (Pin A2) <-> SW4 (Button DOWN)
 * RA3 (Pin A3) <-> SW3 (Button UP)
 * 
 * 4. Connect jumper for Buzzer (PWM), it will ON when input HIGH
 * RC1 (Pin C1) <-> BUZZ (Buzzer)
 */
#define LCD_RS_PIN      PIN_B3  // Register Select
#define LCD_RW_PIN      PIN_B2  // Read/Write
#define LCD_ENABLE_PIN  PIN_B1  // Enable
#define LCD_DATA4       PIN_D4  // Data 4
#define LCD_DATA5       PIN_D5  // Data 5
#define LCD_DATA6       PIN_D6  // Data 6
#define LCD_DATA7       PIN_D7  // Data 7

#define BUTTON_UP       PIN_A3
#define BUTTON_DOWN     PIN_A2
#define BUTTON_SET      PIN_A1

/* Need include library of lcd after function delay */
#include <LCD.c>

void main()
{
  lcd_init();           // Initializes the LCD module with above defined connections
  lcd_putc('\f');       // Clear display and set cursor to upper left
  lcd_putc("Intensity: ");

  setup_ccp2(CCP_PWM);  // Configure CCP2 as a PWM, use PIN_RC1
  delay_ms(100);

  /*
   * PR2 = 249
   * TMR2 Prescale = 16
   * MCU_Frequency = 20x10^6
   *
   * Set PWM frequency to 1250Hz
   * Therefore, 100% duty cycle have value 1000
   */
  setup_timer_2(T2_DIV_BY_16, 249, 1);

  long intensity=0;
  while(true)
  {
    if( input_change_a())       // The value has changed
    {
      if(!input(BUTTON_SET))    // Press button Set
      {
        set_pwm2_duty(intensity);
      }
      else
      {
        set_pwm2_duty(0);
      }
    }

    if(!input(BUTTON_DOWN))     // Press button Down
    {
      if(intensity>0) intensity--;
      lcd_gotoxy(12, 1); printf(lcd_putc, "%Ld    ", intensity);
      delay_ms(10);
    }
    else if(!input(BUTTON_UP))  // Press button Up
    {
      if(intensity<1000) intensity++;
      lcd_gotoxy(12, 1); printf(lcd_putc, "%Ld    ", intensity);
      delay_ms(10);
    }
  }
}
