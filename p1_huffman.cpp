#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
    int freq;
    char ch;
    Node *left, *right;
    Node(char c, int f) : freq(f), ch(c), left(nullptr), right(nullptr) {}
    Node(int f, Node* l, Node* r) : freq(f), ch('\0'), left(l), right(r) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) { return a->freq > b->freq; }
};

void buildCodes(Node* node, const string& code, unordered_map<char, string>& codes) {
    if (!node->left && !node->right) {
        codes[node->ch] = code;
        return;
    }
    if (node->left)  buildCodes(node->left,  code + '0', codes);
    if (node->right) buildCodes(node->right, code + '1', codes);
}

void deleteTree(Node* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    getline(cin, s);

    if (s.empty()) {
        cout << "0 0\n";
        return 0;
    }

    unordered_map<char, int> freq;
    for (char c : s) freq[c]++;

    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto& [ch, f] : freq) {
        pq.push(new Node(ch, f));
    }

    if (pq.size() == 1) {
        char ch = pq.top()->ch;
        string code = "0";
        string encoded;
        for (char c : s) encoded += code;
        cout << "1 " << encoded.size() << '\n';
        cout << ch << ": " << code << '\n';
        cout << encoded << '\n';
        deleteTree(pq.top());
        return 0;
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        pq.push(new Node(left->freq + right->freq, left, right));
    }

    Node* root = pq.top();

    unordered_map<char, string> codes;
    buildCodes(root, "", codes);

    string encoded;
    encoded.reserve(s.size() * 5);
    for (char c : s) encoded += codes[c];

    cout << freq.size() << ' ' << encoded.size() << '\n';
    for (auto& [ch, code] : codes) {
        cout << ch << ": " << code << '\n';
    }
    cout << encoded << '\n';

    deleteTree(root);
    return 0;
}
