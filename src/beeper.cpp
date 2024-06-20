
#include "periph.h"
#include "beeper.h"

void initPeeper()
{

    //OCR0A = 128; //sets duty cycle to 50% since TOP is always 256
    TCCR1A |= (1 << WGM11) | (1 << COM1A1); //COM1A1 sets it to channel A
    TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11); //CS11 sets the prescaler to be 8

}

void Peep_On(int freq) {
  ICR1 = freq;
  OCR1A = ICR1/2; //sets duty cycle to 50% since TOP is always 256
}

void Peep_Off() {
  OCR1A = ICR1;
}


