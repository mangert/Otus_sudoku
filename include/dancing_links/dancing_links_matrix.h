#pragma once
#define DEBUG

#include <vector>
#include <stack>
#include <span>
#include <optional>

class DancingLinksMatrix final {
private:
    // Forward declarations
    struct Node;

public:
    explicit DancingLinksMatrix(int num_columns);
    ~DancingLinksMatrix();

    DancingLinksMatrix(const DancingLinksMatrix&) = delete;
    DancingLinksMatrix& operator=(const DancingLinksMatrix&) = delete;

    // Конструктор перемещения
    DancingLinksMatrix(DancingLinksMatrix&& other) noexcept;        

    // Оператор перемещения
    DancingLinksMatrix& operator=(DancingLinksMatrix&& other) noexcept;

    void addRow(std::span<const int> col_indices, int row_id);
    
    void cover(int col_idx);
    void uncover(int col_idx);

    std::optional<std::vector<int>> search(int expected_solution_size = 0);
    
    // Проверка состояния    
    bool isCovered(int col_idx) const;
    bool isFullyUncovered() const;
    int countColumns() const;

#ifdef DEBUG        
    void print() const;
#endif // DEBUG

private:
    
    void cover_impl(Node* col);
    void uncover_impl(Node* col);
    bool search_impl(std::vector<int>& solution);

    // Откат всех cover (для cleanup)
    void rollbackAll();

    Node* getColumn(int idx) const;
    // Проверка состояния    
    bool isCovered(Node* col) const;    

    //выбор столбца с минимальным size
    Node* chooseColumn() const;
    
    // Полная очистка
    void cleanup();

    Node* root;
    std::vector<Node*> columns;
    std::stack<Node*> cover_stack;  // стек вызовов cover (для проверки порядка)
    
};