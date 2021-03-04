//// This code to demo function Interrupt of Timer0
//// It make Interrupt every 0.01s to count every 1s and display time over on LCD
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

// -------------------------------------------------------------------------------------------------- //

unsigned int8 n = 0;            // To do 100 times Interrupt of Timer0
unsigned long count = 0;        // Number Seconds
/*
 * This directives specify the following function is an interrupt function
 * Interrupt fires when timer0 overflows
 */
#INT_TIMER0                     // Timer 0 (RTCC) overflow, 
void Timer0_isr(void)           // Do this function, when Interrupt happens
{
  n++;
  if(n > 99)                    // Do 100 times
  {
    n = 0;
    count++;
  }
  set_timer0(61);               // Timer0 preload value, initializes the timer0 clock/counter, value 8 bit. All timers count up
  clear_interrupt(INT_TIMER0);  // Clear Timer0 overflow bit (interrupt flag of timer0)
}

// -------------------------------------------------------------------------------------------------- //

void main()
{
  lcd_init();         // Initializes the LCD module with above defined connections
  delay_ms(100);
  lcd_putc('\f');     // Clear display and set cursor to upper left
  lcd_putc("Time: ");

  /*
   * Internal oscillator used @ 20MHz
   *
   * Timer0_freq = MCU_freq / {4 * Prescaler * (256 - TMR0)}
   * 1/0.009984  = 20,000,000 / (4 * 256 * (256 - 61))
   *
   * Tolerance: 0.000016s for per 0.01s
   * That mean it will wrong 1s after 62500s ~ 17h21min40s
   */
  clear_interrupt(INT_TIMER0);              // Clear Timer0 overflow bit (interrupt flag of timer0)
  enable_interrupts(INT_TIMER0);            // Enable Timer0 interrupt
  enable_interrupts(GLOBAL);                // Enable global interrupts
  setup_timer_0(T0_INTERNAL | T0_DIV_256);  // Timer0 configuration: internal clock source + 256 prescaler
  set_timer0(61);                           // Timer0 preload value, initializes the timer0 clock/counter, value 8 bit. All timers count up
  
  while(true)
  {
    lcd_gotoxy(7, 1); // Set write position on LCD (Column 7, Row 1)
    printf(lcd_putc,"%Lum%Lus", count/60, count%60);
    delay_ms(501);
  }
}
