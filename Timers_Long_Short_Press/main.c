#include <msp430.h>
#include <stdint.h>

void initClocks(){
    // Configure SMCLK at 12 kHz
    BCSCTL3 = LFXT1S_2;           // LFXT1 = VLO
    IFG1 &= ~OFIFG;               // Clear OSCFault flag
    BCSCTL2 = SELS | DIVS_0;      // SMCLK = LFXT1/1
}

void initTimer(){
    // Configure Timer to use SMCLK in stop mode
    TACTL = TASSEL_2 | MC_0 | ID_3;      // SMCLK, stop mode, div / 8
}

void initGpio(){
    // Button config
    P1DIR &= ~BIT3; // set button as input
    P1REN |= BIT3;  // enable internal resistor
    P1OUT |= BIT3;  // enable pull-up

    // LED configs
    P1DIR |= BIT0 | BIT6;   // set green and red LED outputs
}

void init(){
    WDTCTL = WDTPW | WDTHOLD;    // stop the watchdog
    initClocks();
    initGpio();
    initTimer();
}

int main(void){
    init();

    while(1){
        if((P1IN & BIT3) == 0x00){
            TACTL |= MC_2;                                  // start continuous mode
            while((P1IN & BIT3) == 0x00);                   // wait until button is released
            TACTL = TASSEL_2 | MC_0 | ID_3;                 // stop the timer
            uint16_t count = TA0R;                          // record count value
            float delay_s = ((float)count) * 8.0/12000.0;   // convert counts to seconds

            if(delay_s >= 0.5){
                // long press detected!
                P1OUT ^= BIT0;  // toggle green LED
            } else {
                // short press detected!
                P1OUT ^= BIT6;  // toggle red LED
            }
            TA0R = 0x0000; // reset count value
        }
    }
}
