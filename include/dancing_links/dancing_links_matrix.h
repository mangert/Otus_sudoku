#pragma once
#include <vector>

class DancingLinksMatrix {
private:
    struct ColumnNode;
    struct DataNode;

    ColumnNode* root;
    std::vector<ColumnNode*> columns;
    std::vector<DataNode*> all_nodes;

public:
    explicit DancingLinksMatrix(int num_columns);    
    
    // Запрецаем копирования и перемещения
    DancingLinksMatrix(const DancingLinksMatrix&) = delete;
    DancingLinksMatrix& operator=(const DancingLinksMatrix&) = delete;
    DancingLinksMatrix(DancingLinksMatrix&&) = delete;
    DancingLinksMatrix& operator=(DancingLinksMatrix&&) = delete;

    ~DancingLinksMatrix();

    void addRow(const std::vector<int>& col_indices, int row_id);
    
    void cover(ColumnNode* col);
    void uncover(ColumnNode* col);

private:
    void cleanup();
};