
#include "intrinsics.h"
#include <msp430g2553.h>
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#define LED1 BIT0
#define LED2 BIT1
#define LED3 BIT2
#define Cyan BIT6
#define LED4 BIT5
#define LED5 BIT4
#define LED6 BIT3

#define P2_LED_MASK (LED1 | LED2 | LED3 | LED4 | LED5 | LED6)

typedef enum {WAITING, RUNNING, WIN, LOSE} state_t;

bool globalButtonFlag = false;

void turnOffAllLeds(){
    P1OUT &= ~Cyan;
    P2OUT &= ~P2_LED_MASK;
}

void setLEDState(int LEDNum){
    turnOffAllLeds();
    switch(LEDNum){
        case 0:
            P2OUT |= LED3;
            break;
        case 1:
            P2OUT |= LED2;
            break;
        case 2:
            P2OUT |= LED1;
            break;
        case 3:
            P1OUT |= Cyan;
            break;
        case 4:
            P2OUT |= LED4;
            break;
        case 5:
            P2OUT |= LED5;
            break;
        case 6:
            P2OUT |= LED6;
            break;
        default:
            turnOffAllLeds();
            break;
    }
}

void runtime(){
    volatile unsigned int i;
    for (i=0; i < 20000; i++);
}

void pause(){
    volatile unsigned int i;
    for (i=0; i < 60000; i++);
}

void loserSequence(int LEDNum){

    setLEDState(LEDNum);
    pause();
    turnOffAllLeds();
    
}

void winnerSequence(){
    int i;

    for(i = 0; i < 3; i++){
        P1OUT |= Cyan;
        P2OUT |= P2_LED_MASK;
        runtime();
        turnOffAllLeds();
        runtime();
    }

    turnOffAllLeds();
    
}

void init(){
    WDTCTL = 0x5A80; // Stop watchdog timer
    
    P1DIR |= BIT6;
    P2DIR |= P2_LED_MASK;
    P1DIR &= ~BIT3; //button pin
    P1REN |= BIT3; //pull-up resistor for button
    P1OUT |= BIT3; //set pull-up for button
    
    P1IES |= BIT3;
    P1IFG &= ~BIT3;
    P1IE |= BIT3;

    __enable_interrupt();

    turnOffAllLeds();
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){

    P1OUT ^= BIT6;
    globalButtonFlag = true;
    __delay_cycles(200);
    P1IFG &= ~BIT3;
}

void main(void)
{
    init();
    volatile bool movingForward = true;
    volatile state_t currentState = WAITING;
    volatile state_t nextState = currentState;
    volatile uint8_t ledPosition = 0;

    while(1){
        /*
        switch(currentState){
            case WAITING:
                if(globalButtonFlag){
                    globalButtonFlag = false;
                    nextState = RUNNING;
                }
                break;
            case RUNNING:
                setLEDState(ledPosition);
                if(movingForward){
                    ledPosition++;
                }
                else{
                    ledPosition--;
                }
                
                if(ledPosition >= 6){
                    movingForward = false;
                }
                if(ledPosition <= 0){
                    movingForward = true;
                }
                runtime();
                if (globalButtonFlag){
                    globalButtonFlag = false;
                    if(ledPosition == 3){
                        nextState = WIN;
                    }
                    else{
                        nextState = LOSE;
                    }
                }
                else{
                    nextState = RUNNING;
                }
                break;
            case WIN:
                //winnerSequence();
                nextState = WAITING;
                ledPosition = 0;
                break;
            case LOSE:
                //loserSequence(ledPosition);
                nextState = WAITING;
                ledPosition = 0;
                break;
            default:
                nextState = WAITING;
                break;
        }
        currentState = nextState;
        */
    }
}
