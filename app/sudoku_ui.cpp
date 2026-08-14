#include "sudoku_ui.h"
#include "sudoku_io.h"
#include "sudoku_json_io.h"
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
std::string jsonErrorToString(const SudokuIO::JsonIoError& error) {
    using Code = SudokuIO::JsonIoError::Code;
    switch (error.code) {
    case Code::FILE_OPEN_FAILED:
        return "не удалось открыть файл";
    case Code::FILE_READ_FAILED:
        return "не удалось прочитать файл";
    case Code::INVALID_SYNTAX:
        return "нарушен синтаксис JSON";
    case Code::INVALID_SCHEMA:
        return "JSON не соответствует схеме size + board";
    case Code::SIZE_MISMATCH:
        return "размер в JSON (" + std::to_string(error.actual_size) +
            ") не совпадает с выбранным размером (" + std::to_string(N) + ")";
    case Code::INVALID_BOARD_SHAPE:
        return "board должен содержать " + std::to_string(N) +
            " строк по " + std::to_string(N) + " значений";
    case Code::INVALID_CELL_VALUE:
        return "значения клеток должны быть целыми числами от 0 до " + std::to_string(N);
    case Code::FILE_WRITE_FAILED:
        return "не удалось записать файл";
    }
    return "неизвестная ошибка JSON";
}

template<size_t N>
void processSudoku() {
    using Board = std::array<std::array<int, N>, N>;

    while (true) {
        std::vector<std::string> sourceMenu = {
            "Загрузить из TXT-файла",
            "Загрузить из JSON-файла",
            "Ввести с клавиатуры"
        };
        int choice = ConsoleUtils::showMenu(sourceMenu, "Выберите источник данных");

        if (choice == -1) {  // выход
            ConsoleUtils::clearScreen();
            return;
        }

        Board board{};
        std::string filename;

        if (choice == 0) {  // TXT-файл
            filename = ConsoleUtils::askString("Введите имя TXT-файла: ");
            if (!SudokuIO::readBoardFromFile<N>(filename, board)) {
                std::cout << "Ошибка загрузки TXT-файла.\n";
                continue;
            }
            ConsoleUtils::clearScreen();
            std::cout << "Загружено из TXT-файла: " << filename << "\n";
        }
        else if (choice == 1) {  // JSON-файл
            filename = ConsoleUtils::askString("Введите имя JSON-файла: ");
            auto json_result = SudokuIO::readBoardFromJsonFile<N>(filename);
            if (!json_result.has_value()) {
                std::cout << "Ошибка загрузки JSON-файла: "
                    << jsonErrorToString<N>(json_result.error()) << ".\n";
                continue;
            }
            board = json_result.value();
            ConsoleUtils::clearScreen();
            std::cout << "Загружено из JSON-файла: " << filename << "\n";
        }
        else if (choice == 2) {  // клавиатура
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
        std::vector<std::string> postMenu = {
            "Сохранить в TXT-файл",
            "Сохранить в JSON-файл",
            "Решить другую"
        };
        int postChoice = ConsoleUtils::showMenu(postMenu, "Что дальше?");

        if (postChoice == -1) {  // выход
            ConsoleUtils::clearScreen();
            return;
        }
        else if (postChoice == 0) {  // сохранить в TXT
            std::string saveFilename = ConsoleUtils::askString("Имя TXT-файла для сохранения: ");
            if (SudokuIO::writeBoardToFile(saveFilename, result.value())) {
                std::cout << "Сохранено в TXT-файл.\n";
            }
            else {
                std::cout << "Ошибка сохранения TXT-файла.\n";
            }
        }
        else if (postChoice == 1) {  // сохранить в JSON
            std::string saveFilename = ConsoleUtils::askString("Имя JSON-файла для сохранения: ");
            auto write_result = SudokuIO::writeBoardToJsonFile(saveFilename, result.value());
            if (write_result.has_value()) {
                std::cout << "Сохранено в JSON-файл.\n";
            }
            else {
                std::cout << "Ошибка сохранения JSON-файла: "
                    << jsonErrorToString<N>(write_result.error()) << ".\n";
            }
        }
        else if (postChoice == 2) {  // решить другую
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
