#include "DrunkardOwnedStates.h"
#include "../../Common/FSM/State.h"
#include "DRUNKARD.H"
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
                pDrunkard->GetFSM()->ChangeState(DoOddThings::Instance());
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


//------------------------------------------------------------------------methods for DoOddThings
DoOddThings* DoOddThings::Instance()
{
  static DoOddThings instance;
  return &instance;
}

void DoOddThings::Enter(Drunkard* pDrunkard)
{
    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "In da place for the show...";
}

void DoOddThings::Execute(Drunkard* pDrunkard)
{
    pDrunkard->IncreaseFatigue();

    if (pDrunkard->Fatigued())
    {
        //pDrunkard->GetFSM()->ChangeState(TakeANap::Instance());
    }
    else
    {
        //send a message to the miner, check if he is in the saloon
        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pDrunkard->ID(),          //ID of sender
                              ent_Miner_Bob,            //ID of recipient
                              Msg_SomeoneWantToFight,   //the message
                              NO_ADDITIONAL_INFO);

        SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        //drunkrard is going to sing
        switch(RandInt(0,2))
        {
            case 0:
                cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Country road, take me home...";
                break;
            case 1:
                cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "West Quebec, Saguenay river...";
                break;
            case 2:
                cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Miner's lady, stranger to blue whiskey...";
                break;
        }
    }
}

void DoOddThings::Exit(Drunkard* pDrunkard)
{

}

bool DoOddThings::OnMessage(Drunkard* pDrunkard, const Telegram& msg)
{
    SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

    switch(msg.Msg)
    {
        case Msg_MinerHereToFight:
        {
            cout << "\nMessage handled by " << GetNameOfEntity(pDrunkard->ID()) << " at time: "
            << Clock->GetCurrentTime();

            SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

            cout << "\n" << GetNameOfEntity(pDrunkard->ID()) <<
                ": Oi, somethin' wrong with my performance ?";

            //pDrunkard->GetFSM()->ChangeState(Quarrel::Instance());
        }

        return true;

    }//end switch

    return false;
}
