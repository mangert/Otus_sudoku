#pragma once

#include <vector>
#include <stack>
#include <span>
#include <optional>

class DancingLinksMatrix final {
private:
    // Forward declarations
    struct Node;

public:
    /**
     * @brief Конструктор матрицы с заданным количеством столбцов
     * @param num_columns количество столбцов в матрице
     */
    explicit DancingLinksMatrix(int num_columns);

    /**
     * @brief Деструктор, освобождает всю память
     */
    ~DancingLinksMatrix();

    DancingLinksMatrix(const DancingLinksMatrix&) = delete;
    DancingLinksMatrix& operator=(const DancingLinksMatrix&) = delete;

    /**
     * @brief Конструктор перемещения
     */
    DancingLinksMatrix(DancingLinksMatrix&& other) noexcept;

    /**
     * @brief Оператор перемещения
     */
    DancingLinksMatrix& operator=(DancingLinksMatrix&& other) noexcept;

    /**
     * @brief Добавляет строку в матрицу
     * @param col_indices список индексов столбцов, в которых в этой строке стоят единицы
     * @param row_id уникальный идентификатор строки (возвращается в решении)
     */
    void addRow(std::span<const int> col_indices, int row_id);

    /**
     * @brief Покрывает столбец и все конфликтующие с ним строки
     * @param col_idx индекс столбца
     */
    void cover(int col_idx);

    /**
     * @brief Восстанавливает ранее покрытый столбец (LIFO порядок)
     * @param col_idx индекс столбца
     */
    void uncover(int col_idx);

    /**
     * @brief Поиск точного покрытия (алгоритм X)
     * @param expected_solution_size ожидаемый размер решения (для резервирования памяти)
     * @return решение в виде списка row_id, либо std::nullopt если решения нет
     */
    std::optional<std::vector<int>> search(int expected_solution_size = 0);

    /**
     * @brief Проверяет, покрыт ли столбец
     * @param col_idx индекс столбца
     * @return true если столбец уже покрыт, false иначе
     */
    bool isCovered(int col_idx) const;

    /**
     * @brief Проверяет, все ли столбцы раскрыты
     * @return true если нет покрытых столбцов
     */
    bool isFullyUncovered() const;

    /**
     * @brief Возвращает количество столбцов в матрице
     */
    int countColumns() const;

#ifdef DEBUG        
    /**
     * @brief Отладочная печать матрицы (доступна только в Debug)
     */
    void print() const;
#endif // DEBUG

private:

    void cover_impl(Node* col);
    void uncover_impl(Node* col);
    bool search_impl(std::vector<int>& solution);

    // Откат всех cover (для cleanup)
    void rollbackAll();

    Node* getColumn(int idx) const;

    /**
     * @brief Проверяет, покрыт ли столбец (внутренняя, по указателю)
     * @param col указатель на узел-заголовок столбца
     * @return true если столбец уже покрыт, false иначе
     */
    bool isCovered(Node* col) const;

    /**
     * @brief Выбирает столбец с минимальным количеством единиц (эвристика)
     * @return указатель на выбранный столбец
     */
    Node* chooseColumn() const;

    /**
     * @brief Полная очистка всей памяти, используемой матрицей
     */
    void cleanup();

    Node* root;
    std::vector<Node*> columns;
    std::stack<Node*> cover_stack;  // стек вызовов cover (для проверки порядка)

};