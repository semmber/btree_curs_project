#include "BTree.h"

using namespace std;

// -------------BTreeNode-------------

// Конструктор: инициализация узла степени t1 и флага leaf1
BTreeNode::BTreeNode(int t1, bool leaf1) : t(t1), leaf(leaf1), n(0) {
    keys = new int[2 * t - 1];        // Узел может хранить до 2t-1 ключей
    C = new BTreeNode*[2 * t];        // И до 2t потомков
}

BTreeNode::~BTreeNode() {
    delete[] keys;
    delete[] C;
}

// Обход (Traversal) всех ключей в поддереве
void BTreeNode::traverse() {
    // Проход по всем ключам и дочерним поддеревьям
    int i;
    for (i = 0; i < n; ++i) {
        if (!leaf)
            C[i]->traverse();       // Обход дочернего узла
        cout << " " << keys[i];   // Вывод ключа
    }
    if (!leaf)
        C[i]->traverse();           // Обход последнего дочернего узла
}

// Поиск ключа k в поддереве этого узла
BTreeNode* BTreeNode::search(int k) {
    int i = 0;
    // Найти первый ключ >= k
    while (i < n && keys[i] < k)
        ++i;

    // Если ключ найден в этом узле
    if (i < n && keys[i] == k)
        return this;

    // Если узел листовой, ключ отсутствует
    if (leaf)
        return nullptr;

    // Иначе переходим к соответствующему потомку
    return C[i]->search(k);
}

// Вставка ключа k в узел, который гарантированно не полный
void BTreeNode::insertNonFull(int k) {
    int i = n - 1;

    if (leaf) {
        // Сдвигаем ключи вправо, пока не найдём место для k
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            --i;
        }
        keys[i + 1] = k;
        ++n;
    } else {
        // Находим дочерний узел для спуска
        while (i >= 0 && keys[i] > k)
            --i;
        ++i;  // Переход к правому потомку

        // Если дочерний узел полный, нужно его разделить
        if (C[i]->n == 2 * t - 1) {
            splitChild(i, C[i]);
            // После разделения средний ключ поднялся вверх
            if (keys[i] < k)
                ++i;
        }
        C[i]->insertNonFull(k);
    }
}

// Разбиение полного дочернего узла y на два: y и новый узел z
void BTreeNode::splitChild(int i, BTreeNode* y) {
    // Создаём новый узел z, который будет содержать t-1 ключей из y
    BTreeNode* z = new BTreeNode(y->t, y->leaf);
    z->n = t - 1;

    // Переносим ключи из y в z
    for (int j = 0; j < t - 1; ++j)
        z->keys[j] = y->keys[j + t];

    // Если y не лист, переносим и его потомков
    if (!y->leaf) {
        for (int j = 0; j < t; ++j)
            z->C[j] = y->C[j + t];
    }

    // Уменьшаем количество ключей в y
    y->n = t - 1;

    // Сдвигаем указатели в текущем узле, чтобы вставить z
    for (int j = n; j >= i + 1; --j)
        C[j + 1] = C[j];
    C[i + 1] = z;

    // Сдвигаем ключи в текущем узле и вставляем средний ключ y
    for (int j = n - 1; j >= i; --j)
        keys[j + 1] = keys[j];
    keys[i] = y->keys[t - 1];
    ++n;
}
// Диапазонный поиск: собирает все ключи в диапазоне [k1, k2]
void BTreeNode::rangeSearch(int k1, int k2, std::vector<int>& result) {
    int i = 0;
    // Находим первый ключ >= k1
    while (i < n && keys[i] < k1)
        ++i;
    // Проходим по ключам и поддеревьям
    for (; i < n && keys[i] <= k2; ++i) {
        if (!leaf)
            C[i]->rangeSearch(k1, k2, result);
        result.push_back(keys[i]);
    }
    // Проверка последнего потомка
    if (!leaf)
        C[i]->rangeSearch(k1, k2, result);
}

// -------------BTree-------------

// Конструктор: создаём пустое дерево
BTree::BTree(int _t) : root(nullptr), t(_t) {}

BTree::~BTree() {
    delete root;
}

// Обход всего дерева
void BTree::traverse() {
    if (root)
        root->traverse();
}

// Поиск ключа k в дереве
BTreeNode* BTree::search(int k) {
    return (root == nullptr) ? nullptr : root->search(k);
}

// Вставка ключа k в дерево
void BTree::insert(int k) {
    if (!root) {
        // Если дерево пустое, создаём корневой узел-лист
        root = new BTreeNode(t, true);
        root->keys[0] = k;
        root->n = 1;
    } else {
        // Если корень полный, разделяем его
        if (root->n == 2 * t - 1) {
            BTreeNode* s = new BTreeNode(t, false);
            s->C[0] = root;
            s->splitChild(0, root);

            // Определяем, в какой из двух узлов вставлять новый ключ
            int i = 0;
            if (s->keys[0] < k)
                ++i;
            s->C[i]->insertNonFull(k);

            root = s;  // Новый корень
        } else {
            // Вставляем в неполный корень
            root->insertNonFull(k);
        }
    }
}
// Обёртка для диапазонного поиска
vector<int> BTree::rangeSearch(int k1, int k2) {
    vector<int> result;
    if (root)
        root->rangeSearch(k1, k2, result);
    return result;
}