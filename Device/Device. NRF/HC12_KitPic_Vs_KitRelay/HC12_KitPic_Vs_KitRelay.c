//// This code have function, when you push buttons (LEFT/RIGHT/UP/DOWN) on KIT PIC it will send "data" via HC-12
//// And then KIT RELAY will receive that "data" to decision what turn on/off relay
//// KIT RELAY will reply KIT PIC to notification what turn on/off led
//// Versa, if you push buttons (A0/A1/A2/A3) it will repeat the same process as above
////
//// Note:  Here to make simple, i don't do functions check error when transfer/receiver data
////        Default when plug power for KIT, all Relay will turn off
////        HC-12 is setup default is Baudrate 9600, Channel 50
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
 * To use with KIT DEV PIC:
 * 
 * 1. Connect jumper for Button
 * RA2 (Pin A2) <-> SW4 (DOWN)  ... for Relay 4
 * RA3 (Pin A3) <-> SW3 (UP)    ... for Relay 3
 * RA4 (Pin A4) <-> SW2 (RIGHT) ... for Relay 2
 * RA5 (Pin A5) <-> SW1 (LEFT)  ... for Relay 1
 */
#define BUTTON_RELAY_1  PIN_A5  // Button LEFT
#define BUTTON_RELAY_2  PIN_A4  // Button RIGHT
#define BUTTON_RELAY_3  PIN_A3  // Button UP
#define BUTTON_RELAY_4  PIN_A2  // Button DOWN

/*
 * 2. Connect jumper for Led
 * RD7 (Pin D7) <-> L1 (Led 1)  ... of Relay 1
 * RD6 (Pin D6) <-> L2 (Led 2)  ... of Relay 2
 * RD5 (Pin D5) <-> L3 (Led 3)  ... of Relay 3
 * RD4 (Pin D4) <-> L4 (Led 4)  ... of Relay 4
 */
#define LED_1           PIN_D7
#define LED_2           PIN_D6
#define LED_3           PIN_D5
#define LED_4           PIN_D4

/*
 * The #USE DELAY directive must appear before this directive can be used
 * This directive enables use of built-in functions such as GETC, PUTC, and PRINTF
 *
 * 3. Connect jumper for UART, and "switch signal level between 3.3V <-> 5V"
 * TH <-> (default) TX <-> PIN_C6
 * RH <-> (default) RX <-> PIN_C7
 * 
 * 4. Connect for HC-12, here I have solder 1 small jumper to connect pin C0 (PIC) and pin SET (HC-12)
 * RXD (HC-12) <-> TXL (PIC)
 * TXD (HC-12) <-> RXL (PIC)
 * SET (HC-12) <-> STA <-> PIN_C0
 */
#define TX              PIN_C6
#define RX              PIN_C7
#define SET             PIN_C0
//// #use rs232(uart1, baud = 9600)
#use rs232(xmit = TX, rcv = RX, baud = 9600)  // Initialize UART module
#include <string.h>

// -------------------------------------------------------------------------------------------------- //

#define SIZE_STRING 5
char temp;                                    // Store 1 character in one time
char text[SIZE_STRING];                       // The limit length of string
int cursor = 0;                               // Point to positions on array

/* To use with function "strcmp" we need use 1 variable to store string */
char relay_1_on[]  = "11\n";
char relay_1_off[] = "10\n";
char relay_2_on[]  = "21\n";
char relay_2_off[] = "20\n";
char relay_3_on[]  = "31\n";
char relay_3_off[] = "30\n";
char relay_4_on[]  = "41\n";
char relay_4_off[] = "40\n";

/* To delete all data in array */
void clear_text(char bufferName[], int bufferSize)
{
  for(int n=0; n <= bufferSize; n++) bufferName[n] = 0;
}

// -------------------------------------------------------------------------------------------------- //

