#include <avr/io.h> // AVR 기본 헤더

int testbit(char pin,char mask) // PINx 레지스터의 값의  mask bit가 0인지 1인지 판별
{
	if(pin & mask !=0)
	{
		return 1;
	}
	
	return 0;
}

void Standby (void)
{
	DDRG = ~0x01; //G Port 5번 bit 입력
	PORTG |= 0x01; // PG4 pull-up
	
	while(1)
	{
		if(testbit(PING, 0x10)) break;
	}
	
	while(1)
	{
		if((PING & 0x10) == 0)
		{
			break;
		}
		
	}
}