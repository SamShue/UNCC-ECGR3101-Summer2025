#include <msp430.h>
#include <stdint.h>

void initTimer(){
    // Configure Timer to use SMCLK in Up to CCR0 mode
    CCTL0 = CCIE;                       // CCR0 interrupt enabled
    CCR0 = 4167;                        // Count value for 1s delay @ 12kHz
    TACTL = TASSEL_2 | MC_1 | ID_2;     // SMCLK, upmode, div/4
}

void setFrequency(float hertz){
    static const float CLOCK_FREQ_HZ = 1000000.0/4.0;
    volatile float period = 1.0 / (hertz * 2.0);
    volatile uint16_t count = period * CLOCK_FREQ_HZ;
    CCR0 = count;
}

int main(void){
  WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer

  initTimer();

  P1DIR |= 0x01;                        // Set P1.0 to output direction

  __enable_interrupt();

  setFrequency(1000.0);

  while(1){
  }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void){
    P1OUT ^= 0x01;                      // Toggle P1.0 using exclusive-OR
}