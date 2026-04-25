#include "console_utils.h"
#include <iostream>
#include <limits>
#include <algorithm>

namespace ConsoleUtils {

    int showMenu(const std::vector<std::string>& options, const std::string& prompt) {
        while (true) {
            std::cout << "\n=== " << prompt << " ===\n";
            for (size_t i = 0; i < options.size(); ++i) {
                std::cout << i + 1 << ". " << options[i] << "\n";
            }
            std::cout << "0. Выход\n";
            std::cout << "Ваш выбор: ";

            int choice;
            std::cin >> choice;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка: введите число.\n";
                continue;
            }

            if (choice == 0) {
                return -1;  // сигнал выхода
            }
            if (choice >= 1 && choice <= static_cast<int>(options.size())) {
                return choice - 1;
            }
            std::cout << "Ошибка: выберите 0 (выход) или 1-" << options.size() << ".\n";
        }
    }

    bool askYesNo(const std::string& question) {
        while (true) {
            std::cout << question << " (y/n): ";
            std::string answer;
            std::cin >> answer;

            if (answer == "y" || answer == "Y" || answer == "yes" || answer == "Yes") {
                return true;
            }
            if (answer == "n" || answer == "N" || answer == "no" || answer == "No") {
                return false;
            }
            std::cout << "Пожалуйста, введите y или n.\n";
        }
    }

    std::string askString(const std::string& prompt, bool allowEmpty) {
        while (true) {
            std::cout << prompt;
            std::string result;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, result);

            if (!result.empty() || allowEmpty) {
                return result;
            }
            std::cout << "Строка не может быть пустой. Попробуйте снова.\n";
        }
    }

    int askInt(const std::string& prompt, int min, int max) {
        while (true) {
            std::cout << prompt;
            int value;
            std::cin >> value;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка: введите целое число.\n";
                continue;
            }

            if (value >= min && value <= max) {
                return value;
            }
            std::cout << "Ошибка: число должно быть от " << min << " до " << max << "\n";
        }
    }

    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

} // namespace ConsoleUtils