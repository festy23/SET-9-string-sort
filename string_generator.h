#pragma once

#include <vector>
#include <string>
#include <random>

class StringGenerator {
public:
    // 74 символа: A-Z, a-z, 0-9, !@#%:;^&*()-
    static const std::string ALPHABET;

    static std::string randomString(std::mt19937& rng);
    static std::vector<std::string> randomArray(size_t n, std::mt19937& rng);
    static std::vector<std::string> reverseSortedArray(size_t n, std::mt19937& rng);
    static std::vector<std::string> nearlySortedArray(size_t n, std::mt19937& rng);
    static std::vector<std::string> sharedPrefixArray(size_t n, std::mt19937& rng);

    // Размеры для тестирования: 100, 200, ..., 3000
    static std::vector<size_t> getTestSizes();
};
