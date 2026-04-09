#include "codeWrapper.h"

CodeWrapper::CodeWrapper():VirtualControler()
{
    
    clockCounter = 0;
    TOP0 = 255; TOP1 = 0xFFFF; TOP2 = 255;
    OCR0Acpy = 0;OCR0Bcpy = 0;OCR1AHcpy = 0;OCR1ALcpy = 0;OCR1BHcpy = 0;OCR1BLcpy = 0;OCR2Acpy = 0;OCR2Bcpy = 0;

    ADCstarted=0; ADCcounter=0;

}



