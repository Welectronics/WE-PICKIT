//// This code displayed type of button, when button push it will show name in LCD
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
 * To use with KIT DEV PIC (Button + LCD)
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
 * RA1 (Pin A1) <-> SW5 (SET)
 * RA2 (Pin A2) <-> SW4 (DOWN)
 * RA3 (Pin A3) <-> SW3 (UP)
 * RA4 (Pin A4) <-> SW2 (RIGHT)
 * RA5 (Pin A5) <-> SW1 (LEFT)
 */
#define LCD_RS_PIN      PIN_B3  // Register Select
#define LCD_RW_PIN      PIN_B2  // Read/Write
#define LCD_ENABLE_PIN  PIN_B1  // Enable
#define LCD_DATA4       PIN_D4  // Data 4
#define LCD_DATA5       PIN_D5  // Data 5
#define LCD_DATA6       PIN_D6  // Data 6
#define LCD_DATA7       PIN_D7  // Data 7

#define BUTTON_LEFT     PIN_A5
#define BUTTON_RIGHT    PIN_A4
#define BUTTON_UP       PIN_A3
#define BUTTON_DOWN     PIN_A2
#define BUTTON_SET      PIN_A1

/* Need include library of lcd after function delay */
#include <LCD.c>

void main()
{
  lcd_init();         // Initializes the LCD module with above defined connections

  while(true)
  {
    lcd_gotoxy(1, 1); // Set write position on LCD (upper left)
    if(!input(BUTTON_LEFT))       lcd_putc(" BUTTON LEFT    ");
    else if(!input(BUTTON_RIGHT)) lcd_putc(" BUTTON RIGHT   ");
    else if(!input(BUTTON_UP))    lcd_putc(" BUTTON UP      ");
    else if(!input(BUTTON_DOWN))  lcd_putc(" BUTTON DOWN    ");
    else if(!input(BUTTON_SET))   lcd_putc(" BUTTON SET     ");
    else                          lcd_putc(" none           ");
    delay_ms(100);
  }
}
