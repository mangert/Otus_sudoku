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

    ASSERT_TRUE(is_valid_sudoku(solved));

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
    
    ASSERT_TRUE(is_valid_sudoku(solved));

    return true;
}

bool test_easy9_sudoku_cells() {
    using Board = std::array<std::array<int, 9>, 9>;
    using FixedCell = std::tuple<int, int, int>;  // row, col, digit
    std::vector<FixedCell> cells = { {0,1,5}, {0,3,8}, {0,4,1}, {0,7,4},
    {1,2,3}, {1,4,7}, {1,6,9}, {2,3,9}, {2,5,5}, {3,0,9}, {3,2,5}, {3,6,8}, {3,8,6},
    {4,1,2} };

    SudokuSolver<9> solver(cells);

    auto start = std::chrono::high_resolution_clock::now();
    auto result = solver.solve();
    auto end = std::chrono::high_resolution_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Time: " << ns << " us\n";

    ASSERT_TRUE(result.has_value());

    const auto& solved = result.value();

    ASSERT_TRUE(is_valid_sudoku(solved));

    return true;
}

bool test_medium9_sudoku() {
    using Board = std::array<std::array<int, 9>, 9>;

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
    SudokuSolver<9> solver(board);

    auto start = std::chrono::high_resolution_clock::now();
    auto result = solver.solve();
    auto end = std::chrono::high_resolution_clock::now();
    
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Time: " << ns << " us\n";
    
    ASSERT_TRUE(result.has_value());
    
    const auto& solved = result.value();
    
    ASSERT_TRUE(is_valid_sudoku(solved));
    
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

    ASSERT_TRUE(is_valid_sudoku(solved));

    return true;
}

bool test_sudoku16() {
    
    using Board = std::array<std::array<int, 16>, 16>;

    Board board = { {
    {0, 12, 7, 0, 0, 1, 6, 0, 0, 4, 8, 0, 0, 5, 3, 0}, // 0xC = 12
    {9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13}, // 0xD = 13
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 11}, // 0xF = 15, 0xB = 11
    {0, 0, 0, 0, 0, 4, 0, 15, 14, 0, 0, 0, 0, 0, 0, 0}, // 0xE = 14
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {10, 0, 0, 4, 0, 0, 8, 0, 0, 0, 0, 2, 0, 0, 14, 0}, // 0xA = 10, 0xE = 14
    {14, 0, 0, 0, 0, 13, 2, 0, 0, 11, 7, 0, 0, 0, 0, 5}, // 0xD = 13, 0xB = 11
    {0, 0, 0, 11, 0, 0, 0, 10, 9, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 5, 0, 0, 0, 13, 12, 0, 0, 0, 0, 0, 0, 0}, // 0xD = 13, 0xC = 12
    {1, 0, 0, 0, 0, 6, 3, 0, 0, 15, 11, 0, 0, 0, 0, 12}, // 0xF = 15, 0xB = 11, 0xC = 12
    {4, 0, 0, 13, 0, 0, 11, 0, 0, 1, 0, 0, 10, 0, 0, 2},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 15, 0, 1, 11, 0, 4, 0, 0, 0, 0, 0}, // 0xF = 15, 0xB = 11
    {6, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 1},
    {12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7},
    {0, 3, 14, 0, 0, 9, 13, 0, 0, 7, 5, 0, 0, 15, 12, 0} // 0xE = 14, 0xD = 13, 0xF = 15, 0xC = 12
    } };

    SudokuSolver<16> solver(board);

    auto start = std::chrono::high_resolution_clock::now();
    auto result = solver.solve();
    auto end = std::chrono::high_resolution_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Time: " << ns << " us\n";

    ASSERT_TRUE(result.has_value());

    const auto& solved = result.value();

    ASSERT_TRUE(is_valid_sudoku(solved));

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
    runner.addTest("Easy Sudoku 9x9 with cells", test_easy9_sudoku_cells);
    runner.addTest("Medium Sudoku 9x9", test_medium9_sudoku);
    runner.addTest("Al Escargot Sudoku 9x9", test_Escargot_sudoku);
    runner.addTest("Sudoku 16x16", test_sudoku16);
    runner.addTest("No solution Sudoku", test_4x4_no_solution);
    runner.addTest("Invalid Sudoku", test_invalid_sudoku);

    return runner.runAll() ? 0 : 1;
}