//// This code measure data from PZEM-004T and display that data on LCD
//// When push button SET, it will "Reset Energy"
//// And push button DOWN, to display "Voltage, Current, Power, Energy"
//// And push button UP, to display "Frequency, Power factor"
//// 
//// Note:  To make simple, I don't create function Set CRC and Get CRC
////        Therefore, sometime maybe appear some value error
////        And the address of PZEM default 0x01, suitable for all command below
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
 * To use with KIT DEV PIC (LCD + PZEM)

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
 * 3. Connect jumper for Button
 * RA1 (Pin A1) <-> SW5 (Button SET)
 * RA2 (Pin A2) <-> SW4 (Button DOWN)
 * RA3 (Pin A3) <-> SW3 (Button UP)
 */
#define LCD_RS_PIN      PIN_B3  // Register Select
#define LCD_RW_PIN      PIN_B2  // Read/Write
#define LCD_ENABLE_PIN  PIN_B1  // Enable
#define LCD_DATA4       PIN_D4  // Data 4
#define LCD_DATA5       PIN_D5  // Data 5
#define LCD_DATA6       PIN_D6  // Data 6
#define LCD_DATA7       PIN_D7  // Data 7

#define BUTTON_SET      PIN_A1
#define BUTTON_DOWN     PIN_A2
#define BUTTON_UP       PIN_A3

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

/*
 * PZEM-004T had a default communication interface
 * 
 * Baudrate   : 9600 bps
 *            : 8 data bits
 *            : 1 stop bit
 *            : No Parity
 *
 * CRC check  : CRC-16 (Modbus-RTU)
 *            : Polynomial = X^16 + X^15 + X^2 + 1
 *            : Poly Normal               = 0x8005
 *            : Poly Reversed             = 0xA001
 *            : Poly Reciprocal           = 0x4003
 *            : Poly Reversed reciprocal  = 0xC002
 *
 * Here I use this website to calculate CRC: http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
 * Predefined       : CRC16_MODBUS
 * Input reflected  : true
 * Result reflected : true
 * Polynomial       : 0x8005
 * Initial Value    : 0xFFFF
 * Final Xor Value  : 0x0000
 *
 * Broadcast Address  : 0x00
 * Slave Address      : 0x01 ~ 0xF7
 * General Address    : 0xF8
 *
 * List command for PZEM-004T:
 *    1. Set Address (default 0x01) -> 0xF8 + (0x06) + 0x00 + 0x02 + 0x00 + 0x01 + [0xFD + 0xA3]
 *    2. Reset Energy (Wh)          -> 0x01 + (0x42)                             + [0x80 + 0x11] 
 *    3. Voltage (V)                -> 0x01 + (0x04) + 0x00 + 0x00 + 0x00 + 0x01 + [0x31 + 0xCA]
 *    4. Current (A)                -> 0x01 + (0x04) + 0x00 + 0x01 + 0x00 + 0x02 + [0x20 + 0x0B]
 *    5. Power (W)                  -> 0x01 + (0x04) + 0x00 + 0x03 + 0x00 + 0x02 + [0x81 + 0xCB]
 *    6. Energy (Wh)                -> 0x01 + (0x04) + 0x00 + 0x05 + 0x00 + 0x02 + [0x61 + 0xCA]
 *    7. Frequency (Hz)             -> 0x01 + (0x04) + 0x00 + 0x07 + 0x00 + 0x01 + [0x80 + 0x0B]
 *    8. Power Factor               -> 0x01 + (0x04) + 0x00 + 0x08 + 0x00 + 0x01 + [0xB0 + 0x08]
 */

// -------------------------------------------------------------------------------------------------- //

float get_voltage()
{
  int8 cursor = 0;        // Point to positions on array
  int8 data[7] = {};      // Store temp raw data
  int16 voltage = 0;      // Store value "Voltage"

  putc(0x01); putc(0x04); // Call PZEM have address 0x01, to take value measure
  putc(0x00); putc(0x00); // Take value "Voltage"
  putc(0x00); putc(0x01); // It have 2 bytes
  putc(0x31); putc(0xCA); // CRC of this command

  while(true)
  {
    if(kbhit())                                         // If data has been received
    {
      data[cursor] = getc();                            // UART read
      cursor++;
      if(cursor==7)                                     // When you receive full string of data
      {
        voltage = ((int16)data[3]<<8) + (int16)data[4]; // Calculate value raw
        return ((float)voltage / (float)10.0);          // Return value "Voltage"
      }
    }
  }
}

float get_current()
{
  int8 cursor = 0;        // Point to positions on array
  int8 data[9] = {};      // Store temp raw data
  int32 current = 0;      // Store value "Current"

  putc(0x01); putc(0x04); // Call PZEM have address 0x01, to take value measure
  putc(0x00); putc(0x01); // Take value "Current"
  putc(0x00); putc(0x02); // It have 4 bytes
  putc(0x20); putc(0x0B); // CRC of this command

  while(true)
  {
    if(kbhit())                                         // If data has been received
    {
      data[cursor] = getc();                            // UART read
      cursor++;
      if(cursor==9)                                     // When you receive full string of data
      {
        current = ((int32)data[5]<<8) + (int32)data[6]; // Calculate value raw
        current = current << 16;
        current = current + (((int32)data[3]<<8) + (int32)data[4]);
        return ((float)current / (float)1000.0);        // Return value "Current"
      }
    }
  }
}

