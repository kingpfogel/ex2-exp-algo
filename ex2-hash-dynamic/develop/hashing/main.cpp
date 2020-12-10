#include <algorithm>
#include <cassert>
#include <cstring>
#include <chrono>
#include <iostream>
#include <optional>
#include <random>
#include <unordered_map>
#include <vector>

int num_insertions = 1 << 26;

// This hash table uses linear probing.
struct static_table {
	static constexpr const char *name = "static";

	struct cell {
		int key;
		int value;
		bool valid = false;
	};

	static_table(float max_fill)
	: cells{new cell[m]{}} {
		(void)max_fill; // Ignore the fill factor.
	}

	// Note: to simply the implementation, destructors and copy/move constructors are missing.

	// This function turns a hash into an index in [0, m).
	// This computes h % m but since m is a power of 2,
	// we can compute the modulo by using a bitwise AND to cut off the leading binary digits.
	// As hash, we just use the integer key directly.
	int hash_to_index(int h) {
		return h & (m - 1);
	}

	void put(int k, int v) {
		int i = 0;

		while(true) {
			assert(i < m);

			auto idx = hash_to_index(k + i);
			auto &c = cells[idx];

			if(!c.valid) {
				c.key = k;
				c.value = v;
				c.valid = true;
				return;
			}

			if(c.key == k) {
				c.value = v;
				return;
			}

			++i;
		}
	}

	std::optional<int> get(int k) {
		int i = 0;

		while(true) {
			assert(i < m);

			auto idx = hash_to_index(k + i);
			auto &c = cells[idx];

			if(!c.valid)
				return std::nullopt;

			if(c.key == k)
				return c.value;

			++i;
		}
	}

	int m = 2 * num_insertions;
	cell *cells = nullptr;
};

// For comparsion, an implementation that uses std::unordered_map.
// This is apples-to-oranges since std::unordered_map does not respect our fill factor.
struct stl_table {
	static constexpr const char *name = "stl";

	stl_table(float max_fill) {
		(void)max_fill; // Ignore the fill factor.
	}

	void put(int k, int v) {
		map[k] = v;
	}

	std::optional<int> get(int k) {
		auto it = map.find(k);
		if(it == map.end())
			return std::nullopt;
		return it->second;
	}

	std::unordered_map<int, int> map;
};

// This hash table uses linear probing.
struct dynamic_table {
    static constexpr const char *name = "dynamic";

    struct cell {
        int key;
        int value;
        bool valid = false;
    };

    dynamic_table(float max_fill)
            : cells{new cell[m]{}} {
        this->max_fill = max_fill;
    }

    void resize(){
        cell *tmp = new cell[m];
        std::copy(cells, cells + m, tmp);
        delete[] cells;
        cells = nullptr;
        int old_m = m;
        m=m<<1;
        nInserts = 0;
        cells = new cell[m];
        for(int i = 0; i<old_m; ++i){
            auto &c = tmp[i];
            if(c.valid){
                put(c.key, c.value);
            }
        }
    }

    int hash_to_index(int h) {
        return h & (m - 1);
    }

    void put(int k, int v) {
        int i = 0;
        while(true) {
            assert(i < m);

            auto idx = hash_to_index(k + i);
            auto &c = cells[idx];

            if(!c.valid) {
                c.key = k;
                c.value = v;
                c.valid = true;
                ++nInserts;
                if(nInserts>=(max_fill*m)){
                    resize();
                }
                return;
            }

            if(c.key == k) {
                c.value = v;
                return;
            }

            ++i;
        }
    }

    std::optional<int> get(int k) {
        int i = 0;

        while(true) {
            assert(i < m);

            auto idx = hash_to_index(k + i);
            auto &c = cells[idx];

            if(!c.valid)
                return std::nullopt;

            if(c.key == k)
                return c.value;

            ++i;
        }
    }

    int m = 8;
    int nInserts = 0;
    float max_fill = 1.0;
    cell *cells = nullptr;
};

struct static_scaling_table {
    static constexpr const char *name = "static_scaling";

    struct cell {
        int key;
        int value;
        bool valid = false;
    };

    static_scaling_table(float max_fill)
            : cells{new cell[m]{}} {
        (void)max_fill; // Ignore the fill factor.
    }

    // Note: to simply the implementation, destructors and copy/move constructors are missing.
    int hash_to_index(int h) {
        unsigned int x = 1 << (31-27);
        return h/x;
    }

    void put(int k, int v) {
        int i = 0;
        auto idx = hash_to_index(k);
        while(true) {
            assert(i<m);
            if(idx == m){
                idx = 0;
            }
            auto &c = cells[idx];
            if(!c.valid) {
                c.key = k;
                c.value = v;
                c.valid = true;
                return;
            }
            if(c.key == k) {
                c.value = v;
                return;
            }
            ++idx;
            ++i;
        }
    }

    std::optional<int> get(int k) {
        int i = 0;
        auto idx = hash_to_index(k);
        while(true) {
            assert(i<m);
            if(idx == m){
                idx = 0;
            }

            auto &c = cells[idx];

            if(!c.valid)
                return std::nullopt;

            if(c.key == k)
                return c.value;
            ++idx;
            ++i;
        }
    }

