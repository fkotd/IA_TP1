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
                //send a message to the miner, check if he is in the saloon
                Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,       //time delay
                                            pDrunkard->ID(),          //ID of sender
                                            ent_Miner_Bob,            //ID of recipient
                                            Msg_SomeoneWantToFight,   //the message
                                            NO_ADDITIONAL_INFO);
                break;
        }
    }
}

void HaveADrink::Exit(Drunkard* pDrunkard)
{
    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "I feel like a want some entertainment !";
}

bool HaveADrink::OnMessage(Drunkard* pDrunkard, const Telegram& msg)
{
    //si on reçoit une reponse du mineur
    SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

    switch(msg.Msg)
    {
        case Msg_MinerHereToFight:
        {
            cout << "\nMessage handled by " << GetNameOfEntity(pDrunkard->ID()) << " at time: "
            << Clock->GetCurrentTime();

            SetTextColor(FOREGROUND_BLUE | FOREGROUND_GREEN);

            cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": Oi, somethin' wrong with my performance ?";

            pDrunkard->GetFSM()->ChangeState(QuarrelDrunkard::Instance());
        }

        return true;

    }//end switch

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
        pDrunkard->GetFSM()->ChangeState(TakeANap::Instance());
    }
    else
    {
        //send a message to the miner, check if he is in the saloon
        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pDrunkard->ID(),          //ID of sender
                              ent_Miner_Bob,            //ID of recipient
                              Msg_SomeoneWantToFight,   //the message
                              NO_ADDITIONAL_INFO);

        SetTextColor(FOREGROUND_BLUE | FOREGROUND_GREEN);
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

            SetTextColor(FOREGROUND_BLUE | FOREGROUND_GREEN);

            cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": Oi, somethin' wrong with my performance ?";

            pDrunkard->GetFSM()->ChangeState(QuarrelDrunkard::Instance());
        }

        return true;

    }//end switch

    return false;
}


//------------------------------------------------------------------------methods for TakeANap
TakeANap* TakeANap::Instance()
{
  static TakeANap instance;
  return &instance;
}

void TakeANap::Enter(Drunkard* pDrunkard)
{
    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Hard time is comin'..just need...to...sleep...";
}

void TakeANap::Execute(Drunkard* pDrunkard)
{
    pDrunkard->DecreaseAlcohol();
    pDrunkard->DecreaseFatigue();
    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "RRRrrr...RRRrrr";

    if (pDrunkard->Rested())
    {
        pDrunkard->GetFSM()->ChangeState(HaveADrink::Instance());
    }
}

void TakeANap::Exit(Drunkard* pDrunkard)
{
    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Ouch, my head hurts every day a little less...";
}

bool TakeANap::OnMessage(Drunkard* pDrunkard, const Telegram& msg)
{
    //send msg to global message handler
    return false;
}


//------------------------------------------------------------------------methods for SoberUpOutside
SoberUpOutside* SoberUpOutside::Instance()
{
  static SoberUpOutside instance;
  return &instance;
}

void SoberUpOutside::Enter(Drunkard* pDrunkard)
{
    if (pDrunkard->Location() != waterTrough)
    {
        cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Damn miner ! I can't even see where I am walkin'...Ahh!!";
        pDrunkard->ChangeLocation(waterTrough);
    }

}

void SoberUpOutside::Execute(Drunkard* pDrunkard)
{
    pDrunkard->DecreaseAlcohol();
    pDrunkard->DecreaseFatigue();
    pDrunkard->DecreaseAnger();
    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "RRRrrr...RRRrrr (in the water trough)";

    if (pDrunkard->Rested())
    {
        pDrunkard->GetFSM()->ChangeState(HaveADrink::Instance());
    }
}

void SoberUpOutside::Exit(Drunkard* pDrunkard)
{
    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Wha' Am I doin in this water trough ? Hum...goin' to the saloon";
}

bool SoberUpOutside::OnMessage(Drunkard* pDrunkard, const Telegram& msg)
{
    //send msg to global message handler
    return false;
}


//------------------------------------------------------------------------methods for QuarrelDrunkard
QuarrelDrunkard* QuarrelDrunkard::Instance()
{
    static QuarrelDrunkard instance;
    return &instance;
}

void QuarrelDrunkard::Enter(Drunkard* pDrunkard)
{
    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Miner, stop sneering";
}

void QuarrelDrunkard::Execute(Drunkard* pDrunkard)
{
    pDrunkard->IncreaseAnger();

    if (pDrunkard->TooAngry())
    {
        //envoyer un message au mineur pour lui dire que l'ivrogne s'en va
        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,   //time delay
                                pDrunkard->ID(),          //ID of sender
                                ent_Miner_Bob,            //ID of recipient
                                Msg_DrunkardStopQuarrel,  //the message
                                NO_ADDITIONAL_INFO);

        //SetTextColor(FOREGROUND_BLUE | FOREGROUND_GREEN);

        pDrunkard->GetFSM()->ChangeState(SoberUpOutside::Instance());
    }
    else
    {
        switch(RandInt(0,4))
        {
            case 0:
                cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Miners you've no skill for listenin' to music";
                break;
            case 1:
                cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "I was just takin' about a lady, not your lady !";
                break;
            case 2:
                cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Why can't we be friends ?";
                break;
            case 3:
                cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Do ya remember when ya drank my wine ?";
                break;
            case 4:
                cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "I'd kinda like to be the owner than ya !";
                break;
        }
    }

}

void QuarrelDrunkard::Exit(Drunkard* pDrunkard)
{
    cout << "\n" << GetNameOfEntity(pDrunkard->ID()) << ": " << "Okay...stopin' my mess owner...";
}

bool QuarrelDrunkard::OnMessage(Drunkard* pDrunkard, const Telegram& msg)
{
    //send msg to global message handler
    return false;
}
