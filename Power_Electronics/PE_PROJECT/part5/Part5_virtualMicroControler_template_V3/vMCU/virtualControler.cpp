#include "virtualControler.h"


VirtualControler::VirtualControler()
{
    DDRA = 0;
    DDRB = 0;
    DDRC = 0;
    DDRD = 0;

    PORTA = 0;PORTB = 0;PORTC = 0;PORTD=0;
    TCNT0 = 0;TCCR0A = 0;TCCR0B = 0;OCR0A = 0;OCR0B = 0;TIMSK0 = 0;TIFR0=0;
    TCNT1H = 0;TCNT1L = 0;TCCR1A = 0;TCCR1B = 0;TCCR1C = 0;OCR1AH = 0;OCR1AL = 0;OCR1BH = 0;OCR1BL = 0;ICR1H = 0;ICR1L = 0;TIMSK1 = 0;TIFR1=0;
    TCNT2 = 0;TCCR2A = 0;TCCR2B = 0;OCR2A = 0;OCR2B = 0;TIMSK2 = 0;TIFR2=0;
    ADCL = 0; ADCH = 0;
    SREG=0b10000000;
}


void VirtualControler::setup(){}
void VirtualControler::loop(){}

//ISR(TIMER0_COMPA_vect){}
void VirtualControler::ISR_TIMER0_COMPA_vect_callback(){}
//ISR(TIMER0_COMPB_vect){}
void VirtualControler::ISR_TIMER0_COMPB_vect_callback(){}
//ISR(TIMER0_OVF_vect){}
void VirtualControler::ISR_TIMER0_OVF_vect_callback(){}


//ISR(TIMER1_COMPA_vect){}
void VirtualControler::ISR_TIMER1_COMPA_vect_callback(){}
//ISR(TIMER1_COMPB_vect){}
void VirtualControler::ISR_TIMER1_COMPB_vect_callback(){}
//ISR(TIMER1_OVF_vect){}
void VirtualControler::ISR_TIMER1_OVF_vect_callback(){}


//ISR(TIMER2_COMPA_vect){}
void VirtualControler::ISR_TIMER2_COMPA_vect_callback(){}
//ISR(TIMER2_COMPB_vect){}
void VirtualControler::ISR_TIMER2_COMPB_vect_callback(){}
//ISR(TIMER2_OVF_vect){}
void VirtualControler::ISR_TIMER2_OVF_vect_callback(){}

//ISR(ADC_vect){}
void VirtualControler::ISR_ADC_vect_callback(){}
