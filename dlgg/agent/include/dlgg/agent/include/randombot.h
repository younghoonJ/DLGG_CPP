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

    std::vector<game::gotypes::Point> all_points;

public:
    RandomBot(size_t board_size, std::mt19937 *rng) : rng(rng) {
        for (size_t r = 1; r < board_size + 1; ++r) {
            for (size_t c = 1; c < board_size + 1; ++c) {
                all_points.emplace_back(r, c);
            }
        }
    }

    game::gotypes::Move seleceMoveImpl(const GameStateImpl &gameState) const {
        std::vector<game::gotypes::Point> candidates;
        for (const auto &candidate : all_points) {
            if (gameState.isValidMove(game::gotypes::Move::play(candidate)) and
                not gameState.board().isPointAnEye(candidate,
                                                   gameState.nextPlayer())) {
                candidates.push_back(candidate);
            }
        }
        if (candidates.empty()) return game::gotypes::Move::pass();
        return game::gotypes::Move::play(
            candidates[(*rng)() % candidates.size()]);
    }
};

}  // namespace dlgg::agent::bot
#endif  //DLGG_CPP_DLGG_AGENT_INCLUDE_DLGG_INCLUDE_AGENT_RANDOMBOT_H
