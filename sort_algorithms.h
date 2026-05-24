#pragma once

#include <vector>
#include <string>
#include <cstddef>

// Все функции модифицируют массив и инкрементируют cmpCount
// при каждом посимвольном сравнении

void stdQuickSort(std::vector<std::string>& arr, size_t& cmpCount);
void stdMergeSort(std::vector<std::string>& arr, size_t& cmpCount);
void stringQuickSort(std::vector<std::string>& arr, size_t& cmpCount);
void stringMergeSort(std::vector<std::string>& arr, size_t& cmpCount);
void msdRadixSort(std::vector<std::string>& arr, size_t& cmpCount);
void msdRadixQuickSort(std::vector<std::string>& arr, size_t& cmpCount);
