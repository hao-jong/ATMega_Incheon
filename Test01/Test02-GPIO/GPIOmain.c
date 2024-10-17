/*
 * Test02-GPIO.c
 *
 * Created: 2024-10-10 오전 9:36:07
 * Author : user
 */ 
#define __DELAY_BACKWARD_COMPATIBLE__
#define F_CPU 16000000UL

#include <avr/io.h> // AVR 기본 헤더
#include <avr/delay.h>
#include <avr/interrupt.h>

#define SW1 PORTG4 // (1<<0)
#define SW2 PORTD0 // 
#define SW3 PORTD1
#define LED1 PORTG0

volatile int ival = 1000;

ISR(INT0_vect) // 점점 느리게
{
	ival += 100;
}

ISR(INT1_vect) // 점점 빠르게
{
	ival -=100;
}

int main(void)
{
    /* Replace with your application code */
	DDRG &= ~0x0C;
	DDRG |= 0x03;
	PORTG |= _BV(LED1);
	Standby();	
	PORTG &= ~_BV(LED1);	
	
	// Mask register : EIMSK
	EIMSK |= 0x03;
	//Create register : EICRA
	EICRA |= 0x0f;
	EICRA = (EICRA & 0xF0)|0x0A;
	
	
	sei();
	
	//while(1) // PINg0의 초기값이 1(open)임을 가정
	//{
		//if((PING & 0x01) == 1)
		//{
			//break;
		//}
	//}
	//
	

	while (1) 
    {
			//if((PING & _BV(SW2)) == 0) //G1
				//{
					//ival += 500;
					//
				//}
			//else if((PING & _BV(SW3))==0) //G2
				//{
					//ival -= 200;
					//if(ival < 10) ival  = 100;
				//}
			PORTG |= _BV(LED1); //LED ON
			_delay_ms(ival); // 1/1000 sec 
			PORTG &= ~_BV(LED1); //LED OFF
			_delay_ms(ival);
    }
}

