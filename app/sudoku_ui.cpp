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
        std::vector<std::string> sourceMenu = { "Загрузить из файла", "Ввести с клавиатуры" };
        int choice = ConsoleUtils::showMenu(sourceMenu, "Выберите источник данных");

        if (choice == -1) {  // выход
            ConsoleUtils::clearScreen();
            return;
        }

        Board board;
        std::string filename;

        if (choice == 0) {  // файл
            filename = ConsoleUtils::askString("Введите имя файла: ");
            if (!SudokuIO::readBoardFromFile<N>(filename, board)) {
                std::cout << "Ошибка загрузки файла.\n";
                continue;
            }
            ConsoleUtils::clearScreen();
            std::cout << "Загружено из файла: " << filename << "\n";
        }
        else if (choice == 1) {  // клавиатура
            ConsoleUtils::clearScreen();
            std::cout << "Ввод с клавиатуры...\n";
            std::cout << "Введите " << N << " строк по " << N << " чисел (0 для пустых):\n";
            if (!SudokuIO::readBoardFromStdin<N>(board)) {
                std::cout << "Ошибка ввода.\n";
                continue;
            }
            ConsoleUtils::clearScreen();
            std::cout << "Загружено с клавиатуры\n";
        }

        // Показать загруженную доску
        std::cout << "\nЗагруженная доска:\n";
        SudokuIO::printBoard(board);

        if (!ConsoleUtils::askYesNo("Решить эту доску?")) {
            ConsoleUtils::clearScreen();
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
        std::vector<std::string> postMenu = { "Сохранить в файл", "Решить другую" };
        int postChoice = ConsoleUtils::showMenu(postMenu, "Что дальше?");

        if (postChoice == -1) {  // выход
            ConsoleUtils::clearScreen();
            return;
        }
        else if (postChoice == 0) {  // сохранить
            std::string saveFilename = ConsoleUtils::askString("Имя файла для сохранения: ");
            if (SudokuIO::writeBoardToFile(saveFilename, result.value())) {
                std::cout << "Сохранено.\n";
            }
            else {
                std::cout << "Ошибка сохранения.\n";
            }
            // после сохранения остаёмся в меню после решения
            // (не выходим, чтобы пользователь мог выбрать "Решить другую")
        }
        else if (postChoice == 1) {  // решить другую
            ConsoleUtils::clearScreen();
            continue;
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