#include "fst.hpp"


bool state::isFinal(){
    return _isFinal;
}
state*state:: getTransition(char c){
    return _transitions[c];
}

void state::setFinal(bool value){
    _isFinal = value;
}
void state::setTransition(char c, state* s){
    _transitions[c] = s;
}
void state::clear(){
    _transitions.clear();
    _isFinal = false;
}

void state::copy(state& s){
    _transitions = s._transitions;
    _isFinal = s._isFinal;
}

std::string state::getHash(){
    std::string hash;
    for (auto &pair : _transitions) {
        hash += pair.first; // Append transition character
        hash += std::to_string(reinterpret_cast<uintptr_t>(pair.second)); // Append memory address of state
    }
    return hash;
}


state* Automaton::findMinimized(state& s) {
    std::string hash = s.getHash();
    if (dict.find(hash) == dict.end()) {
        dict[hash] = new state();
        dict[hash]->copy(s);
    }
    return dict[hash];
}

state* Automaton::createMininmalTranducerForList() {
    state* tempStates[MAX_WORD_SIZE];
    state initialState;
    std::string previousWord, currentWord;
    int i, prefixLengthPlus1;

    for(i = 0; i < MAX_WORD_SIZE; i++) {
        tempStates[i] = new state();
    }
    previousWord = "";
    tempStates[0]->clear();

    for(int k = 0; k < input.size(); k++) {
        currentWord = input[k];
        i = 1;
        while(i < std::min(previousWord.size(), currentWord.size()) && previousWord[i-1] == currentWord[i-1]) 
            i++;

        prefixLengthPlus1 = i;

        for(i = previousWord.size(); i >= prefixLengthPlus1; i--) 
            tempStates[i-1]->setTransition(previousWord[i-1], findMinimized(*tempStates[i]));

        for(i = prefixLengthPlus1; i <= currentWord.size(); i++) {
            tempStates[i]->clear();
            tempStates[i-1]->setTransition(currentWord[i-1], tempStates[i]);
        }
        if(previousWord != currentWord) tempStates[currentWord.size()]->setFinal(true);
        previousWord = currentWord;
    }

    for(i = currentWord.size(); i >= 1; i--) {
        tempStates[i-1]->setTransition(previousWord[i-1], findMinimized(*tempStates[i]));
    }
    initialState = state();
    state* temp = findMinimized(*tempStates[0]);
    initialState.copy(*temp);
    
    return findMinimized(initialState);
}


void Automaton::_dfs(state* currentState, std::string path, int max, int* n, std::vector<std::string>& result) {
    if (currentState->isFinal() && *n < max) {
        (*n)++;
        return;
    } else if (*n == max) {
        return;
    }

    for (char c = 'a'; c <= 'z'; ++c) { // Iterate through the alphabet
        state* nextState = currentState->getTransition(c);
        if (nextState != nullptr) {
            _dfs(nextState, path + c, max, n, result); // Recursively call dfs for the next state
        }
    }
}

std::vector<std::string>  Automaton::dfs(state* initialState, std::string path, int max, int current = 0){
    int n = 0;
    state* nextState = initialState->getTransition(path[current]);
    if (nextState != nullptr) {
        dfs(nextState, path, max, ++current); // Recursively call dfs for the next state
    }
    else {
        std::vector<std::string> result;
        _dfs(initialState, path, max, &n, result);
        return result;
    }
}

void Automaton::insert(std::string word) {
    input.push_back(word);
}