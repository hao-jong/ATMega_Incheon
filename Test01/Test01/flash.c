/*
 * Test01.cpp
 *
 * Created: 2024-10-02 오후 12:17:31
 * Author : user
 */ 

#define F_CPU 16000000UL
#include <avr/io.h> // AVR 기본 헤더
#include <util/delay.h>
//SW : PG3
//LED Port : PG4
//int check()
//{
	//char v = PING & 0x00;
	//if (v == 0) return 1;
	//return 0;
	//
//}
//int main(void)
//{
    ///* Replace with your application code */
	//DDRG |= 0x10; // xxxx xxxx ==> xxx1 xxxx:  1 = 출력 , 0 = 입력
	//DDRG &= ~0x08; // xxxx xxxx ==> xxxx 0xxx:  1 = 출력 , 0 = 입력
	////DDRG3 = 1; // bit에 직접 접근, but const (상수), read only ==> 
	//char mode = 0; // mode = 0 : disable, mode = 1 : active
	//char v;
	//while (1) // loop until SW
	//{
		//v = PING & 0x08; // v = G 그룹 3번 bit의 값
		//if(check())
		//{
			//mode = 1; 
			//break;	
		//}
	//}
	//while(mode == 1 )
		//{
			//PORTG |= 0x10; // bit on
			//_delay_ms(200);
			//PORTG &= ~0x10; // bit off
			//_delay_ms(200);
		//}
		//v = PING & 0x08;
		//if(check())
		//{
			//if(mode ==1) mode = 0;
			//else         mode = 1;				
			//_delay_ms(500);
		//}
//}

int main(void)
{
	DDRG |= 0x10; // xxxx xxxx ==> xxx1 xxxx:  1 = 출력 , 0 = 입력
	DDRG &= ~0x07; // xxxx xxxx ==> xxxx x000:  1 = 출력 , 0 = 입력

	
	char in_a,in_b,in_c;

	int i = 0;
	int j = 0;
	
	while(1)
	{
		in_a = PING & 0x01;
		if(in_a == 0)
		{
			break;
		}
	}
	
	
	while(1)
	{
		in_b = (PING & 0x02);  // PING1 비트를 0 또는 1로 변환
		in_c = (PING & 0x04);
	
		if(!(in_b) && (in_c))
		{
			_delay_ms(300);
			i++;
			_delay_ms(300);
		
		}
		else if((in_b) && !(in_c))
		{
			_delay_ms(300);
			j++;
			_delay_ms(300);
		
		}

        PORTG |= 0x10; // bit on
        for(int k = 0; k < 200+10*i-10*j; k++) 
		{
	        _delay_ms(1);
        }
        PORTG &= ~0x10; // bit off
        for(int k = 0; k < 200+10*i-10*j; k++) 
		{
	        _delay_ms(1);
		}
	}
	
}