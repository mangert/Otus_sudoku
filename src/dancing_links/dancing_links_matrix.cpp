#include "dancing_links/dancing_links_matrix.h"
#include <iostream>

// ============================================================================
// Определения структур
// ============================================================================

struct DancingLinksMatrix::Node {
    Node* left, * right, * up, * down;
    Node* column;
    int size;
    int id;

    // Конструктор для заголовка столбца
    explicit Node(int col_index)
        : left(this), right(this), up(this), down(this)
        , column(this), size(0), id(col_index) {}

    // Конструктор для обычного узла
    Node(int row_id, Node* col_header)
        : left(this), right(this), up(this), down(this)
        , column(col_header), size(-1), id(row_id) {}

    bool isHeader() const { return column == this; }
};

// ============================================================================
// Реализация методов класса
// ============================================================================

DancingLinksMatrix::DancingLinksMatrix(int num_columns) {
    root = new Node(-1);
    root->column = nullptr;  // корень не является заголовком

    columns.reserve(num_columns);

    Node* prev = root;
    for (int i = 0; i < num_columns; ++i) {
        Node* col = new Node(i);
        columns.emplace_back(col);

        col->left = prev;
        col->right = prev->right;
        prev->right->left = col;
        prev->right = col;

        prev = col;
    }
}

void DancingLinksMatrix::cleanup() {
    // 1. Удаляем все обычные узлы (обходим каждый столбец)
    Node* col = root->right;
    while (col != root) {
        Node* node = col->down;
        while (node != col) {
            Node* to_delete = node;
            node = node->down;  // сохраняем следующий перед удалением
            delete to_delete;
        }
        col = col->right;
    }

    // 2. Удаляем все заголовки столбцов
    Node* cur = root->right;
    while (cur != root) {
        Node* to_delete = cur;
        cur = cur->right;  // сохраняем следующий перед удалением
        delete to_delete;
    }

    // 3. Удаляем корень
    delete root;
    root = nullptr;
}

DancingLinksMatrix::~DancingLinksMatrix() {
    cleanup();
}

void DancingLinksMatrix::addRow(const std::vector<int>& col_indices, int row_id) {
    if (col_indices.empty()) return;

    std::vector<Node*> nodes;
    nodes.reserve(col_indices.size());

    for (int col_idx : col_indices) {
        Node* col = columns[col_idx];
        Node* node = new Node(row_id, col);
        nodes.emplace_back(node);

        node->up = col->up;
        node->down = col;
        col->up->down = node;
        col->up = node;

        ++col->size;
    }

    for (size_t i = 0; i < nodes.size(); ++i) {
        Node* left = nodes[i];
        Node* right = nodes[(i + 1) % nodes.size()];
        left->right = right;
        right->left = left;
    }
}

void DancingLinksMatrix::print() const {
    std::cout << "=== Dancing Links Matrix ===\n";
    std::cout << "Columns (index:size): ";

    Node* col = root->right;
    while (col != root) {
        std::cout << col->id << ":" << col->size << " ";
        col = col->right;
    }
    std::cout << "\n\n";

    col = root->right;
    while (col != root) {
        std::cout << "Column " << col->id << " (size=" << col->size << "): ";
        Node* node = col->down;
        if (node == col) {
            std::cout << "(empty)";
        }
        else {
            while (node != col) {
                std::cout << "row" << node->id << " ";
                node = node->down;
            }
        }
        std::cout << "\n";
        col = col->right;
    }
    std::cout << "===========================\n" << std::endl;
}