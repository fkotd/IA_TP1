#ifndef DRUNKARDOWNEDSTATES_H
#define DRUNKARDOWNEDSTATES_H

#include "../../Common/FSM/State.h"

class Drunkard;

//------------------------------------------------------------------------
//
//  In this state the drunkard will drink. If he'll get too drunk, he will
//  do odd things or if the miner is present, quarrel with him.
//------------------------------------------------------------------------
class HaveADrink : public State<Drunkard>
{
    private:

        HaveADrink(){}

        //copy ctor and assignment should be private
        HaveADrink(const HaveADrink&);
        HaveADrink& operator=(const HaveADrink&);

    public:

        //this is a singleton
        static HaveADrink* Instance();

        virtual void Enter(Drunkard* drunkard);

        virtual void Execute(Drunkard* drunkard);

        virtual void Exit(Drunkard* drunkard);

        virtual bool OnMessage(Drunkard* agent, const Telegram& msg);

};

//------------------------------------------------------------------------
//
//  Do Odd Thing will inscrease drunkard'fatigue. If he'll get too much
//  he is gonna take a nap. Else if the miner is here, he can quarrel.
//------------------------------------------------------------------------
class DoOddThings : public State<Drunkard>
{
    private:

        DoOddThings(){}

        //copy ctor and assignment should be private
        DoOddThings(const DoOddThings&);
        DoOddThings& operator=(const DoOddThings&);

    public:

        //this is a singleton
        static DoOddThings* Instance();

        virtual void Enter(Drunkard* drunkard);

        virtual void Execute(Drunkard* drunkard);

        virtual void Exit(Drunkard* drunkard);

        virtual bool OnMessage(Drunkard* agent, const Telegram& msg);

};

//------------------------------------------------------------------------
//
//  Take a nap decrease drunker's fatigue. When the level is acceptable
//  the drunkard go back to state have a drink.
//------------------------------------------------------------------------
class TakeANap : public State<Drunkard>
{
    private:

        TakeANap(){}

        //copy ctor and assignment should be private
        TakeANap(const TakeANap&);
        TakeANap& operator=(const TakeANap&);

    public:

        //this is a singleton
        static TakeANap* Instance();

        virtual void Enter(Drunkard* drunkard);

        virtual void Execute(Drunkard* drunkard);

        virtual void Exit(Drunkard* drunkard);

        virtual bool OnMessage(Drunkard* agent, const Telegram& msg);

};

//------------------------------------------------------------------------
//
//  Quarrel with miner will inscrease the anger level. If it get too high
//  drunkard will be throw out by the owner.
//------------------------------------------------------------------------
class QuarrelDrunkard : public State<Drunkard>
{
    private:

        QuarrelDrunkard(){}

        //copy ctor and assignment should be private
        QuarrelDrunkard(const QuarrelDrunkard&);
        QuarrelDrunkard& operator=(const QuarrelDrunkard&);

    public:

        //this is a singleton
        static QuarrelDrunkard* Instance();

        virtual void Enter(Drunkard* drunkard);

        virtual void Execute(Drunkard* drunkard);

        virtual void Exit(Drunkard* drunkard);

        virtual bool OnMessage(Drunkard* agent, const Telegram& msg);

};

//------------------------------------------------------------------------
//
//  The drunker rest outside. His fatigue, alcohol and anger level decrease.
//  When he is ok he come back, lean on this elbow at the bar, having another
//  drink.
//------------------------------------------------------------------------
class SoberUpOutside : public State<Drunkard>
{
    private:

        SoberUpOutside(){}

        //copy ctor and assignment should be private
        SoberUpOutside(const SoberUpOutside&);
        SoberUpOutside& operator=(const SoberUpOutside&);

    public:

        //this is a singleton
        static SoberUpOutside* Instance();

        virtual void Enter(Drunkard* drunkard);

        virtual void Execute(Drunkard* drunkard);

        virtual void Exit(Drunkard* drunkard);

        virtual bool OnMessage(Drunkard* agent, const Telegram& msg);

};

#endif // DRUNKARDOWNEDSTATES_H
