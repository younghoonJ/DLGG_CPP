//
// Created by younghoon on 24. 2. 16.
//

#ifndef DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_INCLUDE_GAME_GOBOARD_H
#define DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_INCLUDE_GAME_GOBOARD_H
#include <algorithm>
#include <cstddef>
#include <memory>
#include <ostream>
#include <unordered_map>

#include "gostring.h"
#include "gotypes.h"
#include "zobrist.h"

namespace dlgg::game::goboard {

// ABC Board
template<class Derived>
class Board {
protected:
    std::unordered_map<gotypes::Point, gostring::GoString *, gotypes::PointHash>
        grid;
    //  we wrap gostring ptr to keep address of strings unchanged.
    std::vector<std::unique_ptr<gostring::GoString>> strings_;

public:
    std::size_t num_rows;
    std::size_t num_cols;

    virtual ~Board() = default;

    Board(size_t numRows, size_t numCols)
        : num_rows(numRows), num_cols(numCols) {}

    Board(const Board<Derived> &other)
        : num_rows(other.num_rows), num_cols(other.num_cols) {
        for (const auto &ptr : other.strings_)
            strings_.emplace_back(new gostring::GoString(*ptr));
        for (const auto &ptr : strings_) {
            for (const auto &stone : ptr->stones) {
                grid[stone] = ptr.get();
            }
        }
    }

    Derived deepcopy() const {
        return static_cast<const Derived *>(this)->deepCopyImpl();
    }

    friend std::ostream &operator<<(std::ostream &os, const Board &board) {
        for (auto row = board.num_rows; row > 0; --row) {
            os << (row <= board.num_rows ? " " : "") << std::to_string(row)
               << " ";
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

    bool operator==(const Board &rhs) const {
        return grid.size() == rhs.grid.size() and
               std::equal(grid.begin(), grid.end(), rhs.grid.begin(),
                          [&](const auto &left, const auto &right) {
                              return left.first == right.first;
                          });
    }

    bool operator!=(const Board &rhs) const { return !(rhs == *this); }

    gostring::GoString &get(const gotypes::Point &point) {
        if (auto it = grid.find(point); it != grid.end()) return *it->second;
        return const_cast<gostring::GoString &>(gostring::NullString);
    }

    const gostring::GoString &get(const gotypes::Point &point) const {
        if (auto it = grid.find(point); it != grid.end()) return *it->second;
        return gostring::NullString;
    }

    gotypes::Player getColor(const gotypes::Point &point) const {
        return get(point).color;
    }

    bool isOnGrid(const gotypes::Point &point) const {
        return 1 <= point.row and point.row <= num_rows and 1 <= point.col and
               point.col <= num_cols;
    }

    bool isPointAnEye(const game::gotypes::Point &point,
                      game::gotypes::Player color) const {
        if (get(point) != game::gostring::NullString) return false;
        for (auto &nbr : point.neighbors()) {
            if (getColor(nbr) != color) return false;
        }

        auto friendly_conners  = 0;
        auto off_board_corners = 0;
        for (const auto &corner : point.corners()) {
            if (isOnGrid(corner)) {
                if (getColor(corner) == color) friendly_conners += 1;
            } else {
                off_board_corners += 1;
            }
        }
        if (off_board_corners > 0)
            return off_board_corners + friendly_conners == 4;
        return friendly_conners >= 3;
    }

    void placeStone(gotypes::Player player, const gotypes::Point &point) {
        static_cast<Derived *>(this)->placeStoneImpl(player, point);
    }
};

class BoardZob : public Board<BoardZob> {
    zobrist::ZobHash_t hash_ = zobrist::ZobHashEmptyBoard;

public:
    ~BoardZob() override = default;
    BoardZob(size_t numRows, size_t numCols);
    BoardZob(const BoardZob &other);

private:
    void removeStringFromBoard(const gostring::GoString &goString);
    void removeStringFromStrings(const gostring::GoString &goString);

private:
    friend class Board<BoardZob>;
    void placeStoneImpl(gotypes::Player player, const gotypes::Point &point);
    BoardZob deepCopyImpl() const;
};

class BoardNaive : public Board<BoardNaive> {
public:
    ~BoardNaive() override = default;
    BoardNaive(size_t numRows, size_t numCols);

    BoardNaive(const BoardNaive &other) : Board<BoardNaive>(other) {}

private:
    void removeStringFromStrings(const gostring::GoString &goString);
    void removeStringFromBoard(const gostring::GoString &goString);

private:
    friend class Board<BoardNaive>;
    void placeStoneImpl(gotypes::Player player, const gotypes::Point &point);
    BoardNaive deepCopyImpl() const;
};


}  // namespace dlgg::game::goboard

#endif  //DLGG_CPP_DLGG_GAME_INCLUDE_DLGG_INCLUDE_GAME_GOBOARD_H
