#include <msp430.h>

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer

    // Configure LED on P1.0
    P1DIR |= BIT0;               // Set P1.0 to output
    P1OUT &= ~BIT0;              // LED off initially

    // Configure Timer_A
    BCSCTL3 |= LFXT1S_2;         // ACLK = VLO (~12kHz)
    TACTL = TASSEL_1 | ID_3 | MC_1 | TACLR; // ACLK, /8 divider, up mode, clear TAR
    CCR0 = 1500;                 // Timer period (~1 second with VLO and /8)
    CCTL0 = CCIE;                // Enable Timer_A interrupt

    __enable_interrupt();      // Enable global interrupts

    while (1) {
        _low_power_mode_3(); // Enter LPM3 with interrupts enabled
        __no_operation(); // For debugger
    }
}

// Timer A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
    P1OUT ^= BIT0;              // Toggle LED
    _low_power_mode_off_on_exit(); // Wake up from LPM3
}
