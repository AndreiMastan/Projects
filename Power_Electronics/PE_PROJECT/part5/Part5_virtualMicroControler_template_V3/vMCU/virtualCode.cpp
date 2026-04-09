#include "virtualCode.h"

VirtualCode::VirtualCode():CodeWrapper()
{
    setup();
}

void VirtualCode::stepProcess()
{   
    loop();
    triggerInterrupts();
    stepADC();
}


void VirtualCode::encodeOutports(sOutports* varOutports)
{
    outNormalPortOperation(varOutports);
    stepTimer(varOutports);
    ocTimer(varOutports);

    
}

void VirtualCode::decodeInports(sInports* varInports, sOutports* varOutports)
{
    uint8_t pinNum=0;
    for(pinNum=0; pinNum<8; pinNum++)
    {
        if(DDRA & (1<<pinNum)) 
        {
            PINA |= ((varOutports->vPA_O_D[pinNum] == 0?0:1)<<pinNum);
            ADCbuffer[pinNum] = varOutports->vPA_O_D[pinNum] == 0 ? 0 : 5000;
        }
        else 
        {
            PINA |= ((varInports->vPA_I_D[pinNum] == 0?0:1)<<pinNum);
            ADCbuffer[pinNum] = varInports->vPA_I_A[pinNum];
        }
        if(DDRB & (1<<pinNum)) PINB |= ((varOutports->vPB_O_D[pinNum] == 0?0:1)<<pinNum);
        else PINB |= ((varInports->vPB_I_D[pinNum] == 0?0:1)<<pinNum);
        if(DDRC & (1<<pinNum)) PINC |= ((varOutports->vPC_O_D[pinNum] == 0?0:1)<<pinNum);
        else PINC |= ((varInports->vPC_I_D[pinNum] == 0?0:1)<<pinNum);
        if(DDRD & (1<<pinNum)) PIND |= ((varOutports->vPD_O_D[pinNum] == 0?0:1)<<pinNum);
        else PIND |= ((varInports->vPD_I_D[pinNum] == 0?0:1)<<pinNum);
    }
}


void VirtualCode::outNormalPortOperation(sOutports* varOutports)
{
    uint8_t COM0A = (TCCR0A & 0xC0)>>6;
    uint8_t COM0B = (TCCR0A & 0x30)>>4;
    uint8_t COM1A = (TCCR1A & 0xC0)>>6;
    uint8_t COM1B = (TCCR1A & 0x30)>>4;
    uint8_t COM2A = (TCCR2A & 0xC0)>>6;
    uint8_t COM2B = (TCCR2A & 0x30)>>4;

    uint8_t pinNum=0;
    for(pinNum=0; pinNum<8; pinNum++)
    {
        if(DDRA & (1<<pinNum))
            varOutports->vPA_O_D[pinNum] = PORTA & (1<<pinNum) ? 1 : 0;
        if(DDRB & (1<<pinNum))
        {
            if((pinNum == 1 && COM1A != 0) || (pinNum == 2 && COM1B != 0) || (pinNum == 3 && COM2A != 0)){}
            else varOutports->vPB_O_D[pinNum] = PORTB & (1<<pinNum) ? 1 : 0;
        }

        if(DDRC & (1<<pinNum))
            varOutports->vPC_O_D[pinNum] = PORTC & (1<<pinNum) ? 1 : 0;
        if(DDRD & (1<<pinNum))
        {
            if((pinNum == 6 && COM0A != 0) || (pinNum == 5 && COM0B != 0) || (pinNum == 3 && COM2B != 0)){}
            else varOutports->vPD_O_D[pinNum] = PORTD & (1<<pinNum) ? 1 : 0;
        }
    }
}


