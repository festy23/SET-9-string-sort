#include "string_generator.h"
#include <algorithm>

const std::string StringGenerator::ALPHABET =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!@#%:;^&*()-";

std::string StringGenerator::randomString(std::mt19937& rng) {
    static std::uniform_int_distribution<size_t> lenDist(10, 200);
    static std::uniform_int_distribution<size_t> charDist(0, ALPHABET.size() - 1);

    size_t len = lenDist(rng);
    std::string s(len, '\0');
    for (size_t i = 0; i < len; ++i) {
        s[i] = ALPHABET[charDist(rng)];
    }
    return s;
}

std::vector<std::string> StringGenerator::randomArray(size_t n, std::mt19937& rng) {
    std::vector<std::string> arr(n);
    for (size_t i = 0; i < n; ++i) {
        arr[i] = randomString(rng);
    }
    return arr;
}

std::vector<std::string> StringGenerator::reverseSortedArray(size_t n, std::mt19937& rng) {
    auto arr = randomArray(n, rng);
    std::sort(arr.begin(), arr.end(), std::greater<std::string>());
    return arr;
}

std::vector<std::string> StringGenerator::nearlySortedArray(size_t n, std::mt19937& rng) {
    auto arr = randomArray(n, rng);
    std::sort(arr.begin(), arr.end());

    size_t swaps = std::max(size_t(1), n / 20);
    std::uniform_int_distribution<size_t> idxDist(0, n - 2);
    for (size_t i = 0; i < swaps; ++i) {
        size_t j = idxDist(rng);
        std::swap(arr[j], arr[j + 1]);
    }
    return arr;
}

std::vector<std::string> StringGenerator::sharedPrefixArray(size_t n, std::mt19937& rng) {
    auto arr = randomArray(n, rng);

    static std::uniform_int_distribution<size_t> prefixLenDist(20, 50);
    static std::uniform_int_distribution<size_t> charDist(0, ALPHABET.size() - 1);

    size_t prefixLen = prefixLenDist(rng);
    std::string prefix(prefixLen, '\0');
    for (size_t i = 0; i < prefixLen; ++i) {
        prefix[i] = ALPHABET[charDist(rng)];
    }

    size_t halfN = n / 2;
    static std::uniform_int_distribution<size_t> suffixLenDist(10, 150);
    for (size_t i = 0; i < halfN; ++i) {
        size_t suffixLen = suffixLenDist(rng);
        std::string suffix(suffixLen, '\0');
        for (size_t j = 0; j < suffixLen; ++j) {
            suffix[j] = ALPHABET[charDist(rng)];
        }
        arr[i] = prefix + suffix;
    }

    return arr;
}

std::vector<size_t> StringGenerator::getTestSizes() {
    std::vector<size_t> sizes;
    for (size_t n = 100; n <= 3000; n += 100) {
        sizes.push_back(n);
    }
    return sizes;
}
