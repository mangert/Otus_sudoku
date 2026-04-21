#include <algorithm>
#include <array>
#include <set>

template<size_t N>
bool is_valid_sudoku(const std::array<std::array<int, N>, N>& board) {
    // Проверка, что все цифры в диапазоне 1..N или 0 (пусто)
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            int val = board[r][c];
            if (val != 0 && (val < 1 || val > N)) {
                return false;
            }
        }
    }

    // Вспомогательная лямбда для проверки последовательности
    auto has_duplicates = [](const std::vector<int>& nums) {
        std::set<int> seen;
        for (int x : nums) {
            if (x != 0) {
                if (seen.count(x)) return true;
                seen.insert(x);
            }
        }
        return false;
        };

    // Проверка строк
    for (int r = 0; r < N; ++r) {
        std::vector<int> row;
        for (int c = 0; c < N; ++c) {
            row.push_back(board[r][c]);
        }
        if (has_duplicates(row)) return false;
    }

    // Проверка столбцов
    for (int c = 0; c < N; ++c) {
        std::vector<int> col;
        for (int r = 0; r < N; ++r) {
            col.push_back(board[r][c]);
        }
        if (has_duplicates(col)) return false;
    }

    // Проверка блоков (для N = 9 или 16, где блок = sqrt(N) x sqrt(N))
    constexpr size_t block_size = constexpr_sqrt(N);
    for (int br = 0; br < N; br += block_size) {
        for (int bc = 0; bc < N; bc += block_size) {
            std::vector<int> block;
            for (int r = br; r < br + block_size; ++r) {
                for (int c = bc; c < bc + block_size; ++c) {
                    block.push_back(board[r][c]);
                }
            }
            if (has_duplicates(block)) return false;
        }
    }

    return true;
}