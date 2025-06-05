#define P1IN    *((unsigned char*) 0x020)
#define P1OUT   *((unsigned char*) 0x021)
#define P1DIR   *((unsigned char*) 0x022)
#define P1REN   *((unsigned char*) 0x027)

#define WDCTL   *((unsigned int*) 0x0120)

int main(){
    WDCTL = 0x5A80; // Stop the watchdog timer

    P1DIR = 0xF7; // writes 11110111 to p1 direction register
    P1REN = 0x08; // writes 00001000 to p1 resistor enable register
    P1OUT = 0x08; // writes 00001000 to p1 output register

    while(1){
        if(P1IN == 0x00){
            P1OUT = 0x09; // 00001001 (LED on, pull-up enabled)
        } else {
            P1OUT = 0x08; // 00001000 (LED off, pull-up enabled)
        }
    }
}