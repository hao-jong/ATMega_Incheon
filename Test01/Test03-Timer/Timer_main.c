/*
 * Test03-Timer.c
 *
 * Created: 2024-10-11 오후 2:33:03
 * Author : user
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

#define DDR DDRG
#define PORT PORTG
#define LED0 PORTG0
#define LED1 PORTG1
#define LED2 PORTG2
volatile int cnt = 0, st = 0;
volatile int icnt = 32;

ISR(INT0_vect) // 점점 느리게
{
	icnt +=6;
}

ISR(INT1_vect) // 점점 빠르게
{
	icnt -=6;
	if (icnt<2)
	{
	icnt =2;
	}
}

ISR(TIMER0_OVF_vect)
{
	cnt++;
	if(cnt > icnt)
	{
		cnt = 0;
		if(st)
		{
			PORTG =PORTG& ~(0x01);
			st = 0;
		}
		else
		{
			PORTG = PORTG|0x01;
			st = 1;
		}
	}
}

volatile int ocnt = 0, ost = 0;

ISR(TIMER0_COMP_vect)
{
	ocnt++;
	if(ocnt > icnt)
	{
		ocnt = 0;
		if(ost)
		{
		PORTG =PORTG&~(0x02);
		ost = 0;
		}
		else
		{
			PORTG = PORTG|0x02;
			ost = 1;
		}
	}
}



int main(void)
{
	DDR = PORTG|0x01; //LED Port 설정
	PORTG =PORTG|0x01;
	
	Standby();
	
	PORTG = PORTG&~(0x01);
	
	TIMSK |= 0x01; // TOIE0 // overflow 인터럽트 enable
	TCCR0 |= 0x07; //분주비 1024
	
	OCR0 =127; //타이머 127/255 에서 인터럽트 요청 보냄
	TIMSK |= 0x02; //OCIE0 비교일치 인터럽트 enable
	EIMSK |= 0x03; // Port D 0,1 인터럽트 활성화
	
	EICRA |= 0x0f; // 상승 edge 에서 인터럽트 요청을 발생시킴.
	//EICRA = (EICRA & 0xF0)|0x0A; 
	
	sei();
	
	/* Replace with your application code */
    while (1) 
    {
		
    }
}

