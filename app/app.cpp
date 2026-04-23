#include "console_utils.h"
#include "sudoku_ui.h"
#include <iostream>

int main() {
    setlocale(LC_ALL, "russian");
    using namespace ConsoleUtils;

    while (true) {
        std::vector<std::string> mainMenu = { "Выбрать размер судоку", "Выход" };
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