#include "sort_algorithms.h"
#include <algorithm>

inline bool countedLess(const std::string& a, const std::string& b, size_t& cmpCount) {
    size_t i = 0;
    size_t minLen = std::min(a.size(), b.size());
    while (i < minLen) {
        ++cmpCount;
        if (a[i] < b[i]) return true;
        if (a[i] > b[i]) return false;
        ++i;
    }
    return a.size() < b.size();
}

inline int charAtCount(const std::string& s, size_t d, size_t& cmpCount) {
    ++cmpCount;
    if (d < s.size()) return static_cast<unsigned char>(s[d]);
    return -1;
}

static void stdQuickSortImpl(std::vector<std::string>& arr, int lo, int hi, size_t& cmpCount) {
    if (lo >= hi) return;

    std::string pivot = arr[lo];
    int i = lo;
    int j = hi;

    while (i <= j) {
        while (countedLess(arr[i], pivot, cmpCount)) ++i;
        while (countedLess(pivot, arr[j], cmpCount)) --j;
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            ++i;
            --j;
        }
    }

    stdQuickSortImpl(arr, lo, j, cmpCount);
    stdQuickSortImpl(arr, i, hi, cmpCount);
}

void stdQuickSort(std::vector<std::string>& arr, size_t& cmpCount) {
    cmpCount = 0;
    if (arr.size() > 1) {
        stdQuickSortImpl(arr, 0, static_cast<int>(arr.size()) - 1, cmpCount);
    }
}

static void stdMerge(std::vector<std::string>& arr, int left, int mid, int right, size_t& cmpCount) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<std::string> L(arr.begin() + left, arr.begin() + mid + 1);
    std::vector<std::string> R(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (countedLess(L[i], R[j], cmpCount)) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

static void stdMergeSortImpl(std::vector<std::string>& arr, int left, int right, size_t& cmpCount) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        stdMergeSortImpl(arr, left, mid, cmpCount);
        stdMergeSortImpl(arr, mid + 1, right, cmpCount);
        stdMerge(arr, left, mid, right, cmpCount);
    }
}

void stdMergeSort(std::vector<std::string>& arr, size_t& cmpCount) {
    cmpCount = 0;
    if (arr.size() > 1) {
        stdMergeSortImpl(arr, 0, static_cast<int>(arr.size()) - 1, cmpCount);
    }
}

static void stringQuickSortImpl(std::vector<std::string>& arr, int lo, int hi, size_t d, size_t& cmpCount) {
    if (hi <= lo) return;

    int lt = lo, gt = hi;
    int v = charAtCount(arr[lo], d, cmpCount);
    int i = lo + 1;

    while (i <= gt) {
        int t = charAtCount(arr[i], d, cmpCount);
        if (t < v) {
            std::swap(arr[lt], arr[i]);
            ++lt; ++i;
        } else if (t > v) {
            std::swap(arr[i], arr[gt]);
            --gt;
        } else {
            ++i;
        }
    }

    stringQuickSortImpl(arr, lo, lt - 1, d, cmpCount);
    if (v >= 0) {
        stringQuickSortImpl(arr, lt, gt, d + 1, cmpCount);
    }
    stringQuickSortImpl(arr, gt + 1, hi, d, cmpCount);
}

void stringQuickSort(std::vector<std::string>& arr, size_t& cmpCount) {
    cmpCount = 0;
    if (arr.size() > 1) {
        stringQuickSortImpl(arr, 0, static_cast<int>(arr.size()) - 1, 0, cmpCount);
    }
}

static bool lcpCompareCount(const std::string& a, const std::string& b, size_t& cmpCount) {
    size_t minLen = std::min(a.size(), b.size());
    size_t lcp = 0;
    while (lcp < minLen) {
        ++cmpCount;
        if (a[lcp] != b[lcp]) {
            return a[lcp] < b[lcp];
        }
        ++lcp;
    }
    return a.size() <= b.size();
}

static void stringMerge(std::vector<std::string>& arr, int left, int mid, int right, size_t& cmpCount) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<std::string> L(arr.begin() + left, arr.begin() + mid + 1);
    std::vector<std::string> R(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (lcpCompareCount(L[i], R[j], cmpCount)) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

static void stringMergeSortImpl(std::vector<std::string>& arr, int left, int right, size_t& cmpCount) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        stringMergeSortImpl(arr, left, mid, cmpCount);
        stringMergeSortImpl(arr, mid + 1, right, cmpCount);
        stringMerge(arr, left, mid, right, cmpCount);
    }
}

void stringMergeSort(std::vector<std::string>& arr, size_t& cmpCount) {
    cmpCount = 0;
    if (arr.size() > 1) {
        stringMergeSortImpl(arr, 0, static_cast<int>(arr.size()) - 1, cmpCount);
    }
}

static const int R = 256;

static void msdRadixSortImpl(std::vector<std::string>& arr, int lo, int hi, size_t d,
                              std::vector<std::string>& aux, size_t& cmpCount) {
    if (hi <= lo) return;

    std::vector<int> count(R + 2, 0);

    for (int i = lo; i <= hi; ++i) {
        int c = charAtCount(arr[i], d, cmpCount);
        ++count[c + 2];
    }

    for (int r = 0; r < R + 1; ++r) {
        count[r + 1] += count[r];
    }

    for (int i = lo; i <= hi; ++i) {
        int c = charAtCount(arr[i], d, cmpCount);
        aux[count[c + 1]++] = arr[i];
    }

    for (int i = lo; i <= hi; ++i) {
        arr[i] = aux[i - lo];
    }

    for (int r = 0; r < R; ++r) {
        msdRadixSortImpl(arr, lo + count[r], lo + count[r + 1] - 1, d + 1, aux, cmpCount);
    }
}

void msdRadixSort(std::vector<std::string>& arr, size_t& cmpCount) {
    cmpCount = 0;
    if (arr.size() > 1) {
        std::vector<std::string> aux(arr.size());
        msdRadixSortImpl(arr, 0, static_cast<int>(arr.size()) - 1, 0, aux, cmpCount);
    }
}

static const int CUTOFF = 74;

static void msdRadixQuickImpl(std::vector<std::string>& arr, int lo, int hi, size_t d,
                               std::vector<std::string>& aux, size_t& cmpCount) {
    if (hi <= lo) return;

    if (hi - lo + 1 < CUTOFF) {
        stringQuickSortImpl(arr, lo, hi, d, cmpCount);
        return;
    }

    std::vector<int> count(R + 2, 0);

    for (int i = lo; i <= hi; ++i) {
        int c = charAtCount(arr[i], d, cmpCount);
        ++count[c + 2];
    }

    for (int r = 0; r < R + 1; ++r) {
        count[r + 1] += count[r];
    }

    for (int i = lo; i <= hi; ++i) {
        int c = charAtCount(arr[i], d, cmpCount);
        aux[count[c + 1]++] = arr[i];
    }

    for (int i = lo; i <= hi; ++i) {
        arr[i] = aux[i - lo];
    }

    for (int r = 0; r < R; ++r) {
        msdRadixQuickImpl(arr, lo + count[r], lo + count[r + 1] - 1, d + 1, aux, cmpCount);
    }
}

void msdRadixQuickSort(std::vector<std::string>& arr, size_t& cmpCount) {
    cmpCount = 0;
    if (arr.size() > 1) {
        std::vector<std::string> aux(arr.size());
        msdRadixQuickImpl(arr, 0, static_cast<int>(arr.size()) - 1, 0, aux, cmpCount);
    }
}
