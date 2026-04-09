/*
 * proiect_lab.c
 *
 * Created: 12/27/2024 11:48:07 PM
 * Author : Andrei
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
int flag,s,ms,digit;

void display( uint8_t p, char c){
	PORTA&=0b11110000;
	PORTC&=0;
	switch (c)
	{
		case '0': PORTC |= 0b00111111; break;
		case '1': PORTC |= 0b00000110; break;
		case '2': PORTC |= 0b01011011; break;
		case '3': PORTC |= 0b01001111; break;
		case '4': PORTC |= 0b01100110; break;
		case '5': PORTC |= 0b01101101; break;
		case '6': PORTC |= 0b01111101; break;
		case '7': PORTC |= 0b00000111; break;
		case '8': PORTC |= 0b01111111; break;
		case '9': PORTC |= 0b01101111; break;
		case 'D': PORTC|=0b01011100;break;
		case 'O': PORTC|=0b00111111;break;
		case 'N': PORTC|=0b00110110;break;
		case 'E': PORTC|=0b01111001;break;
	}
	switch(p)
	{
		case 1: PORTA|=1<<0;break;
		case 2: PORTA|=1<<1;break;
		case 3: PORTA|=1<<2;break;
		case 4: PORTA|=1<<3;break;
	}

}

void init_PWM2(){//PWM INT
	DDRD |= 1<<7; //PD7 – pin de iesire
	TCCR2 = 0b01101001; //FastPWM neinversat, N=1
	OCR2 = (40 * 256) / 100;
}

void init_timer_0_int(){
	SREG = 1<<7;               // Global Interrupt Enable
	TCCR0 = 0b00001011;
	//CTC-3,6; Prescaler-0,1,2
	TCNT0 = 0;
	OCR0 = 125;
	TIMSK |= 0b00000010;
}

ISR(TIMER0_COMP_vect) { //ISR
	/*digit++;
	switch(digit){
		case 1: display(1,s%10); break;
		case 2: display(2,(s/10)%10);
		//PORTC|=0b10000000; //dot
		break;
		case 3: display(3,m%10); break;
		case 4: display(4,(m/10)%10); digit=0; break;
	}
	*/
	if (PINB & (1 << 4))
	{
		flag = 1;
	}
	if (PINB & (1 << 5))
	{
		flag = 0;
	}
	if (PINB & (1 << 6))
	{
		flag = 0;
		s = 0;
	}
	if (flag){
		
		ms++;
		if(ms>=999)
		{
			s++;
			ms=0;
		}
		 if (s == 5) OCR2 = (65 * 256) / 100;
		 if (s == 10) OCR2 = (90 * 256) / 100;
		if(s>=15){ OCR2=0; 
			digit++;
			switch(digit){
				case 1: display(1,'e'); break;
				case 2: display(2,'N');break;
				case 3: display(3,'O'); break;
				case 4: display(4,'D'); digit=0; break;
			}
		}

}
}

int main(void)
{	DDRB |=0b00000000;
	DDRA |= 0b00001111; //prin PORTA activam BCD urile/display urile
	DDRC |= 0b11111111; //prin PORTC activam segmentele
	
	 init_timer_0_int();
	 init_PWM2();
	
    while (1) 
    {
    }
	
}

