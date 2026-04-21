#define _CRTDBG_MAP_ALLOC
#define DEBUG
#include <crtdbg.h>
#include "dancing_links/dancing_links_matrix.h"
#include <iostream>
#include "sudoku/sudoku_solver.h"

void printBoard(const std::array<std::array<int, 9>, 9>& board) {
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            std::cout << board[r][c] << " ";
            if (c == 2 || c == 5) std::cout << "| ";
        }
        std::cout << "\n";
        if (r == 2 || r == 5) {
            std::cout << "------+-------+------\n";
        }
    }
}

int main() {
    setlocale(LC_ALL, "russian");

    using Board = std::array<std::array<int, 9>, 9>;

    Board board = { {
        {{4, 0, 0, 5, 0, 3, 0, 0, 6}},
        {{0, 0, 6, 0, 8, 0, 2, 0, 0}},
        {{0, 0, 0, 2, 0, 4, 0, 0, 0}},
        {{0, 2, 0, 0, 0, 0, 0, 6, 0}},
        {{0, 1, 0, 0, 0, 0, 0, 9, 0}},
        {{0, 4, 0, 0, 0, 0, 0, 1, 0}},
        {{0, 0, 0, 3, 0, 2, 0, 0, 0}},
        {{0, 0, 4, 0, 9, 0, 6, 0, 0}},
        {{8, 0, 0, 6, 0, 7, 0, 0, 1}}
    } };

    std::cout << "=== Исходная доска ===\n";
    printBoard(board);

    SudokuSolver<9> solver(board);
    auto result = solver.solve();

    if (result.has_value()) {
        std::cout << "\n=== Решение ===\n";
        printBoard(result.value());
    }
    else {
        std::cout << "\nОшибка: "
            << (result.error() == SudokuSolver<9>::SolverError::INVALID_CONSTRAINTS
                ? "некорректные начальные данные"
                : "решения не существует")
            << "\n";
    }

    return 0;
    
    
}