#include <chrono>
#include <iostream>
#include <thread>

#include "dlgg/agent/include/randombot.h"
#include "dlgg/game/include/gamestate_impl.h"
#include "dlgg/game/include/gotypes.h"

void
printMove(dlgg::game::gotypes::Player player,
          const dlgg::game::gotypes::Move &move) {
    if (move.isPass()) {
        std::cout << player << ' ' << "passes\n";
    } else if (move.isResign()) {
        std::cout << player << ' ' << "resigns\n";
    } else if (move.isPlay()) {
        std::cout << player << ' ' << move.point << '\n';
    } else {
        throw std::runtime_error("code should not be reached!");
    }
}

template<class T>
void
printBoard(const T &board) {
    std::cout << board << '\n';
}

auto
getRng(long seed = -1) {
    if (seed == -1) {
        seed = std::random_device()() ^
               std::chrono::duration_cast<std::chrono::seconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count() ^
               std::chrono::duration_cast<std::chrono::microseconds>(
                   std::chrono::high_resolution_clock::now().time_since_epoch())
                   .count();
    }
    return std::mt19937(seed);
}

template<class GameStateImpl>
void
run_(GameStateImpl &gameState, size_t board_size,
     dlgg::agent::bot::RandomBot<GameStateImpl> &black_bot,
     dlgg::agent::bot::RandomBot<GameStateImpl> &white_bot) {
    auto game_ptr    = &gameState;
    auto point_cache = dlgg::game::gotypes::getAllPoints(board_size);
    black_bot.setPointCache(&point_cache);
    white_bot.setPointCache(&point_cache);
    while (not game_ptr->isOver()) {
        std::cout << "\033[2J\033[1;1H";
        //        printBoard(game_ptr->board());
        auto next_move = [&] {
            if (dlgg::game::gotypes::isBlack(game_ptr->nextPlayer()))
                return black_bot.selectMove(*game_ptr);
            else {
                return white_bot.selectMove(*game_ptr);
            }
        }();
        //        printMove(game_ptr->nextPlayer(), next_move);
        game_ptr = game_ptr->applyMove(next_move);
    }
}

void
run_naive(size_t board_size = 19, long seed = -1) {
    using gameState_T = dlgg::game::gamestate::impl::GameStateNaive;
    auto rng          = getRng(seed);
    auto game_        = gameState_T::newGame(board_size);
    auto black_bot    = dlgg::agent::bot::RandomBot<gameState_T>(&rng);
    auto white_bot    = dlgg::agent::bot::RandomBot<gameState_T>(&rng);
    run_(game_, board_size, black_bot, white_bot);
}

void
run_zob(size_t board_size = 19, long seed = -1) {
    using gameState_T = dlgg::game::gamestate::impl::GameStateZob;
    auto rng          = getRng(seed);
    auto game_        = gameState_T::newGame(board_size);
    auto black_bot    = dlgg::agent::bot::RandomBot<gameState_T>(&rng);
    auto white_bot    = dlgg::agent::bot::RandomBot<gameState_T>(&rng);
    run_(game_, board_size, black_bot, white_bot);
}

auto pointFromCoords = [](const std::string &s) {
    for (int col = 1; col < 22; ++col) {
        if (dlgg::game::gotypes::COLS[col] == toupper(s[0])) {
            try {
                int row = std::stoi(s.substr(1));
                return dlgg::game::gotypes::Point{static_cast<int16_t>(row),
                                                  static_cast<int16_t>(col)};
            } catch (std::invalid_argument &expn) {
                return dlgg::game::gotypes::Point{-1, -1};
            }
        }
    }
    return dlgg::game::gotypes::Point{-1, -1};
};

void
user_vs_bot(size_t board_size = 9, long seed = -1) {
    using gameState_T = dlgg::game::gamestate::impl::GameStateNaive;

    auto rng         = getRng(seed);
    auto game_       = gameState_T::newGame(board_size);
    auto game_ptr    = &game_;
    auto bot         = dlgg::agent::bot::RandomBot<gameState_T>(&rng);
    auto point_cache = dlgg::game::gotypes::getAllPoints(board_size);
    bot.setPointCache(&point_cache);

    std::string human_in;
    dlgg::game::gotypes::Move next_move;
    while (not game_ptr->isOver()) {
        std::cout << "\x1B[2J\x1B[H";
        printBoard(game_ptr->board());
        if (dlgg::game::gotypes::isBlack(game_ptr->nextPlayer())) {
            while (true) {
                std::cout << "your move:";
                std::cin >> human_in;
                next_move =
                    dlgg::game::gotypes::Move::play(pointFromCoords(human_in));
                auto it = std::find_if(
                    point_cache.begin(), point_cache.end(),
                    [next_move](const dlgg::game::gotypes::Point &p) {
                        return p.col == next_move.point.col and
                               p.row == next_move.point.row;
                    });
                if (it == point_cache.end()) {
                    std::cout << "wrong input.";
                } else {
                    std::swap(*it, point_cache.back());
                    point_cache.pop_back();
                    break;
                }
            }
        } else {
            next_move = bot.selectMove(*game_ptr);
        }
        printMove(game_ptr->nextPlayer(), next_move);
        game_ptr = game_ptr->applyMove(next_move);
    }
}

int
main() {
    int loop_cnt         = 1;
    int boardSize        = 19;
    auto t_measure_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < loop_cnt; ++i) {
        run_naive(boardSize, i);
    }
    auto duration0 =
        static_cast<double>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - t_measure_start)
                .count()) /
        1000000.0;

    t_measure_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < loop_cnt; ++i) {
        run_zob(boardSize, i);
    }
    auto duration1 =
        static_cast<double>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - t_measure_start)
                .count()) /
        1000000.0;

    std::cout << "0: " << duration0 / loop_cnt << " mils" << std::endl;
    std::cout << "1: " << duration1 / loop_cnt << " mils" << std::endl;

    //    user_vs_bot(9, 1);

    return 0;
}
