#include <msp430.h>
#include <stdint.h>

void initClocks(){
    // Configure SMCLK at 12 kHz
    BCSCTL3 = LFXT1S_2;           // LFXT1 = VLO
    IFG1 &= ~OFIFG;               // Clear OSCFault flag
    BCSCTL2 = SELS | DIVS_0;      // SMCLK = LFXT1/1
}

void initTimer(){
    // Configure Timer to use SMCLK in Up to CCR0 mode
    CCTL0 = CCIE;                 // CCR0 interrupt enabled
    CCR0 = 18000;                 // Count value for 1.5s delay @ 12kHz
    TACTL = TASSEL_2 | MC_1;      // SMCLK, countmode
}

int main(void){
  WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog timer

  initClocks();
  initTimer();

  P1DIR |= 0x01;                 // Set P1.0 to output direction

  __enable_interrupt();

  while(1){
  }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void){
    P1OUT ^= 0x01;               // Toggle P1.0 using exclusive-OR
}
