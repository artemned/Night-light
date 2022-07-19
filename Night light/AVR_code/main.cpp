/*
 * NightLight.cpp
 *
 * Created: 08.07.2022 10:35:27
 * Author : ArmNeko
 */ 

#define F_CPU 9600000
#include <util/delay.h>
#include "Bit_manip.h"
#include "pinDefines.h"

volatile uint8_t count=0;
volatile bool on_off=false;

volatile enum StateNightLight
{
	OFF=0,
	ON
	
}state;

volatile enum ModeNightLight
{
  LEFT=0,
  RIGHT,
  BOTH
  
}mode;


ISR(PCINT0_vect)
{
	_delay_ms(50);
	if(bit_is_clear(READ_PORTS,BUTTON_1))
	{
	    on_off = !on_off;
		state=static_cast<StateNightLight>(on_off);
	}
	else if(bit_is_clear(READ_PORTS,BUTTON_2))
	{
		 count++;
		 if(count==3)count=0; 
 		 mode = static_cast<ModeNightLight>(count);
	}
	
}

int main(void)
{
	sei();// Enable all interrupts  
	setUp();
    ADCsetUp();
	PWMsetUp();
	setInterrupt();

   
    while (1) 
    { 
		  
		
		   switch(state)
	 {
		case ON:switch(mode){
          
		  case LEFT:{
		              CONFIGURATION_PORTS |= _BV(LED_2);
					  TCCR0A |= _BV(COM0B1);
					  TCCR0A &= ~_BV(COM0A1);
                      CONFIGURATION_PORTS &= ~_BV(LED_1);
			          OCR0B=(ReadADC(POT_PIN)>>2);
				      break;
		            }
		  case RIGHT:{
			           CONFIGURATION_PORTS |= _BV(LED_1);
			           TCCR0A |= _BV(COM0A1);
		               TCCR0A &= ~_BV(COM0B1);
		               CONFIGURATION_PORTS &= ~_BV(LED_2);
		               OCR0A=(ReadADC(POT_PIN)>>2);
				   	   break;
		           	  } 
	      case BOTH:{
		              CONFIGURATION_PORTS |= (_BV(LED_1) | _BV(LED_2));
		              TCCR0A |= _BV(COM0A1);
		              TCCR0A |= _BV(COM0B1);
		              OCR0A=(ReadADC(POT_PIN)>>2);
		              OCR0B=(ReadADC(POT_PIN)>>2);	
					  break;	    
		            }							   
		}
		break;
		case OFF:{
		           turnOFFPWM();
		           mode=LEFT;
				   count=0;
				 }
		break;
		default:break;
		   
	  }
		
	
    }
	
}

