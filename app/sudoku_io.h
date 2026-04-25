#pragma once
#include <array>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include "sudoku/sudoku_solver.h"

/**
 * @brief Утилиты для ввода/вывода досок судоку
 */
namespace SudokuIO {

    /**
     * @brief Чтение доски из текстового файла
     * @tparam N размер доски (4, 9, 16...)
     * @param filename имя файла
     * @param board ссылка на доску, в которую будет прочитано содержимое
     * @return true если чтение успешно, false в случае ошибки (файл не найден, неверный формат)
     *
     * Формат файла: N строк, в каждой строке N целых чисел, разделённых пробелами.
     * Пустые клетки обозначаются 0.
     */
    template<size_t N>
    bool readBoardFromFile(const std::string& filename, std::array<std::array<int, N>, N>& board) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file " << filename << "\n";
            return false;
        }

        for (size_t row = 0; row < N; ++row) {
            for (size_t col = 0; col < N; ++col) {
                if (!(file >> board[row][col])) {
                    std::cerr << "Error: Invalid format at row " << row << ", col " << col << "\n";
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * @brief Чтение доски из стандартного ввода (клавиатуры)
     * @tparam N размер доски (4, 9, 16...)
     * @param board ссылка на доску, в которую будет прочитано содержимое
     * @return true если чтение успешно, false в случае неверного формата ввода
     *
     * Пользователю выводится приглашение ввести N строк по N чисел.
     * Пустые клетки обозначаются 0.
     */
    template<size_t N>
    bool readBoardFromStdin(std::array<std::array<int, N>, N>& board) {
        std::cout << "Enter " << N << " rows of " << N << " numbers (0 for empty):\n";
        for (size_t row = 0; row < N; ++row) {
            for (size_t col = 0; col < N; ++col) {
                if (!(std::cin >> board[row][col])) {
                    std::cerr << "Error: Invalid input at row " << row << ", col " << col << "\n";
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * @brief Вывод доски в консоль с форматированием
     * @tparam N размер доски (4, 9, 16...)
     * @param board доска для вывода
     *
     * Вывод включает:
     * - Вертикальные и горизонтальные разделители блоков (для наглядности)
     * - Пустые клетки выводятся как " ."
     * - Числа выводятся с шириной поля 2 символа
     */
    template<size_t N>
    void printBoard(const std::array<std::array<int, N>, N>& board) {
        constexpr int block_size = static_cast<int>(constexpr_sqrt(N));
        const int cell_width = 2;  // ширина поля для одного числа

        for (size_t row = 0; row < N; ++row) {
            // Горизонтальный разделитель блоков (перед строкой блока)
            if (row % block_size == 0 && row != 0) {
                for (size_t i = 0; i < N; ++i) {
                    std::cout << "---";
                    if (i % block_size == block_size - 1 && i != N - 1) {
                        std::cout << "+";
                    }
                }
                std::cout << "\n";
            }

            for (size_t col = 0; col < N; ++col) {
                // Вертикальный разделитель блоков
                if (col % block_size == 0 && col != 0) {
                    std::cout << "|";
                }

                int val = board[row][col];
                if (val == 0) {
                    std::cout << " .";
                }
                else {
                    std::cout << std::setw(cell_width) << val;
                }

                // Пробел между клетками (кроме последней)
                if (col < N - 1) std::cout << " ";
            }
            std::cout << "\n";
        }
    }

    /**
     * @brief Запись доски в файл
     * @tparam N размер доски (4, 9, 16...)
     * @param filename имя файла для сохранения
     * @param board доска для записи
     * @return true если запись успешна, false если не удалось открыть файл
     *
     * Формат файла: N строк, в каждой строке N чисел, разделённых пробелами.
     */
    template<size_t N>
    bool writeBoardToFile(const std::string& filename, const std::array<std::array<int, N>, N>& board) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file " << filename << " for writing\n";
            return false;
        }

        for (size_t row = 0; row < N; ++row) {
            for (size_t col = 0; col < N; ++col) {
                file << board[row][col];
                if (col != N - 1) file << " ";
            }
            file << "\n";
        }
        return true;
    }

} // namespace SudokuIO