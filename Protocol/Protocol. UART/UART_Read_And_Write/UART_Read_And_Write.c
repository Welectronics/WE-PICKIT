//// This code demo communicate via protocol UART, between PIC and PC
//// We use sofware Termite with setup Baudrate 9600bps, Transmitted Text Append CR-LF
//// 
//// The program begin with PIC->PC : "Hello world!" next "PIC16F877A UART example"
//// After that, it will wait you sent data PC->PIC and reply to back anything you sent
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

// -------------------------------------------------------------------------------------------------- //

void main()
{
  char i;                             // Store 1 character in one time

  putc(13);                           // Go to first column ('\r)
  printf("Hello world!");             // UART write
  putc(13); putc(10);                 // \r\n
  delay_ms(1000);

  printf("PIC16F877A UART example");  // UART Write
  putc(13); putc(10);                 // \r\n

  while(true)
  {
    /*
     * Return '0' if getc() will need to wait for a character to come in
     * Return '1' if a character is ready for getc()
     */
    if(kbhit())   // If data has been received
    {
      i = getc(); // UART read
      putc(i);    // Send it back
    }
  }
}
