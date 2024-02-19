//
// Created by younghoon on 24. 2. 16.
//

#ifndef DLGG_CPP_DLGG_INCLUDE_DLGG_INCLUDE_GOTYPES_H
#define DLGG_CPP_DLGG_INCLUDE_DLGG_INCLUDE_GOTYPES_H

#include <array>
#include <cstdint>
#include <ostream>
#include <variant>
#include <vector>

namespace dlgg::game::gotypes {
constexpr char COLS[22] = "%ABCDEFGHIJKLMNOPQRST";

enum Player : int { Black = 0, White = 1, None = 2 };

std::ostream &operator<<(std::ostream &os, const Player &player);
bool isBlack(const Player &color);
Player other(const Player &color);
char toChar(const Player &color);

class Point {
public:
    int16_t row = 0;
    int16_t col = 0;

    Point() = default;

    constexpr Point(int16_t row, int16_t col) : row(row), col(col) {}

    friend std::ostream &operator<<(std::ostream &os, const Point &point);
};

bool operator==(const Point &lhs, const Point &rhs);
bool operator!=(const Point &lhs, const Point &rhs);
bool operator<(const Point &lhs, const Point &rhs);
bool operator>(const Point &lhs, const Point &rhs);
bool operator<=(const Point &lhs, const Point &rhs);
bool operator>=(const Point &lhs, const Point &rhs);

struct PointHash {
    auto operator()(const Point &point) const -> std::size_t {
        return std::hash<int16_t>{}(point.row) ^
               std::hash<int16_t>{}(point.col);
    }
};

struct PreComputeCache {
    constexpr PreComputeCache() {
        for (int16_t row = 1; row < 20; ++row) {
            for (int16_t col = 1; col < 20; ++col) {
                neighbors[row][col] = {gotypes::Point{int16_t(row - 1), col},
                                       gotypes::Point{int16_t(row + 1), col},
                                       gotypes::Point{row, int16_t(col - 1)},
                                       gotypes::Point{row, int16_t(col + 1)}};
                corners[row][col]   = {
                    gotypes::Point{int16_t(row - 1), int16_t(col - 1)},
                    gotypes::Point{int16_t(row - 1), int16_t(col + 1)},
                    gotypes::Point{int16_t(row + 1), int16_t(col - 1)},
                    gotypes::Point{int16_t(row + 1), int16_t(col + 1)},
                };
            }
        }
    };

    std::array<gotypes::Point, 4> corners[20][20];
    std::array<gotypes::Point, 4> neighbors[20][20];
};

constexpr auto PointCache = PreComputeCache();

std::vector<Point>
getAllPoints(size_t board_size);

class Move {
public:
    enum Kind {
        NONE   = 0,
        PASS   = 1,
        RESIGN = 2,
        PLAY   = 3,
    };

    Kind kind = Kind::NONE;
    Point point{};
    Move() = default;

    Move(Kind kind, const Point &point) : kind(kind), point(point) {}

    friend std::ostream &operator<<(std::ostream &os, const Move &move);

    [[nodiscard]] bool isNone() const;
    [[nodiscard]] bool isPass() const;
    [[nodiscard]] bool isPlay() const;
    [[nodiscard]] bool isResign() const;

    static Move none();
    static Move pass();
    static Move resign();
    static Move play(const Point &point);
};

}  // namespace dlgg::game::gotypes
#endif  //DLGG_CPP_DLGG_INCLUDE_DLGG_INCLUDE_GOTYPES_H
