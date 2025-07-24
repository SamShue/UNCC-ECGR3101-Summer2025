#include "intrinsics.h"
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

void initTimer0(){
    P1DIR |= BIT6;    // Output PWM Pin
    P1SEL |= BIT6;    // Let P1.6 be controlled by Timer

    TA0CCR0 = 1000;    // PWM Period
    TA0CCR1 = 500;    // PWM duty cycle
    TA0CCTL1 = OUTMOD_7;  // TA0CCR1 high voltage below count, low voltage above count

    TA0CTL = TASSEL_2 | MC_0 | ID_3;
}

void initAdc10(){
    ADC10CTL1 = INCH_0 | ADC10DIV_3;
    ADC10CTL0 = SREF_0 | ADC10SHT_3 | REFON | ADC10ON;  // Vref+ = Vcc, Vref- = Vss
}

float getAdc10Voltage(){
    volatile uint16_t adcVal = ADC10MEM;
    volatile float voltage = ((float)adcVal) * (3.3/1024.0);
    __no_operation();
    return voltage;
}

static inline void startAdc10(){
    ADC10CTL0 |= ENC | ADC10SC;             // Sampling and conversion start
}

static inline bool adcIsBusy(){
    return ADC10CTL1 & ADC10BUSY;
}

static inline void startTimer0(){
    TA0CTL = TASSEL_2 | MC_1 | ID_3;
}

static inline void stopTimer0(){
    TA0CTL = TASSEL_2 | MC_0 | ID_3;
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;

    initAdc10();
    initTimer0();

    while(1){
        startAdc10();
        while(adcIsBusy());
        volatile float voltage = getAdc10Voltage();

        __no_operation();                       // SET BREAKPOINT HERE

        if(voltage > 1.8){
            startTimer0();
        } else {
            stopTimer0();
        }
    }
}