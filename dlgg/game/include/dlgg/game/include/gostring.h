//
// Created by younghoon on 24. 2. 16.
//

#ifndef DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_GAME_INCLUDE_GOBOARD_H
#define DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_GAME_INCLUDE_GOBOARD_H
#include <unordered_set>
#include <vector>

#include "gotypes.h"

namespace dlgg::game::gostring {

class GoString {
public:
    gotypes::Player color;
    std::unordered_set<gotypes::Point, gotypes::PointHash> stones;
    std::unordered_set<gotypes::Point, gotypes::PointHash> liberties;

    explicit GoString(gotypes::Player color) : color(color){};
    GoString(
        gotypes::Player color,
        const std::unordered_set<gotypes::Point, gotypes::PointHash> &stones,
        const std::unordered_set<gotypes::Point, gotypes::PointHash>
            &liberties);
    
    bool operator==(const GoString &rhs) const;
    bool operator!=(const GoString &rhs) const;

    void mergeInplace(const GoString &other);
};

const auto NullString = GoString(gotypes::Player::None);
}  // namespace dlgg::game::gostring
#endif  //DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_GAME_INCLUDE_GOBOARD_H
