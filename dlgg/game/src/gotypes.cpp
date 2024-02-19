//
// Created by younghoon on 24. 2. 16.
//
#include "dlgg/game/include/gotypes.h"

namespace dlgg::game::gotypes {


Player
other(const dlgg::game::gotypes::Player &color) {
    return isBlack(color) ? Player::White : Player::Black;
}

bool
isBlack(const Player &color) {
    return color == Player::Black;
}

std::ostream &
operator<<(std::ostream &os, const Point &point) {
    os << '(' << COLS[point.col] << ',' << point.row << ')';
    return os;
}

bool
operator==(const Point &lhs, const Point &rhs) {
    return lhs.row == rhs.row && lhs.col == rhs.col;
}

bool
operator!=(const Point &lhs, const Point &rhs) {
    return !(lhs == rhs);
}

bool
operator<(const Point &lhs, const Point &rhs) {
    if (lhs.row == rhs.row) return lhs.col < rhs.col;
    return lhs.row < rhs.row;
}

bool
operator>(const Point &lhs, const Point &rhs) {
    return rhs < lhs;
}

bool
operator<=(const Point &lhs, const Point &rhs) {
    return !(rhs < lhs);
}

bool
operator>=(const Point &lhs, const Point &rhs) {
    return !(lhs < rhs);
}

std::ostream &
operator<<(std::ostream &os, const Player &player) {
    switch (player) {
        case Black:
            return os << "Black", os;
        case White:
            return os << "White", os;
        case None:
            return os << "None", os;
    }
    return os;
}

char
toChar(const Player &color) {
    switch (color) {
        case Black:
            return 'x';
        case White:
            return 'o';
        default:
            return '.';
    }
}

Move
Move::pass() {
    return {Kind::PASS, {}};
}

Move
Move::none() {
    return {};
}

Move
Move::resign() {
    return {Kind::RESIGN, {}};
}

Move
Move::play(const Point &point) {
    return {Kind::PLAY, point};
}

bool
Move::isNone() const {
    return kind == Kind::NONE;
}

bool
Move::isPass() const {
    return kind == Kind::PASS;
}

bool
Move::isPlay() const {
    return kind == Kind::PLAY;
}

bool
Move::isResign() const {
    return kind == Kind::RESIGN;
}

std::ostream &
operator<<(std::ostream &os, const Move &move) {
    os << "kind: " << move.kind << " point: " << move.point;
    return os;
}

std::vector<Point>
getAllPoints(size_t board_size) {
    std::vector<Point> vec;
    for (size_t r = 1; r < board_size + 1; ++r) {
        for (size_t c = 1; c < board_size + 1; ++c) {
            vec.emplace_back(r, c);
        }
    }
    return vec;
}
}  // namespace dlgg::game::gotypes
