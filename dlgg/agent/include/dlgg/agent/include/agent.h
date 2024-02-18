//
// Created by younghoon on 24. 2. 17.
//

#ifndef DLGG_CPP_DLGG_AGENT_INCLUDE_DLGG_INCLUDE_AGENT_AGENT_H
#define DLGG_CPP_DLGG_AGENT_INCLUDE_DLGG_INCLUDE_AGENT_AGENT_H
#include "dlgg/game/include/gamestate.h"
#include "dlgg/game/include/gotypes.h"

namespace dlgg::agent {

template<class Derived, class GameStateImpl>
class Agent {
public:
    game::gotypes::Move selectMove(const GameStateImpl &gameState) const {
        return (static_cast<const Derived *>(this))->seleceMoveImpl(gameState);
    };
};
}  // namespace dlgg::agent
#endif  //DLGG_CPP_DLGG_AGENT_INCLUDE_DLGG_INCLUDE_AGENT_AGENT_H
