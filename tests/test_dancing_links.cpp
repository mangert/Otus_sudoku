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
    DancingLinksMatrix matrix(3);
    matrix.addRow({ 0, 1, 2 }, 1);
    matrix.addRow({ 0, 2 }, 2);
    matrix.cover(1);
    // Здесь деструктор вызовется автоматически, и если что-то не так — упадёт

    return true;  // дошли сюда — значит, не упало
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

    bool success = runner.runAll();
    return success ? 0 : 1;
}