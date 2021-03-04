//// This code is display custom character of users on monitor LCD
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

char running_man_custom_chars[8][8] = {
  { // *** Note: can't use with variable "const"
    // Char Number 0
    0b00000110,
    0b00000110,
    0b00000100,
    0b00001110,
    0b00010101,
    0b00000100,
    0b00001010,
    0b00010001
  },
  {
    // Char Number 1
    0b00000110,
    0b00000110,
    0b00000100,
    0b00001110,
    0b00001110,
    0b00000100,
    0b00001010,
    0b00001010
  },
  {
    // Char Number 2
    0b00000110,
    0b00000110,
    0b00000100,
    0b00011110,
    0b00000101,
    0b00000100,
    0b00001010,
    0b00010010
  },
  {
    // Char Number 3
    0b00000110,
    0b00000110,
    0b00000100,
    0b00001110,
    0b00001101,
    0b00000100,
    0b00001010,
    0b00010100
  },
  {
    // Char Number 4
    0b00000110,
    0b00000110,
    0b00000100,
    0b00001111,
    0b00010100,
    0b00000100,
    0b00001010,
    0b00010100
  },
  {
    // Char Number 5
    0b00000110,
    0b00000110,
    0b00000100,
    0b00011110,
    0b00010111,
    0b00000100,
    0b00001010,
    0b00010010
  },
  {
    // Char Number 6
    0b00000110,
    0b00000110,
    0b00000100,
    0b00001110,
    0b00010101,
    0b00000100,
    0b00001010,
    0b00010010
  },
  {
    // Char Number 7
    0b00000110,
    0b00000110,
    0b00000100,
    0b00001110,
    0b00001110,
    0b00000100,
    0b00001010,
    0b00001010
  }
};

void main()
{
  lcd_init();           // Initializes the LCD module with above defined connections
  delay_ms(100);

  /*
   * Write a custom character to the ram
   * 'which' is position of Ram
   * '*ptr' is name data char
   */
  lcd_set_cgram_char(0, &running_man_custom_chars[0]);
  lcd_set_cgram_char(1, &running_man_custom_chars[1]);
  lcd_set_cgram_char(2, &running_man_custom_chars[2]);
  lcd_set_cgram_char(3, &running_man_custom_chars[3]);
  lcd_set_cgram_char(4, &running_man_custom_chars[4]);
  lcd_set_cgram_char(5, &running_man_custom_chars[5]);
  lcd_set_cgram_char(6, &running_man_custom_chars[6]);
  lcd_set_cgram_char(7, &running_man_custom_chars[7]);

  lcd_putc('\f');       // Clear display and set cursor to upper left
  while(true)
  {
    for(int frame=0; frame<8; frame++)
    {
      lcd_gotoxy(1, 1); // Set write position on LCD (upper left is 1,1)
      lcd_putc(frame);  // Display Running Man animation
      delay_ms(500);
    }
  }
}