void main()
{
  /* To remember state button, if not press now is HIGH */
  int1 state_button_relay_1 = input(BUTTON_RELAY_1);
  int1 state_button_relay_2 = input(BUTTON_RELAY_2);
  int1 state_button_relay_3 = input(BUTTON_RELAY_3);
  int1 state_button_relay_4 = input(BUTTON_RELAY_4);

  /* To know status of Relay now */
  int1 stateRelay_1 = 0, stateRelay_2 = 0, stateRelay_3 = 0, stateRelay_4 = 0;

  /* Turn off all Led */
  output_high(LED_1); output_high(LED_2); output_high(LED_3); output_high(LED_4);

  /* HC-12 normal, transparent mode */
  output_high(SET);

  while(true)
  {
    /* Process received data from KIT RELAY -> KIT PIC */
    if(kbhit())                                                 // If data has been received
    {
      temp = getc();                                            // UART read
      text[cursor] = temp;
      cursor++;
      if(temp == '\n')                                          // When you have sent data
      {
        if(!strcmp(text, relay_1_on))       {output_low(LED_1);   stateRelay_1=1;}
        else if(!strcmp(text, relay_1_off)) {output_high(LED_1);  stateRelay_1=0;}
        else if(!strcmp(text, relay_2_on))  {output_low(LED_2);   stateRelay_2=1;}
        else if(!strcmp(text, relay_2_off)) {output_high(LED_2);  stateRelay_2=0;}
        else if(!strcmp(text, relay_3_on))  {output_low(LED_3);   stateRelay_3=1;}
        else if(!strcmp(text, relay_3_off)) {output_high(LED_3);  stateRelay_3=0;}
        else if(!strcmp(text, relay_4_on))  {output_low(LED_4);   stateRelay_4=1;}
        else if(!strcmp(text, relay_4_off)) {output_high(LED_4);  stateRelay_4=0;}

        clear_text(text, SIZE_STRING);                          // Clear all data in 'text'
        cursor = 0;                                             // Back position 0
      }
    }

    /* Process send data from KIT PIC -> KIT RELAY */
    if(!input(BUTTON_RELAY_1) && state_button_relay_1)                      // If the button is pressed, and the EdgeDown happen
    {
      delay_ms(10);                                                         // Button Debounce
      if(!input(BUTTON_RELAY_1) && state_button_relay_1)                    // If the button is still pressed, happen status EdgeDown
      {
        state_button_relay_1 = 0;
        if(stateRelay_1)  printf("10\n");                                   // Turn off Relay 1
        else              printf("11\n");                                   // Turn on Relay 1
        stateRelay_1 = !stateRelay_1;
        output_bit(LED_1, !stateRelay_1);                                   // Change state Led after push button
      }
    }
    if(!state_button_relay_1) state_button_relay_1 = input(BUTTON_RELAY_1); // Check when happen the EdgUp
    //////////////////////////////////////////////////////////////////////////
    if(!input(BUTTON_RELAY_2) && state_button_relay_2)                      // If the button is pressed, and the EdgeDown happen
    {
      delay_ms(10);                                                         // Button Debounce
      if(!input(BUTTON_RELAY_2) && state_button_relay_2)                    // If the button is still pressed, happen status EdgeDown
      {
        state_button_relay_2 = 0;
        if(stateRelay_2)  printf("20\n");                                   // Turn off Relay 2
        else              printf("21\n");                                   // Turn on Relay 2
        stateRelay_2 = !stateRelay_2;
        output_bit(LED_2, !stateRelay_2);                                   // Change state Led after push button
      }
    }
    if(!state_button_relay_2) state_button_relay_2 = input(BUTTON_RELAY_2); // Check when happen the EdgUp
    //////////////////////////////////////////////////////////////////////////
    if(!input(BUTTON_RELAY_3) && state_button_relay_3)                      // If the button is pressed, and the EdgeDown happen
    {
      delay_ms(10);                                                         // Button Debounce
      if(!input(BUTTON_RELAY_3) && state_button_relay_3)                    // If the button is still pressed, happen status EdgeDown
      {
        state_button_relay_3 = 0;
        if(stateRelay_3)  printf("30\n");                                   // Turn off Relay 3
        else              printf("31\n");                                   // Turn on Relay 3
        stateRelay_3 = !stateRelay_3;
        output_bit(LED_3, !stateRelay_3);                                   // Change state Led after push button
      }
    }
    if(!state_button_relay_3) state_button_relay_3 = input(BUTTON_RELAY_3); // Check when happen the EdgUp
    //////////////////////////////////////////////////////////////////////////
    if(!input(BUTTON_RELAY_4) && state_button_relay_4)                      // If the button is pressed, and the EdgeDown happen
    {
      delay_ms(10);                                                         // Button Debounce
      if(!input(BUTTON_RELAY_4) && state_button_relay_4)                    // If the button is still pressed, happen status EdgeDown
      {
        state_button_relay_4 = 0;
        if(stateRelay_4)  printf("40\n");                                   // Turn off Relay 4
        else              printf("41\n");                                   // Turn on Relay 4
        stateRelay_4 = !stateRelay_4;
        output_bit(LED_4, !stateRelay_4);                                   // Change state Led after push button
      }
    }
    if(!state_button_relay_4) state_button_relay_4 = input(BUTTON_RELAY_4); // Check when happen the EdgUp
  }
}
