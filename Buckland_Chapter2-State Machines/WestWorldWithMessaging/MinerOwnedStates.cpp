#include "MinerOwnedStates.h"
#include "../../Common/FSM/State.h"
#include "Miner.h"
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


//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance()
{
  static EnterMineAndDigForNugget instance;

  return &instance;
}


void EnterMineAndDigForNugget::Enter(Miner* pMiner)
{
  //if the miner is not already located at the goldmine, he must
  //change location to the gold mine
  if (pMiner->Location() != goldmine)
  {
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Walkin' to the goldmine";

    pMiner->ChangeLocation(goldmine);
  }
}


void EnterMineAndDigForNugget::Execute(Miner* pMiner)
{
  //Now the miner is at the goldmine he digs for gold until he
  //is carrying in excess of MaxNuggets. If he gets thirsty during
  //his digging he packs up work for a while and changes state to
  //gp to the saloon for a whiskey.
  pMiner->AddToGoldCarried(1);

  pMiner->IncreaseFatigue();

  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Pickin' up a nugget";

  //if enough gold mined, go and put it in the bank
  if (pMiner->PocketsFull())
  {
    pMiner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
  }

  if (pMiner->Thirsty())
  {
    pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
  }
}


void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
       << "Ah'm leavin' the goldmine with mah pockets full o' sweet gold";
}


bool EnterMineAndDigForNugget::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

//------------------------------------------------------------------------methods for VisitBankAndDepositGold

VisitBankAndDepositGold* VisitBankAndDepositGold::Instance()
{
  static VisitBankAndDepositGold instance;

  return &instance;
}

void VisitBankAndDepositGold::Enter(Miner* pMiner)
{
  //on entry the miner makes sure he is located at the bank
  if (pMiner->Location() != bank)
  {
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Goin' to the bank. Yes siree";

    pMiner->ChangeLocation(bank);
  }
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
  //deposit the gold
  pMiner->AddToWealth(pMiner->GoldCarried());

  pMiner->SetGoldCarried(0);

  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
       << "Depositing gold. Total savings now: "<< pMiner->Wealth();

  //wealthy enough to have a well earned rest?
  if (pMiner->Wealth() >= ComfortLevel)
  {
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
         << "WooHoo! Rich enough for now. Back home to mah li'lle lady";

    pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
  }

  //otherwise get more gold
  else
  {
    pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }
}


void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Leavin' the bank";
}


bool VisitBankAndDepositGold::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}
//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested

GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance()
{
  static GoHomeAndSleepTilRested instance;

  return &instance;
}

void GoHomeAndSleepTilRested::Enter(Miner* pMiner)
{
  if (pMiner->Location() != shack)
  {
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Walkin' home";

    pMiner->ChangeLocation(shack);

    //let the wife know I'm home
    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,     //time delay
                              pMiner->ID(),             //ID of sender
                              ent_Elsa,                 //ID of recipient
                              Msg_HiHoneyImHome,        //the message
                              NO_ADDITIONAL_INFO);
  }
}

void GoHomeAndSleepTilRested::Execute(Miner* pMiner)
{
  //if miner is not fatigued start to dig for nuggets again.
  if (!pMiner->Fatigued())
  {
     cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
          << "All mah fatigue has drained away. Time to find more gold!";

     pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }

  else
  {
    //sleep
    pMiner->DecreaseFatigue();

    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "ZZZZ... ";
  }
}

void GoHomeAndSleepTilRested::Exit(Miner* pMiner)
{
}


bool GoHomeAndSleepTilRested::OnMessage(Miner* pMiner, const Telegram& msg)
{
   SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

   switch(msg.Msg)
   {
   case Msg_StewReady:

     cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
     << " at time: " << Clock->GetCurrentTime();

     SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

     cout << "\n" << GetNameOfEntity(pMiner->ID())
          << ": Okay Hun, ahm a comin'!";

     pMiner->GetFSM()->ChangeState(EatStew::Instance());

     return true;

   }//end switch

   return false; //send message to global message handler
}

//------------------------------------------------------------------------QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
  static QuenchThirst instance;

  return &instance;
}

void QuenchThirst::Enter(Miner* pMiner)
{
  if (pMiner->Location() != saloon)
  {
    pMiner->ChangeLocation(saloon);

    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Boy, ah sure is thusty! Walking to the saloon";
  }
}

void QuenchThirst::Execute(Miner* pMiner)
{
  pMiner->BuyAndDrinkAWhiskey();

  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "That's mighty fine sippin' liquer";

  pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
}


void QuenchThirst::Exit(Miner* pMiner)
{
  SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);
//  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Need to do somethin'!";
}


bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{
  SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

   switch(msg.Msg)
   {
   case Msg_SomeoneWantToFight:

     cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
     << " at time: " << Clock->GetCurrentTime();

     SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

     cout << "\n" << GetNameOfEntity(pMiner->ID())
          << ": Damn the drunkard !";

    //message pour drunkard
    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,     //time delay
                              pMiner->ID(),             //ID of sender
                              ent_Sam,                 //ID of recipient
                              Msg_MinerHereToFight,    //the message
                              NO_ADDITIONAL_INFO);


     pMiner->GetFSM()->ChangeState(Quarrel::Instance());

     return true;

   }//end switch

   return false; //send message to global message handler
}

//------------------------------------------------------------------------EatStew

EatStew* EatStew::Instance()
{
  static EatStew instance;

  return &instance;
}


void EatStew::Enter(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Smells Reaaal goood Elsa!";
}

void EatStew::Execute(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Tastes real good too!";

  pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Thankya li'lle lady. Ah better get back to whatever ah wuz doin'";
}


bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

//--------------------------------------------------------------------------Quarrel
Quarrel* Quarrel::Instance()
{
  static Quarrel instance;

  return &instance;
}

void Quarrel::Enter(Miner* pMiner) //Enter quarrel mode with drunkard
{
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Hey, ya wanna fight me?";
}

void Quarrel::Execute(Miner* pMiner) //quarrel with drunkard
{
    if (pMiner->Fatigued())
  {
     cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": "
          << "Ooh nah ah'm tired now. Ah'm out !";

    //message pour drunkard
    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,     //time delay
                              pMiner->ID(),             //ID of sender
                              ent_Sam,                 //ID of recipient
                              Msg_MinerOut,    //the message
                              NO_ADDITIONAL_INFO);

     pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
  }


  switch(RandInt(0,2))
  {
  case 0:

    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": Shaddup you !";

    break;

  case 1:

    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": Dun' talk back !";

    break;

  case 2:

    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": Ya li'lle shit !";

    break;
  }
  //pMiner->IncreaseFatigue();
    //

}

void Quarrel::Exit(Miner* pMiner) //
{
  SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Peace finally.";
}


bool Quarrel::OnMessage(Miner* pMiner, const Telegram& msg)
{
SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

   switch(msg.Msg)
   {
   case Msg_DrunkardStopQuarrel:

     cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
     << " at time: " << Clock->GetCurrentTime();

     SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

     cout << "\n" << GetNameOfEntity(pMiner->ID())
          << ": Bye loser !";

     pMiner->GetFSM()->RevertToPreviousState();

     return true;

   }//end switch

   return false; //send message to global message handler
}


