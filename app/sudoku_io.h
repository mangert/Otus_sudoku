#pragma once
#include <array>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include "sudoku/sudoku_solver.h"

namespace SudokuIO {

    // Чтение доски из файла
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

    // Чтение доски из stdin
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

    // Вывод доски в консоль    
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

    // Запись доски в файл
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