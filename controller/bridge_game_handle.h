#ifndef BRIDGEGAMEHANDLE_H
#define BRIDGEGAMEHANDLE_H

#include "controller/IGameController.h"
#include "model/IGameService.h"

class BridgeGameHandle : public IGameHandle
{
public:
    BridgeGameHandle(IGameService* game_service, IPresenter* presenter);
    virtual void Init(unsigned char total_step);
    virtual void Start();
    virtual void SelectItem(ESelection selection);
    virtual void Resume();

private:
    IGameService* game_service;
    IPresenter* presenter;
};

#endif // BRIDGEGAMEHANDLE_H
