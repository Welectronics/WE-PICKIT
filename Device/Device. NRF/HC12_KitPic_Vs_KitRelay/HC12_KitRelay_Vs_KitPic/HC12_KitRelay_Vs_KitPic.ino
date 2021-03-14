//// This code uploaded for KIT RELAY to use with KIT PIC
#include <SoftwareSerial.h>
SoftwareSerial HC12(11, 10);         // HC-12 TX Pin, HC-12 RX Pin

#define RELAY_1 5
#define RELAY_2 6
#define RELAY_3 7
#define RELAY_4 8

#define BUTTON_1 A0
#define BUTTON_2 A1
#define BUTTON_3 A2
#define BUTTON_4 A3

#define SET 12

char incomingByte;
String readBuffer = "";

/* To remember state button, if not press now is HIGH */
boolean state_button_relay_1 = digitalRead(RELAY_1);
boolean state_button_relay_2 = digitalRead(RELAY_2);
boolean state_button_relay_3 = digitalRead(RELAY_3);
boolean state_button_relay_4 = digitalRead(RELAY_4);

/* To know status of Relay now */
boolean stateRelay_1 = 0, stateRelay_2 = 0, stateRelay_3 = 0, stateRelay_4 = 0;

void setup()
{
  /* Set up pin for Relay */
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);

  /* Set up pin for Button */
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);
  pinMode(BUTTON_4, INPUT);

  /* Turn off all relay */
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);
  digitalWrite(RELAY_3, HIGH);
  digitalWrite(RELAY_4, HIGH);

  pinMode(SET, OUTPUT);     // Set up pin of HC-12
  digitalWrite(SET, HIGH);  // HC-12 normal, transparent mode
  HC12.begin(9600);         // Open serial port to HC12
}

void loop()
{
  /* Storing the incoming data into a String variable */
  while(HC12.available())       // If HC-12 has data
  {
    incomingByte = HC12.read(); // Store each icoming byte from HC-12
    readBuffer += incomingByte; // Add each byte to ReadBuffer string variable

    if(incomingByte == '\n')
    {
      /* Process data had receiver */
      if(readBuffer == "11\n")      {digitalWrite(RELAY_1, LOW);  stateRelay_1 = !stateRelay_1;}
      else if(readBuffer == "10\n") {digitalWrite(RELAY_1, HIGH); stateRelay_1 = !stateRelay_1;}
      else if(readBuffer == "21\n") {digitalWrite(RELAY_2, LOW);  stateRelay_2 = !stateRelay_2;}
      else if(readBuffer == "20\n") {digitalWrite(RELAY_2, HIGH); stateRelay_2 = !stateRelay_2;}
      else if(readBuffer == "31\n") {digitalWrite(RELAY_3, LOW);  stateRelay_3 = !stateRelay_3;}
      else if(readBuffer == "30\n") {digitalWrite(RELAY_3, HIGH); stateRelay_3 = !stateRelay_3;}
      else if(readBuffer == "41\n") {digitalWrite(RELAY_4, LOW);  stateRelay_4 = !stateRelay_4;}
      else if(readBuffer == "40\n") {digitalWrite(RELAY_4, HIGH); stateRelay_4 = !stateRelay_4;}
      readBuffer = "";          // Clear readBuffer
    }
  }

  /* Process button */
  if(!digitalRead(BUTTON_1) && state_button_relay_1)                            // If the button is pressed, and the EdgeDown happen
  {
    delay(10);                                                                  // Button Debounce
    if(!digitalRead(BUTTON_1) && state_button_relay_1)                          // If the button is still pressed, happen status EdgeDown
    {
      state_button_relay_1 = 0;
      if(stateRelay_1)  {HC12.write("10\n"); digitalWrite(RELAY_1, HIGH);}      // Turn off Relay 1
      else              {HC12.write("11\n"); digitalWrite(RELAY_1, LOW);}       // Turn on Relay 1
      stateRelay_1 = !stateRelay_1;
    }
  }
  if(!state_button_relay_1) state_button_relay_1 = digitalRead(BUTTON_1);       // Check when happen the EdgUp
  ////////////////////////////////////////////////////////////////////////////////
  if(!digitalRead(BUTTON_2) && state_button_relay_2)                            // If the button is pressed, and the EdgeDown happen
  {
    delay(10);                                                                  // Button Debounce
    if(!digitalRead(BUTTON_2) && state_button_relay_2)                          // If the button is still pressed, happen status EdgeDown
    {
      state_button_relay_2 = 0;
      if(stateRelay_2)  {HC12.write("20\n"); digitalWrite(RELAY_2, HIGH);}      // Turn off Relay 2
      else              {HC12.write("21\n"); digitalWrite(RELAY_2, LOW);}       // Turn on Relay 2
      stateRelay_2 = !stateRelay_2;
    }
  }
  if(!state_button_relay_2) state_button_relay_2 = digitalRead(BUTTON_2);       // Check when happen the EdgUp
  ////////////////////////////////////////////////////////////////////////////////
  if(!digitalRead(BUTTON_3) && state_button_relay_3)                            // If the button is pressed, and the EdgeDown happen
  {
    delay(10);                                                                  // Button Debounce
    if(!digitalRead(BUTTON_3) && state_button_relay_3)                          // If the button is still pressed, happen status EdgeDown
    {
      state_button_relay_3 = 0;
      if(stateRelay_3)  {HC12.write("30\n"); digitalWrite(RELAY_3, HIGH);}      // Turn off Relay 3
      else              {HC12.write("31\n"); digitalWrite(RELAY_3, LOW);}       // Turn on Relay 3
      stateRelay_3 = !stateRelay_3;
    }
  }
  if(!state_button_relay_3) state_button_relay_3 = digitalRead(BUTTON_3);       // Check when happen the EdgUp
  ////////////////////////////////////////////////////////////////////////////////
  if(!digitalRead(BUTTON_4) && state_button_relay_4)                            // If the button is pressed, and the EdgeDown happen
  {
    delay(10);                                                                  // Button Debounce
    if(!digitalRead(BUTTON_4) && state_button_relay_4)                          // If the button is still pressed, happen status EdgeDown
    {
      state_button_relay_4 = 0;
      if(stateRelay_4)  {HC12.write("40\n"); digitalWrite(RELAY_4, HIGH);}      // Turn off Relay 4
      else              {HC12.write("41\n"); digitalWrite(RELAY_4, LOW);}       // Turn on Relay 4
      stateRelay_4 = !stateRelay_4;
    }
  }
  if(!state_button_relay_4) state_button_relay_4 = digitalRead(BUTTON_4);       // Check when happen the EdgUp
}