void VirtualCode::ocTimer(sOutports* varOutports)
{
    uint8_t WGM0 = (TCCR0B & 0x8)/2 + (TCCR0A & 0x3);
    uint8_t WGM1 = (TCCR1B & 0x18)/2 + (TCCR1A & 0x3);
    uint8_t WGM2 = (TCCR2B & 0x8)/2 + (TCCR2A & 0x3);
    uint8_t COM0A = (TCCR0A & 0xC0)>>6;
    uint8_t COM0B = (TCCR0A & 0x30)>>4;
    uint8_t COM1A = (TCCR1A & 0xC0)>>6;
    uint8_t COM1B = (TCCR1A & 0x30)>>4;
    uint8_t COM2A = (TCCR2A & 0xC0)>>6;
    uint8_t COM2B = (TCCR2A & 0x30)>>4;

    if(TCNT0 == OCR0Acpy)
    {
        TIFR0 |= 2;
        if(DDRD & (1<<6)) switch(WGM0)
        {
            case 0:
            case 2: //non PWM
                    switch(COM0A)
                    {
                        case 1: varOutports->vPD_O_D[6] ^= 1; break;
                        case 2: varOutports->vPD_O_D[6] = 0; break;
                        case 3: varOutports->vPD_O_D[6] = 1; break;
                        default: break;
                    }break;
            case 1:
            case 3:
            case 5:
            case 7:  //PWM
                    switch(COM0A)
                    {
                        case 2: varOutports->vPD_O_D[6] = 0; break;
                        case 3: varOutports->vPD_O_D[6] = 1; break;
                        default: break;
                    }break;
            default: break;
        }
    }

    if(TCNT0 == OCR0Bcpy)
    {
        TIFR0 |= 4;
        if(DDRD & (1<<5)) switch(WGM0)
        {
            case 0:
            case 2: //non PWM
                    switch(COM0B)
                    {
                        case 1: varOutports->vPD_O_D[5] ^= 1; break;
                        case 2: varOutports->vPD_O_D[5] = 0; break;
                        case 3: varOutports->vPD_O_D[5] = 1; break;
                        default: break;
                    }break;
            case 1:
            case 3:
            case 5:
            case 7:  //PWM
                    switch(COM0B)
                    {
                        case 2: varOutports->vPD_O_D[5] = 0; break;
                        case 3: varOutports->vPD_O_D[5] = 1; break;
                        default: break;
                    }break;
            default: break;
        }
    }

    if((TCNT1H<<8+TCNT1L) == (OCR1AHcpy<<8+OCR1ALcpy))
    {
        TIFR1 |= 2;
        if(DDRB & (1<<1)) switch(WGM1)
        {
            case 0:
            case 4:
            case 12: //non PWM
                    switch(COM1A)
                    {
                        case 1: varOutports->vPB_O_D[1] ^= 1; break;
                        case 2: varOutports->vPB_O_D[1] = 0; break;
                        case 3: varOutports->vPB_O_D[1] = 1; break;
                        default: break;
                    }break;
            case 1:
            case 2:
            case 3:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 14:
            case 15:  //PWM
                    switch(COM1A)
                    {
                        case 2: varOutports->vPB_O_D[1] = 0; break;
                        case 3: varOutports->vPB_O_D[1] = 1; break;
                        default: break;
                    }break;
            default: break;
        }
    }


    if((TCNT1H<<8+TCNT1L) == (OCR1BHcpy<<8+OCR1BLcpy))
    {   
        TIFR1 |= 4;
        if(DDRB & (1<<2)) switch(WGM1)
        {
            case 0:
            case 4:
            case 12: //non PWM
                    switch(COM1B)
                    {
                        case 1: varOutports->vPB_O_D[2] ^= 1; break;
                        case 2: varOutports->vPB_O_D[2] = 0; break;
                        case 3: varOutports->vPB_O_D[2] = 1; break;
                        default: break;
                    }break;
            case 1:
            case 2:
            case 3:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 14:
            case 15:  //PWM
                    switch(COM1B)
                    {
                        case 2: varOutports->vPB_O_D[2] = 0; break;
                        case 3: varOutports->vPB_O_D[2] = 1; break;
                        default: break;
                    }break;
            default: break;
        }
    }
    
    if(TCNT2 == OCR2Acpy)
    {
        TIFR2 |= 2;
        if(DDRB & (1<<3)) switch(WGM2)
        {
            case 0:
            case 2: //non PWM
                    switch(COM2A)
                    {
                        case 1: varOutports->vPB_O_D[3] ^= 1; break;
                        case 2: varOutports->vPB_O_D[3] = 0; break;
                        case 3: varOutports->vPB_O_D[3] = 1; break;
                        default: break;
                    }break;
            case 1:
            case 3:
            case 5:
            case 7:  //PWM
                    switch(COM2A)
                    {
                        case 2: varOutports->vPB_O_D[3] = 0; break;
                        case 3: varOutports->vPB_O_D[3] = 1; break;
                        default: break;
                    }break;
            default: break;
        }
    }

    if(TCNT2 == OCR2Bcpy)
    {
        TIFR2 |= 4;
        if(DDRD & (1<<3)) switch(WGM2)
        { 
            case 0:
            case 2: //non PWM
                    switch(COM2B)
                    {
                        case 1: varOutports->vPD_O_D[3] ^= 1; break;
                        case 2: varOutports->vPD_O_D[3] = 0; break;
                        case 3: varOutports->vPD_O_D[3] = 1; break;
                        default: break;
                    }break;
            case 1:
            case 3:
            case 5:
            case 7:  //PWM
                    switch(COM2B)
                    {
                        case 2: varOutports->vPD_O_D[3] = 0; break;
                        case 3: varOutports->vPD_O_D[3] = 1; break;
                        default: break;
                    }break;
            default: break;
        }
    }

}

