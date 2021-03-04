//// This code demo function read status of Button, it only accept Edge Up, and display of number click on Lcd
//// Have process Debounce of button
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
 * 3. Connect jumper for Button and Relay
 * RA1 (Pin A1) <-> SW5 (SET)
 */
#define LCD_RS_PIN      PIN_B3  // Register Select
#define LCD_RW_PIN      PIN_B2  // Read/Write
#define LCD_ENABLE_PIN  PIN_B1  // Enable
#define LCD_DATA4       PIN_D4  // Data 4
#define LCD_DATA5       PIN_D5  // Data 5
#define LCD_DATA6       PIN_D6  // Data 6
#define LCD_DATA7       PIN_D7  // Data 7

#define BUTTON_SET      PIN_A1

/* Need include library of lcd after function delay */
#include <LCD.c>

void main()
{
  int1 state_button = input(BUTTON_SET);      // To remember state button, if not press now is HIGH
  unsigned int count = 0;

  lcd_init();       // Initializes the LCD module with above defined connections
  lcd_putc('\f');   // Clear display and set cursor to upper left
  printf(lcd_putc,"Count = %d       ", count);

  while(true)
  {
    if(!input(BUTTON_SET) && state_button)    // If the button is pressed
    {
      delay_ms(10);                           // Button Debounce
      if(!input(BUTTON_SET) && state_button)  // If the button is still pressed, happen status edge down
      {
        state_button = 0;                     // *** Can't use with "HIGH", "high" or "LOW", "low"
        count++;
        lcd_gotoxy(1, 1);
        printf(lcd_putc,"Count = %d       ", count);
      }
    }
    if(!state_button) state_button = input(BUTTON_SET); // Check when happen status edge up
  }
}
