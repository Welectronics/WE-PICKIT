//// This library has been modified for suitable with DEV KIT PIC16F877A
//// It is used for Led 7 Segment with 74HC595
////
//// NOTE: THE DEFAULT INTEGER VARIABLE DECLARATIONS IN CCS ARE "unsigned"
////
//// AUTHOR: TRUNG_KURO
#ifndef LEGSEG_H
#define LEGSEG_H

/* With 1 pulse over, logic value at PIN_DATA went to the "Shift Register" */
void clock_signal()
{
  output_high(PIN_CLOCK);
  output_low(PIN_CLOCK);
}

/* With 1 pulse over, logic value of the "Shift Register" transferred to the "Storage Register" */
void latch_enable()
{
  output_high(PIN_LATCH);
  output_low(PIN_LATCH);
}

// -------------------------------------------------------------------------------------------------- //

/* Take every bit of data, then transfer to the "Shift Register", and then transfer to the "Storage Register" */
void send_data(int8 data_led, int8 data_digit)
{
  signed int8 i;
  for(i=7; i>=0; i--)
  { // Transfer first bit from MSB to LSB of data Led
    output_bit(PIN_DATA, bit_test(data_led, i));
    clock_signal(); // Allows bit-by-bit transfer
  }
  for(i=7; i>=0; i--)
  { // Transfer first bit from MSB to LSB of data Digit
    output_bit(PIN_DATA, bit_test(data_digit, i));
    clock_signal(); // Allows bit-by-bit transfer
  }
  latch_enable();   // Data finally submitted
}

#endif
