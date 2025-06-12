#include <msp430.h>
#include <stdbool.h>

static inline void setGreenLed(bool enable){
    P1OUT = enable ? P1OUT | BIT0 : P1OUT & ~BIT0;
}

static inline bool isButtonPressed(){
    return (P1IN & BIT3) == 0x00;
}

void init(){
    WDTCTL = 0x5A80; // Stop the watchdog timer

    P1DIR = ~BIT3;
    P1REN = BIT3;
    P1OUT = BIT3;
}

int main(){
    init();

    while(1){
        if(isButtonPressed()){
            setGreenLed(true);
        } else {
            setGreenLed(false);
        }
    }
}
