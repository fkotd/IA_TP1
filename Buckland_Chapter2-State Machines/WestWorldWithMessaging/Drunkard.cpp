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
