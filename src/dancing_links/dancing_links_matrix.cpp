#include "dancing_links/dancing_links_matrix.h"
#include <cassert>
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
    
    if (!cover_stack.empty()) {
        std::cerr << "Warning: cleaning up with " << cover_stack.size()
            << " uncovered columns. Forcing rollback.\n";
        rollbackAll();
    }
    
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

void DancingLinksMatrix::cover(Node* col) {

    assert(col != nullptr && "cover: col is nullptr");
    assert(col->isHeader() && "cover: col is not a header");
    assert(std::find(cover_stack.begin(), cover_stack.end(), col) == cover_stack.end()
        && "cover: column already covered");    

    // Добавляем в стек
    cover_stack.emplace_back(col);
    // 1. Удаляем столбец из горизонтального списка заголовков
    col->right->left = col->left;
    col->left->right = col->right;

    // 2. Обходим все строки в этом столбце
    for (Node* i = col->down; i != col; i = i->down) {
        // 3. Для каждой строки обходим все её узлы
        for (Node* j = i->right; j != i; j = j->right) {
            // 4. Удаляем узел j из его столбца
            j->down->up = j->up;
            j->up->down = j->down;
            // Уменьшаем счётчик в столбце j
            --j->column->size;
        }
    }
}

void DancingLinksMatrix::uncover(Node* col) {
    
    assert(col != nullptr && "uncover: col is nullptr");
    assert(!cover_stack.empty() && "uncover: no columns to uncover");
    assert(cover_stack.back() == col && "uncover: column order mismatch (LIFO required)");
    

    // Убираем из стека
    cover_stack.pop_back();
    
    // Обход в обратном порядке (снизу вверх, справа налево)
    for (Node* i = col->up; i != col; i = i->up) {
        for (Node* j = i->left; j != i; j = j->left) {
            ++j->column->size;
            j->down->up = j;
            j->up->down = j;
        }
    }
    col->right->left = col;
    col->left->right = col;


}
#ifdef DEBUG
DancingLinksMatrix::Node* DancingLinksMatrix::getColumn(int idx) const { 
    return columns[idx]; 
}

void DancingLinksMatrix::rollbackAll() {
    
    while (!cover_stack.empty()) {
        uncover(cover_stack.back());
    }
}

bool DancingLinksMatrix::isFullyUncovered() const {
    return cover_stack.empty();
};

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

    // Показываем состояние стека
    if (!cover_stack.empty()) {
        std::cout << "\nCover stack (top last): ";
        for (Node* c : cover_stack) {
            std::cout << c->id << " ";
        }
        std::cout << "\n";
    }

    std::cout << "===========================\n" << std::endl;
}
#endif // DEBUG

