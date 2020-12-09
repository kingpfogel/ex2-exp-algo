#include <omp.h>
#include <algorithm>
#include <atomic>
#include <cassert>
#include <cstring>
#include <chrono>
#include <iostream>
#include <mutex>
#include <optional>
#include <random>
#include <vector>

constexpr int M = 1 << 26;

// This function turns a hash into an index in [0, M).
// This computes h % M but since M is a power of 2,
// we can compute the modulo by using a bitwise AND to cut off the leading binary digits.
// As hash, we just use the integer key directly.
int hash_to_index(int h) {
    return h & (M - 1);
}

// Note: this table only implements put() and no values.
// It also does not store the number of present keys (yet?).

struct concurrent_chaining_table {
    struct chain {
        chain *next;
        unsigned int key;
    };

    concurrent_chaining_table()
            : heads{new chain *[M]{}}, mutexes{new std::mutex[M]} { }

    // Note: to simply the implementation, destructors and copy/move constructors are missing.
    ~concurrent_chaining_table() {
        delete[] heads; delete[] mutexes;
    }

    void put(unsigned int k) {
        auto idx = hash_to_index(k);

        // Take a mutex while accessing the buckets.
        std::lock_guard lock{mutexes[idx]};
        auto p = heads[idx];
        if(!p) {
            heads[idx] = new chain{nullptr, k};
            counter.fetch_add(1);

            return;
        }

        while(true) {
            assert(p);

            if(p->key == k)
                return;

            if(!p->next) {
                p->next = new chain{nullptr, k};
                counter.fetch_add(1);
                return;
            }

            p = p->next;
        }
    }
    std::atomic<int> counter = 0;
    chain **heads = nullptr;
    std::mutex *mutexes = nullptr;
};

size_t count_substrings(const std::vector<uint8_t> &input) {
    int c1 = 0;
    int c2 = 1;
    int c3 = 2;
    int c4 = 3;

    std::atomic<int> sum = 0;
    for(int i = 0; i < input.size(); ++i){
        std::string str  = std::bitset<8>(input[i]).to_string();
        auto hash_table = new concurrent_chaining_table();
        #pragma omp parallel for
        for(int niklas = 0; niklas < 5; ++niklas){
            unsigned int key =   (static_cast<unsigned int>(str[c1+niklas]) << 24)
                                 | (static_cast<unsigned int>(str[c2+niklas]) << 16)
                                 | (static_cast<unsigned int>(str[c3+niklas]) << 8)
                                 |  static_cast<unsigned int>(str[c4+niklas]);

            hash_table->put(key);
        }
        sum.fetch_add(hash_table->counter);
        delete hash_table;
    }

    return sum;
}

int main() {
    std::vector<uint8_t> input;
    input.resize(1 << 1);

    std::mt19937 prng{42};

    std::cerr << "Generating random data..." << std::endl;

    std::uniform_int_distribution<int> distrib{0, 255};
    for(size_t i = 0; i < input.size(); ++i)
        input[i] = distrib(prng);

    std::cerr << "Running benchmark..." << std::endl;

    auto bench_start = std::chrono::high_resolution_clock::now();
    auto result = count_substrings(input);
    auto bench_elapsed = std::chrono::high_resolution_clock::now() - bench_start;

    std::cout << "num_threads: " << omp_get_max_threads() << std::endl;
    std::cout << "result: " << result << std::endl;
    std::cout << "time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(bench_elapsed).count()
              << " # ms" << std::endl;

}
