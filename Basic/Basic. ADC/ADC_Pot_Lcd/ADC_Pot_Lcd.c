//// This code demo read analog from pot (ADC) to adj the light of LED, have display value on LCD
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
#device ADC=10          // Set ADC resolution to 10-bit
#use delay(clock=20MHZ) //

/*
 * To use with KIT DEV PIC (ADC, PWM, LCD)
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
 * 3. Connect jumper for ADC
 * RA0 (Pin A0) <-> A0 <-> ADC0 (Pot)
 * 
 * 4. Connect wire for PWM, you can connect PIN_C1 with any LED you want, here i chosen
 * RC1 (Pin C1) <-> L1 (Led 1)
 */
#define LCD_RS_PIN      PIN_B3  // Register Select
#define LCD_RW_PIN      PIN_B2  // Read/Write
#define LCD_ENABLE_PIN  PIN_B1  // Enable
#define LCD_DATA4       PIN_D4  // Data 4
#define LCD_DATA5       PIN_D5  // Data 5
#define LCD_DATA6       PIN_D6  // Data 6
#define LCD_DATA7       PIN_D7  // Data 7

/* Need include library of lcd after function delay */
#include <LCD.c>

/*
 * Store value read by analog, with resolution 10bit
 * And calculate mapping for unit direct on variable
 * Therefore we need the variable have large space
 */
long long value;
void main()
{
  lcd_init();     // Initializes the LCD module with above defined connections
  lcd_putc('\f'); // Clear display and set cursor to upper left
  lcd_putc("Pot: "); lcd_gotoxy(1, 2); lcd_putc("PWM (%): ");

  /* With this config, 100% duty cycle is value 1000 */
  setup_ccp2(CCP_PWM);                  // Configure CCP2 as a PWM, use PIN_RC1
  setup_timer_2(T2_DIV_BY_16, 249, 1);  // Set PWM frequency to 1250Hz

  setup_adc(ADC_CLOCK_DIV_32);  // Set ADC conversion time to 32Tosc
  setup_adc_Pins(AN0);         // Configure AN0 as analog
  set_adc_channel(0);           // Select channel 0 input

  delay_ms(100);                                              // Wait 100ms, before begin
  while(true)
  {
    value = read_adc();                                       // The value is between 0 - 1023
    lcd_gotoxy(6, 1); printf(lcd_putc, "%Ld       ", value);

    value = value * 1000 / 1023;                              // Convert scale ADC to scale PWM
    set_pwm2_duty(value);
    lcd_gotoxy(10, 2); printf(lcd_putc, "%Ld   ", value/10);  // Display PWM(%)

    delay_ms(300);                                            // Wait 300ms
  }
}
