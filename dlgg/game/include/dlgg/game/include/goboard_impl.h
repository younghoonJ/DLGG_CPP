//
// Created by younghoon on 24. 2. 18.
//

#ifndef DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_GAME_INCLUDE_GOBOARD_IMPL_H
#define DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_GAME_INCLUDE_GOBOARD_IMPL_H
#include "goboard.h"

namespace dlgg::game::goboard ::impl {

class BoardNaive : public Board<BoardNaive> {
public:
    ~BoardNaive() override = default;

    BoardNaive(size_t numRows, size_t numCols)
        : Board<BoardNaive>(numRows, numCols) {}

private:
    void removeStringFromBoard(const gostring::GoString &goString);

private:
    friend class Board<BoardNaive>;

    BoardNaive deepCopyImpl() const { return {*this}; }

    void placeStoneImpl(gotypes::Player player, const gotypes::Point &point);
};

class BoardZob : public Board<BoardZob> {
    zobrist::ZobHash_t hash_ = zobrist::ZobHashEmptyBoard;

public:
    ~BoardZob() override = default;

    BoardZob(size_t numRows, size_t numCols) : Board(numRows, numCols) {}

    zobrist::ZobHash_t hash() { return hash_; }

private:
    void removeStringFromBoard(const gostring::GoString &goString);


private:
    friend class Board<BoardZob>;

    void placeStoneImpl(gotypes::Player player, const gotypes::Point &point);

    BoardZob deepCopyImpl() const { return {*this}; }
};


}  // namespace dlgg::game::goboard::impl
#endif  //DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_GAME_INCLUDE_GOBOARD_IMPL_H
