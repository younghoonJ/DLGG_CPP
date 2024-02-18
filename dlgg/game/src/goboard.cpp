//
// Created by younghoon on 24. 2. 16.
//
#include "dlgg/game/include/goboard.h"

#include <algorithm>
#include <stdexcept>

#include "dlgg/game/include/gostring.h"

namespace dlgg::game::goboard {
BoardNaive::BoardNaive(size_t numRows, size_t numCols)
    : Board<BoardNaive>(numRows, numCols) {}

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
    for (const auto &neighbor : point.neighbors()) {
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
    auto new_string = new gostring::GoString(player, {point}, liberties);
    strings_.emplace_back(new_string);
    for (auto same_color_string : adjacent_same_color) {
        new_string->mergeInplace(*same_color_string);
        removeStringFromStrings(*same_color_string);
    }
    for (const auto &stone : new_string->stones)
        grid[stone] = new_string;
    for (const auto other_color_string : adjacent_opposite_color) {
        other_color_string->liberties.erase(point);
        if (other_color_string->liberties.empty())
            removeStringFromBoard(*other_color_string);
    }
}

void
BoardNaive::removeStringFromStrings(const gostring::GoString &goString) {
    auto it = std::find_if(
        strings_.begin(), strings_.end(),
        [goString](const std::unique_ptr<gostring::GoString> &ptr) {
            return *ptr == goString;
        });
    if (it != strings_.end()) {
        std::swap(*it, strings_.back());
        strings_.pop_back();
    }
}

void
BoardNaive::removeStringFromBoard(const gostring::GoString &goString) {
    for (const auto &stone : goString.stones) {
        for (const auto &nbr : stone.neighbors()) {
            if (auto it = grid.find(nbr); it != grid.end()) {
                auto nbr_string = it->second;
                if (*nbr_string != goString)
                    nbr_string->liberties.emplace(stone);
            }
        }
        grid.erase(stone);
    }
    removeStringFromStrings(goString);
}

BoardNaive
BoardNaive::deepCopyImpl() const {
    return {*this};
}

std::ostream &
operator<<(std::ostream &os, const BoardNaive &board) {
    for (auto row = board.num_rows; row > 0; --row) {
        os << (row <= board.num_rows ? " " : "") << std::to_string(row) << " ";
        for (int col = 1; col < board.num_cols + 1; ++col) {
            os << game::gotypes::toChar(board.getColor(
                game::gotypes::Point(int16_t(row), int16_t(col))));
        }
        os << '\n';
    }
    os << "   ";
    for (int i = 1; i < board.num_cols + 1; ++i)
        os << gotypes::COLS[i];
    return os;
}

BoardZob::BoardZob(size_t numRows, size_t numCols) : Board(numRows, numCols) {}

void
BoardZob::removeStringFromStrings(const gostring::GoString &goString) {
    auto it = std::find_if(
        strings_.begin(), strings_.end(),
        [goString](const std::unique_ptr<gostring::GoString> &ptr) {
            return *ptr == goString;
        });
    if (it != strings_.end()) {
        std::swap(*it, strings_.back());
        strings_.pop_back();
    }
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
    for (const auto &neighbor : point.neighbors()) {
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
    auto new_string = new gostring::GoString(player, {point}, liberties);
    strings_.emplace_back(new_string);
    for (auto same_color_string : adjacent_same_color) {
        new_string->mergeInplace(*same_color_string);
        removeStringFromStrings(*same_color_string);
    }
    for (const auto &stone : new_string->stones) {
        grid[stone] = new_string;
    }

    this->hash_ ^= zobrist::zobHash(point.row, point.col, player);

    for (const auto other_color_string : adjacent_opposite_color) {
        other_color_string->liberties.erase(point);
        if (other_color_string->liberties.empty()) {
            removeStringFromBoard(*other_color_string);
        }
    }
}

void
BoardZob::removeStringFromBoard(const gostring::GoString &goString) {
    for (const auto &stone : goString.stones) {
        for (const auto &nbr : stone.neighbors()) {
            if (auto it = grid.find(nbr); it != grid.end()) {
                auto nbr_string = it->second;
                if (*nbr_string != goString)
                    nbr_string->liberties.emplace(stone);
            }
        }
        grid.erase(stone);
        hash_ ^= zobrist::zobHash(stone.row, stone.col, goString.color);
    }
    removeStringFromStrings(goString);
}

BoardZob::BoardZob(const BoardZob &other)
    : Board<BoardZob>(other.num_rows, other.num_cols) {
    hash_ = other.hash_;
    for (const auto &ptr : other.strings_)
        strings_.emplace_back(new gostring::GoString(*ptr));
    for (const auto &ptr : strings_) {
        for (const auto &stone : ptr->stones) {
            grid[stone] = ptr.get();
        }
    }
}

BoardZob
BoardZob::deepCopyImpl() const {
    return {*this};
}
}  // namespace dlgg::game::goboard
