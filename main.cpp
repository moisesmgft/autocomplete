#include "trie.cpp"

int main() {
    Trie trie(5);

    std::wifstream file("american-english");
    file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    std::wcout.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    std::wcin.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));


    if (!file.is_open()) {
        std::wcerr << L"Error opening file!" << std::endl;
        return 1;
    }

    std::wstring word;
    while (std::getline(file, word))
        trie.insert(word);

    file.close();

    wcout << L"Created!\n";
    std::wstring prefix;
    std::vector<std::wstring> res;

    while (std::wcin >> prefix) {
        res = trie.getWords(prefix);
        for (const auto &r : res)
            std::wcout << r << L", ";
        std::wcout << std::endl;
    }

    return 0;
}
