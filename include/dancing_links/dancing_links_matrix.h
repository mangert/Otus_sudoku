#pragma once
#define DEBUG

#include <vector>

class DancingLinksMatrix {
private:
    // Forward declarations
    struct Node;

public:
    explicit DancingLinksMatrix(int num_columns);
    ~DancingLinksMatrix();

    DancingLinksMatrix(const DancingLinksMatrix&) = delete;
    DancingLinksMatrix& operator=(const DancingLinksMatrix&) = delete;

    void addRow(const std::vector<int>& col_indices, int row_id);

    void cover(Node* col);
    void uncover(Node* col);

#ifdef DEBUG

    Node* getColumn(int idx) const;
    void print() const;

#endif // DEBUG

private:

    Node* root;
    std::vector<Node*> columns;
    std::vector<Node*> cover_stack;  // стек вызовов cover (для проверки порядка)

    
    // Откат всех cover (для cleanup)
    void rollbackAll();
    // Проверка состояния
    bool isFullyUncovered() const;

    void cleanup();
};