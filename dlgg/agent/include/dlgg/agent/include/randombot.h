//
// Created by younghoon on 24. 2. 17.
//

#ifndef DLGG_CPP_DLGG_AGENT_INCLUDE_DLGG_INCLUDE_AGENT_RANDOMBOT_H
#define DLGG_CPP_DLGG_AGENT_INCLUDE_DLGG_INCLUDE_AGENT_RANDOMBOT_H
#include <chrono>
#include <random>

#include "agent.h"
#include "dlgg/game/include/gotypes.h"

namespace dlgg::agent::bot {

template<class GameStateImpl>
class RandomBot : public Agent<RandomBot<GameStateImpl>, GameStateImpl> {
    std::mt19937 *rng;

    std::vector<game::gotypes::Point> *points_cache{};

public:
    explicit RandomBot(std::mt19937 *rng) : rng(rng) {}

    void setPointCache(std::vector<game::gotypes::Point> *points_cache_) {
        points_cache = points_cache_;
    }

    game::gotypes::Move seleceMoveImpl(const GameStateImpl &gameState) const {
        std::vector<int> candidates;
        for (auto idx = 0; idx < points_cache->size(); ++idx) {
            const auto &candidate = points_cache->at(idx);
            if (gameState.isValidMove(game::gotypes::Move::play(candidate)) and
                not gameState.board().isPointAnEye(candidate,
                                                   gameState.nextPlayer())) {
                candidates.push_back(idx);
            }
        }
        if (candidates.empty()) return game::gotypes::Move::pass();
        auto sel_idx = candidates[(*rng)() % candidates.size()];
        std::swap(points_cache->at(sel_idx), points_cache->back());
        auto pt = points_cache->back();
        points_cache->pop_back();
        return game::gotypes::Move::play(pt);
    }
};

}  // namespace dlgg::agent::bot
#endif  //DLGG_CPP_DLGG_AGENT_INCLUDE_DLGG_INCLUDE_AGENT_RANDOMBOT_H
