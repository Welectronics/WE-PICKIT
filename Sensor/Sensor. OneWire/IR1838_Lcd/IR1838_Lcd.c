//// This code to demo IR1838, it will read signal RAW from Remote (use protocol NEC)
//// And then display value had received into monitor LCD
////
//// This is source to refer: https://www.sbprojects.net/knowledge/ir/nec.php
//// Note:  _ The signal we talk about here is of the RECEIVER
////        _ It different signal the source mention, is TRANSMITTER
////
////        _ The level logic of pin IR1838 received in state normal is alway HIGH
////        _ WITH THIS PROTOCOL THE "LSB" IS TRANSMITTED FIRST
////
//// The structure data that pin received is below:
////    1.  _ A message start (AGC burst) will pull-down signal to LOW in 9ms = 9000us
////        _ Followed, it will release the signal pin to HIGH in 4.5ms = 4500 us
////
////    2.  _ Then followed by the "Address" (8 bit)
////        _ And 8 bit inverted of "Address"
////
////    3.  _ Finally, followed by the "Command" (8 bit)
////        _ And 8 bit inverted of "Command"
////
//// The "Logic 1" will pull-down signal to LOW (burst) in 560us
//// Then release the signal pin to HIGH in about 1.69ms = 1690us
//// Total, one "Logic 1" takes 2.25ms = 2250us to transmit
////
//// The "Logic 0" will pull-down signal to LOW (burst) in 560us
//// Then release the signal pin to HIGH in about 560us
//// Total, one "Logic 0" takes 1.12ms = 1120us to transmit
////
//// *** The bit inverted used for verification of the received message
//// *** The total transmission time is constant because every bit is repeated with its inverted length
//// *** Therefore, total time for every transmit about ~ 67.42ms
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

/*
 * To use with KIT DEV PIC (IR1838)
 * 
 * 1. Adj of the switch SW1 to connect IR1838 with ...
 * RD1 (Pin D1) <-> S-IR (Signal of IR1838)
 */
#define SIG             PIN_D1
#device ADC=16          // Set ADC resolution to 16-bit
#use delay(clock=20MHZ) //

/*
 * 2. Connect jumper for 3 pin controller
 * RB1 (Pin B1) <-> B1 <-> ENA (Enable)
 * RB2 (Pin B2) <-> B2 <-> R/W (Read/Write)
 * RB3 (Pin B3) <-> B3 <-> RS  (Register Select)
 * 
 * 3. Connect jumper for 4 pin data-line
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

int8 address = 0, notAddress = 0; // Store value "Address" and value inverted "Address"
int8 data = 0, notData = 0;       // Store value "Command" and value inverted "Command"

// -------------------------------------------------------------------------------------------------- //

/*
 * Check that is right the message start (AGC burst)?
 * 
 * _ A message start (AGC burst) will pull-down signal to LOW in 9ms = 9000us
 * _ Followed, it will release the signal pin to HIGH in 4.5ms = 4500 us
 */
int1 check_start()
{
  if(!input_state(SIG))             // EdgeDown -> signal LOW ... first
  {
    delay_us(8900);                 // Wait 9ms (8.9ms + 0.1ms)
    if(input_state(SIG)) return 0;  // Change HIGH -> Wrong
    else                            // Continue LOW -> Right   
    {
      while(!input_state(SIG));     // Wait until EdgeUp -> signal HIGH
      delay_us(4400);               // Wait 4.5ms (4.4ms + 0.1ms)
      if(input_state(SIG))          // Continue HIGH -> Right
      {
        while(input_state(SIG));    // Wait until EdgeDown -> signal LOW
        return 1;
      }
      else return 0;                // Change LOW -> Wrong
    }
  }
  else return 0;
}

/*
 * To view that is bit 0 or bit 1, and store the value had received
 * 
 * The "Logic 1" will pull-down signal to LOW (burst) in 560us
 * Then release the signal pin to HIGH in about 1.69ms = 1690us
 * Total, one "Logic 1" takes 2.25ms = 2250us to transmit
 * 
 * The "Logic 0" will pull-down signal to LOW (burst) in 560us
 * Then release the signal pin to HIGH in about 560us
 * Total, one "Logic 0" takes 1.12ms = 1120us to transmit
 */
void take_data_IR(int8* data)
{
  for(int8 i=0; i<8; i++)
  {
    while(input_state(SIG));    // Wait until the signal LOW, begin to get value
    delay_us(460);              // Wait 560us = 0.56ms (0.46ms + 0.1ms)
    if(input_state(SIG)) {}     // Change HIGH -> Wrong ... Do Nothing
    else                        // Continue LOW -> Right
    {
      while(!input_state(SIG)); // Wait until EdgeUp -> signal HIGH
      delay_us(660);            // Wait 560us = 0.56ms, here I for delay plus 0.1ms. Mean total delay is 0.66ms
      // ---------------------- //
      if(input_state(SIG))      // Continue HIGH -> Bit 1
      {
        bit_set(*data, i);      // Store bit 1
      }
      else                      // Change LOW -> Bit 0
      {
        bit_clear(*data, i);    // Store bit 0
      }
    }
  }
  while(input_state(SIG));      // Wait until the signal LOW, end to get value
}

// -------------------------------------------------------------------------------------------------- //

/* Display data had received into Lcd */
void display_lcd()
{
  lcd_gotoxy(1, 1); printf(lcd_putc,"Address: 0x%X.%X", address, notAddress);
  lcd_gotoxy(1, 2); printf(lcd_putc,"Data   : 0x%X.%X", data, notData);
}

// -------------------------------------------------------------------------------------------------- //

void main()
{
  lcd_init();                   // Initializes the LCD module with above defined connections
  lcd_putc('\f');               // Clear display and set cursor to upper left
  lcd_putc(" --- IR1838 --- "); //

  while(true)
  {
    /*
     * I had include picture of Remote I use in folder code, here is string data you will receive when push buttons number:
     * 
     *            Address       Command
     *            |             |
     * Number 1:  0x00 - 0xFF - 0x0C - 0xF3
     * Number 2:  0x00 - 0xFF - 0x18 - 0xE7
     * Number 3:  0x00 - 0xFF - 0x5E - 0xA1
     * Number 4:  0x00 - 0xFF - 0x08 - 0xF7
     * Number 5:  0x00 - 0xFF - 0x1C - 0xE3
     * Number 6:  0x00 - 0xFF - 0x5A - 0xA5
     * Number 7:  0x00 - 0xFF - 0x42 - 0xBD
     * Number 8:  0x00 - 0xFF - 0x52 - 0xAD
     * Number 9:  0x00 - 0xFF - 0x4A - 0xB5
     */
    if(check_start())             // Check that is right the message start (AGC burst)?
    {
      take_data_IR(&address);     // Take value "Address"
      take_data_IR(&notAddress);  // Take value inverted of "Address"

      take_data_IR(&data);        // Take value "Command"
      take_data_IR(&notData);     // Take value inverted of "Command"

      while(!input_state(SIG));   // Wait until the signal HIGH, THE END !!!
      /*
       * Here I make delay, because if some button have "Repeat Code" or "Other Code"
       * 
       * A command is transmitted only once, even when the key on the remote control remains pressed
       * Every 110ms a repeat code is transmitted for as long as the key remains down
       * This repeat code is simply a 9ms AGC pulse followed by a 2.25ms space and a 560µs burst
       * 
       * To make code simply, I don't process "Repeat Code". Therefore I only need delay overtime of that signal
       */
      delay_ms(50);

      display_lcd();              // Display the string of data has received in format HEX into monitor Lcd
    }
  }
}
