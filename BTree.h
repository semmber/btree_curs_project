#include <iostream>
#include <vector>
class BTreeNode {
    int *keys;            // Массив ключей
    int t;                // Минимальная степень дерева
    BTreeNode **C;        // Массив указателей на дочерние узлы
    int n;                // Текущее количество ключей в узле
    bool leaf;            // Является ли узел листом

public:
    BTreeNode(int t1, bool leaf1);

    ~BTreeNode();

    void traverse();

    BTreeNode *search(int k);

    void insertNonFull(int k);

    void splitChild(int i, BTreeNode *y);

    void rangeSearch(int k1, int k2, std::vector<int> &result);

    friend class BTree;
};

class BTree {
    BTreeNode *root;  // Указатель на корень дерева
    int t;            // Минимальная степень дерева

public:
    explicit BTree(int _t);

    ~BTree();

    void traverse();

    BTreeNode *search(int k);

    void insert(int k);

    std::vector<int> rangeSearch(int k1, int k2);
};