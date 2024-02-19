//
// Created by younghoon on 24. 2. 19.
//

#ifndef DLGG_CPP_DLGG_AGENT_INCLUDE_DLGG_AGENT_INCLUDE_MINMAXBOT_H
#define DLGG_CPP_DLGG_AGENT_INCLUDE_DLGG_AGENT_INCLUDE_MINMAXBOT_H
#include "agent.h"

namespace dlgg::agent::bot {
template<class GameStateImpl>
class MinMaxBot : public Agent<MinMaxBot<GameStateImpl>, GameStateImpl> {



};


}  // name1pace dlgg::agent::bot
#endif  //DLGG_CPP_DLGG_AGENT_INCLUDE_DLGG_AGENT_INCLUDE_MINMAXBOT_H
