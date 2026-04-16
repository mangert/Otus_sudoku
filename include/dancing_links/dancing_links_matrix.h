#pragma once

#include <vector>

class DancingLinksMatrix {
public:
    explicit DancingLinksMatrix(int num_columns);
    ~DancingLinksMatrix();

    DancingLinksMatrix(const DancingLinksMatrix&) = delete;
    DancingLinksMatrix& operator=(const DancingLinksMatrix&) = delete;

    void addRow(const std::vector<int>& col_indices, int row_id);
    void print() const;

private:
    // Forward declarations (только имена структур)
    struct Node;

    Node* root;
    std::vector<Node*> columns;

    void cleanup();
};