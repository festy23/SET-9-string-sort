#include "string_sort_tester.h"
#include "string_generator.h"
#include "sort_algorithms.h"
#include <chrono>
#include <iostream>
#include <fstream>

StringSortTester::StringSortTester(unsigned seed)
    : rng_(seed)
    , sizes_(StringGenerator::getTestSizes())
{}

std::vector<StringSortTester::Measurement>
StringSortTester::testAlgorithm(SortFn sortFn,
                                 const std::vector<std::string>& masterArray,
                                 const std::vector<size_t>& sizes,
                                 int repetitions) {
    std::vector<Measurement> results;
    results.reserve(sizes.size());

    for (size_t n : sizes) {
        double totalTime = 0.0;
        size_t totalComparisons = 0;

        std::vector<std::string> subarray(masterArray.begin(), masterArray.begin() + n);

        for (int rep = 0; rep < repetitions; ++rep) {
            auto copy = subarray;
            size_t cmpCount = 0;

            auto start = std::chrono::high_resolution_clock::now();
            sortFn(copy, cmpCount);
            auto end = std::chrono::high_resolution_clock::now();

            totalTime += static_cast<double>(
                std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
            totalComparisons += cmpCount;
        }

        results.push_back({
            n,
            totalTime / repetitions,
            totalComparisons / repetitions
        });
    }

    return results;
}

void StringSortTester::writeCSV(
    const std::string& filename,
    const std::vector<std::pair<std::string, std::vector<Measurement>>>& results) {

    std::ofstream out(filename);
    out << "size,algorithm,time_us,comparisons\n";
    for (const auto& [algoName, measurements] : results) {
        for (const auto& m : measurements) {
            out << m.size << ',' << algoName << ',' << m.time_us << ',' << m.comparisons << '\n';
        }
    }
    out.close();
    std::cout << "[CSV] " << filename << " written\n";
}

void StringSortTester::runAllTests() {
    std::cout << "Generating master arrays (3000 strings each)...\n";

    auto masterRandom = StringGenerator::randomArray(3000, rng_);
    auto masterReverse = StringGenerator::reverseSortedArray(3000, rng_);
    auto masterNearly = StringGenerator::nearlySortedArray(3000, rng_);
    auto masterPrefix = StringGenerator::sharedPrefixArray(3000, rng_);

    struct DataSet {
        std::string name;
        std::string csvFilename;
        const std::vector<std::string>& master;
    };

    std::vector<DataSet> dataSets = {
        {"random",         "data/results_random.csv",          masterRandom},
        {"reverse_sorted", "data/results_reverse.csv",         masterReverse},
        {"nearly_sorted",  "data/results_nearly_sorted.csv",   masterNearly},
        {"shared_prefix",  "data/results_shared_prefix.csv",   masterPrefix},
    };

    struct Algo {
        std::string name;
        SortFn fn;
    };

    std::vector<Algo> algorithms = {
        {"std_quick",         stdQuickSort},
        {"std_merge",         stdMergeSort},
        {"string_quick",      stringQuickSort},
        {"string_merge",      stringMergeSort},
        {"msd_radix",         msdRadixSort},
        {"msd_radix_quick",   msdRadixQuickSort},
    };

    for (const auto& ds : dataSets) {
        std::cout << "\n=== " << ds.name << " ===\n";
        std::vector<std::pair<std::string, std::vector<Measurement>>> datasetResults;

        for (const auto& algo : algorithms) {
            std::cout << "  " << algo.name << "... " << std::flush;
            auto measurements = testAlgorithm(algo.fn, ds.master, sizes_, REPETITIONS);
            datasetResults.push_back({algo.name, measurements});
            std::cout << "done. (last: n=" << measurements.back().size
                      << " time=" << measurements.back().time_us << "us"
                      << " cmps=" << measurements.back().comparisons << ")\n";
        }

        writeCSV(ds.csvFilename, datasetResults);
    }

    std::cout << "\nAll tests complete.\n";
}
