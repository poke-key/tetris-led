#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <util/delay.h>

#ifndef HELPER_H
#define HELPER_H

//Functionality - finds the greatest common divisor of two values
//Parameter: Two long int's to find their GCD
//Returns: GCD else 0
unsigned long int findGCD(unsigned long int a, unsigned long int b);

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b);

unsigned char GetBit(unsigned char x, unsigned char k);

void outNum(int num);

void outNum2(int x);
#endif /* HEPLER_H */