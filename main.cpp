#include "string_sort_tester.h"
#include <iostream>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout << "SET 9 — String Sort Empirical Analysis\n";
    std::cout << "=======================================\n";

    StringSortTester tester(42);
    tester.runAllTests();

    return 0;
}
