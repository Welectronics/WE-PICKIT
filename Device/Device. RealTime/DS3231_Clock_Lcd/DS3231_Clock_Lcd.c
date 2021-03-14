//// This code demo function RealTime, it read data from DS3231 by protocol I2C
//// It display TIME "Hour : Minute : Second" on Lcd
//// And display DAY "Date : Month : Year"
//// Beside you can setup TIME and DAY by Button ...
////    SET  -> 1 Click ... chosen Minute/Hour/Date/Month/Year to setup
////    DOWN -> 1 Click or Hold Click ... Increase value
////    UP   -> 1 Click or Hold Click ... Decrease value
//// Note: to easy i don't have add functions check the time from setup of user is available or not
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
 * Options for I2C:
 * MASTER         Sets to the master mode
 * FAST=nnnnnn    Sets the speed to "nnnnnn" HZ
 * 
 * Remember "Address I2C" only have 7-bit or large is 10-bit
 * Here, we use with DS3231, it have "Address I2C" is 0x68 = 0110.1000
 * When we want access 1 device to Write/Read -> we use 7-bit "Address I2C" + 1-bit "Read/Write" to call that device
 * 
 * Ex:                      Bit 7 - 6 - 5 - 4 - 3 - 2 - 1 | 0
 *                          ------------------------------|---------
 * To Wite (Master -> Device)   1   1   0   1   0   0   0 | 0 = 0xD0
 * To Read (Device -> Master)   1   1   0   1   0   0   0 | 1 = 0xD1
 */
#define PIN_SCL         PIN_C3
#define PIN_SDA         PIN_C4
/*
 * 3. Adj of the switch SW2 to connect DS3231 with ...
 * RC3 (Pin C3) <-> SCL DS
 * RC4 (Pin C4) <-> SDA DS
 */
//// #use i2c(master, i2c1, fast = 100000)
#use i2c(master, scl = PIN_SCL, sda = PIN_SDA, fast = 100000)

/*
 * 4. Connect jumper for Button
 * RA1 (Pin A1) <-> SW5 (SET)
 * RA2 (Pin A2) <-> SW4 (DOWN)
 * RA3 (Pin A3) <-> SW3 (UP)
 */
#define BUTTON_SET      PIN_A1
#define BUTTON_DOWN     PIN_A2
#define BUTTON_UP       PIN_A3

// -------------------------------------------------------------------------------------------------- //

/*
 * Convert BCD to Decimal
 */
void convert_BCD_Decimal(int8 time[])
{ // It include: Seconds, Minute, Hour, Date, Month, Year
  // The format BCD does not store value DEC in normal format of Binary
  // It use 4 bit corresponding for 10 digit "0-9" that is 10 number from "0-9"
  // With 4bit MSB for digit TEN, and 4 bit LSB for digit ONE
  time[0] = (time[0] >> 4) * 10 + (time[0] & 0x0F);
  time[1] = (time[1] >> 4) * 10 + (time[1] & 0x0F);
  time[2] = (time[2] >> 4) * 10 + (time[2] & 0x0F);
  time[3] = (time[3] >> 4) * 10 + (time[3] & 0x0F);
  time[4] = (time[4] >> 4) * 10 + (time[4] & 0x0F);
  time[5] = (time[5] >> 4) * 10 + (time[5] & 0x0F);
}

/*
 * Convert Decimal to BCD
 */
void convert_Decimal_BCD(int8 time[])
{ // It include: Seconds, Minute, Hour, Date, Month, Year
  time[0] = ((time[0] / 10) << 4) + (time[0] % 10);
  time[1] = ((time[1] / 10) << 4) + (time[1] % 10);
  time[2] = ((time[2] / 10) << 4) + (time[2] % 10);
  time[3] = ((time[3] / 10) << 4) + (time[3] % 10);
  time[4] = ((time[4] / 10) << 4) + (time[4] % 10);
  time[5] = ((time[5] / 10) << 4) + (time[5] % 10);
}

// -------------------------------------------------------------------------------------------------- //

/*
 * Write data of time into DS3231
 */
void write_DS3231(int8 time[])
{
  i2c_start();        // Start I2C protocol
  //////////////////////
  i2c_write(0xD0);    // To PIC WRITE data to DS3231
  i2c_write(0x00);    // Access to "Address Map" (0x00) of the DS3231
  i2c_write(0);       // 0x00 -> Reset SECONDS, mean write value 0 and start oscillator
  i2c_write(time[1]); // 0x01 -> Write MINUTE value to DS3231
  i2c_write(time[2]); // 0x02 -> Write HOUR value to DS3231
  i2c_write(1);       // 0x03 -> Write DAY value (not used)
  i2c_write(time[3]); // 0x04 -> Write DATE value to DS3231
  i2c_write(time[4]); // 0x05 -> Write MONTH value to DS3231
  i2c_write(time[5]); // 0x06 -> Write YEAR value to DS3231
  //////////////////////
  i2c_stop();         // Stop I2C protocol
}

/* 
 * Read data time stored in DS3231
 * 
 * Here, when the command "i2c_start()" call sencond before "i2c_stop()" that mean RESET I2C bus
 * 
 * i2c_read(1) -> indicates to ACK
 * i2c_read(0) -> indicates do NOT ACK  -> that mean that the last data of device send, before stop
 */
