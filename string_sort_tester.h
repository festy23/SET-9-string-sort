#pragma once

#include <vector>
#include <string>
#include <random>
#include <functional>

class StringSortTester {
public:
    struct Measurement {
        size_t size;
        double time_us;
        size_t comparisons;
    };

    using SortFn = void(*)(std::vector<std::string>&, size_t&);

    explicit StringSortTester(unsigned seed = 42);

    // Прогоняет алгоритм на всех размерах из массива sizes,
    // берёт подмассивы из masterArray, усредняет по repetitions замеров
    std::vector<Measurement> testAlgorithm(
        SortFn sortFn,
        const std::vector<std::string>& masterArray,
        const std::vector<size_t>& sizes,
        int repetitions = 5
    );

    // Запускает все тесты (6 алгоритмов × 4 типа данных) и пишет CSV
    void runAllTests();

private:
    std::mt19937 rng_;
    std::vector<size_t> sizes_;
    static constexpr int REPETITIONS = 5;

    void writeCSV(const std::string& filename,
                  const std::vector<std::pair<std::string, std::vector<Measurement>>>& results);
};
