#pragma once
#include <array>
#include <vector>
#include <concepts>
#include "dancing_links/dancing_links_matrix.h"
#include <expected>

/**
 * @brief Вычисление целочисленного квадратного корня на этапе компиляции
 * @param n число, из которого извлекается корень
 * @return целая часть квадратного корня (floor)
 */
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

/**
 * @brief Проверяет, является ли число полным квадратом
 * @param n проверяемое число
 * @return true если n — полный квадрат (4, 9, 16, 25...)
 */
constexpr bool isPerfectSquare(size_t n) {
    for (size_t i = 1; i * i <= n; ++i) {
        if (i * i == n) return true;
    }
    return false;
}

/**
 * @brief Концепт: размер судоку должен быть полным квадратом
 * @tparam N размер стороны судоку (4, 9, 16, 25...)
 */
template<size_t N>
concept ValidSudokuSize = isPerfectSquare(N);

/**
 * @brief Решатель судоку на основе алгоритма Dancing Links (точное покрытие)
 * @tparam N размер стороны судоку (должен быть полным квадратом)
 */
template<size_t N> requires ValidSudokuSize<N>
class SudokuSolver final {
public:
    /**
     * @brief Тип доски: квадратная матрица N×N, 0 обозначает пустую клетку
     */
    using Board = std::array<std::array<int, N>, N>;

    /**
     * @brief Тип ячейки с фиксированным значением: (строка, столбец, цифра)
     */
    using FixedCell = std::tuple<int, int, int>;  // row, col, digit

    /**
     * @brief Ошибки, которые может вернуть метод solve()
     */
    enum class SolverError {
        INVALID_CONSTRAINTS,  ///< Начальные данные противоречивы
        NO_SOLUTION           ///< Решения не существует
    };

    /**
     * @brief Конструктор от полной доски
     * @param board начальная доска (0 — пустая клетка)
     */
    explicit SudokuSolver(const Board& board);

    /**
     * @brief Конструктор от списка фиксированных клеток
     * @param fixed_cells список кортежей (row, col, digit) с начальными значениями
     */
    explicit SudokuSolver(const std::vector<FixedCell>& fixed_cells);

    // Правило пяти (явно)
    SudokuSolver(const SudokuSolver&) = default;
    SudokuSolver& operator=(const SudokuSolver&) = default;
    SudokuSolver(SudokuSolver&&) = default;
    SudokuSolver& operator=(SudokuSolver&&) = default;
    ~SudokuSolver() = default;

    /**
     * @brief Решает судоку с заданными начальными условиями
     * @return Если решение найдено — доска Board, иначе SolverError
     */
    std::expected<Board, SolverError> solve() const;

private:
    std::vector<FixedCell> fixed_cells; ///< начальные клетки

    // константы времени компиляции
    static constexpr size_t BLOCK_SIZE = constexpr_sqrt(N);
    static constexpr size_t CELLS = N * N;
    static constexpr size_t CONSTRAINTS_COUNT = 4;
    static constexpr size_t TOTAL_COLUMNS = CONSTRAINTS_COUNT * CELLS;

    // Вспомогательные методы
    /**
     * @brief Преобразует (row, col, digit) в 4 индекса столбцов матрицы покрытия
     * @param row строка (0..N-1)
     * @param col столбец (0..N-1)
     * @param digit цифра (1..N)
     * @return массив из 4 индексов: [клетка, строка, столбец, блок]
     */
    static std::array<int, SudokuSolver<N>::CONSTRAINTS_COUNT> getColumnIndices(int row, int col, int digit);

    /**
     * @brief Кодирует (row, col, digit) в уникальный row_id для матрицы покрытия
     * @param row строка (0..N-1)
     * @param col столбец (0..N-1)
     * @param digit цифра (1..N)
     * @return row_id в диапазоне 0 .. N³-1
     */
    static int encodeRowId(int row, int col, int digit);

    /**
     * @brief Декодирует row_id обратно в (row, col, digit)
     * @param row_id идентификатор строки матрицы покрытия
     * @return кортеж (row, col, digit)
     */
    static FixedCell decodeRowId(int row_id);

    /**
     * @brief Заполняет матрицу Dancing Links всеми возможными вариантами (N³ строк)
     * @param dlx ссылка на пустую матрицу Dancing Links (будет заполнена)
     */
    void fillExactCoverMatrix(DancingLinksMatrix& dlx) const;

    /**
     * @brief Применяет начальные ограничения (фиксированные клетки)
     * @param dlx ссылка на заполненную матрицу Dancing Links
     * @return true если ограничения корректны и успешно применены, false при конфликте
     */
    bool applyInitialConstraints(DancingLinksMatrix& dlx) const;

    /**
     * @brief Восстанавливает доску из найденного решения
     * @param solution список row_id, возвращённый алгоритмом DLX
     * @return доска 9×9 (или N×N), заполненная решением
     */
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