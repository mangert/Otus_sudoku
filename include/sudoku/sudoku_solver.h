#pragma once
#include <array>
#include <vector>
#include <concepts>
#include "dancing_links/dancing_links_matrix.h"
#include <expected>


constexpr size_t constexpr_sqrt(size_t n) {
    if (n == 0 || n == 1) return n;
    size_t lo = 1, hi = n;
    while (lo <= hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (mid * mid == n) return mid;
        if (mid * mid < n) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

constexpr bool isPerfectSquare(size_t n) {
    for (size_t i = 1; i * i <= n; ++i) {
        if (i * i == n) return true;
    }
    return false;
}

template<size_t N>
concept ValidSudokuSize = isPerfectSquare(N);

template<size_t N> requires ValidSudokuSize<N>
class SudokuSolver final {
public:
    using Board = std::array<std::array<int, N>, N>; //тип доски
    using FixedCell = std::tuple<int, int, int>;  // row, col, digit
    
    enum class SolverError {
        INVALID_CONSTRAINTS,  // начальные данные противоречивы
        NO_SOLUTION           // решения нет
    };

    // Конструкторы
    explicit SudokuSolver(const Board& board);
    explicit SudokuSolver(const std::vector<FixedCell>& fixed_cells);
    
    SudokuSolver(const SudokuSolver&) = default;
    SudokuSolver& operator=(const SudokuSolver&) = default;
    SudokuSolver(SudokuSolver&&) = default;
    SudokuSolver& operator=(SudokuSolver&&) = default;
    ~SudokuSolver() = default;

    // Решение
    std::expected<Board, SolverError> solve() const;

private:
    std::vector<FixedCell> fixed_cells; //начальные клетки
    
    //константы времени компиляции
    static constexpr size_t BLOCK_SIZE = constexpr_sqrt(N);
    static constexpr size_t CELLS = N * N;
    static constexpr size_t CONSTRAINTS_COUNT = 4;
    static constexpr size_t TOTAL_COLUMNS = CONSTRAINTS_COUNT * CELLS;

    // Вспомогательные методы
    static std::array<int, SudokuSolver<N>::CONSTRAINTS_COUNT> getColumnIndices(int row, int col, int digit);
    static int encodeRowId(int row, int col, int digit); 
    static FixedCell decodeRowId(int row_id);
    void fillExactCoverMatrix(DancingLinksMatrix& dlx) const;
    bool applyInitialConstraints(DancingLinksMatrix& dlx) const;
    Board rebuildBoardFromSolution(const std::vector<int>& solution) const;
};

// ============================================================================
// Реализация
// ============================================================================

// Конструкторы
template<size_t N> requires ValidSudokuSize<N>
SudokuSolver<N>::SudokuSolver(const std::vector<FixedCell>& fixed_cells)
    : fixed_cells(fixed_cells) {}

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
std::expected<typename SudokuSolver<N>::Board, typename SudokuSolver<N>::SolverError>
SudokuSolver<N>::solve() const {
    
    DancingLinksMatrix dlx(TOTAL_COLUMNS);
    fillExactCoverMatrix(dlx);

    // Проверка начальных ограничений
    if (!applyInitialConstraints(dlx)) {
        return std::unexpected(SolverError::INVALID_CONSTRAINTS);
    }

    auto solution = dlx.search(CELLS - fixed_cells.size());
    if (!solution.has_value()) {
        return std::unexpected(SolverError::NO_SOLUTION);
    }

    return rebuildBoardFromSolution(solution.value());
}

// Служебные методы
template<size_t N> requires ValidSudokuSize<N>
std::array<int, SudokuSolver<N>::CONSTRAINTS_COUNT> SudokuSolver<N>::getColumnIndices
(int row, int col, int digit) {
    
    int d = digit - 1; //индексы с 0, цифры начинаются с 1
    int block = (row / BLOCK_SIZE) * BLOCK_SIZE + (col / BLOCK_SIZE);

    int cell_idx = row * N + col;
    int row_idx = CELLS + row * N + d;
    int col_idx = 2 * CELLS + col * N + d;
    int block_idx = 3 * CELLS + block * N + d;    
    return { cell_idx, row_idx, col_idx, block_idx};
}

template<size_t N> requires ValidSudokuSize<N>
int SudokuSolver<N>::encodeRowId(int row, int col, int digit) {
    return ((row * N + col) * N + (digit - 1));
}

template<size_t N> requires ValidSudokuSize<N>
SudokuSolver<N>::FixedCell SudokuSolver<N>::decodeRowId(int row_id) {
    int digit_minus_1 = row_id % N;
    int cell = row_id / N;
    int row = cell / N;
    int col = cell % N;
    return { row, col, digit_minus_1 + 1 };
}

template<size_t N> requires ValidSudokuSize<N>
void SudokuSolver<N>::fillExactCoverMatrix(DancingLinksMatrix& dlx) const {
    
    for (size_t row = 0; row != N; ++row) {
        for (size_t col = 0; col != N; ++col) {
            for (int digit = 1; digit <= N; ++digit) {
                
                int row_id = encodeRowId(row, col, digit);
                auto indices = getColumnIndices(row, col, digit);

                dlx.addRow(indices, row_id);
            }
        }
    }
}

template<size_t N> requires ValidSudokuSize<N>
bool SudokuSolver<N>::applyInitialConstraints(DancingLinksMatrix& dlx) const {
    for (const auto& [row, col, digit] : fixed_cells) {
        for (int col_idx : getColumnIndices(row, col, digit)) {            
            if (digit < 1 || digit > N || dlx.isCovered(col_idx)) {
                return false;  // конфликт
            }
            dlx.cover(col_idx);
        }
    }
    return true;
}

template<size_t N> requires ValidSudokuSize<N>
typename SudokuSolver<N>::Board
SudokuSolver<N>::rebuildBoardFromSolution(const std::vector<int>& solution) const {
    
    Board board{};
    // Заполняем начальные значения
    for (const auto& [row, col, digit] : fixed_cells) {
        board[row][col] = digit;
    }
    // Заполняем найденные алгоритмом
    for (int row_id : solution) {
        auto [row, col, digit] = decodeRowId(row_id);
        board[row][col] = digit;
    }
    return board;
}