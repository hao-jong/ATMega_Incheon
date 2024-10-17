#define F_CPU 16000000

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define CDDR	DDRA
#define IDDR	DDRB
#define CPORT	PORTA
#define IPORT	PORTB

unsigned char img[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x67};
char digit[4] = {0};
	
volatile int num = 0;	
volatile char btn =0;
	
int sep (int* a)
{	if(a>9999) 
	{
		*a = 0;
		return 0;
	}
	else
	{
		digit[3] = (char)((*a%10) / 1);
		digit[2] = (char)((*a%100) / 10);
		digit[1] = (char)((*a%1000) / 100);
		digit[0] = (char)((*a%10000) / 1000);
	}
		return 1;
}
	


ISR(TIMER0_COMP_vect)
{
		num++;
		if(num>10000)
		{
			num = 0;
		}
	
}


ISR(TIMER2_OVF_vect) // FND refresh
{
	for(int i =0; i<4; i++)
	{
		CPORT = 1<< i;
		if(i!=1)
		{
			IPORT = ~img[digit[i]];
			_delay_ms(3);
		}
		if(i ==1)
		{
			IPORT = ~(img[digit[i]]|0x80);
			_delay_ms(3);
		}
		
	}
}

ISR(INT0_vect)
{
	if(btn == 0)
	{
	num =0;
	btn++;	
	}
	else if(btn == 1)
	{
	TIMSK |= 0x02; // OCIE0
	btn++;
	}
	else
	{
	TIMSK &= ~0x02;
	btn = 0;
	}

	
}
	
int main(void)
{
	//Timer0: 100ms stop-watch, timer2: FND refresh
			
	//TIMSK |= 0x02; // OCIE0
	TIMSK |= 0x40; // 0b 01xx xxxx TOIE2
	TCCR0 |= 0x07; // 분주비 1024  1/16M * 1024 *156
	TCCR2 |= 0x05; // 분주비 110:256, (1/16M) * 1024 * 256 = 0.004096(sec)

	
	EICRA |= (1<<ISC01);    // INT0 하강엣지에서 인터럽트 발생
	EIMSK |= (1<<INT0);     // INT0 인터럽트 활성화
		
	OCR0 = 156; //(1/16M) * 156 * 1024 *  = 0.099904 (~= 100ms)

	CDDR |= 0x0f; // 하위 4bit을 출력으로 설정
	IDDR |= 0xff; // 8bit 전체를 출력으로 설정
	
	CPORT = 0x0f; //모든 자리수 선택
	IPORT = ~0xff; //전체 세그먼트 ON ---> 8.

	
	sei();	
	while (1) 
    {
		sep(&num);
    }
}

