#include <bits/stdc++.h>

using namespace std;

struct Vertex {
    string alphabet;
    bool exist;
    unordered_map<string, Vertex*> child;
    Vertex(string a): alphabet(a), exist(false) { }
};

class Trie
{
private:
    Vertex* root;
    static vector<string> characters;
public:
    Trie() { root = new Vertex("!"); }
    void insert(string word);
    void dfs(Vertex* cur, string word, vector<string>& result);
    vector<string> getWords(string prefix);

};