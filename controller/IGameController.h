#ifndef IGAMECONTROLLER_H
#define IGAMECONTROLLER_H

#include "type_define.h"

class IGameHandle
{
public:
    virtual ~IGameHandle(){}
    virtual void Init(unsigned char total_step) = 0;
    virtual void Start() = 0;
    virtual void SelectItem(ESelection selection) = 0;
    virtual void Resume() = 0;
};

class IPresenter
{
public:
    virtual ~IPresenter(){}
    virtual void ShowImage(EStage stage) = 0;
    virtual void ShowProgress(unsigned char progress) = 0;
};

#endif // IGAMECONTROLLER_H