float get_power()
{
  int8 cursor = 0;        // Point to positions on array
  int8 data[9] = {};      // Store temp raw data
  int32 power = 0;        // Store value "Power"

  putc(0x01); putc(0x04); // Call PZEM have address 0x01, to take value measure
  putc(0x00); putc(0x03); // Take value "Power"
  putc(0x00); putc(0x02); // It have 4 bytes
  putc(0x81); putc(0xCB); // CRC of this command

  while(true)
  {
    if(kbhit())                                       // If data has been received
    {
      data[cursor] = getc();                          // UART read
      cursor++;
      if(cursor==9)                                   // When you receive full string of data
      {
        power = ((int32)data[5]<<8) + (int32)data[6]; // Calculate value raw
        power = power << 16;
        power = power + (((int32)data[3]<<8) + (int32)data[4]);
        return ((float)power / (float)10.0);          // Return value "Power"
      }
    }
  }
}

int32 get_energy()
{
  int8 cursor = 0;        // Point to positions on array
  int8 data[9] = {};      // Store temp raw data
  int32 energy = 0;       // Store value "Energy"

  putc(0x01); putc(0x04); // Call PZEM have address 0x01, to take value measure
  putc(0x00); putc(0x05); // Take value "Energy"
  putc(0x00); putc(0x02); // It have 4 bytes
  putc(0x61); putc(0xCA); // CRC of this command

  while(true)
  {
    if(kbhit())                                         // If data has been received
    {
      data[cursor] = getc();                            // UART read
      cursor++;
      if(cursor==9)                                     // When you receive full string of data
      {
        energy = ((int32)data[5]<<8) + (int32)data[6];  // Calculate value raw
        energy = energy << 16;
        energy = energy + (((int32)data[3]<<8) + (int32)data[4]);
        return energy;                                  // Return value "Energy"
      }
    }
  }
}

float get_frequency()
{
  int8 cursor = 0;        // Point to positions on array
  int8 data[7] = {};      // Store temp raw data
  int16 frequency = 0;    // Store value "Frequency"

  putc(0x01); putc(0x04); // Call PZEM have address 0x01, to take value measure
  putc(0x00); putc(0x07); // Take value "Frequency"
  putc(0x00); putc(0x01); // It have 2 bytes
  putc(0x80); putc(0x0B); // CRC of this command

  while(true)
  {
    if(kbhit())                                           // If data has been received
    {
      data[cursor] = getc();                              // UART read
      cursor++;
      if(cursor==7)                                       // When you receive full string of data
      {
        frequency = ((int16)data[3]<<8) + (int16)data[4]; // Calculate value raw
        return ((float)frequency / (float)10.0);          // Return value "Frequency"
      }
    }
  }
}

float get_power_factor()
{
  int8 cursor = 0;        // Point to positions on array
  int8 data[7] = {};      // Store temp raw data
  int16 powerFactor = 0;  // Store value "Power Factor"

  putc(0x01); putc(0x04); // Call PZEM have address 0x01, to take value measure
  putc(0x00); putc(0x08); // Take value "Power Factor"
  putc(0x00); putc(0x01); // It have 2 bytes
  putc(0xB0); putc(0x08); // CRC of this command

  while(true)
  {
    if(kbhit())                                             // If data has been received
    {
      data[cursor] = getc();                                // UART read
      cursor++;
      if(cursor==7)                                         // When you receive full string of data
      {
        powerFactor = ((int16)data[3]<<8) + (int16)data[4]; // Calculate value raw
        return ((float)powerFactor / (float)100.0);         // Return value "Power Factor"
      }
    }
  }
}

// -------------------------------------------------------------------------------------------------- //

void reset_energy()
{
  putc(0x01); putc(0x42); // Call PZEM have address 0x01, to Reset Energy
  putc(0x80); putc(0x11); // CRC of this command

  delay_ms(500);
}

// -------------------------------------------------------------------------------------------------- //

void main()
{
  float voltage;
  float current;
  float power;
  int32 energy;
  float frequency;
  float powerFactor;

  int1 mode = 1;  // Default display V,A,W,Wh
  lcd_init();     // Initializes the LCD module with above defined connections
  lcd_putc('\f'); // Clear display and set cursor to upper left

  while(true)
  {
    if(!input_state(BUTTON_SET))        reset_energy();
    else if(!input_state(BUTTON_DOWN))  mode = 1;
    else if(!input_state(BUTTON_UP))    mode = 0;

    if(mode)      // Display "Voltage, Current, Power, Energy"
    {
      voltage = get_voltage();
      current = get_current();
      power = get_power();
      energy = get_energy();

      lcd_gotoxy(1, 1); printf(lcd_putc, "%fV  %fA      ", voltage, current);
      lcd_gotoxy(1, 2); printf(lcd_putc, "%fW  %LuWh     ", power, energy);
    }
    else          // Display "Frequency, Power Factor"
    {
      frequency = get_frequency();
      powerFactor = get_power_factor();

      lcd_gotoxy(1, 1); printf(lcd_putc, "Fre %fHz      ", frequency);
      lcd_gotoxy(1, 2); printf(lcd_putc, "PF %f         ", powerFactor);
    }
    delay_ms(500);
  }
}
