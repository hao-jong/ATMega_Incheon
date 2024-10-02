/*
 * Test01.cpp
 *
 * Created: 2024-10-02 오후 12:17:31
 * Author : user
 */ 


#include <avr/io.h> // AVR 기본 헤더
//LED Port : PG4
int main(void)
{
    /* Replace with your application code */
	DDRG |= 0x10; // xxxx xxxx ==> xxx1 0000:  1 = 출력 , 0 = 입력
	while (1) 
    {
		PORTG |= 0x01;
    }
}

