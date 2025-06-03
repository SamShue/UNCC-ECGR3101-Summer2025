
int main(){
    volatile unsigned int* wdtctl = (unsigned int*) 0x0120;
    *wdtctl = 0x5A80;

    volatile unsigned char* p1out = (unsigned char*) 0x021;
    volatile unsigned char* p1dir = (unsigned char*) 0x022;

    *p1dir = 0x01;
    *p1out = 0x01;

    volatile long i;
    while(1){
        *p1out = 0x00;

        for(i = 0; i < 100000; i++);

        *p1out = 0x01;

        for(i = 0; i < 100000; i++);
    }

}