#include "levenshtein.hpp"

SparseLevenshteinAutomaton::SparseLevenshteinAutomaton(const std::string& str, int n) : string(str), max_edits(n) {}


std::pair<std::vector<int>, std::vector<int>> SparseLevenshteinAutomaton::start() {
    return {std::vector<int>(max_edits + 1), std::vector<int>(max_edits + 1)};
}

std::pair<std::vector<int>, std::vector<int>> SparseLevenshteinAutomaton::step(
    const std::pair<std::vector<int>, std::vector<int>>& state, char c) {
    const auto& indices = state.first;
    const auto& values = state.second;

    std::vector<int> new_indices, new_values;

    if (!indices.empty() && indices[0] == 0 && values[0] < max_edits) {
        new_indices.push_back(0);
        new_values.push_back(values[0] + 1);
    }

    for (size_t j = 0; j < indices.size(); ++j) {
        int i = indices[j];
        if (size_t(i) == string.size()) break;

        int cost = (string[i] == c) ? 0 : 1;
        int val = values[j] + cost;

        if (!new_indices.empty() && new_indices.back() == i) {
            val = std::min(val, new_values.back() + 1);
        }

        if (size_t(j+1) < indices.size() && indices[j + 1] == i + 1) {
            val = std::min(val, values[j + 1] + 1);
        }

        if (val <= max_edits) {
            new_indices.push_back(i + 1);
            new_values.push_back(val);
        }
    }

    return {new_indices, new_values};
}

bool SparseLevenshteinAutomaton::is_match(const std::pair<std::vector<int>, std::vector<int>>& state) const {
    const auto& indices = state.first;
    return !indices.empty() && size_t(indices.back())== string.size();
}

bool SparseLevenshteinAutomaton::can_match(const std::pair<std::vector<int>, std::vector<int>>& state) const {
    const auto& indices = state.first;
    return !indices.empty();
}

std::unordered_set<char> SparseLevenshteinAutomaton::transitions(
    const std::pair<std::vector<int>, std::vector<int>>& state) const {
    const auto& indices = state.first;
    std::unordered_set<char> result;

    for (int i : indices) {
        if (size_t(i) < string.size()) {
            result.insert(string[i]);
        }
    }

    return result;
}

std::vector<std::string> SparseLevenshteinAutomaton::filter(const std::vector<std::string>& queries) {
    std::vector<std::string> acceptedStrings;

    for (const auto& query : queries) {
        auto state = start();

        for (char c : query)
            state = step(state, c);

        if (is_match(state))
            acceptedStrings.push_back(query);
    }

    return acceptedStrings;
}
