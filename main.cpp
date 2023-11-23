#include "trie.cpp"

int main() {
    Trie trie;

    std::string file_path = "american-english"; 
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Erro abrindo arquivo: " << file_path << std::endl;
        return 1;
    }

    std::string word;
    while (std::getline(file, word))
        trie.insert(word);

    file.close();

    cout << "Created!\n";
    string prefix; vector<string> res;

    while (cin >> prefix) {
        for (auto r: trie.getWords(prefix))
            cout << r << ", ";
        cout << endl;
        
    }
    
}