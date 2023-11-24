#include <string>
#include <unordered_map>
#include <vector>

class state {
public:
    std::unordered_map<char, state*> _transitions;
    bool _isFinal = false;

    bool isFinal();
    state* getTransition(char c);

    void setFinal(bool value);
    void setTransition(char c, state* s);
    void clear();

    void copy(state& s);

    std::string getHash();
};


class Automaton {
public:
    const int MAX_WORD_SIZE = 120000;
    std::unordered_map<std::string,state*> dict;

    state* findMinimized(state& s);
    state* createMininmalTranducerForList(std::vector<std::string> input);
    void _dfs(state* currentState, std::string path, int max, int* n, std::vector<std::string>& result);
    std::vector<std::string> dfs(state* initialState, std::string path, int max, int current = 0);
};