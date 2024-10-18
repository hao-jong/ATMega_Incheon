	#define F_CPU 16000000

	#include <avr/io.h>
	#include <avr/delay.h>
	#include <avr/interrupt.h>

	#define CDDR	DDRA
	#define IDDR	DDRB
	#define CPORT	PORTA
	#define IPORT	PORTB

	unsigned char img[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x67};

	char digit_0[4] = {0};
	char digit_1[4] = {0};
	
	volatile int num = 0;
	volatile int timee = 0;
	volatile char btn_0 = 0;
	volatile char btn_1 = 0;
	volatile int clock_mode = 0;
	volatile int buzzer = 0;
	
	/*
	자리수 분리
	*/
	
	int sep_0 (int* a)
	{	if(a>9999)
		{
			*a = 0;
			return 0;
		}
		else
		{
			digit_0[3] = (char)((*a%10) / 1);
			digit_0[2] = (char)((*a%100) / 10);
			digit_0[1] = (char)((*a%1000) / 100);
			digit_0[0] = (char)((*a%10000) / 1000);
		}
		return 1;
	}

	int sep_1 (int* a)
	{
		if(a>3600)
		{
			*a = 0;
			return 0;
		}
		else
		{
			int min = (*a)%60;
			int hour = (*a/60)%60;
			digit_1[3] = (char)((min%10) / 1);
			digit_1[2] = (char)((min%100) / 10);
			digit_1[1] = (char)((hour%10) / 1);
			digit_1[0] = (char)((hour%100) / 10);
		}
		return 1;
	}

	/*
	FND refresh 기능
	*/

	ISR(TIMER2_OVF_vect) // FND refresh
	{
		//타이머 동작시 FND refresh
		if (clock_mode == 1)
		{
			
			for(int i =0; i<4; i++)
			{
				CPORT = 1<< i;
				if(i!=1)
				{
					IPORT = ~img[digit_0[i]];
					_delay_ms(3);
				}
				if(i ==1)
				{
					IPORT = ~(img[digit_0[i]]|0x80);
					_delay_ms(3);
				}
				
			}
		}
		// 시계 동작시  FND refresh
		else if(clock_mode == 0)
		{
			for(int i =0; i<4; i++)
			{
				CPORT = 1<< i;
				if(i!=1)
				{
					IPORT = ~img[digit_1[i]];
					_delay_ms(3);
				}
				if(i ==1)
				{
					IPORT = ~(img[digit_1[i]]|0x80);
					_delay_ms(3);
				}
				
			}
		}
		
	}
	
	/*
	Stop watch 기능
	btn_0 => INT 0 => 두번째 노란색 버튼
	digit_0 사용
	*/
	ISR(TIMER0_COMP_vect)
	{
		num--;
		if(num>10000 || num < 0)
		{
			num = 0;
			TIMSK &= ~0x02;
			btn_0 = 0;
			PORTG |= 0x08; // buzzer on
		}
		
	}


	ISR(INT0_vect)
	{
		switch(btn_0)
		{
		case 0:
			PORTG &= ~0x08;//buzzer off
			btn_0++;
			break;
			
		
		case 1: // 타이머 설정 모드 (초)
				
			TIMSK &= ~0x02;
			btn_0++;
			break;
		
		case 2: // 타이머 start 모드 
		
			TIMSK |= 0x02; // OCIE0
			break;
		}

		
	}

	/*
	시계 기능 구현 (16비트 타이머 <1번 카운터>)
	btn_1 => INT 1 => 첫번째 노란색 버튼 clock_mode 변경
	btn_3 => INT 2 => 세번째 노란색 버튼 timee += 60;
	*/
	
	ISR(INT1_vect)
	{
		switch (clock_mode)
		{
			case 0:
				clock_mode = 1;

				break;
			
			case 1:
				clock_mode = 0;

				break;
			
		}
	}

	ISR(INT2_vect)
	{
		
		switch (clock_mode)
		{
			case 0:
					timee += 60;
					break;


			case 1:
				if (btn_0 == 1)
				{
					num += 100;
				}

				break;
					
		}
		
	}



	ISR(TIMER1_COMPA_vect)
	{
		timee++;
		if(timee>3600)
		{
			timee = 0;
		}
		
	}
	
	int main(void)
	{
		//Timer0: 100ms stop-watch, timer2: FND refresh
		
		//TIMSK |= 0x02; // OCIE0
		TIMSK |= 0x42; // 0b 01xx_xx1x 0번 카운터 비교일치 : 1, 2번 카운터 오버플로 : 1
		TIMSK |= 0x10; // 0b xxx1_xxxx 1번 카운터 비교일치 : 1
		
		TCCR0 |= 0x0f; // 분주비 1024  1/16M * 1024 *156
		TCCR2 |= 0x05; // 분주비 111:1024, (1/16M) * 1024 * 256 = 0.004096(sec)
		
		TCCR1A =0;
		
		TCCR1B |=0x0d; // 분주비 101:1, (1/16M) * 1024 * 15625 = 1(sec)
		 //(WGM13, WGM12 on) CTC  mode 동작
		
		
		EIMSK |= 0x07;     // INT0,1,2 인터럽트 활성화
		EICRA |= 0x3a;    // INT0,1,2 하강엣지에서 인터럽트 발생
		
		OCR0 = 156; //(1/16M) * 156 * 1024 *  = 0.099904 (~= 100ms)
		OCR1A = 15625;
		
		CDDR |= 0x0f; // 하위 4bit을 출력으로 설정
		IDDR |= 0xff; // 8bit 전체를 출력으로 설정
		
		CPORT = 0x0f; //모든 자리수 선택
		IPORT = ~0xff; //전체 세그먼트 ON ---> 8.
		
		DDRG |= 0x08; //PORTG 3번 출력 설정
		
		
		btn_0 = 0;
		btn_1 = 0;
		clock_mode = 0;
		sei();
		while (1)
		{
			sep_0(&num);
			sep_1(&timee);
			

		}
	}



