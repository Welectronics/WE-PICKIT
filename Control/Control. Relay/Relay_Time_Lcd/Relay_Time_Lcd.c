//// This code demo of apply set up time run for Relay, has info on Lcd
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
 * _ 4-bit mode of LCD
 * _ Button - A1/A2/A3 of Port A for Button SET/DOWN/UP, it will receive LOW when the button pressed
 * _ Led - D7 of Port D for Led_1, it need output LOW to turn on the led
 * _ Relay - C0 of Port C for turn on/off relay, it need output LOW to turn on relay
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
 * 
 * 3. Connect jumper for Button and Relay
 *           5V <-> VRL (Voltage for Relay)
 * RC0 (Pin C0) <-> SRL (Signal for Relay)
 * RA1 (Pin A1) <-> SW5 (Button SET)
 * RA2 (Pin A2) <-> SW4 (Button DOWN)
 * RA3 (Pin A3) <-> SW3 (Button UP)
 * RD0 (Pin D0) <-> L8  (Led 8)
 */
#define LCD_RS_PIN      PIN_B3  // Register Select
#define LCD_RW_PIN      PIN_B2  // Read/Write
#define LCD_ENABLE_PIN  PIN_B1  // Enable
#define LCD_DATA4       PIN_D4  // Data 4
#define LCD_DATA5       PIN_D5  // Data 5
#define LCD_DATA6       PIN_D6  // Data 6
#define LCD_DATA7       PIN_D7  // Data 7

#define BUTTON_UP       PIN_A3
#define BUTTON_DOWN     PIN_A2
#define BUTTON_SET      PIN_A1
#define LED             PIN_D0
#define RELAY           PIN_C0

/* Need include library of lcd after function delay */
#include <LCD.c>

// -------------------------------------------------------------------------------------------------- //

/*
 * Internal oscillator used @ 20MHz
 * Timer1 period = 0.1s
 *
 * Timer1_freq = MCU_freq / {4 * Prescaler * (65536 - TMR1)}
 * 1/0.1       = 20,000,000 / (4 * 8 * (65536 - 3036))
 *
 * Tolerance: 0
 */

unsigned int8 n = 0;            // To do 10 times Interrupt of Timer1
unsigned long count = 0;        // Number Seconds
unsigned long timeSet = 0;      // Time to run Relay
int1 running = 0;               // Relay is (running) on or off ?

/*
 * This directives specify the following function is an interrupt function
 * Interrupt fires when timer1 overflows
 */
#INT_TIMER1                     // Timer 1 overflow
void Timer1_isr(void)           // Do this function, when Interrupt happens
{
  n++;
  if(n > 9)                     // Do 10 times
  {
    n = 0;
    count++;                    // Every 1s
  }
  set_timer1(3036);             // Timer1 preload value, initializes the timer1 clock/counter, value 16 bit. All timers count up
  clear_interrupt(INT_TIMER1);  // Clear Timer1 overflow bit (interrupt flag of timer1)
}

// -------------------------------------------------------------------------------------------------- //

/* Set Timer and Interrupt, ready for run Relay */
void setup_open_timer1(void)
{
  clear_interrupt(INT_TIMER1);              // Clear Timer1 overflow bit (interrupt flag of timer1)
  enable_interrupts(INT_TIMER1);            // Enable Timer1 interrupt
  enable_interrupts(GLOBAL);                // Enable global interrupts
  setup_timer_1(T1_INTERNAL | T1_DIV_BY_8); // Timer1 configuration: internal clock source + 8 prescaler
  set_timer1(3036);                         // Timer1 preload value, initializes the timer1 clock/counter, value 16 bit. All timers count up
}

/* Set to turn off Interrupt */
void setup_close_timer1(void)
{
  disable_interrupts(INT_TIMER1);           // Disable Timer1 interrupt
  disable_interrupts(GLOBAL);               // Disable global interrupts
}

// -------------------------------------------------------------------------------------------------- //

void main()
{
  output_high(RELAY); // Turn off Relay
  output_high(LED);   // Turn off Led

  lcd_init();         // Initializes the LCD module with above defined connections
  delay_ms(100);
  lcd_putc('\f');     // Clear display and set cursor to upper left
  lcd_putc("Set time: "); printf(lcd_putc,"%Lu(s)  ", timeSet);

  while(true)
  {
    if(!input(BUTTON_UP) && !running)   {timeSet++; lcd_gotoxy(11, 1); printf(lcd_putc,"%Lu(s)  ", timeSet);}
    if(!input(BUTTON_DOWN) && !running) {if(timeSet > 0) timeSet--; lcd_gotoxy(11, 1); printf(lcd_putc,"%Lu(s)  ", timeSet);}

    if(!input(BUTTON_SET) && !running)
    {
      delay_ms(1);                        // Debounce Button
      if(!input(BUTTON_SET) && !running)  // Ok...
      {
        lcd_gotoxy(1, 2); lcd_putc("Remain: ");
        output_low(RELAY);                // Turn on Relay
        output_low(LED);                  // Turn on Led
        n = 0; count = 0; running = 1;
        setup_open_timer1();
      }
    }

    if(running)             // Relay is on
    {
      lcd_gotoxy(9, 2); printf(lcd_putc,"%Lum%Lus ", (timeSet-count)/60, (timeSet-count)%60);
      if(timeSet-count == 0)
      {
        setup_close_timer1();
        lcd_gotoxy(1, 2); lcd_putc("                ");
        output_high(RELAY); // Turn off Relay
        output_high(LED);   // Turn off Led
        running = 0;
      }
    }
    delay_ms(501);
  }
}