    int m = 2 * num_insertions;
    cell *cells = nullptr;
};

struct dynamic_scaling_table {
    static constexpr const char *name = "dynamic_scaling";

    struct cell {
        int key;
        int value;
        bool valid = false;
    };

    dynamic_scaling_table(float max_fill)
            : cells{new cell[m]{}} {
        this->max_fill = max_fill;
    }

    int hash_to_index(int h) {
        unsigned int x = 1 << (31-tsize);
        return h/x;
    }

    void resize(){
        tsize+=1;
        cell *tmp = new cell[m];
        std::copy(cells, cells + m, tmp);
        delete[] cells;
        cells = nullptr;
        int old_m = m;
        m=m<<1;
        nInserts = 0;
        cells = new cell[m];
        for(int i = 0; i<old_m; ++i){
            auto &c = tmp[i];
            if(c.valid){
                put(c.key, c.value);
            }
        }
    }

    void put(int k, int v) {
        int i = 0;
        auto idx = hash_to_index(k);

        while(true) {
            assert(i<m);
            if(idx == m){
                idx = 0;
            }

            auto &c = cells[idx];

            if(!c.valid) {
                c.key = k;
                c.value = v;
                c.valid = true;
                ++nInserts;
                if(nInserts>=(max_fill*m)){
                    resize();
                }
                return;
            }

            if(c.key == k) {
                c.value = v;
                return;
            }
            ++idx;
            ++i;
        }
    }

    std::optional<int> get(int k) {
        int i = 0;
        auto idx = hash_to_index(k);

        while(true) {
            assert(i<m);
            if(idx == m){
                idx = 0;
            }
            auto &c = cells[idx];

            if(!c.valid)
                return std::nullopt;

            if(c.key == k)
                return c.value;
            ++idx;
            ++i;
        }
    }
    int tsize = 3;
    int m = 8;
    int nInserts = 0;
    float max_fill = 1.0;
    cell *cells = nullptr;
};


// Helper function to perform a microbenchmark.
// You should not need to touch this.
template<typename Algo>
void microbenchmark(float max_fill) {
	Algo table{max_fill};

	std::mt19937 prng{42};
	std::uniform_int_distribution<int> distrib;

	std::cerr << "Running microbenchmark..." << std::endl;

	auto start = std::chrono::high_resolution_clock::now();
	int nv = 1;
	for(int i = 0; i < num_insertions; ++i)
		table.put(distrib(prng), nv++);
	auto t = std::chrono::high_resolution_clock::now() - start;

	std::cout << "algo: " << Algo::name << std::endl;
	std::cout << "max_fill: " << max_fill << std::endl;
	std::cout << "time: "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(t).count()
			<< " # ms" << std::endl;

    std::cout << Algo::name << " " <<  max_fill << " " << std::chrono::duration_cast<std::chrono::milliseconds>(t).count() << std::endl;
}
/**
 * 4a)
 * dynamically growing hashtable
 * start with 8 cells
 * double the number of cells if they trespass a certain threshold c
 *
 */
static const char *usage_text =
	"Usage: hashing [OPTIONS]\n"
	"Possible OPTIONS are:\n"
	"    --algo ALGORITHM\n"
	"        Select an algorithm {static,stl,dynamic,dynamic_scaling}.\n"
	"    --max-fill FACTOR\n"
	"        Set the maximal fill factor before the table grows.\n";

int main(int argc, char **argv) {
	std::string_view algorithm;
	float max_fill = 0.5;

	auto error = [] (const char *text) {
		std::cerr << usage_text << "Usage error: " << text << std::endl;
		exit(2);
	};

	// Argument for unary options.
	const char *arg;

	// Parse all options here.

	char **p = argv + 1;

	auto handle_unary_option = [&] (const char *name) -> bool {
		assert(*p);
		if(std::strcmp(*p, name))
			return false;
		++p;
		if(!(*p))
			error("expected argument for unary option");
		arg = *p;
		++p;
		return true;
	};

	while(*p && !std::strncmp(*p, "--", 2)) {
		if(handle_unary_option("--algo")) {
			algorithm = arg;
		}else if(handle_unary_option("--max-fill")) {
			max_fill = std::atof(arg);
		}else{
			error("unknown command line option");
		}
	}

	if(*p)
		error("unexpected arguments");

	// Verify that options are correct and run the algorithm.

	if(algorithm.empty())
		error("no algorithm specified");

	if(algorithm == "static") {
		microbenchmark<static_table>(max_fill);
	}else if(algorithm == "stl") {
		microbenchmark<stl_table>(max_fill);
	}else if(algorithm == "dynamic") {
        microbenchmark<dynamic_table>(max_fill);
    }else if(algorithm == "static_scaling") {
        microbenchmark<static_scaling_table>(max_fill);
    }else if(algorithm == "dynamic_scaling") {
        microbenchmark<dynamic_scaling_table>(max_fill);
    }else{
		error("unknown algorithm");
	}
}
