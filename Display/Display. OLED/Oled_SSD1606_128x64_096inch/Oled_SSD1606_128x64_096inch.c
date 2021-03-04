//// This code demo interfacing PIC16F877A with SSD1306 OLED display (128x64 Pixel) - 0.96inch
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
 * Options for I2C:
 * MASTER         Sets to the master mode
 * FAST=nnnnnn    Sets the speed to "nnnnnn" HZ
 * 
 * Remember "Address I2C" only have 7-bit or large is 10-bit
 * Here, we use with DS3231, it have "Address I2C" is 0x3C = 0011.1100
 * When we want access 1 device to Write/Read -> we use 7-bit "Address I2C" + 1-bit "Read/Write" to call that device
 * 
 * Ex:                      Bit 7 - 6 - 5 - 4 - 3 - 2 - 1 | 0
 *                          ------------------------------|---------
 * To Wite (Master -> Device)   0   1   1   1   1   0   0 | 0 = 0x78
 * To Read (Device -> Master)   0   1   1   1   1   0   0 | 1 = 0x79
 */
#define PIN_SCL PIN_C3
#define PIN_SDA PIN_C4
//// #use i2c(master, i2c1, fast = 400000)
#use i2c(master, scl = PIN_SCL, sda = PIN_SDA, fast = 400000) // Initialize I2C
#include "SSD1306.c"                                          // Include SSD1306 OLED driver source code

// -------------------------------------------------------------------------------------------------- //

void main()
{
  delay_ms(500);
  SSD1306_Init(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);  // Initialize the SSD1306 OLED with an I2C addr = 0x3C

  SSD1306_ClearDisplay();   // Clear the display
  SSD1306_GotoXY(1, 1);     //
  SSD1306_PutC("Interfacing PIC16F877A with SSD1306 OLED display");
  delay_ms(5000);           //

  SSD1306_ClearDisplay();   // Clear the display
  SSD1306_GotoXY(6, 2);     //
  SSD1306_PutC("Hello world!");
  delay_ms(2000);           //
 
  SSD1306_StartScrollRight(1, 1);     delay_ms(3000); SSD1306_StopScroll();
  SSD1306_StartScrollLeft(1, 1);      delay_ms(3000); SSD1306_StopScroll();
  SSD1306_StartScrollDiagRight(1, 1); delay_ms(3000); SSD1306_StopScroll();
  SSD1306_StartScrollDiagLeft(1, 1);  delay_ms(3000); SSD1306_StopScroll();
 
  delay_ms(3000);           //
  SSD1306_ClearDisplay();   // Clear the display
 
  SSD1306_GotoXY(6, 2);     //
  SSD1306_PutC("Hello world!");
  delay_ms(2000);           //

  int8 i = 0;
  while(true)
  {
    SSD1306_GotoXY(10, 5);  //
    printf(SSD1306_PutC, "%03u", i++);
    delay_ms(500);          //
  }
}
