#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>

// tests/sudoku/test_sudoku.cpp или отдельный хедер
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
        std::vector<int> filtered;
        std::copy_if(nums.begin(), nums.end(), std::back_inserter(filtered),
                     [](int x) { return x != 0; });
        std::sort(filtered.begin(), filtered.end());
        return std::adjacent_find(filtered.begin(), filtered.end()) != filtered.end();
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

class TestRunner {
public:
    void addTest(const std::string& name, std::function<bool()> test) {
        tests_.push_back({name, test});
    }
    
    bool runAll() {
        int passed = 0;
        int failed = 0;
        
        for (const auto& [name, test] : tests_) {
            std::cout << "Running: " << name << " ... ";
            try {
                if (test()) {
                    std::cout << "OK\n";
                    ++passed;
                } else {
                    std::cout << "FAILED\n";
                    ++failed;
                }
            } catch (const std::exception& e) {
                std::cout << "EXCEPTION: " << e.what() << "\n";
                ++failed;
            } catch (...) {
                std::cout << "UNKNOWN EXCEPTION\n";
                ++failed;
            }
        }
        
        std::cout << "\n=== Results: " << passed << " passed, " << failed << " failed ===\n";
        return failed == 0;
    }
    
private:
    struct Test {
        std::string name;
        std::function<bool()> func;
    };
    std::vector<Test> tests_;
};

#define ASSERT_TRUE(expr) if (!(expr)) { std::cerr << "Assertion failed: " #expr << " at " << __FILE__ << ":" << __LINE__ << "\n"; return false; }
#define ASSERT_FALSE(expr) ASSERT_TRUE(!(expr))
#define ASSERT_EQ(a, b) ASSERT_TRUE((a) == (b))
#define ASSERT_NE(a, b) ASSERT_TRUE((a) != (b))