//
// Created by younghoon on 24. 2. 18.
//

#ifndef DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_GAME_INCLUDE_GAMESTATE_IMPL_H
#define DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_GAME_INCLUDE_GAMESTATE_IMPL_H
#include "dlgg/game/include/gamestate.h"

namespace dlgg::game::gamestate::impl {

class GameStateNaive
    : public GameState<GameStateNaive, goboard::impl::BoardNaive> {

public:
    ~GameStateNaive() override = default;

    GameStateNaive(const goboard::impl::BoardNaive &board,
                   gotypes::Player nextPlayer, GameStateNaive *prevState,
                   const gotypes::Move &lastMove);

    static GameStateNaive newGame(size_t boardSize);

private:
    friend class GameState<GameStateNaive, goboard::impl::BoardNaive>;

    [[nodiscard]] bool doesMoveViolateKoImpl(gotypes::Player player,
                                             const gotypes::Move &move) const;

    GameStateNaive *applyMoveImpl(const gotypes::Move &move);
};

class GameStateZob : public GameState<GameStateZob, goboard::impl::BoardZob> {
    struct pairhash {
    public:
        template<typename T, typename U>
        std::size_t operator()(const std::pair<T, U> &x) const {
            return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
        }
    };

    std::unordered_set<std::pair<gotypes::Player, zobrist::ZobHash_t>, pairhash>
        prev_state_cache;

public:
    ~GameStateZob() override = default;

    GameStateZob(const goboard::impl::BoardZob &board,
                 const gotypes::Player &nextPlayer, GameStateZob *prevState,
                 const gotypes::Move &lastMove);

    static GameStateZob newGame(size_t boardSize);

private:
    friend class GameState<GameStateZob, goboard::impl::BoardZob>;

    [[nodiscard]] bool doesMoveViolateKoImpl(gotypes::Player player,
                                             const gotypes::Move &move) const;

    GameStateZob *applyMoveImpl(const gotypes::Move &move);
};

}  // namespace dlgg::game::gamestate::impl
#endif  //DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_GAME_INCLUDE_GAMESTATE_IMPL_H