void read_DS3231(int8 time[])
{
  i2c_start();            // Start I2C protocol
  //////////////////////////
  i2c_write(0xD0);        // To PIC WRITE data to DS3231
  i2c_write(0x00);        // Access to "Address Map" (0x00) of the DS3231, chosen position to begin read data of device
  //////////////////////////
  i2c_start();            // Restart I2C -> when you call again this command, before use command stop
  i2c_write(0xD1);        // To PIC READ data to DS3231
  time[0] = i2c_read(1);  // Read SECONDS from register 0x00
  time[1] = i2c_read(1);  // Read MINUTE from register 0x01
  time[2] = i2c_read(1);  // Read HOUR from register 0x02
            i2c_read(1);  // Read DAY from register 0x03 (not used)
  time[3] = i2c_read(1);  // Read DATE from register 0x04
  time[4] = i2c_read(1);  // Read MONTH from register 0x05
  time[5] = i2c_read(0);  // Read YEAR from register 0x06
  //////////////////////////
  i2c_stop();             // Stop I2C protocol
}

// -------------------------------------------------------------------------------------------------- //

/* Seconds, Minute, Hour, Date, Month, Year */
int8 dataTime[6];

void main()
{
  /* To remember state button, if not press now is HIGH */
  int1 state_button_set  = input(BUTTON_SET);
  int1 state_button_down = input(BUTTON_DOWN);
  int1 state_button_up   = input(BUTTON_UP);

  /* To know what display, it base on number point of array "dataTime", with number 0 display TIME + DATE */
  int8 menu = 0;

  lcd_init();     // Initializes the LCD module with above defined connections
  lcd_putc('\f'); // Clear display and set cursor to upper left

  while(true)
  {
    if(!input(BUTTON_SET) && state_button_set)                      // If the button is pressed, Edge Down happen
    {
      state_button_set = 0;
      if(menu==0) // Go out display Time, to come Setup
      {
        read_DS3231(dataTime);
        convert_BCD_Decimal(dataTime);
        lcd_putc('\f');
      }
      //////////////
      if(menu>=5) menu = 0;
      else menu++;
      //////////////
      if(menu==0) // Comeback display Time
      {
        convert_Decimal_BCD(dataTime);
        write_DS3231(dataTime);
        lcd_putc('\f');
      }
    }
    if(!state_button_set) state_button_set = input(BUTTON_SET);     // Check when the button release, Edge Up happen

    switch(menu)
    {
      case 0:
      {
        read_DS3231(dataTime);
        convert_BCD_Decimal(dataTime);
        lcd_gotoxy(1, 1); printf(lcd_putc, "TIME:%02d:%02d:%02d   ", dataTime[2], dataTime[1], dataTime[0]);
        lcd_gotoxy(1, 2); printf(lcd_putc, "DATE:%02d/%02d/20%02d ", dataTime[3], dataTime[4], dataTime[5]);
        delay_ms(250);
        break;
      }
      case 1: lcd_gotoxy(1, 1); printf(lcd_putc, "Set Minute: %02d  ", dataTime[1]); delay_ms(100); break;
      case 2: lcd_gotoxy(1, 1); printf(lcd_putc, "Set Hour  : %02d  ", dataTime[2]); delay_ms(100); break;
      case 3: lcd_gotoxy(1, 1); printf(lcd_putc, "Set Date  : %02d  ", dataTime[3]); delay_ms(100); break;
      case 4: lcd_gotoxy(1, 1); printf(lcd_putc, "Set Month : %02d  ", dataTime[4]); delay_ms(100); break;
      case 5: lcd_gotoxy(1, 1); printf(lcd_putc, "Set Year  : %02d  ", dataTime[5]); delay_ms(100); break;
    }

    if(!input(BUTTON_DOWN) && state_button_down)                    // If the button is pressed, Edge Down happen
    {
      state_button_down = 0;
      switch(menu)
      {
        case 1: if(dataTime[1] > 0) dataTime[1]--;
        case 2: if(dataTime[2] > 0) dataTime[2]--;
        case 3: if(dataTime[3] > 1) dataTime[3]--;
        case 4: if(dataTime[4] > 1) dataTime[4]--;
        case 5: if(dataTime[5] > 0) dataTime[5]--;
      }
    }
    if(!state_button_down) state_button_down = input(BUTTON_DOWN);  // Check when the button release, Edge Up happen

    if(!input(BUTTON_UP) && state_button_up)                        // If the button is pressed, Edge Down happen
    {
      state_button_up = 0;
      switch(menu)
      {
        case 1: if(dataTime[1] < 59) dataTime[1]++;
        case 2: if(dataTime[2] < 23) dataTime[2]++;
        case 3: if(dataTime[3] < 31) dataTime[3]++;
        case 4: if(dataTime[4] < 12) dataTime[4]++;
        case 5: if(dataTime[5] < 99) dataTime[5]++;
      }
    }
    if(!state_button_up) state_button_up = input(BUTTON_UP);        // Check when the button release, Edge Up happen
  }
}