void VirtualCode::stepTimer(sOutports* varOutports)
{
    clockCounter += 1;

    uint8_t WGM0 = (TCCR0B & 0x8)/2 + (TCCR0A & 0x3);
    uint8_t WGM1 = (TCCR1B & 0x18)/2 + (TCCR1A & 0x3);
    uint8_t WGM2 = (TCCR2B & 0x8)/2 + (TCCR2A & 0x3);

    uint8_t COM0A = (TCCR0A & 0xC0)>>6;
    uint8_t COM0B = (TCCR0A & 0x30)>>4;
    uint8_t COM1A = (TCCR1A & 0xC0)>>6;
    uint8_t COM1B = (TCCR1A & 0x30)>>4;
    uint8_t COM2A = (TCCR2A & 0xC0)>>6;
    uint8_t COM2B = (TCCR2A & 0x30)>>4;
    

    switch(WGM0)
    {
        case 0:
        case 1:
        case 3: TOP0 = 0xFF; break;
        case 2:
        case 5:
        case 7: TOP0 = OCR0Acpy; break;
        default: break;
    }

    switch(WGM1)
    {
        case 0: TOP1 = 0xFFFF; break;
        case 1: TOP1 = 0x00FF; break;
        case 2: TOP1 = 0x01FF; break;
        case 3: TOP1 = 0x03FF; break;
        case 4: TOP1 = OCR1AHcpy<<8+OCR1ALcpy; break;
        case 5: TOP1 = 0x00FF; break;
        case 6: TOP1 = 0x01FF; break;
        case 7: TOP1 = 0x03FF; break;
        case 8: TOP1 = ICR1H<<8+ICR1L; break;
        case 9: TOP1 = OCR1AHcpy<<8+OCR1ALcpy; break;
        case 10: TOP1 = ICR1H<<8+ICR1L; break;
        case 11: TOP1 = OCR1AHcpy<<8+OCR1ALcpy; break;
        case 12: TOP1 = ICR1H<<8+ICR1L; break;
        case 14: TOP1 = ICR1H<<8+ICR1L; break;
        case 15: TOP1 = OCR1AHcpy<<8+OCR1ALcpy; break;
        default: break;
    }

    switch(WGM2)
    {
        case 0:
        case 1:
        case 3: TOP2 = 0xFF; break;
        case 2:
        case 5:
        case 7: TOP2 = OCR2Acpy; break;
        default: break;
    }

    switch(TCCR0B & 0x7)
    {
        case 0: break;
        case 1: TCNT0++;
        case 2: TCNT0 += clockCounter % 8 == 0 ? 1 : 0; break;
        case 3: TCNT0 += clockCounter % 64 == 0 ? 1 : 0; break;
        case 4: TCNT0 += clockCounter % 256 == 0 ? 1 : 0; break;
        case 5: TCNT0 += clockCounter % 1024 == 0 ? 1 : 0; break;
        default: break;
    }
    if( TCNT0 / (TOP0+1) > 0)
    {
        TIFR0 |= 1;
        TCNT0 = 0;
        OCR0Acpy = OCR0A;
        OCR0Bcpy = OCR0B;
        if(WGM0 == 1 || WGM0 == 3 || WGM0 == 5 || WGM0 == 7)
        {
            if(DDRD & (1<<6)) switch(COM0A)
            {
                case 2: varOutports->vPD_O_D[6] = 1; break;
                case 3: varOutports->vPD_O_D[6] = 0; break;
                default: break;
            }
            if(DDRD & (1<<5)) switch(COM0B)
            {
                case 2: varOutports->vPD_O_D[5] = 1; break;
                case 3: varOutports->vPD_O_D[5] = 0; break;
                default: break;
            }
        }
    }
    
    switch(TCCR1B & 0x7)
    {
        case 0: break;
        case 1: TCNT1L++;
        case 2: TCNT1L += clockCounter % 8 == 0 ? 1 : 0; break;
        case 3: TCNT1L += clockCounter % 64 == 0 ? 1 : 0; break;
        case 4: TCNT1L += clockCounter % 256 == 0 ? 1 : 0; break;
        case 5: TCNT1L += clockCounter % 1024 == 0 ? 1 : 0; break;
        default: break;
    }
    TCNT1H += TCNT1L/256;
    TCNT1L = TCNT1L % 256;
    if( (TCNT1H<<8 + TCNT1L) / (TOP1+1) > 0)
    {
        TIFR1 |=1;
        TCNT1H = 0;
        TCNT1L = 0;
        OCR1AHcpy = OCR1AH;
        OCR1ALcpy = OCR1AL;
        OCR1BHcpy = OCR1BH;
        OCR1BLcpy = OCR1BL;

        if(WGM1 != 0 && WGM1 != 4 && WGM1 != 12 && WGM1 != 13)
        {
            if(DDRB & (1<<1)) switch(COM1A)
            {
                case 2: varOutports->vPB_O_D[1] = 1; break;
                case 3: varOutports->vPB_O_D[1] = 0; break;
                default: break;
            }
            if(DDRB & (1<<2)) switch(COM1B)
            {
                case 2: varOutports->vPB_O_D[2] = 1; break;
                case 3: varOutports->vPB_O_D[2] = 0; break;
                default: break;
            }
        }

    }
    

    switch(TCCR2B & 0x7)
    {
        case 0: break;
        case 1: TCNT2++;
        case 2: TCNT2 += clockCounter % 8 == 0 ? 1 : 0; break;
        case 3: TCNT2 += clockCounter % 32 == 0 ? 1 : 0; break;
        case 4: TCNT2 += clockCounter % 64 == 0 ? 1 : 0; break;
        case 5: TCNT2 += clockCounter % 128 == 0 ? 1 : 0; break;
        case 6: TCNT2 += clockCounter % 256 == 0 ? 1 : 0; break;
        case 7: TCNT2 += clockCounter % 1024 == 0 ? 1 : 0; break;
        default: break;
    }

    if( TCNT2 / (TOP2+1) > 0)
    {
        TIFR2 |= 1;
        TCNT2 = 0;
        OCR2Acpy = OCR2A;
        OCR2Bcpy = OCR2B;

        if(WGM2 == 1 || WGM2 == 3 || WGM2 == 5 || WGM2 == 7)
        {
            if(DDRB & (1<<3)) switch(COM2A)
            {
                case 2: varOutports->vPB_O_D[3] = 1; break;
                case 3: varOutports->vPB_O_D[3] = 0; break;
                default: break;
            }
            if(DDRD & (1<<3)) switch(COM2B)
            {
                case 2: varOutports->vPD_O_D[3] = 1; break;
                case 3: varOutports->vPD_O_D[3] = 0; break;
                default: break;
            }
        }
    }

}
void VirtualCode::triggerInterrupts()
{
    if((SREG & 0x80) == 0) return;

    if((TIFR0 & (1<<0)) && (TIMSK0 & (1<<0))) ISR_TIMER0_OVF_vect_callback();
    if((TIFR0 & (1<<1)) && (TIMSK0 & (1<<1))) ISR_TIMER0_COMPA_vect_callback();
    if((TIFR0 & (1<<2)) && (TIMSK0 & (1<<2))) ISR_TIMER0_COMPB_vect_callback();

    if((TIFR1 & (1<<0)) && (TIMSK1 & (1<<0))) ISR_TIMER1_OVF_vect_callback();
    if((TIFR1 & (1<<1)) && (TIMSK1 & (1<<1))) ISR_TIMER1_COMPA_vect_callback();
    if((TIFR1 & (1<<2)) && (TIMSK1 & (1<<2))) ISR_TIMER1_COMPB_vect_callback();

    if((TIFR2 & (1<<0)) && (TIMSK2 & (1<<0))) ISR_TIMER2_OVF_vect_callback();
    if((TIFR2 & (1<<1)) && (TIMSK2 & (1<<1))) ISR_TIMER2_COMPA_vect_callback();
    if((TIFR2 & (1<<2)) && (TIMSK2 & (1<<2))) ISR_TIMER2_COMPB_vect_callback();

    if((ADCSRA & (1<<4)) && (ADCSRA & (1<<3))) ISR_ADC_vect_callback();

    TIFR0 = 0;
    TIFR1 = 0;
    TIFR2 = 0;
    ADCSRA &= ~(1<<4);
}

