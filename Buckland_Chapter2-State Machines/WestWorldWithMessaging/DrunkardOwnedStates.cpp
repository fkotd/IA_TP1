#include "DrunkardOwnedStates.h"
#include "../../Common/FSM/State.h"
#include "DRUNKARD.H";
#include "Locations.h"
#include "../../Common/Messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "../../Common/Time/CrudeTimer.h"
#include "EntityNames.h"
#include "../../Common/misc/utils.h"


#include <iostream>
using std::cout;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif


//------------------------------------------------------------------------methods for HaveADrink
HaveADrink* HaveADrink::Instance()
{
  static HaveADrink instance;
  return &instance;
}

void HaveADrink::Enter(Drunkard* pDrunkard)
{
  //if the drunkard is not already located at the saloon, he must
  //change location to the saloon
  if (pDrunkard->Location() != saloon)
  {
    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Back for another round !";
    pDrunkard->ChangeLocation(saloon);
  }
}

void HaveADrink::Execute(Drunkard* pDrunkard)
{
    pDrunkard->IncreaseAlcohol();
    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Just one more drop...";

    if (pDrunkard->Drunk())
    {
        switch(RandInt(0,1))
        {
            case 0:
                //pDrunkard->GetFSM()->ChangeState(DoOddThings::Instance());
                break;
            case 1:
                //pDrunkard->GetFSM()->ChangeState(Quarrel::Instance());
                break;
        }
    }
}

void HaveADrink::Exit(Drunkard* pDrunkard)
{
    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": "
        << "I feel like a want some entertainment !";
}

bool HaveADrink::OnMessage(Drunkard* pDrunkard, const Telegram& msg)
{
    //send msg to global message handler
    return false;
}
