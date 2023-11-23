#include <bits/stdc++.h>

using namespace std;

struct Vertex {
    wstring alphabet;
    bool exist;
    unordered_map<wstring, Vertex*> child;
    Vertex(wstring a): alphabet(a), exist(false) { }
};

class Trie
{
private:
    Vertex* root;
    static vector<wstring> characters;
    uint8_t _n;
public:
    Trie(uint8_t n) : _n(n) { root = new Vertex(L"!"); }
    void insert(wstring word);
    void dfs(Vertex* cur, wstring word, vector<wstring>& result);
    vector<wstring> getWords(wstring prefix);

};