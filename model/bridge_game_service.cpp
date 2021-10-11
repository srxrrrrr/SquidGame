#include "model/bridge_game_service.h"
#include <QRandomGenerator>
#include <QtCore>

BridgeGameService::BridgeGameService()
{
    total_step = 0;
    current_step = 0;
    finished = false;
}

BridgeGameService::~BridgeGameService()
{
    delete answer_arr;
}

void BridgeGameService::Init(unsigned char total_step)
{
    this->total_step = total_step;
    answer_arr = new unsigned char[total_step];
    for(int i=0; i < total_step; i++)
    {
        *(answer_arr+i) = QRandomGenerator::global()->bounded(2);
        qDebug() << "random:" << *(answer_arr+i);
    }
}

bool BridgeGameService::GetResult(ESelection selection)
{
    if((unsigned char)selection == *(answer_arr + current_step))
    {
        qDebug() << "Correct!";
        current_step++;
        if(current_step >= total_step)
        {
            finished = true;
        }
        return true;
    }
    else
    {
        qDebug() << "You dead";
        return false;
    }
}

void BridgeGameService::GetProgress(unsigned char* step, bool* finished)
{
    *step = current_step;
    *finished = this->finished;
}
