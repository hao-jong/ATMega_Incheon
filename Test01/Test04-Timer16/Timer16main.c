#include <avr/io.h>
#include <avr/interrupt.h>

#define DDR DDRG
#define PORT PORTG
#define LED0 PORTG0
#define LED1 PORTG1
#define LED2 PORTG2


volatile int st_0 =0;
volatile int st_1 =0;

ISR(TIMER3_OVF_vect)
{
	if(st_0)
	{
		PORT &= ~_BV(LED1);
		st_0 = 0;
	}
	else
	{
		PORT |= _BV(LED1);
		st_0 = 1;
	}
}


ISR(TIMER1_OVF_vect)
{
	if(st_1)
	{
		PORT &= ~_BV(LED0);
		st_1 = 0;
	}
	else
	{
		PORT |= _BV(LED0);
		st_1 = 1;
	}
}

int main(void)
{
	DDR |= 0x07;
	TIMSK	|= 0x04;
	ETIMSK  |= 0x04;		// _BV(TOIE3) : 0b0100 : 3번 타이머 OVF 인터럽트
	TCCR3B  |= 0x03;			//(1/16M) * (분주비_64:011:0x03:) * 65535 =  OVF Timer 인터럽트 주기 (~250ms)
	TCCR1B	|= 0x04;
	
	sei();
	
	while (1) 
    {
		
    }
}

