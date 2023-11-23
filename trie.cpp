#include "trie.hpp"

vector<string> Trie::characters = {
    "A", "B", "C", "D", "E", "F", "G", "H", "I", 
    "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", 
    "T", "U", "V", "W", "X", "Y", "Z", "a", "b", "c", 
    "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", 
    "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", 
    "x", "y", "z", "Å", "á", "â", "ä", "å", "ç", "è", 
    "é", "ê", "í", "ñ", "ó", "ô", "ö", "û", "ü", "'"
};

void Trie::insert(string word) {
    Vertex* cur = root;
    for (int i=0; i<(int)word.size(); i++) {
        const string idx = word.substr(i,1); 
        if (!cur->child.count(idx))
            cur->child[idx] = new Vertex(idx);
        cur = cur->child[idx];
    }
    cur->exist = true;
}

void Trie::dfs(Vertex* cur, string word, vector<string>& result) {
    if (result.size()==3) return;
    if (cur->exist)
        result.push_back(word);
    for (auto s: characters) {
        if (cur->child.count(s)) {
            word += s;
            dfs(cur->child[s], word, result);
            word.pop_back();
        }
    }
}

vector<string> Trie::getWords(string prefix) {
    Vertex* cur = root;
    vector<string> result;

    for (int i=0; i<(int)prefix.size(); i++) {
        const string idx = prefix.substr(i,1); 
        if (!cur->child.count(idx))
            return result;
        cur = cur->child[idx];
    }
    dfs(cur, prefix, result);
    return result;
} 
