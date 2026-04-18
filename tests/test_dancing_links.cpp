#include "dancing_links/dancing_links_matrix.h"
#include "test_utils.h"
#include <iostream>

// ============================================================================
// Тесты для DancingLinksMatrix
// ============================================================================

bool test_empty_matrix() {
    DancingLinksMatrix matrix(0);
    auto result = matrix.search();
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result->empty());
    return true;
}

bool test_single_column_single_row() {
    DancingLinksMatrix matrix(1);
    matrix.addRow({ 0 }, 100);

    auto result = matrix.search();
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 1);
    ASSERT_EQ(result->at(0), 100);

    return true;
}

bool test_no_solution() {
    // Матрица 3×3 без точного покрытия
    DancingLinksMatrix matrix(3);
    matrix.addRow({ 0, 1 }, 1);   // 1 1 0
    matrix.addRow({ 1, 2 }, 2);   // 0 1 1
    matrix.addRow({ 0, 2 }, 3);   // 1 0 1

    auto result = matrix.search();
    ASSERT_FALSE(result.has_value());

    return true;
}

bool test_simple_matrix_with_solution() {
    DancingLinksMatrix matrix(4);
    matrix.addRow({ 0, 2 }, 1);
    matrix.addRow({ 1, 3 }, 2);
    matrix.addRow({ 0, 3 }, 3);

    auto result = matrix.search(2);
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 2);

    bool valid = ((*result)[0] == 2 && (*result)[1] == 1) ||
        ((*result)[0] == 1 && (*result)[1] == 3);
    ASSERT_TRUE(valid);

    return true;
}

bool test_isFullyUncovered() {
    DancingLinksMatrix matrix(2);
    matrix.addRow({ 0, 1 }, 1);

    ASSERT_TRUE(matrix.isFullyUncovered());  // изначально всё раскрыто

    matrix.cover(0);
    ASSERT_FALSE(matrix.isFullyUncovered());  // есть покрытый столбец

    matrix.uncover(0);
    ASSERT_TRUE(matrix.isFullyUncovered());  // снова раскрыто

    return true;
}

bool test_cleanup_does_not_crash() {
    // Просто создаём и уничтожаем матрицу — деструктор проверит себя сам
    {
        DancingLinksMatrix matrix(3);
        matrix.addRow({ 0, 1, 2 }, 1);
        matrix.addRow({ 0, 2 }, 2);
        matrix.cover(1);
    }
    // Здесь деструктор вызовется автоматически, и если что-то не так — упадёт

    return true;  // дошли сюда — значит, не упало
}

bool test_Wiki_example() {
    // U = {0,1,2,3,4,5,6} (7 элементов)
    // A = {0,3,6}
    // B = {0,3}
    // C = {3,4,6}
    // D = {2,4,5}
    // E = {1,2,5,6}
    // F = {1,6}

    DancingLinksMatrix matrix(7);

    matrix.addRow({ 0, 3, 6 }, 1);  // A
    matrix.addRow({ 0, 3 }, 2);     // B
    matrix.addRow({ 3, 4, 6 }, 3);  // C
    matrix.addRow({ 2, 4, 5 }, 4);  // D
    matrix.addRow({ 1, 2, 5, 6 }, 5); // E
    matrix.addRow({ 1, 6 }, 6);     // F

    auto result = matrix.search();
    ASSERT_TRUE(result.has_value());

    auto& solution = result.value();
    // Ожидаемое решение: B, D, F (или B, C, E, F? Проверим)
    // По статье Википедии: {B, D, F}
    ASSERT_EQ(solution.size(), 3);
    std::vector<int> expected{ 2,4,6 };
    //сравним с решением
    for (int row = 0; row != solution.size(); ++row) {
        ASSERT_TRUE(solution[row] == expected[row]);
    }

    return true;
}

// ============================================================================
// Запуск всех тестов
// ============================================================================

int main() {
    TestRunner runner;
    
    runner.addTest("Empty matrix", test_empty_matrix);
    runner.addTest("Single column single row", test_single_column_single_row);
    runner.addTest("No solution", test_no_solution);
    runner.addTest("Simple matrix with solution", test_simple_matrix_with_solution);
    runner.addTest("isFullyUncovered works", test_isFullyUncovered);
    runner.addTest("Cleanup does not crash", test_cleanup_does_not_crash);
    runner.addTest("BONUS - example from Wikipedia", test_Wiki_example);
    bool success = runner.runAll();
    return success ? 0 : 1;
}