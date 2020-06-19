#include <avr/io.h>
#include "LCD_4bit.h"
#include <string.h>


int main (void)
{
  int a = 4, b=5;
  int c = a+b;
  int i;
  char *displayString = "LCD is ready to work!";
  
  //Switching on an LED
  DDRB |= (1 << PINB0);
  PORTB |= (1 << PINB0);
  
  // Initialize LCD
  initializeLCD();
  
  writeToLCD(displayString);
  
  displayString = "Good luck with code execution";
  writeToLCD(displayString);
  
  writeToLCD("LCD ready to be used as debugger");
  
  snprintf(displayString, 22, "this is the result: %d", c );
  writeToLCD(displayString);
  
  // remove when being called from another program as error display
  while(1);
  
  free(displayString);
  
  return(0);
}



