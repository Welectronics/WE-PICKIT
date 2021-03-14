//// This code receiver data from Bluetooth HC-05 or HC-06
//// Via "Commands" or "Voices" sent from app on phone, and display data that to LCD
////
//// If you send or talk command "Turn on" it will turn on Relay, and "Turn off" to turn off Relay
//// Beside when you can press the button "Switch 1" to turn on/off Led 1, and "Switch 2" to turn on/off Led 2
//// When you set "Device 1" it will dimmer Led 1, corresponding to "Device 2" for Led 2
////
//// Here my app I use: "Arduino Automatic" - author Himanshu Sharma
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
 * _ Led - It need output LOW to turn on the led
 * _ Relay - It need output LOW to turn on relay
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
 * 3. Connect jumper for Relay
 *                 5V <-> VRL (Voltage for Relay)
 *       RC0 (Pin C0) <-> SRL (Signal for Relay)
 * 
 * 4. Connect wire for Led
 * CCP2: RC1 (Pin C1) <-> L1 (Led 1)
 * CCP1: RC2 (Pin C2) <-> L2 (Led 2)
 */
#define LCD_RS_PIN      PIN_B3  // Register Select
#define LCD_RW_PIN      PIN_B2  // Read/Write
#define LCD_ENABLE_PIN  PIN_B1  // Enable
#define LCD_DATA4       PIN_D4  // Data 4
#define LCD_DATA5       PIN_D5  // Data 5
#define LCD_DATA6       PIN_D6  // Data 6
#define LCD_DATA7       PIN_D7  // Data 7

#define RELAY           PIN_C0
#define LED_1           PIN_C1
#define LED_2           PIN_C2

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
char relayOn[] = "Turn On\r\n";
char relayOff[] = "Turn Off\r\n";
char hello[] = "Hello\r\n";

/* To delete all data in array */
void clear_text(char bufferName[], int bufferSize)
{
  for(int n=0; n <= bufferSize; n++) bufferName[n] = 0;
}

// -------------------------------------------------------------------------------------------------- //

void main()
{
  lcd_init();     // Initializes the LCD module with above defined connections
  lcd_putc('\f'); // Clear display and set cursor to upper left
  lcd_putc("Waiting Command ");
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
        else if(!strcmp(text, hello)) printf("Who are you !!!\r\n");
        else printf("none\r\n");

        clear_text(text, 20);                         // Clear all data in 'text'
        cursor = 0;                                   // Back position 0
      }
    }
  }
}
