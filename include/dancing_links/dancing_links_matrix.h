#pragma once
#define DEBUG

#include <vector>
#include <stack>

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
    
    void cover(int col_idx);
    void uncover(int col_idx);

    bool DancingLinksMatrix::search(std::vector<int>& solution);
    
    bool isCovered(int col_idx) const;
    int getNumColumns() const;

#ifdef DEBUG        
    void print() const;
#endif // DEBUG

private:

    Node* root;
    std::vector<Node*> columns;
    std::stack<Node*> cover_stack;  // стек вызовов cover (для проверки порядка)

    void cover(Node* col);
    void uncover(Node* col);    

    // Откат всех cover (для cleanup)
    void rollbackAll();

    Node* getColumn(int idx) const;
    // Проверка состояния
    bool isFullyUncovered() const;
    bool isCovered(Node* col) const;
    // Полная очистка
    void cleanup();

    Node* chooseColumn() const;
};