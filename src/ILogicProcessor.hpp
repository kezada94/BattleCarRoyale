#ifndef LOGICPROCESSOR_H
#define LOGICPROCESSOR_H

class ILogicProcessor{

public:
    
    virtual void win() = 0;
    virtual void loose() = 0;

    virtual void checkStatus() = 0;
};

#endif