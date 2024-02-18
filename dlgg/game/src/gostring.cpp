//
// Created by younghoon on 24. 2. 16.
//
#include "dlgg/game/include/gostring.h"

#include <stdexcept>

namespace dlgg::game::gostring {


void
GoString::mergeInplace(const GoString &other) {
    if (color != other.color) throw std::runtime_error("color != other.color");
    for (auto &item : other.stones)
        stones.insert(item);
    for (auto &item : other.liberties)
        liberties.insert(item);
    for (auto &item : stones) {
        if (liberties.find(item) != liberties.end()) {
            liberties.erase(item);
        }
    }
}

GoString::GoString(
    gotypes::Player color,
    const std::unordered_set<gotypes::Point, gotypes::PointHash> &stones,
    const std::unordered_set<gotypes::Point, gotypes::PointHash> &liberties)
    : color(color), stones(stones), liberties(liberties) {}

bool
GoString::operator==(const GoString &rhs) const {
    return color == rhs.color && stones == rhs.stones &&
           liberties == rhs.liberties;
}

bool
GoString::operator!=(const GoString &rhs) const {
    return !(rhs == *this);
}


}  // namespace dlgg::game::gostring
