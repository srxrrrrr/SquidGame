#include "bridge_game_handle.h"

BridgeGameHandle::BridgeGameHandle(IGameService* game_service, IPresenter* presenter)
{
    this->game_service = game_service;
    this->presenter = presenter;
}

void BridgeGameHandle::Init(unsigned char total_step)
{
    presenter->ShowBehavior(EStage::Title);
    game_service->Init(total_step);
}

void BridgeGameHandle::Start()
{
    unsigned char step = 0;
    bool finished = false;

    presenter->ShowBehavior(EStage::Playing);
    game_service->GetProgress(&step, &finished);
    presenter->ShowProgress(step);
}

void BridgeGameHandle::SelectItem(ESelection selection)
{
    bool ret = false;
    unsigned char step = 0;
    bool finished = false;

    ret = game_service->CheckResult(selection);
    game_service->GetProgress(&step, &finished);

    if(ret)
    {
        if(finished)
        {
            presenter->ShowBehavior(EStage::Win);
            presenter->ShowProgress(step);
        }
        else
        {
            presenter->ShowBehavior(EStage::Correct);
            presenter->ShowProgress(step);
        }
    }
    else
    {
        presenter->ShowBehavior(EStage::Dead);
        presenter->ShowProgress(step);
    }
}

void BridgeGameHandle::Resume()
{
    presenter->ShowBehavior(EStage::Playing);
}
