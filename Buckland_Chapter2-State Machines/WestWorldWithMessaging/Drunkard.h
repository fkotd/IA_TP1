#ifndef DRUNKARD_H
#define DRUNKARD_H

#include "BaseGameEntity.h"
#include "Locations.h"
#include "../../Common/fsm/StateMachine.h"
#include "../../Common/misc/ConsoleUtils.h"
#include "DrunkardOwnedStates.h";

//the amount of alcohol a drunkard must have before he do odd things
const int AlcoholLevel       = 5;
//above this value a drunkard is sleepy
const int TirednessThreshold = 5;
//above this value a quarrel will stop
const int QuarrelThreshold   = 3;


class Drunkard : public BaseGameEntity
{
    public:
        Drunkard(int id):m_Location(saloon),
                            m_iAlcohol(0),
                            m_iFatigue(0),
                            m_iAnger(0),
                            BaseGameEntity(id)
        {
            m_pStateMachine = new StateMachine<Drunkard>(this);

            //m_pStateMachine->SetCurrentState(HaveADrink::Instance());
        }

        ~Drunkard(){delete m_pStateMachine;}

        //this must be implemented
        void Update();

        //so must this
        virtual bool  HandleMessage(const Telegram& msg);

        StateMachine<Drunkard>* GetFSM()const{return m_pStateMachine;}

    private:
        //an instance of the state machine class
        StateMachine<Drunkard>* m_pStateMachine;
        location_type m_Location;
        //the higher the value, the more drunk the drunkard
        int m_iAlcohol;
        //the higher the value, the more tired the drunkard
        int m_iFatigue;
        //the higher the value, the more angry the drunkard
        int m_iAnger;

};

#endif // DRUNKARD_H
