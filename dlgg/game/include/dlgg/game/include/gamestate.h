//
// Created by younghoon on 24. 2. 17.
//

#ifndef DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_INCLUDE_GAME_GAMESTATE_H
#define DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_INCLUDE_GAME_GAMESTATE_H

#include "goboard_impl.h"
#include "gotypes.h"

namespace dlgg::game::gamestate {
template<class GameStateImpl_t, class BoardType_t>
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

    [[nodiscard]] bool isOver() const {
        if (last_move_.isResign()) return true;
        if (last_move_.isNone()) return false;
        if (prev_state->last_move_.isNone()) return false;
        return last_move_.isPass() and prev_state->last_move_.isPass();
    }

    [[nodiscard]] bool isMoveSelfCapture(gotypes::Player player,
                                         const gotypes::Move &move) const {
        if (not move.isPlay()) return false;
        auto next_board = this->board().deepcopy();
        next_board.placeStone(player, move.point);
        return next_board.get(move.point).liberties.empty();
    }

    [[nodiscard]] bool isValidMove(const gotypes::Move &move) const {
        if (isOver()) return false;
        if (move.isPass() or move.isResign()) return true;
        return board_.get(move.point) == gostring::NullString and
               not isMoveSelfCapture(this->nextPlayer(), move) and
               not doesMoveViolateKo(this->nextPlayer(), move);
    }

    // crtp

    GameStateImpl_t *applyMove(const gotypes::Move &move) {
        return (static_cast<GameStateImpl_t *>(this))->applyMoveImpl(move);
    }

    [[nodiscard]] bool doesMoveViolateKo(gotypes::Player player,
                                         const gotypes::Move &move) const {
        return (static_cast<const GameStateImpl_t *>(this))
            ->doesMoveViolateKoImpl(player, move);
    }
};

}  // namespace dlgg::game::gamestate
#endif  //DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_INCLUDE_GAME_GAMESTATE_H
