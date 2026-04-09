#ifndef __VIRTUALCONTROLER_H__
#define __VIRTUALCONTORLER_H__

#include "simstruc.h"

#define ISR(vector) void ISR_##vector##_callback()
#define MIN(X, Y) ((X)<(Y)?(X):(Y))
#define MAX(X, Y) ((X)<(Y)?(Y):(X))

typedef uint8_T uint8_t;
typedef uint16_T uint16_t;


class VirtualControler
{
public:
    VirtualControler();

    uint8_t DDRA, DDRB, DDRC, DDRD;
    uint8_t PORTA, PORTB, PORTC, PORTD;
    uint8_t PINA, PINB, PINC, PIND;
    uint16_t TCNT0;
    uint8_t TCCR0A, TCCR0B, OCR0A, OCR0B, TIMSK0, TIFR0;
    uint16_t TCNT1H, TCNT1L;
    uint8_t TCCR1A, TCCR1B, TCCR1C, OCR1AH, OCR1AL, OCR1BH, OCR1BL, ICR1H, ICR1L, TIMSK1, TIFR1;
    uint16_t TCNT2;
    uint8_t TCCR2A, TCCR2B, OCR2A, OCR2B, TIMSK2, TIFR2;
    uint8_t SREG;

    uint8_t ADMUX, ADCSRA, ADCL, ADCH;

    void setup();
    void loop();

    ISR(TIMER0_COMPA_vect);
    ISR(TIMER0_COMPB_vect);
    ISR(TIMER0_OVF_vect);

    ISR(TIMER1_COMPA_vect);
    ISR(TIMER1_COMPB_vect);
    ISR(TIMER1_OVF_vect);

    ISR(TIMER2_COMPA_vect);
    ISR(TIMER2_COMPB_vect);
    ISR(TIMER2_OVF_vect);

    ISR(ADC_vect);
    

};

#endif //__VIRTUALCONTROLER_H__