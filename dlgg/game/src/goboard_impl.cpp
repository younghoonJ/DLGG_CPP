//
// Created by younghoon on 24. 2. 18.
//
#include "dlgg/game/include/goboard_impl.h"

namespace dlgg::game::goboard ::impl {


void
BoardNaive::removeStringFromBoard(const gostring::GoString &goString) {
    for (const auto &stone : goString.stones) {
        for (const auto &nbr : neighbors(stone)) {
            if (auto it = grid.find(nbr); it != grid.end()) {
                auto &nbr_string = *it->second;
                if (nbr_string != goString) nbr_string.liberties.emplace(stone);
            }
        }
        grid.erase(stone);
    }
    dropString_(goString);
}

void
BoardNaive::placeStoneImpl(gotypes::Player player,
                           const gotypes::Point &point) {
    if (not isOnGrid(point)) throw std::runtime_error("point is not on grid.");
    if (grid.find(point) != grid.end())
        throw std::runtime_error("there is a stone.");

    auto pushIfNotExist = [](std::vector<gostring::GoString *> &vec,
                             gostring::GoString &item) {
        if (std::find_if(vec.begin(), vec.end(), [&](gostring::GoString *s) {
                return *s == item;
            }) == vec.end()) {
            vec.push_back(&item);
        }
    };

    std::vector<gostring::GoString *> adjacent_same_color;
    std::vector<gostring::GoString *> adjacent_opposite_color;
    std::unordered_set<gotypes::Point, gotypes::PointHash> liberties;
    for (const auto &neighbor : neighbors(point)) {
        if (not isOnGrid(point)) continue;
        auto &neighbor_string = get(neighbor);
        if (neighbor_string == gostring::NullString) {
            liberties.emplace(neighbor);
        } else if (neighbor_string.color == player) {
            pushIfNotExist(adjacent_same_color, neighbor_string);
        } else {
            pushIfNotExist(adjacent_opposite_color, neighbor_string);
        }
    }
    //    update same color string
    auto new_string = new gostring::GoString(player, {point}, liberties);
    strings_.emplace_back(new_string);
    for (auto same_color_string : adjacent_same_color) {
        new_string->mergeInplace(*same_color_string);
        dropString_(*same_color_string);
    }
    for (const auto &stone : new_string->stones)
        grid[stone] = new_string;

    //    update opposite color string
    for (const auto other_color_string : adjacent_opposite_color) {
        other_color_string->liberties.erase(point);
        if (other_color_string->liberties.empty())
            removeStringFromBoard(*other_color_string);
    }
}

void
BoardZob::removeStringFromBoard(const gostring::GoString &goString) {
    for (const auto &stone : goString.stones) {
        for (const auto &nbr : neighbors(stone)) {
            if (auto it = grid.find(nbr); it != grid.end()) {
                auto &nbr_string = *it->second;
                if (nbr_string != goString) nbr_string.liberties.emplace(stone);
            }
        }
        grid.erase(stone);
        hash_ ^= zobrist::zobHash(stone.row, stone.col, goString.color);
    }
    dropString_(goString);
}

void
BoardZob::placeStoneImpl(gotypes::Player player, const gotypes::Point &point) {
    if (not isOnGrid(point)) throw std::runtime_error("point is not on grid.");
    if (grid.find(point) != grid.end())
        throw std::runtime_error("there is a stone.");

    auto pushIfNotExist = [](std::vector<gostring::GoString *> &vec,
                             gostring::GoString &item) {
        if (std::find_if(vec.begin(), vec.end(), [&](gostring::GoString *s) {
                return *s == item;
            }) == vec.end()) {
            vec.push_back(&item);
        }
    };

    std::vector<gostring::GoString *> adjacent_same_color;
    std::vector<gostring::GoString *> adjacent_opposite_color;
    std::unordered_set<gotypes::Point, gotypes::PointHash> liberties;
    for (const auto &neighbor : neighbors(point)) {
        if (not isOnGrid(point)) continue;
        auto &neighbor_string = get(neighbor);
        if (neighbor_string == gostring::NullString) {
            liberties.emplace(neighbor);
        } else if (neighbor_string.color == player) {
            pushIfNotExist(adjacent_same_color, neighbor_string);
        } else {
            pushIfNotExist(adjacent_opposite_color, neighbor_string);
        }
    }
    //    update same color string
    auto new_string = new gostring::GoString(player, {point}, liberties);
    strings_.emplace_back(new_string);
    for (auto same_color_string : adjacent_same_color) {
        new_string->mergeInplace(*same_color_string);
        dropString_(*same_color_string);
    }
    for (const auto &stone : new_string->stones)
        grid[stone] = new_string;

    this->hash_ ^= zobrist::zobHash(point.row, point.col, player);

    //    update opposite color string
    for (const auto other_color_string : adjacent_opposite_color) {
        other_color_string->liberties.erase(point);
        if (other_color_string->liberties.empty())
            removeStringFromBoard(*other_color_string);
    }
}
}  // namespace dlgg::game::goboard::impl