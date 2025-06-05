#include <msp430.h>

int main(){
    WDTCTL = 0x5A80; // Stop the watchdog timer

    P1DIR = 0x01;
    P1REN = 0x08;
    P1OUT = 0x08;

    while(1){
        if((P1IN & 0x08) == 0x00){ // Masks P1 with 0b0000 1000 to clear all bits other than bit3
            P1OUT |= 0x01;   // Masks P1OUT with 0b0000 0001 to set bit0
        } else {
            P1OUT &= 0xFE;   // Masks P1OUT with 0b1111 1110 to clear bit0
        }
    }
}