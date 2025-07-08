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
    CCTL0 = CCIE;                   // CCR0 interrupt enabled
    CCR0 = 1500;                   // Count value for 1s delay @ 12kHz
    TACTL = TASSEL_2 | MC_0 | ID_3; // SMCLK, stop mode, /8
}

static inline void startTimer(){
    TACTL |= MC_1;  // start count mode
}

int main(void){
  WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog timer

  initClocks();
  initTimer();

  P1DIR |= BIT0;
  P1OUT |= BIT3;
  P1REN |= BIT3;

  __enable_interrupt();


  while(1){
      if((P1IN & BIT3) == 0x00){
          P1OUT |= BIT0;
          startTimer();
      }
  }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void){
    P1OUT &= ~BIT0;         // Turn off LED

    // Set timer mode to stop to create "one-shot"
    // Re-writing entire control register to avoid dealing
    // with weird masks to set MC bits to "00".
    TACTL = TASSEL_2 | MC_0 | ID_3; // SMCLK, stop mode, /8
}
