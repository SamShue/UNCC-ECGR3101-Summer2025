#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 1600

uint16_t adcBuffer[BUFFER_SIZE];
uint16_t bufferIdx = 0;

void initTimerA0(){
    CCTL0 = CCIE;                       // CCR0 interrupt enabled
    CCR0 = 125;                         // Count value for 125 us delay @ 1 MHz
    TACTL = TASSEL_2 | MC_0 | ID_2;     // SMCLK, stop mode, div/4
}

void initAdc10(){
    ADC10CTL1 = ADC10SSEL_3 | INCH_0 | ADC10DIV_0;
    ADC10CTL0 = SREF_1 | ADC10SHT_2 | REFON | REF2_5V | ADC10ON;  // Vref+ = 2.5 V, Vref- = Vss, 16 Sample and Hold Cycles
}

void initGpio(){
    // Button
    P1DIR &= ~BIT3;
    P1REN |= BIT3;
    P1OUT |= BIT3;

    // Green and Red LED
    P1DIR |= BIT0 | BIT6;
}

static inline void startTimerA0(){
    TACTL = TASSEL_2 | MC_1 | ID_2;     // SMCLK, up mode, div/4
}

static inline void stopTimerA0(){
    TACTL = TASSEL_2 | MC_0 | ID_2;     // SMCLK, stop mode, div/4
}


static inline void startAdc10(){
    ADC10CTL0 |= ENC | ADC10SC;             // Sampling and conversion start
}

static inline bool isButtonPressed(){
    return (P1IN & BIT3) == 0x00;
}

static inline void setGreenLed(bool enable){
    if(enable){
        P1OUT |= BIT0;
    }else{
        P1OUT &= ~BIT0;
    }
}

int main(void){
  WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer

  initGpio();
  initTimerA0();
  initAdc10();

  P1DIR |= 0x01;                        // Set P1.0 to output direction

  __enable_interrupt();

  while(1){
      if(isButtonPressed()){
          startTimerA0();
          setGreenLed(true);
          while(bufferIdx < BUFFER_SIZE);
          setGreenLed(false);
      }
  }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void ta0_isr(void){
    startAdc10();
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void){
    adcBuffer[bufferIdx] = ADC10MEM;
    bufferIdx++;

    if(bufferIdx >= BUFFER_SIZE){
        stopTimerA0();
    }
}
