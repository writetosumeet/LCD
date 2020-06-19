#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

//#define F_CPU 1000000UL - change & uncomment if clock freq is other than 1 MHz

// mapping of RS /EN / D7-D0 pins
// RS - PB0 / EN - PD7 / D7 - PC5 / D6 - PC4 / D5 - PC3 / D4 - PC2 / D3 - PC1 / D2 - PC0 / D1 - PB2 / D0 - PB1 
// In case any other pins are used - update byteToLCD and main functions

void byteToLCD(unsigned char flag, unsigned char byteToWrite)
{
  // ANDing with 1 does not change value / ORing with 0 does not change value
  // write byte on selected Ports
  PORTC = (PORTC & 0b11000000) | ((byteToWrite & 0b11111100)>>2); // mask not to be changed bits / shift and align byteToWrite with PORTC pins
  PORTB = (PORTB & 0b11111001) | ((byteToWrite & 0b00000011)<<1); // mask not to be changed bits / shift and align byteToWrite with PORTB pins
  
  // RS = 0 for command / 1 for data
  if (flag == 0)
    PORTB = (PORTB & 0b11111110) | (0b00000000);
  else
    PORTB = (PORTB & 0b11111110) | (0b00000001);
  
  // Enable = 1
  PORTD = (PORTD & 0b01111111) | (0b10000000);
  
  // wait 100 micro second
  _delay_us(100);
  
  // Enable = 0
  PORTD = (PORTD & 0b01111111) | (0b00000000);
}

void initializeLCD()
{
  _delay_ms(50); // delay for power stabilization
  
  // Set LCD as 8-bit 2 line device
  byteToLCD(0, 0x38); // 0 0 1 DL N F x x  DL- 1:8-bit 0:4-bit ; N- 1:2 lines 0:1 line display ; F- Font 0:5x8 char 1:5x10 char
  
  // Switch on display
  byteToLCD(0, 0x0F); // 0 0 0 0 1 D C B    D- display; C- cursor; B- Blinking
  
  // Set cursor increment after each write
  byteToLCD(0, 0x06);  // 0 0 0 0 0 1 I/D S   I/D- Increment: 1;  S - Shift cursor right(0) after writing character
  
  // Clear display
  byteToLCD(0, 0x01);
}

void writeToLCD(char* str)
{
  unsigned char strLength = strlen(str);
  unsigned char i;
  
  // initial delay for stabilization
  _delay_ms(10);
  
  for (i=0; i<strLength; i++)
  {
    // if more than 16 char - move cursor to next line on screen
    if(i==16)
    {
        byteToLCD(0, 0xC0);
        // if next character is blank - avoid printing it
        if(str[i]==' ')
          i++;
    }
    byteToLCD(1, str[i]);
    
    /*
     // to enable rolling display - upto 40 char per line
    // if more than 16 char - shift display to left
    if(i==16)
        byteToLCD(0, 0x07);  // 0 0 0 0 0 1 I/D S   I/D- Increment: 1;  S - Shift cursor left(1) after writing character
    
    if(i>=16)
      _delay_ms(400);*/
    // return cursor and display to home
    //byteToLCD(0, 0x02);
  }
}


int main (void)
{
  // set data direction of avr pins as output
  //1 = output, 0 = input
  
  // RS - PB0 / EN - PD7 / D7 - PC5 / D6 - PC4 / D5 - PC3 / D4 - PC2 / D3 - PC1 / D2 - PC0 / D1 - PB2 / D0 - PB1
  // ANDing with 1 does not change value / ORing with 0 does not change value
  DDRB = (DDRB & 0b11111000) | (0b00000111); // mask others and make Pins 2 / 1 / 0 as 1
  DDRC = (DDRC & 0b11000000) | (0b00111111); // mask others and make Pins 5-0 as 1
  DDRD = (DDRD & 0b01111111) | (0b10000000); // mask others and make Pin 7 as 1
  
  // Initialize LCD
  initializeLCD();
  
  // Write to LCD
  writeToLCD("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  
  // remove when being called from another program as error display
  while(1);
  
  return(0);
}



