#include <msp430.h>

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;               // Stop WDT

    P1SEL = BIT1 | BIT2;                    // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 | BIT2;
    UCA0CTL1 |= UCSSEL_2;                   // SMCLK
    UCA0BR0 = 8;                            // 1MHz 115200
    UCA0BR1 = 0;                            // 1MHz 115200
    UCA0MCTL = UCBRS2 | UCBRS1;             // Modulation UCBRSx = 6
    UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                        // Enable USCI_A0 RX interrupt

    __enable_interrupt();
    
    while(1){
        UCA0TXBUF = 'A';                        // Print 'A' to UART
        __delay_cycles(1000000);
    }
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void){
    while (!(IFG2&UCA0TXIFG));              // USCI_A0 TX buffer ready?
    UCA0TXBUF = UCA0RXBUF;                  // TX -> RXed character
}