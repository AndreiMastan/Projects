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


    //***USER DEFINED CODE***//


};