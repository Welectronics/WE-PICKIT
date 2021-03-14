//// This code demo communicate via protocol RS485, between PIC and PC
//// We use sofware Termite with setup Baudrate 9600bps, Transmitted Text Append CR-LF
//// 
//// The program begin with PIC->PC : "Hello world!"
//// After that, it will wait you sent data PC->PIC and display on LCDC
//// If sent "WE", it reply "WELECTRONICS"
//// If sent "TEAM", it reply sent in turn "Trung"+"Lai"+"Bao"+"Thanh"+"Hiep"
//// If sent another, it reply "none"
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
 * To use with KIT DEV PIC (LCD) in 4-bit mode of LCD
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
/* Need include library of lcd after function delay */
#include <LCD.c>

/*
 * The #USE DELAY directive must appear before this directive can be used
 * This directive enables use of built-in functions such as GETC, PUTC, and PRINTF
 *
 * To use with KIT DEV PIC (UART)
 * (default) TX <-> PIN_C6
 * (default) RX <-> PIN_C7
 */
#define TX  PIN_C6
#define RX  PIN_C7
//// #use rs232(uart1, baud = 9600)
#use rs232(xmit = TX, rcv = RX, baud = 9600)  // Initialize UART module
#include <string.h>

// -------------------------------------------------------------------------------------------------- //

char temp;                                    // Store 1 character in one time
char text[20];                                // The limit length of string
int cursor = 0;                               // Point to positions on array

/* To use with function "strcmp" we need use 1 variable to store string */
char we[] = "WE\r\n";
char team[] = "TEAM\r\n";

/* To delete all data in array */
void clear_text(char bufferName[], int bufferSize)
{
  for(int n=0; n <= bufferSize; n++) bufferName[n] = 0;
}

// -------------------------------------------------------------------------------------------------- //

void main()
{
  lcd_init();                 // Initializes the LCD module with above defined connections
  lcd_putc('\f');             // Clear display and set cursor to upper left
  printf("Hello world!\r\n"); // UART write
  delay_ms(1000);

  while(true)
  {
    if(kbhit())                                       // If data has been received
    {
      temp = getc();                                  // UART read
      text[cursor] = temp;
      cursor++;
      if(temp == '\n')                                // When you have sent data
      {
        lcd_putc('\f'); printf(lcd_putc, "%s", text); // Display infor to LCD
        lcd_gotoxy(cursor-1, 1); lcd_putc("  ");      // Clear character \r and \n on LCD

        if(!strcmp(text, we)) printf("WELECTRONICS\r\n");
        else if(!strcmp(text, team)) printf("Trung\r\nLai\r\nBao\r\nThanh\r\nHiep");
        else printf("none\r\n");

        clear_text(text, 20);                         // Clear all data in 'text'
        cursor = 0;                                   // Back position 0
      }
    }
  }
}
