#include "codeWrapper.h"

class VirtualCode:public CodeWrapper
{
public:    
    VirtualCode();
    void stepProcess();

    void decodeInports(sInports*, sOutports*);
    void encodeOutports(sOutports*);
private:
    
    //encodeOutports
    void outNormalPortOperation(sOutports*);
    void ocTimer(sOutports*);
    void stepTimer(sOutports*);

    //Init
    void initRegisters();

    //stepProcess

    void triggerInterrupts();
    void stepADC();


    uint8_t num;
	uint16_t adcResult[2];
	 float filteringFactor;
	 float voltageDivider;
	
	
	
	void readADC_it(uint8_t ch){
	    ADMUX &= 0b11100000; //Resetează canalul de conversie
	    ADMUX |= ch; //Setează canalul conversiei
	    ADCSRA |= (1<<6); //Începe conversia
	}
	
	ISR(ADC_vect)
	{
	    uint8_t adc_l = ADCL;
	    uint8_t adc_h = ADCH;
	    if((ADMUX & 7) == 0) //if current conversion is for ch0, retrigger for ch1
	    {
	        //
	        adcResult[0] = adcResult[0]*filteringFactor +  ((adc_h << 8) | adc_l)*(1-filteringFactor);
	        readADC_it(1);
	    }
	    else
	    {
	        adcResult[1] = adcResult[1]*filteringFactor +  ((adc_h << 8) | adc_l)*(1-filteringFactor);
	        readADC_it(0);
	    }
	}
	
	ISR(TIMER0_OVF_vect)
	{
	    uint16_t valueADCUout = adcResult[0]; //ch0 contains feedback voltage
	    uint16_t valueADCUsp = adcResult[1]; //ch1 contains setpoint voltage
	
	    float uOut = valueADCUout * 5.0/1024 / voltageDivider;
	    float uSp = valueADCUsp * 5.0/1024 / voltageDivider;
	
	    //error, the input in the controler
	    float error = uSp - uOut;
	
	    //***controler
		static float e1 = 0, e2 = 0, e3 = 0, e4 = 0;
	static float y1 = 0, y2 = 0, y3 = 0, y4 = 0;
			// Current input
			float ek = error;   // u[k]
	
			// Compute output y[k] using recurrence
			float command =
				2.946f  * y1
				- 3.196f  * y2
				+ 1.513f  * y3
				- 0.2636f * y4
				+ 0.008399f * ek
				- 0.02508f  * e1
				+ 0.02935f  * e2
				- 0.01578f  * e3
				+ 0.003254f * e4;
	
			// Shift history (latest moves into y1/e1, oldest is dropped)
			y4 = y3;
			y3 = y2;
			y2 = y1;
			y1 = command;
	
			e4 = e3;
			e3 = e2;
			e2 = e1;
			e1 = ek;
	    //***controler
		command=command*100;
	
	    //limit the command between 0% and 100% duty cycle
	    float command_lim = MIN(100, MAX(command, 0)); //percent
	    
	    //write command to timer
	    OCR2B =(uint8_t) (command_lim*OCR2A/100);
	    DEBUG_PORT = OCR2B;
	
	}
	
	void init_ADC(){
	    ADMUX = 0b01000000; //Referința - AVCC, Interrupt enable
	    ADCSRA= 0b10001111; //Activare ADC; Prescaler = 128;
	}
	
	
	
	void setup()
	{
	    //global variable initialization
	    num = 0;
	    adcResult[0] = 0; 
	    adcResult[1] = 0;
	    filteringFactor = 1;
	    voltageDivider = 0.1;
	
	    DDRA &= ~(1<<0); //PA0 input
	    DDRA &= ~(1<<1); //PA1 input
	    DDRD |= 1<<3; //PD3 output (OC2B)
	
	    //Interrupt setup by Timer0
	    TCCR0A = 0b00000010; //CTC mode
	    TCCR0B = 0b00000011; //prescaler 64
	    TIMSK0 |= 1<<0;
	    OCR0A = 103; //800Hz -> 155, 1200Hz -> 103, 1500Hz -> 82
	
	    //PWM output
	    TCCR2A = 0b00100011; //Fast PWM, OCRA top, Output OCRB active, non inverting
	    TCCR2B = 0b00001100; //prescaler 64
	    OCR2A = 103; //800Hz -> 155, 1200Hz -> 103, 1500Hz -> 82
	    OCR2B = 0;
	
	    init_ADC();
	    //start first ADC conversion
	    readADC_it(0);
	}
	
	void loop()
	{
	    //loop is empty, cyclic tasks are performed in interrupt rutines
	}


};