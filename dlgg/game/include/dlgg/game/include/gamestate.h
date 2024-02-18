//
// Created by younghoon on 24. 2. 17.
//

#ifndef DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_INCLUDE_GAME_GAMESTATE_H
#define DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_INCLUDE_GAME_GAMESTATE_H

#include "goboard.h"
#include "gotypes.h"

namespace dlgg::game::gamestate {
template<class GameStateImpl_t,  // crtp
         class BoardType_t>
class GameState {
    BoardType_t board_;
    gotypes::Player next_player                 = gotypes::Player::None;
    std::unique_ptr<GameStateImpl_t> prev_state = nullptr;
    gotypes::Move last_move_;

public:
    virtual ~GameState() = default;

    GameState(const BoardType_t &board, gotypes::Player nextPlayer,
              GameStateImpl_t *prevState, const gotypes::Move &lastMove)
        : last_move_(lastMove),
          next_player(nextPlayer),
          board_(board),
          prev_state(prevState) {}

    [[nodiscard]] const gotypes::Player &nextPlayer() const {
        return next_player;
    }

    [[nodiscard]] gotypes::Player &nextPlayer() { return next_player; }

    [[nodiscard]] BoardType_t &board() { return board_; }

    [[nodiscard]] const BoardType_t &board() const { return board_; }

    [[nodiscard]] gotypes::Move &lastMove() { return last_move_; }

    [[nodiscard]] const gotypes::Move &lastMove() const { return last_move_; }

    std::unique_ptr<GameStateImpl_t> &prevState() { return prev_state; }

    const std::unique_ptr<GameStateImpl_t> &prevState() const {
        return prev_state;
    }

    // crtp
    [[nodiscard]] bool isValidMove(const gotypes::Move &move) const {
        return (static_cast<const GameStateImpl_t *>(this))
            ->isValidMoveImpl(move);
    }

    [[nodiscard]] bool isOver() const {
        return (static_cast<const GameStateImpl_t *>(this))->isOverImpl();
    }

    GameStateImpl_t *applyMove(const gotypes::Move &move) {
        return (static_cast<GameStateImpl_t *>(this))->applyMoveImpl(move);
    }
};

template<class BoardType_t>
class GameStateNaive
    : public GameState<GameStateNaive<BoardType_t>, BoardType_t> {

    [[nodiscard]] bool isMoveSelfCapture(gotypes::Player player,
                                         const gotypes::Move &move) const {
        if (not move.isPlay()) return false;
        auto next_board = this->board().deepcopy();
        next_board.placeStone(player, move.point);
        return next_board.get(move.point).liberties.empty();
    }

    [[nodiscard]] bool doesMoveViolateKo(gotypes::Player player,
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


public:
    ~GameStateNaive() override = default;

    GameStateNaive(const BoardType_t &board, gotypes::Player nextPlayer,
                   GameStateNaive *prevState, const gotypes::Move &lastMove)
        : GameState<GameStateNaive<BoardType_t>, BoardType_t>(
              board, nextPlayer, prevState, lastMove) {}

    [[nodiscard]] bool isValidMoveImpl(const gotypes::Move &move) const {
        if (isOverImpl()) return false;
        if (move.isPass() or move.isResign()) return true;
        return this->board().get(move.point) == gostring::NullString and
               not isMoveSelfCapture(this->nextPlayer(), move) and
               not doesMoveViolateKo(this->nextPlayer(), move);
    }

    [[nodiscard]] bool isOverImpl() const {
        if (this->lastMove().isNone()) return false;
        if (this->lastMove().isResign()) return true;
        auto prev_last_move = this->prevState()->lastMove();
        if (prev_last_move.isNone()) return false;
        return this->lastMove().isPass() and prev_last_move.isPass();
    }

    GameStateNaive<BoardType_t> *applyMoveImpl(const gotypes::Move &move) {
        if (move.isPlay()) {
            auto next_board = this->board().deepcopy();
            next_board.placeStone(this->nextPlayer(), move.point);
            return new GameStateNaive<BoardType_t>(
                next_board, gotypes::other(this->nextPlayer()), this, move);
        }
        return new GameStateNaive(
            this->board(), gotypes::other(this->nextPlayer()), this, move);
    }

    static GameStateNaive<BoardType_t> newGame(size_t boardSize) {
        return {goboard::BoardNaive(boardSize, boardSize),
                gotypes::Player::Black, nullptr, gotypes::Move::none()};
    }
};
}  // namespace dlgg::game::gamestate
#endif  //DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_INCLUDE_GAME_GAMESTATE_H
