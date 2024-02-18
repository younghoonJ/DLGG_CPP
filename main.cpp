#include <chrono>
#include <ctime>
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

void
run(size_t board_size = 19, long seed = -1) {
    using gameState_T = dlgg::game::gamestate::impl::GameStateNaive;

    if (seed == -1) {
        seed = std::random_device()() ^
               std::chrono::duration_cast<std::chrono::seconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count() ^
               std::chrono::duration_cast<std::chrono::microseconds>(
                   std::chrono::high_resolution_clock::now().time_since_epoch())
                   .count();
    }
    auto rng = std::mt19937(seed);

    auto game_     = gameState_T::newGame(board_size);
    auto game_ptr  = &game_;
    auto black_bot = dlgg::agent::bot::RandomBot<gameState_T>(board_size, &rng);
    auto white_bot = dlgg::agent::bot::RandomBot<gameState_T>(board_size, &rng);

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
run_zob(size_t board_size = 19, long seed = -1) {
    using gameState_T = dlgg::game::gamestate::impl::GameStateZob;

    if (seed == -1) {
        seed = std::random_device()() ^
               std::chrono::duration_cast<std::chrono::seconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count() ^
               std::chrono::duration_cast<std::chrono::microseconds>(
                   std::chrono::high_resolution_clock::now().time_since_epoch())
                   .count();
    }
    auto rng = std::mt19937(seed);

    auto game_     = gameState_T::newGame(board_size);
    auto game_ptr  = &game_;
    auto black_bot = dlgg::agent::bot::RandomBot<gameState_T>(board_size, &rng);
    auto white_bot = dlgg::agent::bot::RandomBot<gameState_T>(board_size, &rng);

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

int
main() {
    int loop_cnt         = 10;
    auto t_measure_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < loop_cnt; ++i) {
        run(19, i);
    }
    auto duration0 =
        static_cast<double>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - t_measure_start)
                .count()) /
        1000000.0;

    t_measure_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < loop_cnt; ++i) {
        run_zob(19, i);
    }
    auto duration1 =
        static_cast<double>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - t_measure_start)
                .count()) /
        1000000.0;

    std::cout << "0: " << duration0 / loop_cnt << " mils" << std::endl;
    std::cout << "1: " << duration1 / loop_cnt << " mils" << std::endl;

    return 0;
}
