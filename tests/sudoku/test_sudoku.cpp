#include "sudoku/sudoku_solver.h"
#include "../test_utils.h"
#include <iostream>
#include <chrono>


bool test_easy4_sudoku() {    
    
    using Board4 = std::array<std::array<int, 4>, 4>;
    Board4 board4 = { {
        {{1, 2, 3, 4}},
        {{3, 4, 1, 2}},
        {{2, 1, 4, 3}},
        {{4, 3, 2, 1}}
    } };

    SudokuSolver<4> solver(board4);

    auto start = std::chrono::high_resolution_clock::now();
    auto result = solver.solve();
    auto end = std::chrono::high_resolution_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Time: " << ns << " us\n";

    ASSERT_TRUE(result.has_value());

    const auto& solved = result.value();

    ASSERT_TRUE(is_valid_sudoku(board4));

    return true;
}

bool test_easy9_sudoku() {
    using Board = std::array<std::array<int, 9>, 9>;

    Board board = { {
        {{5, 3, 0, 0, 7, 0, 0, 0, 0}},
        {{6, 0, 0, 1, 9, 5, 0, 0, 0}},
        {{0, 9, 8, 0, 0, 0, 0, 6, 0}},
        {{8, 0, 0, 0, 6, 0, 0, 0, 3}},
        {{4, 0, 0, 8, 0, 3, 0, 0, 1}},
        {{7, 0, 0, 0, 2, 0, 0, 0, 6}},
        {{0, 6, 0, 0, 0, 0, 2, 8, 0}},
        {{0, 0, 0, 4, 1, 9, 0, 0, 5}},
        {{0, 0, 0, 0, 8, 0, 0, 7, 9}}
    } };

    SudokuSolver<9> solver(board);

    auto start = std::chrono::high_resolution_clock::now();
    auto result = solver.solve();
    auto end = std::chrono::high_resolution_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Time: " << ns << " us\n";

    ASSERT_TRUE(result.has_value());

    const auto& solved = result.value();
    
    ASSERT_TRUE(is_valid_sudoku(board));

    return true;
}

bool test_Escargot_sudoku() {
    using Board = std::array<std::array<int, 9>, 9>;

    Board board = { {
    {{1, 0, 0, 0, 0, 7, 0, 9, 0}},
    {{0, 3, 0, 0, 2, 0, 0, 0, 8}},
    {{0, 0, 9, 6, 0, 0, 5, 0, 0}},
    {{0, 0, 5, 3, 0, 0, 9, 0, 0}},
    {{0, 1, 0, 0, 8, 0, 0, 0, 2}},
    {{6, 0, 0, 0, 0, 4, 0, 0, 0}},
    {{3, 0, 0, 0, 0, 0, 0, 1, 0}},
    {{0, 4, 0, 0, 0, 0, 0, 0, 7}},
    {{0, 0, 7, 0, 0, 0, 3, 0, 0}}
    } };

    SudokuSolver<9> solver(board);

    auto start = std::chrono::high_resolution_clock::now();
    auto result = solver.solve();
    auto end = std::chrono::high_resolution_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Time: " << ns << " us\n";

    ASSERT_TRUE(result.has_value());

    const auto& solved = result.value();

    ASSERT_TRUE(is_valid_sudoku(board));

    return true;
}
//------------ Негативные тесты
bool test_4x4_no_solution() { //нет решения
    
    using Board = std::array<std::array<int, 4>, 4>;
    Board board4 = { {
    {{1, 0, 0, 2}},
    {{0, 0, 4, 0}},
    {{0, 2, 0, 0}},
    {{3, 0, 0, 1}}
    } };

    SudokuSolver<4> solver(board4);  // твой пример
    auto result = solver.solve();
    ASSERT_FALSE(result.has_value());
    ASSERT_EQ(result.error(), SudokuSolver<4>::SolverError::NO_SOLUTION);
    return true;
}

bool test_invalid_sudoku() { // неверные входные данные
    using Board = std::array<std::array<int, 9>, 9>;

    Board board = { {
        {{5, 5, 0, 0, 7, 0, 0, 0, 0}},  // две 5 в первой строке
        // ... остальное не важно
    } };

    SudokuSolver<9> solver(board);
    auto result = solver.solve();

    ASSERT_FALSE(result.has_value());
    ASSERT_EQ(result.error(), SudokuSolver<9>::SolverError::INVALID_CONSTRAINTS);

    return true;
}

int main() {
    TestRunner runner;
    runner.addTest("Easy Sudoku 4x4", test_easy4_sudoku);
    runner.addTest("Easy Sudoku 9x9", test_easy9_sudoku);
    runner.addTest("Al Escargot Sudoku 9x9", test_Escargot_sudoku);
    runner.addTest("No solution Sudoku", test_4x4_no_solution);
    runner.addTest("Invalid Sudoku", test_invalid_sudoku);

    return runner.runAll() ? 0 : 1;
}