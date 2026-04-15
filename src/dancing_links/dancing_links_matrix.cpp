#include "dancing_links/dancing_links_matrix.h"

// Определения структур 
struct DancingLinksMatrix::ColumnNode {
    ColumnNode* left, * right;
    DataNode* up, * down;
    int size;
    int col_index;

    ColumnNode(int idx)
        : left(this), right(this), up(nullptr), down(nullptr), size(0), col_index(idx) {}

    ~ColumnNode() = default;
};

struct DancingLinksMatrix::DataNode {
    
    DataNode* left, * right, * up, * down;
    ColumnNode* column;
    int row_id;

    DataNode(int row)
        : left(this), right(this), up(this), down(this), column(nullptr), row_id(row) {}

    ~DataNode() = default;
};

// Теперь определения методов класса
DancingLinksMatrix::DancingLinksMatrix(int num_columns) {
    
    root = new ColumnNode(-1);  // -1 означает "не столбец, а корень"

    // Создаём заголовки столбцов
    ColumnNode* prev = root;
    for (int i = 0; i != num_columns; ++i) {
        ColumnNode* col = new ColumnNode(i);
        columns.push_back(col);

        // Вставляем в горизонтальный список справа от prev
        col->left = prev;
        col->right = prev->right;
        prev->right->left = col;
        prev->right = col;

        prev = col;
    }
}

DancingLinksMatrix::~DancingLinksMatrix() {
    cleanup();
}

void DancingLinksMatrix::addRow(const std::vector<int>& col_indices, int row_id) {
    if (col_indices.empty()) return;

    // Создаём узлы для каждого столбца в этой строке
    std::vector<DataNode*> nodes;
    for (int col_idx : col_indices) {
        ColumnNode* col = columns[col_idx];
        DataNode* node = new DataNode(row_id);
        node->column = col;
        nodes.push_back(node);
        all_nodes.push_back(node);

        // Увеличиваем счётчик в столбце
        ++col->size;

        // Вставляем вертикально (в конец столбца)
        node->up = col->up;
        node->down = col;
        col->up->down = node;
        col->up = node;
}

void DancingLinksMatrix::cleanup() {
    
    // Удаляем все DataNode
    for (DataNode* node : all_nodes) {
        if(node) 
            delete node;
    }
    all_nodes.clear();

    // Удаляем все ColumnNode
    for (ColumnNode* col : columns) {
        if(col)
            delete col;
    }
    columns.clear();

    delete root;
    root = nullptr;
}