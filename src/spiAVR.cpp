#include "spiAVR.h"


//If SS is on a different port, make sure to change the init to take that into account.
// SPI is full duplex communication protocol i.e. data on master and slave shift registers get interchanged at a same time.
void SPI_INIT(){
    DDRB |= (1 << PIN_SCK) | (1 << PIN_MOSI) | (1 << PIN_SS);//initialize your pins. 
    DDRB &= ~(1<<PIN_MISO); /* Make MISO pin as input pin */
    PORTB |= (1<<PIN_SS); /* Make high on SS pin */
    SPCR |= (1 << SPE) | (1 << MSTR) |(1<<SPR0); //initialize SPI coomunication
    SPSR &= ~(1<<SPI2X); /* Disable speed doubler */
}


void SPI_SEND(char data)
{
    SPDR = data;//set data that you want to transmit
    while (!(SPSR & (1 << SPIF)));// wait until done transmitting
}

/* SPI read data function */
char SPI_READ(){
    SPDR = 0xFF;
    while(!(SPSR & (1<<SPIF))); /* Wait till reception complete */
    return(SPDR); /* Return received data */
}