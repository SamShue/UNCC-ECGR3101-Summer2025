int main(){

    volatile unsigned int* wdtctl = (unsigned int*) 0x0120;
    *wdtctl = 0x5A80; // Stop the watchdog timer

    volatile unsigned char* p1in = (unsigned char*) 0x020;
    volatile unsigned char* p1out = (unsigned char*) 0x021;
    volatile unsigned char* p1dir = (unsigned char*) 0x022;
    volatile unsigned char* p1ren = (unsigned char*) 0x027;

    *p1dir = 0xF7; // writes 11110111 to p1 direction register
    *p1ren = 0x08; // writes 00001000 to p1 resistor enable register
    *p1out = 0x08; // writes 00001000 to p1 output register

    while(1){
        if(*p1in == 0x00){
            *p1out = 0x09; // 00001001 (LED on, pull-up enabled)
        } else {
            *p1out = 0x08; // 00001000 (LED off, pull-up enabled)
        }
    }
}