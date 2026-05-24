#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cin >> k;

    vector<int> codes(k);
    for (int i = 0; i < k; ++i) {
        cin >> codes[i];
    }

    vector<string> dict;
    dict.reserve(20000);
    for (int i = 0; i < 128; ++i) {
        dict.push_back(string(1, static_cast<char>(i)));
    }

    string result;
    string prev = dict[codes[0]];
    result += prev;

    for (int i = 1; i < k; ++i) {
        int code = codes[i];
        string entry;

        if (code < static_cast<int>(dict.size())) {
            entry = dict[code];
        } else {
            entry = prev + prev[0];
        }

        result += entry;
        dict.push_back(prev + entry[0]);
        prev = entry;
    }

    cout << result << '\n';
    return 0;
}
