#include <msp430.h>
#include <stdint.h>

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;

    ADC10CTL1 = INCH_10 | ADC10DIV_3;         // Temp Sensor ADC10CLK/4
    ADC10CTL0 = SREF_1 | ADC10SHT_3 | REFON | ADC10ON;

    while(1){
        ADC10CTL0 |= ENC | ADC10SC;             // Sampling and conversion start
        while(ADC10CTL1 & ADC10BUSY);

        volatile uint16_t adcVal = ADC10MEM;

        volatile float voltage = ((float)adcVal) * (1.5/1024.0);

        volatile float degC = (voltage - 0.986)/0.00355;

        __no_operation();                       // SET BREAKPOINT HERE
    }
}