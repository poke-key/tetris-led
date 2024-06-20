#include "defs.h"
#ifndef LED_H
#define LED_H

inline void initDisplay() {
    // Ensure all devices start in a known state
    for (int addr = 0; addr < MAX_DEVICES; addr++) {
        
        // Wake up and reset each display
        PORTB = SetBit(PORTB, PORTB2, 0);
        SPI_SEND(0xC); // Shutdown register
        SPI_SEND(0x1); // Normal operation
        PORTB = SetBit(PORTB, PORTB2, 1);

        // Set scan limit to max (display all digits)
        PORTB = SetBit(PORTB, PORTB2, 0);
        SPI_SEND(0xB); // Scan limit register
        SPI_SEND(0x7); // All digits (0-7)
        PORTB = SetBit(PORTB, PORTB2, 1);

        PORTB = SetBit(PORTB, PORTB2, 0);
        SPI_SEND(0xB); // Scan limit register
        SPI_SEND(0x7); // All digits (0-7)
        PORTB = SetBit(PORTB, PORTB2, 1);

        PORTB = SetBit(PORTB, PORTB2, 0);
        SPI_SEND(0xA); // Scan limit register
        SPI_SEND(0x2); // All digits (0-7)
        PORTB = SetBit(PORTB, PORTB2, 1);

        // Set Decode Mode (assuming no decode)
        PORTB = SetBit(PORTB, PORTB2, 0);
        SPI_SEND(0x9); // Decode mode register
        SPI_SEND(0x00); // No decode for any digits
        PORTB = SetBit(PORTB, PORTB2, 1);

    }
}
void setRow(int addr,int rownum,unsigned char rowval);
void spiSend(int addr, volatile unsigned char  opcode, volatile unsigned char data);
void setLed(int addr, int row, int column, bool state);
void setLedDisplay(int row, int column, bool state);
void clearDisplay() ;   // clears all led panels
void clearScreen(int addr);
#endif /* LED_H */