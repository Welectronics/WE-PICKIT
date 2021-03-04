//// This code demo function of Relay, when push button SET the Relay will turn on
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
 * To use with KIT DEV PIC (Relay)
 * 
 * Connect Jumpers below:
 *            5V <-> VRL (Voltage for Relay)
 * RC0 (Pin C0) <-> SRL (Signal for Relay)
 * RA1 (Pin A1) <-> SW5 (Button SET)
 * RD7 (Pin D7) <-> L1  (Led 1)
 * 
 * Led - D7 of PortD for Led_1, it need output LOW to turn on the led
 * Relay - C0 of PortC for turn on/off relay, it need output LOW to turn on relay
 * Button - A1 of PortA for Button SET, it will receive LOW when the button pressed
 */
#define LED_1       PIN_D7
#define RELAY       PIN_C0
#define BUTTON_SET  PIN_A1

void main()
{
  while(true)
  {
    output_bit(LED_1, input(BUTTON_SET));  // Led on, when button press
    output_bit(RELAY, input(BUTTON_SET));  // Relay on, when button press
  }
}