void VirtualCode::stepADC()
{
    if(!(ADCSRA & (1<<7))) return;
    if(ADCSRA & (1<<6))
    {

        if(!ADCstarted)
        {
            ADCstarted = 1;
            ADCcounter = 0;
        }
        else 
        {
            switch(ADCSRA & 0x7)
            {
                case 0:
                case 1: ADCcounter += clockCounter % 2 == 0 ? 1 : 0; break;
                case 2: ADCcounter += clockCounter % 4 == 0 ? 1 : 0; break;
                case 3: ADCcounter += clockCounter % 8 == 0 ? 1 : 0; break;
                case 4: ADCcounter += clockCounter % 16 == 0 ? 1 : 0; break;
                case 5: ADCcounter += clockCounter % 32 == 0 ? 1 : 0; break;
                case 6: ADCcounter += clockCounter % 64 == 0 ? 1 : 0; break;
                case 7: ADCcounter += clockCounter % 128 == 0 ? 1 : 0; break;
            }
            if(ADCcounter>=10)
            {
                ADCstarted = 0;
                ADCSRA &= ~(1<<6);
                switch((ADMUX & 0xC0)>>6)
                {
                    case 0:
                    case 1: ADCconv = ADCbuffer[ADMUX & 0x7] * 1024 / 5000; break;
                    case 3: ADCconv = ADCbuffer[ADMUX & 0x7] * 1024 / 1100; break;
                }
                ADCconv = MIN(ADCconv, 1023);
                if(ADMUX & (1<<5)) ADCconv<<6;
                ADCH=(ADCconv>>8) & 0xFF;
                ADCL=ADCconv & 0xFF;
                ADCSRA |= 1<<4;
            }
        }
    }
    else ADCstarted = 0;
}





