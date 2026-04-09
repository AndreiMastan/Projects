#ifndef __CODEWRAPPER_H__
#define __CODEWRAPPER_H__
#include "virtualControler.h"

struct sInports
{
    uint16_t vPA_I_A[8];
    uint8_t vPA_I_D[8]; 
    uint8_t vPB_I_D[8]; 
    uint8_t vPC_I_D[8]; 
    uint8_t vPD_I_D[8]; 
};

struct sOutports
{
    uint8_t vPA_O_D[8];
    uint8_t vPB_O_D[8];
    uint8_t vPC_O_D[8];
    uint8_t vPD_O_D[8];
};



class CodeWrapper:public VirtualControler
{
public:
    CodeWrapper();

    float DEBUG_PORT;


    uint16_t clockCounter;
    uint16_t TOP0, TOP1, TOP2;
    uint8_t OCR0Acpy, OCR0Bcpy, OCR1AHcpy, OCR1ALcpy, OCR1BHcpy, OCR1BLcpy, OCR2Acpy,  OCR2Bcpy;
    uint16_t ADCbuffer[8];
    uint8_t ADCstarted, ADCcounter;
    uint16_t ADCconv;
    

};

#endif //__CODEWRAPPER_H__
