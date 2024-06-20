#include "spiAVR.h"
#include "helper.h"
#include "defs.h"
#include "led.h"

char ledArray[64] ;
#define B10000000 128 


unsigned char  spidata[16];
void spiSend(int addr, volatile unsigned char  opcode, volatile unsigned char data) {
    //Create an array with the data to shift out
    int offset=addr*2;
    int maxbytes=MAX_DEVICES*2;

    for(int i=0;i<maxbytes;i++)
        spidata[i]=(unsigned char)0;
    //put our device data into the array
    spidata[offset+1]=opcode;
    spidata[offset]=data;
    //enable the line 
    PORTB = SetBit(PORTB, PORTB2, 0);
    //Now shift out the data 
    for (int i=maxbytes;i>0;i--) {
        SPI_SEND(spidata[i-1]) ;
    }
    //latch the data onto the display
    PORTB = SetBit(PORTB, PORTB2, 1);
}    

/*function that checks which device to write led to*/
void setLedDisplay(int row, int column, bool state) {
    if(row < 8) 
        setLed(0, row, column, state);
    if(row >= 8 && row < 16)
        setLed(1, row - 8, column, state);
    if(row >= 16 && row < 24)
        setLed(2, row - 8, column, state);
}

void setLed(int addr, int row, int column, bool state) {
    int offset;
    unsigned char  val=0x00;

    if(addr<0 || addr>=MAX_DEVICES)
        return;
    if(row<0 || row>7 || column<0 || column>7)
        return;
    offset=addr*8;
    val=B10000000 >> column;
    if(state)
        ledArray[offset+row]=ledArray[offset+row]|val;
    else {
        val=~val;
        ledArray[offset+row]=ledArray[offset+row]&val;
    }
    spiSend(addr, row+1,ledArray[offset+row]);
}

/* clear Display handles clearing all attached led panels */
void clearDisplay() {
    for (int i= 0; i< MAX_DEVICES; i++)
        clearScreen(i) ;
}

void clearScreen(int addr) {
    int offset;

    if(addr<0 || addr>=MAX_DEVICES)
        return;
    offset=addr*8;

    for(int i=0;i<8;i++) {
        ledArray[offset+i]=0x00;
        PORTB = SetBit(PORTB, PORTB2, 0);
        SPI_SEND(i+1);
        SPI_SEND(ledArray[offset+i]);
        PORTB = SetBit(PORTB, PORTB2, 1);
    }
}

void setRow(int addr,int row, unsigned char  rowval)
{
    int offset;
  
    if(addr<0 || addr>=MAX_DEVICES)
        return;
    if(row<0 || row>7)
        return;

    offset=addr*8;
    ledArray[offset+row]=rowval;
    spiSend(addr, row+1,ledArray[offset+row]);
}