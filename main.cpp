#include "BTree.h"
#include "BPlusTree.h"
#include "RunTest.h"

using namespace std;

int main() {
    const int N = 100000; // Количество данных
    const int M = 10000; // Количество ключей для поиска
    const int R = 1000; // Количество диапазональных поисков

    // Подготавливаем рандомизированные данные
    vector<int> data(N);
    mt19937 rng(123);
    uniform_int_distribution<int> dist(1, 1000000);
    for (int& x : data) x = dist(rng);

    // Тестируем B-дерево
    BTree btree(50);
    runTest("B-Tree", btree, data, M, R, rng, dist);

    // Тестируем B+-дерево
    BPlusTree bptree(50);
    runTest("B+ Tree", bptree, data, M, R, rng, dist);

    return 0;
}
