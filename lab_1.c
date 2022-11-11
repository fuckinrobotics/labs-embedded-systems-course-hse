#include <stdio.h>
#include <stdlib.h>

#define F_CPU 10000000 / 64
#define PORTAbits PORTA
#define RS LATA.B4
#define EN LATA.B5
#define D4 LATD.B4
#define D5 LATD.B5
#define D6 LATD.B6
#define D7 LATD.B7

float temp;
int returnValue;
char parseValue;
int firstValue;
int secondValue;
int i;
int ifFir = 0;
int fir = 0, sec = 0, firMs = 0, secMs = 0, secFir = 0;
unsigned char firChar[6], secChar[6];
char snum[5];
char strReadNum[5];
int msCopy, nn, j;
int isFinish = 0;
int lastNumber = 0;
int lastNumber2 = 0;
int lastNumber3 = 0;
int lastNumber4 = 0;
int numberCount = 0;
int number = 0;
int symbolFinish;
char Word1[] = "Ñòðîêà\0";
char Word2[] = "22222222\0";
char Word3[] = "33333333\0";
char Word4[] = "44444444\0";
char Word5[] = "55555555\0";
char Word6[] = "66666666\0";
char* ptr;
int numberOfMessage = 0;
int sendPointer = 0;

typedef union
{
    unsigned char byte;
    struct
    {
        unsigned char bit0:  1;
        unsigned char bit1:  1;
        unsigned char bit2:  1;
        unsigned char bit3:  1;
        unsigned char bit4:  1;
        unsigned char bit5:  1;
        unsigned char bit6:  1;
        unsigned char bit7:  1;
    }
    ;
    struct
    {
        unsigned char lowNibble: 4;
        unsigned char highNibble: 4;
    }
    ;
}
MyByte;

void interrupt() {
    //if(PIR1.TXIF){
    //while(!PIR1.TXIF){}
    //PIR1.TXIF = 1;
    if (sendPointer < numberOfMessage)
    {
        sendPointer++;
        TXREG = *(ptr + sendPointer);
        return;
    }
    sendPointer = 0;
    PIE1.TXIE = 0;
    //}
}

void uartInit(long baud_rate)
{
    TRISC.B6 = 0;                /* Make Tx pin as output*/
    TRISC.B7 = 1;                  /* Make Rx pin as input*/

    //Baud rate=9600, SPBRG = (F_CPU /(64*9600))-1*/
    temp = (((float)(F_CPU) / (float) baud_rate) -1);
    SPBRG = (int)temp;

    TXSTA = 0x20;          /* Enable Transmit(TX) */
    RCSTA = 0x91;          /* Enable Receive(RX) & Serial */
    PIE1.TXIE = 0;
    PIE1.RCIE = 0;
}

char getSymbolFromKeyboard(){
static char previousKeyDown = 0x3f;
    if (previousKeyDown != PORTA) {
      previousKeyDown = PORTA;
          if (PORTA == 0x3E) {
              return  '0';
          } else if (PORTA == 0x3D) {
              return '1';
          } else if (PORTA == 0x3B) {
              return '2';
          } else if (PORTA == 0x37) {
              return '3';
          } else if (PORTA == 0x2F) {
              return '4';
          } else if (PORTA == 0x1F) {
              return '5';
          }
      }
    return 0x3F;
}

void printWord(char word[]){
     int i = 0;
     int flag2 = 1;
     numberOfMessage = 0;
     ptr = word;
     for(; word[i] != '\0'; i++){
         numberOfMessage++;
     }
     i = 0;
     
     TXREG = *ptr;
      PIE1.TXIE = 1;
}

void main()
{
    ADCON1 |= 0b00001111;
    TRISA = 0b11111111;
    TRISB = 0x00;
    LATB = 0x00;
    CMCON |= 0b00000111;
    uartInit(38400);
    RCON.IPEN = 0;
    INTCON.GIEH = 1;
    INTCON.PEIE = 1;
    while(1){
      char chr = getSymbolFromKeyboard();
      if(chr != 0x3F) {
        switch (chr){
            case '0': {printWord(Word1); break;}
            case '1': {printWord(Word2); break;}
            case '2': {printWord(Word3); break;}
            case '3': {printWord(Word4); break;}
            case '4': {printWord(Word5); break;}
            case '5': {printWord(Word6); break;}
        }
      }
      if (PIR1.RCIF == 1) {
        char getFromUart = RCREG;
          LATB = getFromUart;
      }
    }
}
