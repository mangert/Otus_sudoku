#include "console_utils.h"
#include "sudoku_ui.h"
#include <clocale>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace {

bool configureConsoleEncoding() {
#ifdef _WIN32
    if (SetConsoleCP(CP_UTF8) == 0 ||
        SetConsoleOutputCP(CP_UTF8) == 0) {
        return false;
    }
    return std::setlocale(LC_ALL, ".UTF-8") != nullptr;
#else
    return std::setlocale(LC_ALL, "") != nullptr;
#endif
}

} // namespace

int main() {
    if (!configureConsoleEncoding()) {
        std::cerr << "Failed to configure console encoding.\n";
        return 1;
    }

    using namespace ConsoleUtils;

    while (true) {
        std::vector<std::string> mainMenu = { "Выбрать размер судоку"};
        int choice = showMenu(mainMenu, "Главное меню");

        if (choice == 0) {
            int size = askInt("Введите размер (4, 9, 16): ", 4, 16);
            // Проверка, что размер поддерживается
            if (size != 4 && size != 9 && size != 16) {                
                std::cout << "Неподдерживаемый размер. Доступны: 4, 9, 16.\n";
                continue;
            }
            runSudokuUI(size);
        }
        else {
            std::cout << "До свидания!\n";
            break;
        }       
    }

    return 0;
}
