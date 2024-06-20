#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <util/delay.h>

#ifndef PERIPH_H
#define PERIPH_H


////////// SONAR UTILITY FUNCTIONS ///////////

void init_sonar();

// read_sonar function implmentation moved to timerISR.h file
// double read_sonar()

////////// ADC UTILITY FUNCTIONS ///////////

void ADC_init();

unsigned int ADC_read(unsigned char chnl);

////////// ADC AND SONAR UTILITY FUNCTIONS ///////////

#endif /* PERIPH_H */