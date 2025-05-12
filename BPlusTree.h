#include <iostream>
#include <vector>

using namespace std;

class BPlusTreeNode {
    int* keys;
    int  t;
    BPlusTreeNode** C;
    int  n;
    bool leaf;
    BPlusTreeNode* next;
public:
    BPlusTreeNode(int t1, bool leaf1);
    ~BPlusTreeNode();
    BPlusTreeNode* search(int k) const;
    void insertNonFull(int k);
    void splitChild(int i, BPlusTreeNode* y);
    void rangeSearch(int k1, int k2, vector<int>& result) const;
    friend class BPlusTree;
};

class BPlusTree {
    BPlusTreeNode *root;
    int t;
public:
    explicit BPlusTree(int _t) : root(nullptr), t(_t) {}
    ~BPlusTree() { delete root; }
    void insert(int k);
    BPlusTreeNode *search(int k) const;
    vector<int> rangeSearch(int k1, int k2) const;
    void traverse() const;
};
