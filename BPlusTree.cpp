#include "BPlusTree.h"

// -------------BPlusTreeNode-------------

BPlusTreeNode::BPlusTreeNode(int t1, bool leaf1) : t(t1), n(0), leaf(leaf1), next(nullptr) {
    keys = new int[2*t - 1];
    C    = new BPlusTreeNode*[2*t];
}
BPlusTreeNode::~BPlusTreeNode() {
    delete[] keys;
    delete[] C;
}

// Поиск ключа k
BPlusTreeNode* BPlusTreeNode::search(int k) const {
    int i = 0;
    while (i < n && keys[i] < k) ++i;
    if (leaf) {
        if (i < n && keys[i] == k) return const_cast<BPlusTreeNode*>(this);
        return nullptr;
    }
    return C[i]->search(k);
}

// Вставка ключа, узел гарантированно неполный
void BPlusTreeNode::insertNonFull(int k) {
    int i = n - 1;
    if (leaf) {
        while (i >= 0 && keys[i] > k) {
            keys[i+1] = keys[i];
            --i;
        }
        keys[i+1] = k;
        ++n;
    } else {
        while (i >= 0 && keys[i] > k) --i;
        ++i;
        if (C[i]->n == 2*t - 1) {
            splitChild(i, C[i]);
            if (keys[i] < k) ++i;
        }
        C[i]->insertNonFull(k);
    }
}

// Разделение полного узла y
void BPlusTreeNode::splitChild(int i, BPlusTreeNode* y) {
    BPlusTreeNode* z = new BPlusTreeNode(y->t, y->leaf);
    z->n = t - 1;
    // Перенос ключей
    for (int j = 0; j < t-1; ++j)
        z->keys[j] = y->keys[j + t];
    // Если внутренний узел, переносим дочерние указатели
    if (!y->leaf) {
        for (int j = 0; j < t; ++j)
            z->C[j] = y->C[j + t];
    }
    y->n = t - 1;
    // Вставка нового узла в массив детей
    for (int j = n; j >= i+1; --j)
        C[j+1] = C[j];
    C[i+1] = z;
    // Поднятие ключа
    for (int j = n-1; j >= i; --j)
        keys[j+1] = keys[j];
    // Для B+ дерева продвижение первого ключа правого узла
    keys[i] = z->keys[0];
    ++n;

    // Если лист, обновляем указатели next
    if (y->leaf) {
        z->next = y->next;
        y->next = z;
    }
}

// Сбор ключей в диапазоне [k1, k2]
void BPlusTreeNode::rangeSearch(int k1, int k2, vector<int>& result) const {
    int i = 0;
    // Определяем индекс i: первый ключ >= k1
    while (i < n && keys[i] < k1) ++i;
    if (leaf) {
        // В листе: собираем ключи в диапазоне
        for (; i < n; ++i) {
            if (keys[i] > k2) return;  // ранний выход
            result.push_back(keys[i]);
        }
        // Переходим по связному списку листов
        const BPlusTreeNode* node = next;
        while (node) {
            for (int j = 0; j < node->n; ++j) {
                if (node->keys[j] > k2) return;  // выходим, как только вышли за диапазон
                result.push_back(node->keys[j]);
            }
            node = node->next;
        }
    } else {
        // Иначе спускаемся в соответствующее поддерево
        C[i]->rangeSearch(k1, k2, result);
    }
}

// -------------BPlusTree-------------

void BPlusTree::insert(int k) {
    if (!root) {
        root = new BPlusTreeNode(t, true);
        root->keys[0] = k;
        root->n = 1;
        return;
    }
    if (root->n == 2*t - 1) {
        BPlusTreeNode* s = new BPlusTreeNode(t, false);
        s->C[0] = root;
        s->splitChild(0, root);
        int i = (s->keys[0] < k) ? 1 : 0;
        s->C[i]->insertNonFull(k);
        root = s;
    } else {
        root->insertNonFull(k);
    }
}

BPlusTreeNode* BPlusTree::search(int k) const {
    return root ? root->search(k) : nullptr;
}

vector<int> BPlusTree::rangeSearch(int k1, int k2) const {
    vector<int> result;
    if (root) root->rangeSearch(k1, k2, result);
    return result;
}

void BPlusTree::traverse() const {
    // Обход всех ключей через листы
    const BPlusTreeNode* node = root;
    if (!node) return;
    while (!node->leaf) node = node->C[0];
    while (node) {
        for (int i = 0; i < node->n; ++i)
            cout << ' ' << node->keys[i];
        node = node->next;
    }
}
