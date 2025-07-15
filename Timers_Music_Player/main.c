#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>
#include "notes.h"
#include "songs.h"

void initGpio();
void initTimerA0();
bool isButtonPressed();

void delay_ms(uint64_t delayTime);
void playSong(int* notes, int* tempo, int size);

void setFrequency(float hertz){
    static const float CLOCK_PERIOD_HZ = 1000000.0/4.0;
    volatile float period = 1.0 / (hertz * 2.0);
    volatile uint16_t count = period * CLOCK_PERIOD_HZ;
    CCR0 = count;
}

void initTimerA0(){
    // Configure Timer to use SMCLK in Up to CCR0 mode
    CCTL0 = CCIE;                        // CCR0 interrupt enabled
    CCR0 = 4167;                         // Count value for 1s delay @ 12kHz
    TACTL = TASSEL_2 | MC_1 | ID_2;      // SMCLK, upmode
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    initGpio();
    initTimerA0();

    __enable_interrupt();

    while(!isButtonPressed());

    playSong(underworld_melody, underworld_tempo, sizeof(underworld_melody)/sizeof(underworld_melody[0]));

    while(1){

    }
}

void playSong(int* notes, int* tempo, int size){
    int i;
    for(i = 0; i < size; i++){
        int noteDuration = 1000 / tempo[i];
        setFrequency(notes[i]);
        delay_ms(noteDuration);
    }
}

void initGpio(){
    // Button
    P1DIR &= ~BIT3;
    P1REN |= BIT3;
    P1OUT |= BIT3;
    // Red LED
    P1DIR |= BIT6;
    // Speaker
    P1DIR |= BIT0;
}

void delay_ms(uint64_t milliseconds){
    static const uint64_t CYCLES_PER_MS = 1000000/1000;
    uint16_t i = 0;
    for(i = 0; i < milliseconds; i++){
        __delay_cycles(CYCLES_PER_MS);
    }
}

bool isButtonPressed(){
    if((P1IN & BIT3) == 0x00){
        // debounce
        __delay_cycles(1000);
        if((P1IN & BIT3) == 0x00){
            return true;
        }
    }

    return false;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void){
    P1OUT ^= BIT0;               // Toggle P1.0 using exclusive-OR
}