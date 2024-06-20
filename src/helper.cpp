#include "helper.h"

//Functionality - finds the greatest common divisor of two values
//Parameter: Two long int's to find their GCD
//Returns: GCD else 0
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a % b;
		if( c == 0 ) { return b; }
		a = b;
		b = c;
	}
	return 0;
}

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
   return (b ?  (x | (0x01 << k))  :  (x & ~(0x01 << k)) );
              //   Set bit to 1           Set bit to 0
}

unsigned char GetBit(unsigned char x, unsigned char k) {
   return ((x & (0x01 << k)) != 0);
}

 
int nums[16] = {0b1111110, 0b0110000, 0b1101101, 0b1111001, 0b0110011, 0b1011011, 0b1011111, 0b1110000, 0b1111111, 0b1111011, 0b1110111, 0b0011111, 0b1001110, 0b0111101, 0b1001111, 0b1000111 }; 
// a  b  c  d  e  f  g

void outNum(int num)
{
	
	PORTD = nums[num] << 1;
	//PORTB = SetBit(PORTB, 1 ,nums[num]&0x01); original changed as our g pin goes to port C pin 3
  	PORTC = SetBit(PORTC, 3 ,nums[num]&0x01);
	
}


//aFirst/Second: First range of values
//bFirst/Second: Range of values to map to
//inVal: value being mapped
unsigned int map_value(unsigned int aFirst, unsigned int aSecond, unsigned int bFirst, unsigned int bSecond, unsigned int inVal)
{
	return bFirst + (long((inVal - aFirst))*long((bSecond-bFirst)))/(aSecond - aFirst);
}

int a = 2;
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 10; //portC 5
int LEDS[7] {a, b, c, d, e, f, g};
//Some values for a-g are provided as examples, implement the remaining a-g values corresponding to the comments below:
//1 = Segment ON, 0 = Segment OFF
int nums2[17][7] {
  {1, 1, 1, 1, 1, 1, 0}, //0
  {0, 1, 1, 0, 0, 0, 0}, //1
  {1, 1, 0, 1, 1, 0, 1}, //2
  {1, 1, 1, 1, 0, 0, 1}, //3
  {0, 1, 1, 0, 0, 1, 1}, //4         <- Implement!
  {1, 0, 1, 1, 0, 1, 1}, //5         <- Implement!
  {1, 0, 1, 1, 1, 1, 1}, //6         <- Implement!
  {1, 1, 1, 0, 0, 0, 0}, //7         <- Implement!
  {1, 1, 1, 1, 1, 1, 1}, //8         <- Implement!
  {1, 1, 1, 0, 0, 1, 1}, //9         <- Implement!
  {1, 1, 1, 0, 1, 1, 1}, //10 (A)
  {0, 0, 1, 1, 1, 1, 1}, //11 (b)
  {1, 0, 0, 1, 1, 1, 0}, //12 (C)    <- Implement in hex!
  {0, 1, 1, 1, 1, 0, 1}, //13 (d)    <- Implement in hex!
  {1, 0, 0, 1, 1, 1, 1}, //14 (E)    <- Implement in hex!
  {1, 0, 0, 0, 1, 1, 1}, //15 (F)    <- Implement in hex!
  {0, 0, 0, 0, 0, 0, 0} //off
// a  b  c  d  e  f  g
};
void outNum2(int x)
{

	///PORTD = nums[num] << 1;
	//PORTB = SetBit(PORTB, 1 ,nums[num]&0x01); original changed as our g pin goes to port C pin 3
  	//PORTC = SetBit(PORTC, 3 ,nums[num]&0x01);

    for (int i = 0; i < 7; i++) {
		if (nums2[x][i] == 1) {
			if(i != 6)
				PORTD = SetBit(PORTD, LEDS[i], 1 );
			if(i ==6)
				PORTC = SetBit(PORTC, 5, 1 );
			}
		else {
			if(i != 6)
				PORTD = SetBit(PORTD, LEDS[i], 0 );
			if(i == 6)
				PORTC = SetBit(PORTC, 5, 0);
		}
  }
}
