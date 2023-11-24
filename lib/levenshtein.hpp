#ifndef LEVENSHTEIN_HPP
#define LEVENSHTEIN_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

class SparseLevenshteinAutomaton {
public:
    SparseLevenshteinAutomaton(const std::string& str, int n);

    std::vector<std::string> filter(const std::vector<std::string>& queries);

    std::pair<std::vector<int>, std::vector<int>> start();
    std::pair<std::vector<int>, std::vector<int>> step(const std::pair<std::vector<int>, std::vector<int>>& state, char c);
    bool is_match(const std::pair<std::vector<int>, std::vector<int>>& state) const;
    bool can_match(const std::pair<std::vector<int>, std::vector<int>>& state) const;
    std::unordered_set<char> transitions(const std::pair<std::vector<int>, std::vector<int>>& state) const;

private:
    std::string string;
    int max_edits;
};

#endif // LEVENSHTEIN_HPP
