#pragma once

#include <vector>
#include <string>
#include <random>

class StringGenerator {
public:
    static const std::string ALPHABET;

    static std::string randomString(std::mt19937& rng);
    static std::vector<std::string> randomArray(size_t n, std::mt19937& rng);
    static std::vector<std::string> reverseSortedArray(size_t n, std::mt19937& rng);
    static std::vector<std::string> nearlySortedArray(size_t n, std::mt19937& rng);
    static std::vector<std::string> sharedPrefixArray(size_t n, std::mt19937& rng);

    static std::vector<size_t> getTestSizes();
};
