#include <msp430.h>
#include <stdint.h>

void initTimer(){
    // Configure Timer to use SMCLK in Up to CCR0 mode
    CCTL0 = CCIE;                           // CCR0 interrupt enabled
    CCTL1 = CCIE;                           // CCR1 interrupt enabled
    TACTL = TASSEL_2 | MC_1 | ID_3;         // SMCLK, upmode, div/2
}

void setPwmFrequency(float hertz, float dutyCycle){
    static const float CLOCK_PERIOD_HZ = 1000000.0/8.0;
    volatile float period = 1.0 /hertz;
    volatile uint16_t CCR0count = period * CLOCK_PERIOD_HZ;
    volatile uint16_t CCR1count = (uint16_t)((float) CCR0count * dutyCycle);
    CCR0 = CCR0count;
    CCR1 = CCR1count;
}

int main(void){
    WDTCTL = WDTPW + WDTHOLD;               // Stop watchdog timer
    P1DIR |= BIT0;                          // Set P1.0 to output direction

    initTimer();

    __enable_interrupt();

    while(1){
        setPwmFrequency(200.0, 0.05);       // 200 Hz, 5% Duty Cycle
        __delay_cycles(1000000);
        setPwmFrequency(200.0, 0.10);       // 200 Hz, 10% Duty Cycle
        __delay_cycles(1000000);
        setPwmFrequency(200.0, 0.25);       // 200 Hz, 25% Duty Cycle
        __delay_cycles(1000000);
        setPwmFrequency(200.0, 0.5);        // 200 Hz, 50% Duty Cycle
        __delay_cycles(1000000);
        setPwmFrequency(200.0, 0.75);       // 200 Hz, 75% Duty Cycle
        __delay_cycles(1000000);
        setPwmFrequency(200.0, 0.99);       // 200 Hz, 99% Duty Cycle
        __delay_cycles(1000000);
    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void){
    P1OUT |= BIT0;                          // Set green LED
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR (void){
    P1OUT &= ~BIT0;                         // Clear green LED
    CCTL1 &= ~CCIFG;                        // Clear ISR Flag
}