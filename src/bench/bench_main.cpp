#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "dict/AVLTreeDictionary.hpp"

using Clock = std::chrono::steady_clock;

struct Args {
    std::string dataset = "random";     // random | sorted | reverse | collisions (для хеш-версий)
    std::vector<std::string> ops = {"insert", "upsert", "erase"};
    std::size_t n = 10000;
    std::string out = "results.csv";    // путь к CSV
};

static Args parse_args(int argc, char **argv) {
    Args a;
    for (int i = 1; i < argc; ++i) {
        std::string_view t = argv[i];
        auto next_val = [&](std::string &dst) {
            if (i + 1 >= argc) return;
            dst = argv[++i];
        };
        auto next_val_size = [&](std::size_t &dst) {
            if (i + 1 >= argc) return;
            dst = static_cast<std::size_t>(std::stoull(argv[++i]));
        };
        if (t == "--dataset") next_val(a.dataset);
        else if (t == "--n") next_val_size(a.n);
        else if (t == "--out") next_val(a.out);
        else if (t == "--ops") {
            std::string v; next_val(v);
            a.ops.clear();
            std::stringstream ss(v);
            std::string item;
            while (std::getline(ss, item, ',')) if (!item.empty()) a.ops.push_back(item);
        }
    }
    return a;
}

static std::vector<int> make_keys(const std::string &dataset, std::size_t n) {
    std::vector<int> keys;
    keys.reserve(n);
    if (dataset == "random") {
        std::mt19937 rng(123456u);
        std::uniform_int_distribution<int> dist(1, static_cast<int>(n * 10 + 100));
        for (std::size_t i = 0; i < n; ++i) keys.push_back(dist(rng));
    } else if (dataset == "sorted") {
        for (std::size_t i = 0; i < n; ++i) keys.push_back(static_cast<int>(i + 1));
    } else if (dataset == "reverse") {
        for (std::size_t i = 0; i < n; ++i) keys.push_back(static_cast<int>(n - i));
    } else {
        // по умолчанию random
        std::mt19937 rng(123456u);
        std::uniform_int_distribution<int> dist(1, static_cast<int>(n * 10 + 100));
        for (std::size_t i = 0; i < n; ++i) keys.push_back(dist(rng));
    }
    return keys;
}

static inline uint64_t ns_between(const Clock::time_point a, const Clock::time_point b) {
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(b - a).count());
}

int main(int argc, char **argv) {
    Args args = parse_args(argc, argv);

    // Данные
    const auto keys = make_keys(args.dataset, args.n);
    const std::string value(16, 'x');

    // CSV
    std::ofstream csv(args.out);
    if (!csv.is_open()) {
        std::cerr << "Не удалось открыть файл для записи: " << args.out << "\n";
        return 2;
    }
    csv << "op,scenario,latency_ns\n";

    // Небольшой прогрев
    {
        dict::AVLTreeDictionary warm;
        for (std::size_t i = 0; i < std::min<std::size_t>(args.n, 1000); ++i) {
            warm.upsert(static_cast<int>(i), value);
        }
    }

    // Основные замеры
    for (const auto &op : args.ops) {
        dict::AVLTreeDictionary dict;

        if (op == "insert") {
            for (std::size_t i = 0; i < args.n; ++i) {
                const int k = keys[i];
                const auto t0 = Clock::now();
                (void)dict.insert(k, value);
                const auto t1 = Clock::now();
                csv << "insert," << args.dataset << ',' << ns_between(t0, t1) << "\n";
            }
        } else if (op == "upsert") {
            for (std::size_t i = 0; i < args.n; ++i) {
                const int k = keys[i];
                const auto t0 = Clock::now();
                dict.upsert(k, value);
                const auto t1 = Clock::now();
                csv << "upsert," << args.dataset << ',' << ns_between(t0, t1) << "\n";
            }
        } else if (op == "erase") {
            // сначала заполним
            for (std::size_t i = 0; i < args.n; ++i) dict.upsert(keys[i], value);
            for (std::size_t i = 0; i < args.n; ++i) {
                const int k = keys[i];
                const auto t0 = Clock::now();
                (void)dict.erase(k);
                const auto t1 = Clock::now();
                csv << "erase," << args.dataset << ',' << ns_between(t0, t1) << "\n";
            }
        }
    }

    std::cout << "CSV сохранён: " << args.out << "\n";
    return 0;
}


