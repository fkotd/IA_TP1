#include "Drunkard.h"

bool Drunkard::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}


void Drunkard::Update()
{
  SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

  m_pStateMachine->Update();
}

bool Drunkard::Drunk() const
{
    return m_iAlcohol > AlcoholLevel;
}

bool Drunkard::Fatigued() const
{
    return m_iFatigue > TirednessThreshold;
}

bool Drunkard::TooAngry() const
{
    return m_iAnger > QuarrelThreshold;
}
