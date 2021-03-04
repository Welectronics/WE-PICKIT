//// This code reads value of the thermal sensor (DS18B20) - OneWire Digital
//// Every 1s it will update temperature 1 time
//// With the default resolution of the temperature is 12 bits -> 0.0625°C
//// And display value to LedSeg, range from 00.00 - 99.99
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

#use timer (timer=1, tick=1ms, bits=16, noisr)
unsigned int16 current_tick, previous_tick;
unsigned int16 tick_difference(unsigned int16 current, unsigned int16 previous)
{
  return(current - previous);
}

/*
 * To use with KIT DEV PIC (Led Seg Common Anode, and 74HC595 Serial Shift Register)
 *
 * 1. Connect jumper for 3 pin
 * ST_CP (RCLK) <-> E2 <-> RE2 (Pin E2)
 * SH_CP (SCLK) <-> E1 <-> RE1 (Pin E1)
 * DS (DIO)     <-> E0 <-> RE0 (Pin E0)
 *
 * Setup all pins below is input 74HC595, is output PIC
 */
#define DIGIT_ONE         0b00001000
#define DIGIT_TEN         0b00000100
#define DIGIT_HUNDRED     0b00000010
#define DIGIT_THOUSAND    0b00000001
#define TURN_OFF_DIGIT    0b00000000
#define TURN_OFF_LED      0b11111111

#define PIN_CLOCK PIN_E2  // Pin ST_CP (RCLK)
#define PIN_LATCH PIN_E1  // Pin SH_CP (SCLK)
#define PIN_DATA  PIN_E0  // Pin DS (DIO)
#include "LegSeg.h"       // You must declare all pins above before include add this library

/*
 * 2. Adj of the switch SW1 to connect DS18B20 with ...
 * RD0 (Pin D0) <-> S-DS (Signal of DS18B20)
 */
#define PIN_DATA_DS18B20  PIN_D0
#include "Ds18b20.h"      // You must declare all pins above before include add this library

// -------------------------------------------------------------------------------------------------- //

/* Stores code "HGFE.DCBA" to display character of number for Led 7 Segment */
char segCode[] = {
  0xC0, // 0
  0xF9, // 1
  0xA4, // 2
  0xB0, // 3
  0x99, // 4
  0x92, // 5
  0x82, // 6
  0xF8, // 7
  0x80, // 8
  0x90, // 9

  0xF7  // _
};

void display_error()
{
  while(tick_difference(current_tick, previous_tick) < 1000)
  { // Had been ERROR!
    current_tick = get_ticks();                               // Check time now!
    send_data(segCode[10], DIGIT_ONE);          delay_ms(1);  // Digit 1/100
    send_data(segCode[10], DIGIT_TEN);          delay_ms(1);  // Digit 1/10
    send_data(segCode[10]&0x7F, DIGIT_HUNDRED); delay_ms(1);  // Digit One, use 0x7F to make dot
    send_data(segCode[10], DIGIT_THOUSAND);     delay_ms(1);  // Digit Ten
  }
  previous_tick = current_tick;                               // Update time
}

// -------------------------------------------------------------------------------------------------- //

void main()
{
  current_tick = previous_tick = get_ticks();
  int16 rawTemp;                            // Store the value raw temperature, read from sensor DS18B20
  send_data(TURN_OFF_LED , TURN_OFF_DIGIT); // Turn off all digit

  while(true)
  {
    if(ds18b20_request_read())              // Request sensor convert temperature
    {
      while(ds18b20_read_byte() == 0)       // Wait for conversion complete, you can do other something
      { // Display old value
        // With resolution 12 bit, it need time to convert temperature about max 750ms
        while(tick_difference(current_tick, previous_tick) < 750)
        {
          current_tick = get_ticks();       // Check time now!
          int16 temp = rawTemp;
          send_data(segCode[temp%10], DIGIT_ONE);          temp = temp/10; delay_ms(1);  // Digit 1/100
          send_data(segCode[temp%10], DIGIT_TEN);          temp = temp/10; delay_ms(1);  // Digit 1/10
          send_data(segCode[temp%10]&0x7F, DIGIT_HUNDRED); temp = temp/10; delay_ms(1);  // Digit One, use 0x7F to make dot
          send_data(segCode[temp], DIGIT_THOUSAND);                        delay_ms(1);  // Digit Ten
        }
        previous_tick = current_tick;       // Update time
      }
      if(ds18b20_take_result(&rawTemp))
      { // Read successful
        // We need convert temperature raw value into degree Celsius, by use it divide to 16 (in °C = raw/16)
        // To take 2 digit of decimal, we need multiply to 100
        // Equation : rawTemp / 16 * 100 = rawTemp * 25 / 4
        // After waiting 750ms. We only need 250ms to display on LedSeg with new value
        rawTemp = rawTemp * 25 / 4;

        while(tick_difference(current_tick, previous_tick) < 250)
        {
          current_tick = get_ticks();       // Check time now!
          int16 temp = rawTemp;
          send_data(segCode[temp%10], DIGIT_ONE);          temp = temp/10; delay_ms(1);  // Digit 1/100
          send_data(segCode[temp%10], DIGIT_TEN);          temp = temp/10; delay_ms(1);  // Digit 1/10
          send_data(segCode[temp%10]&0x7F, DIGIT_HUNDRED); temp = temp/10; delay_ms(1);  // Digit One, use 0x7F to make dot
          send_data(segCode[temp], DIGIT_THOUSAND);                        delay_ms(1);  // Digit Ten
        }
        previous_tick = current_tick;       // Update time
      }
      else display_error();
    }
    else display_error();
  }
}
