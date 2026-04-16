#define _CRTDBG_MAP_ALLOC
#define DEBUG

#include <crtdbg.h>
#include "dancing_links/dancing_links_matrix.h"
#include <iostream>
#include <thread>

int main() {
    setlocale(LC_ALL, "russian");

    std::cout << "=== Тест cover/uncover ===\n\n";

    DancingLinksMatrix matrix(4);
    matrix.addRow({ 0, 2 }, 1);
    matrix.addRow({ 1, 3 }, 2);
    matrix.addRow({ 0, 3 }, 3);

    std::cout << "Исходная матрица:\n";
    matrix.print();

    // Покрываем столбец 1
    auto col1 = matrix.getColumn(1);
    std::cout << "После cover(столбец 1):\n";
    matrix.cover(col1);
    matrix.print();

    // Покрываем столбец 0
    auto col0 = matrix.getColumn(0);
    std::cout << "После cover(столбец 0):\n";
    matrix.cover(col0);
    matrix.print();

    // Восстанавливаем в обратном порядке
    std::cout << "После uncover(столбец 0):\n";
    matrix.uncover(col0);
    matrix.print();

    std::cout << "После uncover(столбец 1):\n";
    matrix.uncover(col1);
    matrix.print();

    std::cout << "=== Тест пройден ===\n";

    
    // Матрица 3x4 из примера:
    //     C0 C1 C2 C3
    // R1: 1  0  1  0
    // R2: 0  1  0  1
    // R3: 1  0  0  1
    /*
    DancingLinksMatrix matrix(4);

    matrix.addRow({ 0, 2 }, 1);   // строка 1: столбцы 0 и 2
    matrix.addRow({ 1, 3 }, 2);   // строка 2: столбцы 1 и 3
    matrix.addRow({ 0, 3 }, 3);   // строка 3: столбцы 0 и 3

    matrix.print();
    std::cout << "--------------------------" << std::endl;
    auto* col1 = matrix.getColumn(1);
    matrix.cover(col1);
    matrix.print();

    matrix.uncover(col1);
    std::cout << "=== ПОСЛЕ uncover ===\n";
    matrix.print();*/
    /*
    // Включаем автоматическую проверку утечек
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    std::cout << "=== Тест утечек памяти ===\n";

    // Создаём матрицу
    {
        DancingLinksMatrix matrix(4);

        // Добавляем строки
        matrix.addRow({ 0, 2 }, 1);
        matrix.addRow({ 1, 3 }, 2);
        matrix.addRow({ 0, 3 }, 3);

        matrix.print();
    }
    std::cout << "\nПрограмма завершена. Проверь Diagnostic Tools\n";
    std::cout << "Нажми Enter для выхода...\n";
    std::cin.get();  // Пауза, чтобы успеть посмотреть диагностику
    */
    return 0;
}