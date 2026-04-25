#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <array>
#include <set>

/**
 * @brief Проверяет, является ли доска судоку корректно заполненной
 * @tparam N размер доски (4, 9, 16...)
 * @param board доска для проверки
 * @return true если доска удовлетворяет всем правилам судоку, false иначе
 *
 * Проверяет:
 * - Все цифры в диапазоне 1..N (0 игнорируется)
 * - В каждой строке нет повторяющихся цифр
 * - В каждом столбце нет повторяющихся цифр
 * - В каждом блоке (√N×√N) нет повторяющихся цифр
 */
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

    // Вспомогательная лямбда для проверки наличия дубликатов
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

/**
 * @brief Простой фреймворк для юнит-тестирования
 *
 * Позволяет добавлять тесты с именем и проверять их результаты.
 * Пример использования:
 * @code
 * TestRunner runner;
 * runner.addTest("Test name", []() {
 *     ASSERT_TRUE(condition);
 *     return true;
 * });
 * return runner.runAll() ? 0 : 1;
 * @endcode
 */
class TestRunner {
public:
    /**
     * @brief Добавляет тест в набор
     * @param name имя теста (выводится при запуске)
     * @param test функция, возвращающая true если тест пройден
     */
    void addTest(const std::string& name, std::function<bool()> test) {
        tests_.push_back({ name, test });
    }

    /**
     * @brief Запускает все добавленные тесты
     * @return true если все тесты пройдены, false если хотя бы один упал
     */
    bool runAll() {
        int passed = 0;
        int failed = 0;

        for (const auto& [name, test] : tests_) {
            std::cout << "Running: " << name << " ... ";
            try {
                if (test()) {
                    std::cout << "OK\n";
                    ++passed;
                }
                else {
                    std::cout << "FAILED\n";
                    ++failed;
                }
            }
            catch (const std::exception& e) {
                std::cout << "EXCEPTION: " << e.what() << "\n";
                ++failed;
            }
            catch (...) {
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

// ============================================================================
// Макросы для упрощения написания тестов
// ============================================================================

/**
 * @brief Проверяет, что выражение истинно
 * @param expr выражение для проверки
 *
 * При неудаче выводит сообщение с файлом и строкой.
 */
#define ASSERT_TRUE(expr) if (!(expr)) { std::cerr << "Assertion failed: " #expr << " at " << __FILE__ << ":" << __LINE__ << "\n"; return false; }

 /**
  * @brief Проверяет, что выражение ложно
  * @param expr выражение для проверки
  */
#define ASSERT_FALSE(expr) ASSERT_TRUE(!(expr))

  /**
   * @brief Проверяет, что два значения равны
   * @param a первое значение
   * @param b второе значение
   */
#define ASSERT_EQ(a, b) ASSERT_TRUE((a) == (b))

   /**
    * @brief Проверяет, что два значения не равны
    * @param a первое значение
    * @param b второе значение
    */
#define ASSERT_NE(a, b) ASSERT_TRUE((a) != (b))