#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    getline(cin, s);

    unordered_map<string, int> dict;
    for (int i = 0; i < 128; ++i) {
        dict[string(1, static_cast<char>(i))] = i;
    }

    vector<int> output;
    int nextCode = 128;

    string current;
    for (char c : s) {
        string candidate = current + c;
        if (dict.count(candidate)) {
            current = candidate;
        } else {
            output.push_back(dict[current]);
            dict[candidate] = nextCode++;
            current = string(1, c);
        }
    }

    if (!current.empty()) {
        output.push_back(dict[current]);
    }

    cout << output.size() << '\n';
    for (size_t i = 0; i < output.size(); ++i) {
        if (i > 0) cout << ' ';
        cout << output[i];
    }
    cout << '\n';

    return 0;
}
