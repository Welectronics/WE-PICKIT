//// This library has been modified for suitable with DEV KIT PIC16F877A
//// It is used for thermal sensor DS18B20
////
//// NOTE: THE DEFAULT INTEGER VARIABLE DECLARATIONS IN CCS ARE "unsigned"
////
//// AUTHOR: TRUNG_KURO
#ifndef DS18B20_H
#define DS18B20_H

/* Check to know when the sensor ready to begin using */
int1 ds18b20_start()
{
  output_low(PIN_DATA_DS18B20);   // PIC send RESET PULSE to the DS18B20 sensor (minimum of 480µs)
  delay_us(480);

  output_float(PIN_DATA_DS18B20); // Configure PIN_DATA_DS18B20 pin as input, when the DS18B20 detects this rising edge, it waits 15µs to 60µs
  delay_us(60);                   // PIC need wait before read the DS18B20 sensor response

  if(!input(PIN_DATA_DS18B20))    // DS18B20 send PRESENCE PULSE to PIC, mean Reset Ok
  {
    delay_us(420);                // PIC need wait about 60µs to 240µs before read the DS18B20 sensor response, and until minimum of 480µs
    return 1;                     // DS18B20 sensor is present, it ready
  }
  return 0;                       // Reset failed
}

// -------------------------------------------------------------------------------------------------- //

/* Make sensor send data of 1 bit we want */
void ds18b20_write_bit(int1 value)
{
  output_low(PIN_DATA_DS18B20);     // PIC pulling the 1-Wire bus low

  if(value)                         // To send bit 1
  { // The bus master must release the 1-Wire bus within 15µs, and wait in WRITE TIME SLOT (at least 60µs)
    delay_us(1);
    output_float(PIN_DATA_DS18B20); // Configure PIN_DATA_DS18B20 pin as input
                                    // When the bus is released, the 5kΩ pullup resistor will pull the bus high
    delay_us(14);
    delay_us(45);
  }
  else                              // To send bit 0
  { // The bus master must continue to hold the bus low for the duration of the WRITE TIME SLOT (at least 60µs)
    delay_us(60);
    output_float(PIN_DATA_DS18B20); // Configure PIN_DATA_DS18B20 pin as input
                                    // When the bus is released, the 5kΩ pullup resistor will pull the bus high
  }                

  delay_us(1);                      // (T.REC) A minimum of a 1µs recovery time between individual write slots
}

/* Make sensor send data of 1 Byte we want */
void ds18b20_write_byte(int8 value)
{
  int8 i;
  for(i=0; i<8; i++) ds18b20_write_bit(bit_test(value, i));
}

// -------------------------------------------------------------------------------------------------- //

/* Return the value of 1 bit read from sensor */
int1 ds18b20_read_bit()
{
  output_low(PIN_DATA_DS18B20);         // PIC pulling the 1-Wire bus low
  delay_us(1);                          // (T.INT) For a minimum of 1µs and then releasing the bus

  // The master must release the bus and then sample the bus state within 15µs from the start of the READ TIME SLOT
  output_float(PIN_DATA_DS18B20);       // Configure PIN_DATA_DS18B20 pin as input
  delay_us(4);                          // Wait (T.RC) and then take "samples"... 
  int1 value = input(PIN_DATA_DS18B20); // Store value of bit read from sensor
  delay_us(10);

  delay_us(45);                         // For at least 60µs of TIME SLOT
  delay_us(1);                          // (T.REC) A minimum of a 1µs recovery time between individual write slots
  return value;
}

/* Return the value of 1 Byte read from sensor */
int8 ds18b20_read_byte()
{
  int8 i, value=0;
  for(i=0; i<8; i++) shift_right(&value, 1, ds18b20_read_bit());
  return value;
}

// -------------------------------------------------------------------------------------------------- //

int1 ds18b20_request_read()
{ // This function only returns 0 if had been ERROR
  if(!ds18b20_start()) return 0;  // Send start pulse,
  ds18b20_write_byte(0xCC);       // Send "Skip ROM" command
  ds18b20_write_byte(0x44);       // Send "Convert T" command
  return 1;                       // OK --> return 1
}

/* Remember use "while(ds18b20_read_byte() == 0);" to check when the convert of sensor finished and then read result */

int1 ds18b20_take_result(int16 *raw_temp_value)
{ // This function only returns 0 if had been ERROR
  if(!ds18b20_start()) return 0;                        // Send start pulse
  ds18b20_write_byte(0xCC);                             // Send "Skip ROM" command
  ds18b20_write_byte(0xBE);                             // Send "Read Scratchpad" command
  *raw_temp_value = ds18b20_read_byte();                // Read temperature LSB byte and store it on raw_temp_value LSB byte
  *raw_temp_value |= (int16)(ds18b20_read_byte()) << 8; // Read temperature MSB byte and store it on raw_temp_value MSB byte
  return 1;                                             // OK --> return 1
}

// -------------------------------------------------------------------------------------------------- //

#endif
