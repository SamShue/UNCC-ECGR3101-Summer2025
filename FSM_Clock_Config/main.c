#include <msp430.h>
#include <stdbool.h>

static inline void setGreenLed(bool enable){
    P1OUT = enable ? P1OUT | BIT0 : P1OUT & ~BIT0;
}

static inline bool isButtonPressed(){
    return (P1IN & BIT3) == 0x00;
}

void initClocks(){
    BCSCTL3 = LFXT1S_2;         // Configure VLO clock as LFXT1CLK
    IFG1 &= ~OFIFG;             // Clear fault flag
    BCSCTL2 = SELM_3 | DIVM_0;  // Select VLO clock as MCLK, set /1 MCLK divider
}

void init(){
    WDTCTL = 0x5A80; // Stop the watchdog timer

    initClocks();

    P1DIR = ~BIT3;
    P1REN = BIT3;
    P1OUT = BIT3;
}

int main(){
    init();

    while(1){
        setGreenLed(true);
        __delay_cycles(32768);
        setGreenLed(false);
        __delay_cycles(32768);
    }
}
