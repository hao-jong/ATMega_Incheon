#define F_CPU 16000000L
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
volatile int Prescaler = 1024;
volatile int idx = 0;
enum {
	DO = 130, 
	Do = 138,
	RE = 146,
	Re = 155,
	MI = 164,
	FA = 174,
	Fa = 185,
	SL = 196,
	Sl = 207,
	LA = 220,
	La = 233,
	SI = 246
};

long song[] = {SL,SL,LA,LA,SL,SL,MI,SL,SL,MI,MI,RE,SL,SL,LA,LA,SL,SL,MI,SL,MI,RE,MI,DO};
long song1[24];

void conv()
{
    for (int i = 0; i < 24; i++) 
	{
		song1[i] = (long)((long)(F_CPU ) * (long)((long)Prescaler * (long)song[i] * 4));
	}
}



ISR(TIMER1_COMPA_vect)
{
	TCNT0 = 0;
}




int main(void)
{

    TIMSK |= (1 << OCIE1A);     
    TCCR1A = 0xaa; 
    TCCR1B = 0x1d;
	DDRB |= 0x20;
	PORTB =0x00;
	ICR1 = 5000;
	
	conv();
	sei();

	
	while (1) 
    {
		OCR1A = (int)song1[idx];
		_delay_ms(500);
		ICR1 = 2*OCR1A;
		idx++;
		if(idx > 24)
		{
			idx = 0;
		}

    }
}

