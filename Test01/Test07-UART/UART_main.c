#define F_CPU 16000000L
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
//#inculde "avr/iom128.h"

void UART0_init()
{	/*
	initiate UART0
	UART REgister setting 1 -Baud Rate : 9600 N 8 1
	*/
	UCSR0A |= (1<<U2X0); // 2배속 설정
	UBRR0H = 0x00;
	UBRR0L = 207; //Baud Rate : 9600
	
	UCSR0C &= ~((1<<UPM00)|(1<<UPM00)); // Parity : 00-None(default) 10-Even 11-odd 
	
	UCSR0B &= ~(1<<UCSZ02); 
	UCSR0C |= (1<<UCSZ01) |(1<<UCSZ00); // Data bits : 011-8bit(default), 111-9bit, 010-7bit
	
	UCSR0C &= ~(1<<USBS0); //Stop bit  : 0-1(default) 1-2 	
    
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0); // UART Open
}

void UART0_putc(char c)
{
	while(1) //입출력 버퍼가 준비될 때까지 대기
	{
		if((UCSR0A & (1<<UDRE0)))
		{
			break;
		}
	}
			
	UDR0 = c; // send
}
char buf[256];
char* str = "button pushed!!!/r/n";
int n = 0;

void UART0_puts(char *str)
{
	while(*str)
	{
		UART0_putc(*str++);
	}
}

ISR(INT0_vect)
{	
	sprintf(buf, "button pushed %d times.\r\n",n++);
	UART0_puts(buf);
	
}


int main(void)
{	
	UART0_init();
	UART0_puts("\033[2J"); // screen clear - ESC[2J
	UART0_puts("\033[0;0H"); // move cursor to Left_top - ESC[0;0H

	EIMSK |= (1<<INT0);
	EICRA |= 0x02; // descending edge
	sei();
	
	while (1) 
    {
    
	}
}

