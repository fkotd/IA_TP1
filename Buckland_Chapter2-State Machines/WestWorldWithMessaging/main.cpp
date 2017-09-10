#include <fstream>
#include <time.h>

#include "Locations.h"
#include "Miner.h"
#include "MinersWife.h"
#include "DRUNKARD.H"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "../../Common/misc/ConsoleUtils.h"
#include "EntityNames.h"


std::ofstream os;

int main()
{
//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
  os.open("output.txt");
#endif

  //seed random number generator
  srand((unsigned) time(NULL));

  //create a miner
  Miner* Bob = new Miner(ent_Miner_Bob);

  //create his wife
  MinersWife* Elsa = new MinersWife(ent_Elsa);

  //create a drunkard
  Drunkard* Sam = new Drunkard(ent_Sam);

  //register them with the entity manager
  EntityMgr->RegisterEntity(Bob);
  EntityMgr->RegisterEntity(Elsa);
  EntityMgr->RegisterEntity(Sam);

  //run Bob, Elsa and Sam through a few Update calls
  for (int i=0; i<30; ++i)
  {
    Bob->Update();
    Elsa->Update();
    Sam->Update();

    //dispatch any delayed messages
    Dispatch->DispatchDelayedMessages();

    Sleep(800);
  }

  //tidy up
  delete Bob;
  delete Elsa;
  delete Sam;

  //wait for a keypress before exiting
  PressAnyKeyToContinue();


  return 0;
}






