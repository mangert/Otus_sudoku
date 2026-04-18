#pragma once
#define DEBUG

#include <vector>
#include <stack>
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

    void addRow(const std::vector<int>& col_indices, int row_id);
    
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
    
    void cover(Node* col);
    void uncover(Node* col);
    bool search_recursive(std::vector<int>& solution);

    // Откат всех cover (для cleanup)
    void rollbackAll();

    Node* getColumn(int idx) const;
    // Проверка состояния    
    bool isCovered(Node* col) const;
    // Полная очистка
    void cleanup();

    Node* chooseColumn() const;

    Node* root;
    std::vector<Node*> columns;
    std::stack<Node*> cover_stack;  // стек вызовов cover (для проверки порядка)
    
};