#ifndef IGAMESERVICE_H
#define IGAMESERVICE_H

#include "type_define.h"

class IGameService
{
public:
    virtual ~IGameService(){}
    virtual void Init(unsigned char total_step) = 0;
    virtual bool CheckResult(ESelection selection) = 0;
    virtual void GetProgress(unsigned char* step, bool* finished) = 0;
};

#endif // IGAMESERVICE_H
