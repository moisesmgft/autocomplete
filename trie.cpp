#include "trie.hpp"

vector<wstring> Trie::characters = {
    L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", 
    L"J", L"K", L"L", L"M", L"N", L"O", L"P", L"Q", L"R", L"S", 
    L"T", L"U", L"V", L"W", L"X", L"Y", L"Z", L"a", L"b", L"c", 
    L"d", L"e", L"f", L"g", L"h", L"i", L"j", L"k", L"l", L"m", 
    L"n", L"o", L"p", L"q", L"r", L"s", L"t", L"u", L"v", L"w", 
    L"x", L"y", L"z", L"Å", L"á", L"â", L"ä", L"å", L"ç", L"è", 
    L"é", L"ê", L"í", L"ñ", L"ó", L"ô", L"ö", L"û", L"ü", L"'"
};

void Trie::insert(wstring word) {
    Vertex* cur = root;
    for (auto it = begin(word); it != end(word); ++it) {
        const wstring idx(it,it+1); 
        if (!cur->child.count(idx))
            cur->child[idx] = new Vertex(idx);
        cur = cur->child[idx];
    }
    cur->exist = true;
}

void Trie::dfs(Vertex* cur, wstring word, vector<wstring>& result) {
    if (result.size()==_n) return;
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

vector<wstring> Trie::getWords(wstring prefix) {
    Vertex* cur = root;
    vector<wstring> result;

    for (auto it = begin(prefix); it != end(prefix); ++it) {
        const wstring idx(it,it+1); 
        if (!cur->child.count(idx))
            return result;
        cur = cur->child[idx];
    }
    dfs(cur, prefix, result);
    return result;
} 
