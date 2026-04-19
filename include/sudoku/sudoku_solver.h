#pragma once

#include <array>
#include <optional>
#include <vector>
#include <concepts>

constexpr bool isPerfectSquare(size_t n) {
    for (size_t i = 1; i * i <= n; ++i) {
        if (i * i == n) return true;
    }
    return false;
}

template<size_t N>
concept ValidSudokuSize = isPerfectSquare(N);

template<size_t N> requires ValidSudokuSize<N>
class SudokuSolver {
public:
    using Board = std::array<std::array<int, N>, N>;
    using FixedCell = std::tuple<int, int, int>;  // row, col, digit

    // Конструкторы
    explicit SudokuSolver(const Board& board);
    explicit SudokuSolver(const std::vector<FixedCell>& fixed_cells);
    
    SudokuSolver(const SudokuSolver&) = default;
    SudokuSolver& operator=(const SudokuSolver&) = default;
    SudokuSolver(SudokuSolver&&) = default;
    SudokuSolver& operator=(SudokuSolver&&) = default;
    ~SudokuSolver() = default;

    // Решение
    std::optional<Board> solve();

private:
    std::vector<FixedCell> fixed_cells; //начальные клетки

    // Вспомогательные методы
    static int columnIndex(int row, int col, int digit);
    static void decodeRowId(int row_id, int& row, int& col, int& digit);
    void buildExactCoverMatrix();
    void applyInitialConstraints();
    Board rebuildBoardFromSolution(const std::vector<int>& solution) const;
};

// ============================================================================
// Реализация
// ============================================================================

// Конструкторы
template<size_t N> requires ValidSudokuSize<N>
SudokuSolver<N>::SudokuSolver(const std::vector<FixedCell>& fixed_cells)
    : fixed_cells(fixed_cells)
{
}

template<size_t N> requires ValidSudokuSize<N>
SudokuSolver<N>::SudokuSolver(const Board& board) {
    for (size_t row = 0; row < N; ++row) {
        for (size_t col = 0; col < N; ++col) {
            if (board[row][col] != 0) {
                fixed_cells.emplace_back(row, col, board[row][col]);
            }
        }
    }
}

// Решение
template<size_t N> requires ValidSudokuSize<N>
std::optional<typename SudokuSolver<N>::Board> SudokuSolver<N>::solve() {
    // TODO: реализация
    return std::nullopt;
}

// Служебные методы
template<size_t N> requires ValidSudokuSize<N>
int SudokuSolver<N>::columnIndex(int row, int col, int digit) {
    // TODO
    return 0;
}

template<size_t N> requires ValidSudokuSize<N>
void SudokuSolver<N>::decodeRowId(int row_id, int& row, int& col, int& digit) {
    // TODO
}

template<size_t N> requires ValidSudokuSize<N>
void SudokuSolver<N>::buildExactCoverMatrix() {
    // TODO
}

template<size_t N> requires ValidSudokuSize<N>
void SudokuSolver<N>::applyInitialConstraints() {
    // TODO
}

template<size_t N> requires ValidSudokuSize<N>
typename SudokuSolver<N>::Board
SudokuSolver<N>::rebuildBoardFromSolution(const std::vector<int>& solution) const {
    // TODO
    return {};
}