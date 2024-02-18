//
// Created by younghoon on 24. 2. 16.
//

#ifndef DLGG_CPP_DLGG_INCLUDE_DLGG_INCLUDE_GOTYPES_H
#define DLGG_CPP_DLGG_INCLUDE_DLGG_INCLUDE_GOTYPES_H

#include <array>
#include <cstdint>
#include <ostream>
#include <variant>

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
    Point(int16_t row, int16_t col);

    [[nodiscard]] std::array<Point, 4> neighbors() const;
    [[nodiscard]] std::array<Point, 4> corners() const;
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
