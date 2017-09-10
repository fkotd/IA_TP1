#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
  Msg_HiHoneyImHome,
  Msg_StewReady,
  Msg_SomeoneWantToFight,
  Msg_MinerHereToFight,
};


inline std::string MsgToStr(int msg)
{
  switch (msg)
  {
  case Msg_HiHoneyImHome:

    return "HiHoneyImHome";

  case Msg_StewReady:

    return "StewReady";

  case Msg_SomeoneWantToFight:

    return "SomeoneWantToFight";

  case Msg_MinerHereToFight:

    return "MinerHereToFight";

  default:

    return "Not recognized!";
  }
}

#endif
