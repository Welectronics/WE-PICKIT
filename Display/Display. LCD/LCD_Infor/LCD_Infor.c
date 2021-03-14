//// This code is display info (use character of ASCII) on monitor LCD
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
 * To write an 8-bit character to the LCD module in 8-bit mode
 * ASCII data is send through the data lines DB0- DB7
 *
 * But 4-bit mode uses only 4 data lines
 * In this mode the 8-bit ASCII data is divided into 2 parts which are send sequentially through data lines DB4 – DB7
 * 
 * -----------------------------------------------------------------------------------------------------
 * 
 * To use with KIT DEV PIC (LCD)
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
 */
#define LCD_RS_PIN      PIN_B3  // Register Select
#define LCD_RW_PIN      PIN_B2  // Read/Write
#define LCD_ENABLE_PIN  PIN_B1  // Enable
#define LCD_DATA4       PIN_D4  // Data 4
#define LCD_DATA5       PIN_D5  // Data 5
#define LCD_DATA6       PIN_D6  // Data 6
#define LCD_DATA7       PIN_D7  // Data 7

/* Need include library of LCD after function Delay */
#include <LCD.c>

void main()
{
  lcd_init();       // Initializes the LCD module with above defined connections
  delay_ms(100);

  lcd_putc('\f');   // Clear display and set cursor to upper left
  lcd_putc("  WELECTRONICS  ");
  lcd_gotoxy(1, 2); // Set write position on LCD (column 1, row 2)
  lcd_putc("ELECTRIC FOR YOU");
}
