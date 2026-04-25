#pragma once

#include <string>
#include <vector>

/**
 * @brief Утилиты для работы с консольным вводом/выводом
 */
namespace ConsoleUtils {

    /**
     * @brief Показать меню и получить выбор пользователя
     * @param options список пунктов меню (без пункта "Выход")
     * @param prompt заголовок меню
     * @return -1 если выбран выход (0), иначе индекс выбранного пункта (0..size-1)
     */
    int showMenu(const std::vector<std::string>& options, const std::string& prompt = "Выберите действие");

    /**
     * @brief Запрос подтверждения (y/n)
     * @param question текст вопроса
     * @return true если ответ 'y' или 'yes', false если 'n' или 'no'
     */
    bool askYesNo(const std::string& question);

    /**
     * @brief Запрос строки от пользователя
     * @param prompt приглашение к вводу
     * @param allowEmpty разрешить ли пустую строку
     * @return введённая строка
     */
    std::string askString(const std::string& prompt, bool allowEmpty = false);

    /**
     * @brief Запрос целого числа в заданном диапазоне
     * @param prompt приглашение к вводу
     * @param min минимальное допустимое значение
     * @param max максимальное допустимое значение
     * @return введённое число (гарантированно в диапазоне [min, max])
     */
    int askInt(const std::string& prompt, int min, int max);

    /**
     * @brief Очистка экрана консоли
     *
     * Для Windows использует `cls`, для Linux/macOS — `clear`
     */
    void clearScreen();

} // namespace ConsoleUtils