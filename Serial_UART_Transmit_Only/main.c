#include <msp430.h>

void uart_init(void);
void uart_write_char(char c);
void uart_write_string(const char *str);

void main(void){
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    uart_init();                // Initialize UART

    while (1)
    {
        uart_write_string("Hello, world!\r\n");
        __delay_cycles(1000000); // Delay ~1 second at 1 MHz
    }
}

void uart_init(void){
    // Set pin function for UART
    P1SEL  |= BIT1 + BIT2;    // P1.1 = TXD, P1.2 = RXD
    P1SEL2 |= BIT1 + BIT2;

    UCA0CTL1 |= UCSWRST;      // Put state machine in reset
    UCA0CTL1 |= UCSSEL_2;     // SMCLK

    // Baud rate: 1 MHz / 9600 = ~104.17
    UCA0BR0 = 104;            // 1 MHz 9600
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;        // Modulation UCBRSx = 1

    UCA0CTL1 &= ~UCSWRST;     // Initialize USCI state machine
}

void uart_write_char(char c) {
    while (!(IFG2 & UCA0TXIFG)); // Wait until TX buffer is ready
    UCA0TXBUF = c;
}

void uart_write_string(const char *str){
    while (*str)
        uart_write_char(*str++);
}
