#ifndef BRIDGEGAMESERVICE_H
#define BRIDGEGAMESERVICE_H

#include "model/IGameService.h"

class BridgeGameService : public IGameService
{
public:
    BridgeGameService();
    ~BridgeGameService();
    virtual void Init(unsigned char total_step);
    virtual bool GetResult(ESelection selection);
    virtual void GetProgress(unsigned char* step, bool* finished);

private:
    unsigned char total_step;
    unsigned char current_step;
    unsigned char* answer_arr;
    bool finished;
};

#endif // BRIDGEGAMESERVICE_H
