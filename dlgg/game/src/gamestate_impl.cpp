//
// Created by younghoon on 24. 2. 18.
//
#include "dlgg/game/include/gamestate_impl.h"

namespace dlgg::game::gamestate::impl {


GameStateNaive::GameStateNaive(const goboard::impl::BoardNaive &board,
                               const gotypes::Player nextPlayer,
                               GameStateNaive *prevState,
                               const gotypes::Move &lastMove)
    : GameState<GameStateNaive, goboard::impl::BoardNaive>(
          board, nextPlayer, prevState, lastMove) {}

GameStateNaive
GameStateNaive::newGame(size_t boardSize) {
    return {goboard::impl::BoardNaive(boardSize, boardSize),
            gotypes::Player::Black, nullptr, gotypes::Move::none()};
}

bool
GameStateNaive::doesMoveViolateKoImpl(gotypes::Player player,
                                      const gotypes::Move &move) const {
    if (not move.isPlay()) return false;
    auto next_board = this->board().deepcopy();
    next_board.placeStone(player, move.point);
    auto next_siutation_player = gotypes::other(player);
    auto *past_state           = this->prevState().get();
    while (past_state) {
        if (past_state->nextPlayer() == next_siutation_player and
            past_state->board() == next_board) {
            return true;
        }
        past_state = past_state->prevState().get();
    }
    return false;
}

GameStateNaive *
GameStateNaive::applyMoveImpl(const gotypes::Move &move) {
    if (move.isPlay()) {
        auto next_board = this->board().deepcopy();
        next_board.placeStone(this->nextPlayer(), move.point);
        return new GameStateNaive(
            next_board, gotypes::other(this->nextPlayer()), this, move);
    }
    return new GameStateNaive(this->board(), gotypes::other(this->nextPlayer()),
                              this, move);
}

GameStateZob::GameStateZob(const goboard::impl::BoardZob &board,
                           const gotypes::Player &nextPlayer,
                           GameStateZob *prevState,
                           const gotypes::Move &lastMove)
    : GameState<GameStateZob, goboard::impl::BoardZob>(board, nextPlayer,
                                                       prevState, lastMove) {
    if (prevState != nullptr) {
        prev_state_cache.insert(prevState->prev_state_cache.begin(),
                                prevState->prev_state_cache.end());
        prev_state_cache.emplace(prevState->nextPlayer(),
                                 prevState->board().hash());
    }
}

GameStateZob
GameStateZob::newGame(size_t boardSize) {
    return {goboard::impl::BoardZob(boardSize, boardSize),
            gotypes::Player::Black, nullptr, gotypes::Move::none()};
}

bool
GameStateZob::doesMoveViolateKoImpl(gotypes::Player player,
                                    const gotypes::Move &move) const {
    if (not move.isPlay()) return false;
    auto next_board = this->board().deepcopy();
    next_board.placeStone(player, move.point);
    auto next_situation =
        std::make_pair(gotypes::other(player), next_board.hash());
    return prev_state_cache.find(next_situation) != prev_state_cache.end();
}

GameStateZob *
GameStateZob::applyMoveImpl(const gotypes::Move &move) {
    if (move.isPlay()) {
        auto next_board = this->board().deepcopy();
        next_board.placeStone(this->nextPlayer(), move.point);
        return new GameStateZob(
            next_board, gotypes::other(this->nextPlayer()), this, move);
    }
    return new GameStateZob(this->board(),
                            gotypes::other(this->nextPlayer()), this, move);
}
}  // namespace dlgg::game::gamestate::impl