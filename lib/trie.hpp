#include <string>
#include <unordered_map>
#include <vector>
#include <locale>
#include <codecvt>

struct Vertex {
    std::wstring alphabet;
    bool exist;
    std::unordered_map<std::wstring, Vertex*> child;
    Vertex(std::wstring a): alphabet(a), exist(false) { }
};

class Trie
{
private:
    Vertex* root;
    static std::vector<std::wstring> characters;
    uint8_t n_;
    size_t memory_;
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter_;
    void dfs(Vertex* cur, std::string word, std::vector<std::string>& result);
public:
    Trie() = default;
    Trie(uint8_t n) : n_(n), memory_(0) { root = new Vertex(L"!"); }
    void insert(std::string word);
    std::vector<std::string> getWords(std::string prefix);
    int getMemoryUsage();
};