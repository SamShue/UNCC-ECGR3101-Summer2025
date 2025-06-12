#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#define GREEN_LED_PIN       BIT0
#define GREEN_LED_DIR_REG   P1DIR
#define GREEN_LED_OUT_REG   P1OUT

#define YELLOW_LED_PIN      (BIT1 | BIT3)
#define YELLOW_LED_DIR_REG  P2DIR
#define YELLOW_LED_OUT_REG  P2OUT

#define RED_LED_PIN         BIT6
#define RED_LED_DIR_REG     P1DIR
#define RED_LED_OUT_REG     P1OUT

typedef enum {GREEN, YELLOW, RED} state_t;

void init();
void delay_s(uint16_t seconds);

static inline void setGreenLed(bool enable);
static inline void setYellowLed(bool enable);
static inline void setRedLed(bool enable);

state_t runGreenState(uint16_t* seconds);
state_t runYellowState(uint16_t* seconds);
state_t runRedState(uint16_t* seconds);

int main(void) {
    volatile uint16_t seconds = 0;
    state_t currentState = GREEN;
    state_t nextState = currentState;

    init();

    while(1) {
        switch(currentState){
        case GREEN:
            nextState = runGreenState(&seconds);
            break;
        case YELLOW:
            nextState = runYellowState(&seconds);
            break;
        case RED:
            nextState = runRedState(&seconds);
            break;
        default:
            break;
        }

        currentState = nextState; // Update state
        delay_s(1);
        seconds++; // Update seconds
    }
}

state_t runGreenState(uint16_t* seconds){
    state_t nextState = GREEN;
    setRedLed(false);
    setGreenLed(true);
    setYellowLed(false);

    if(*seconds > 5){
        *seconds = 0;
        nextState = YELLOW;
    }
    return nextState;
}

state_t runYellowState(uint16_t* seconds){
    state_t nextState = YELLOW;
    setRedLed(false);
    setGreenLed(false);
    setYellowLed(true);

    if(*seconds > 2){
        *seconds = 0;
        nextState = RED;
    }
    return nextState;
}

state_t runRedState(uint16_t* seconds){
    state_t nextState = RED;
    setRedLed(true);
    setGreenLed(false);
    setYellowLed(false);

    if(*seconds > 5){
        *seconds = 0;
        nextState = GREEN;
    }
    return nextState;
}

void init(){
    // Stop the watchdog timer
    WDTCTL = 0x5A80;

    // LED configs
    GREEN_LED_DIR_REG |= GREEN_LED_PIN;
    YELLOW_LED_DIR_REG |= YELLOW_LED_PIN;
    RED_LED_DIR_REG |= RED_LED_PIN;
}

static inline void setGreenLed(bool enable){
    if(enable){
        GREEN_LED_OUT_REG |= GREEN_LED_PIN;
    } else {
        GREEN_LED_OUT_REG &= ~GREEN_LED_PIN;
    }
}

static inline void setYellowLed(bool enable){
    if(enable){
        YELLOW_LED_OUT_REG |= YELLOW_LED_PIN;
    } else {
        YELLOW_LED_OUT_REG &= ~YELLOW_LED_PIN;
    }
}

static inline void setRedLed(bool enable){
    if(enable){
        RED_LED_OUT_REG |= RED_LED_PIN;
    } else {
        RED_LED_OUT_REG &= ~RED_LED_PIN;
    }
}

void delay_s(uint16_t seconds){
    volatile uint16_t i;
    for(i = 0; i < seconds; i++){
        __delay_cycles(1000000);
    }
}
