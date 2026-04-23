#pragma once

#include <string>
#include <vector>

namespace ConsoleUtils {

	// Показать меню и вернуть выбранный индекс (0 .. options.size()-1)
	int showMenu(const std::vector<std::string>& options, const std::string& prompt = "Выберите действие");

	// Запрос yes/no
	bool askYesNo(const std::string& question);

	// Запрос строки
	std::string askString(const std::string& prompt, bool allowEmpty = false);

	// Запрос целого числа в диапазоне
	int askInt(const std::string& prompt, int min, int max);

	// Очистка экрана
	void clearScreen();

} // namespace ConsoleUtils
