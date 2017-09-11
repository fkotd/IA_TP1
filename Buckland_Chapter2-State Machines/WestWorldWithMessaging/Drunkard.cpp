#include "Drunkard.h"

bool Drunkard::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}

void Drunkard::Update()
{
  SetTextColor(FOREGROUND_BLUE | FOREGROUND_GREEN);

  m_pStateMachine->Update();
}


bool Drunkard::Drunk() const
{
    return m_iAlcohol > AlcoholLevel;
}

void Drunkard::DecreaseAlcohol()
{
    m_iAlcohol -= 1;
    if(m_iAlcohol < 0) {m_iAlcohol = 0;}
}

void Drunkard::IncreaseAlcohol()
{
     m_iAlcohol += 1;
}

bool Drunkard::Fatigued() const
{
    return m_iFatigue > FatigueLevel;
}

bool Drunkard::Rested() const
{
    return m_iFatigue == 0;
}

void Drunkard::DecreaseFatigue()
{
    m_iFatigue -= 1;
    if(m_iFatigue < 0) {m_iFatigue = 0;}
}

void Drunkard::IncreaseFatigue()
{
    m_iFatigue += 1;
}

bool Drunkard::TooAngry() const
{
    return m_iAnger > QuarrelThreshold;
}

void Drunkard::DecreaseAnger()
{
    m_iAnger -= 1;
    if(m_iAnger < 0) {m_iAnger = 0;}
}

void Drunkard::IncreaseAnger()
{
    m_iAnger += 1;
}
