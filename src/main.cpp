#include "dancing_links/dancing_links_matrix.h"
#include <iostream>

int main() {
    // Матрица 3x4 из примера:
    //     C0 C1 C2 C3
    // R1: 1  0  1  0
    // R2: 0  1  0  1
    // R3: 1  0  0  1

    DancingLinksMatrix matrix(4);

    matrix.addRow({ 0, 2 }, 1);   // строка 1: столбцы 0 и 2
    matrix.addRow({ 1, 3 }, 2);   // строка 2: столбцы 1 и 3
    matrix.addRow({ 0, 3 }, 3);   // строка 3: столбцы 0 и 3

    matrix.print();

    return 0;
}