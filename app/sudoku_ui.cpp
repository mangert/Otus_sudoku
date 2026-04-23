#include "sudoku_ui.h"
#include "sudoku_io.h"
#include "console_utils.h"
#include <iostream>
#include <expected>

template<size_t N>
std::string errorToString(typename SudokuSolver<N>::SolverError error) {
    using E = typename SudokuSolver<N>::SolverError;
    switch (error) {        
    case E::INVALID_CONSTRAINTS:
        return "некорректные начальные данные (конфликт или дубликаты)";
    case E::NO_SOLUTION:
        return "решения не существует";
    }
    return "неизвестная ошибка";
}

template<size_t N>
void processSudoku() {
    using Board = std::array<std::array<int, N>, N>;

    while (true) {
        std::vector<std::string> sourceMenu = { "Загрузить из файла", "Ввести с клавиатуры", "Назад" };
        int choice = ConsoleUtils::showMenu(sourceMenu, "Выберите источник данных");

        Board board;

        if (choice == 0) {
            std::string filename = ConsoleUtils::askString("Введите имя файла: ");
            bool success = SudokuIO::readBoardFromFile<N>(filename, board);
            if (!success) {
                std::cout << "Ошибка загрузки файла.\n";
                continue;
            }
        }
        else if (choice == 1) {
            std::cout << "Введите " << N << " строк по " << N << " чисел (0 для пустых):\n";
            bool success = SudokuIO::readBoardFromStdin<N>(board);
            if (!success) {
                std::cout << "Ошибка ввода.\n";
                continue;
            }
        }
        else {
            return; // назад в главное меню
        }

        // Показать загруженную доску
        std::cout << "\nЗагруженная доска:\n";
        SudokuIO::printBoard(board);

        if (!ConsoleUtils::askYesNo("Решить эту доску?")) {
            continue;
        }

        // Решение
        SudokuSolver<N> solver(board);
        auto result = solver.solve();

        if (!result.has_value()) {
            std::cout << "Ошибка: " << errorToString<N>(result.error()) << "\n";
            continue;
        }

        // Показать решение
        std::cout << "\nРешение:\n";
        SudokuIO::printBoard(result.value());

        // Меню после решения
        std::vector<std::string> postMenu = { "Сохранить в файл", "Решить другую", "Выход" };
        int postChoice = ConsoleUtils::showMenu(postMenu, "Что дальше?");

        if (postChoice == 0) {
            std::string filename = ConsoleUtils::askString("Имя файла для сохранения: ");
            if (SudokuIO::writeBoardToFile(filename, result.value())) {
                std::cout << "Сохранено.\n";
            }
            else {
                std::cout << "Ошибка сохранения.\n";
            }
        }
        else if (postChoice == 1) {
            continue; // решить другую (выбор источника)
        }
        else {
            return; // выход в главное меню
        }
    }
}

void runSudokuUI(int N) {
    if (N == 4) {
        processSudoku<4>();
    }
    else if (N == 9) {
        processSudoku<9>();
    }
    else if (N == 16) {
        processSudoku<16>();
    }
    else {
        std::cout << "Неподдерживаемый размер: " << N << "\n";
    }
}