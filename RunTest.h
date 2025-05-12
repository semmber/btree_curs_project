#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

using namespace std;

template <typename TreeType>
void runTest(const string& name,
             TreeType& tree,
             const vector<int>& data,
             int M,
             int R,
             mt19937& rng,
             uniform_int_distribution<int>& dist) {
    cout << "--- Testing " << name << " ---\n";

    // 1. Вставка
    auto t1 = chrono::high_resolution_clock::now();
    for (int x : data) tree.insert(x);
    auto t2 = chrono::high_resolution_clock::now();
    cout << "Insertion of " << data.size()
         << " keys: "
         << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count()
         << " ms\n";

    // 2. Поиск существующих ключей
    vector<int> search_keys(M);
    uniform_int_distribution<int> idx_dist(0, data.size() - 1);
    for (int i = 0; i < M; ++i)
        search_keys[i] = data[idx_dist(rng)];

    t1 = chrono::high_resolution_clock::now();
    for (int x : search_keys) tree.search(x);
    t2 = chrono::high_resolution_clock::now();
    cout << "Search of " << M
         << " keys: "
         << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count()
         << " ms\n";

    // 3. Диапазонный поиск
    t1 = chrono::high_resolution_clock::now();
    for (int i = 0; i < R; ++i) {
        int a = dist(rng), b = dist(rng);
        tree.rangeSearch(min(a,b), max(a,b));
    }
    t2 = chrono::high_resolution_clock::now();
    cout << "Range search of " << R
         << " ranges: "
         << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count()
         << " ms\n\n";
}