/********************************************************************
	created:	2022/07/01
	created:	1:7:2022   10:58
	filename: 	C:\Users\sWe\Documents\Atmel Studio\7.0\pinDefines.h
	file path:	C:\Users\sWe\Documents\Atmel Studio\7.0
	file base:	pinDefines
	file ext:	h
	author:		ArtNeko.LAB
	
	purpose:	
*********************************************************************/
#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Bit_manip.h"

#define LED_1 PB0
#define LED_2 PB1
#define BUTTON_1 PB2
#define BUTTON_2 PB4
#define BUTTON_3 PB5
#define POT_PIN PB3
#define CONFIGURATION_PORTS PORTB//Pull up enable/disable
#define READ_PORTS PINB// Read state pins
#define DATA_DIRECTION_PORTS DDRB // Input/output




void setInterrupt(void)
{
	GIMSK |= _BV(PCIE);  // PIN change interrupt enable
	PCMSK |= (_BV(BUTTON_1) | _BV(BUTTON_2)); //Pin Change Enable Mask
	//sei();               //Global interrupt enable
}

void setUp(void)
{	
	CONFIGURATION_PORTS|= (_BV(BUTTON_1) | _BV(BUTTON_2));//Set up pull-up resistors
	DATA_DIRECTION_PORTS |=(_BV(LED_1) | _BV(LED_2));//Set up our ports as out		 
}

void PWMsetUp(void)
{
	TCCR0A |= (_BV(WGM00) | _BV(WGM01));// Enable fast PWM mode  
	TCCR0B |= _BV(CS02);                // PWM prescaler / 256
	TCCR0A |= _BV(COM0A1);              // PWM output on OCR0A  
	TCCR0A |= _BV(COM0B1);              // PWM output on OCR0B     	
}

void ADCsetUp(void)
{
   //	ADMUX &= (~_BV(REFS0)); //Internal reference voltage
	ADCSRA |= _BV(ADEN); //Enable ADC
	ADCSRA |= _BV(ADPS1); // ADC clock prescaler / 4                	  	
	DIDR0  |= _BV(POT_PIN); // Turn off pin how digital input
}

uint16_t ReadADC(uint8_t number_pin) //Read data from analog pin
{
   ADMUX = (ADMUX & 0xF0) | number_pin; // Bit mask clear all pin and set our pin
   ADCSRA |= _BV(ADSC);  // Start conversion
   loop_until_bit_is_clear(ADCSRA,ADSC); // wait for conversion
   return(ADC);
}

void turnOFFPWM(void)
{
	TCCR0A &= ~_BV(COM0A1);
	TCCR0A &= ~_BV(COM0B1);
	CONFIGURATION_PORTS &= ~_BV(LED_1);
	CONFIGURATION_PORTS &= ~_BV(LED_2);
}

void turnOnPWM(void)
{
	CONFIGURATION_PORTS |= (_BV(LED_1) | _BV(LED_2));
	TCCR0A |= _BV(COM0A1);
	TCCR0A |= _BV(COM0B1);
	OCR0A=(ReadADC(POT_PIN)>>2);
	OCR0B=(ReadADC(POT_PIN)>>2);
}








