#include <ncurses.h>
#include <cstring>
#include <vector>
#include <chrono>
#include <fstream>
#include "lib/trie.hpp"
#include "lib/fst.hpp"
#include "lib/levenshtein.hpp"


struct Statistics {
	size_t memory;
	long long creation_time, search_time;
};

class Interface {
public:
    Interface() : trie_(30), fst_() {

        std::ifstream file("data/american-english");

        // Creating Trie
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        std::string word;
        while (std::getline(file, word))
            trie_.insert(word);
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        // Saving Trie stats
		trie_stats.creation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
		trie_stats.memory = trie_.getMemoryUsage();

        // go to the beginning of the file
        file.clear();
        file.seekg(0, std::ios::beg);
        //Creating FST
        begin = std::chrono::steady_clock::now();
        while (std::getline(file, word))
            fst_.insert(word);
        
        fst_.initialState = fst_.createMininmalTranducerForList();
        end = std::chrono::steady_clock::now();        

        // Saving FST stats
        fst_stats.creation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        fst_stats.memory = fst_.getMemoryUsage();

        file.close();

        initNcurses();
        userInput[0] = '\0';
        currentFunction = nullptr;
    }

    ~Interface() {
        endwin();
    }

    void run() {
        while (1) {
            drawInterface();
            handleInput();
        }
    }

private:

	Trie trie_;
    Automaton fst_;

	Statistics fst_stats, trie_stats;
	std::vector<std::string> trie_results;
	std::vector<std::string> fst_results;
    bool use_levenshtein_=false, use_fst_=true;

    char userInput[100];
    void (*currentFunction)();

    void initNcurses() {
        initscr();
        raw();
        keypad(stdscr, TRUE);
        noecho();
    }

    void drawInterface() {

        clearLines({1,2,3,11,12,13,14,15,16,17,18,19,20});

        mvprintw(1,1, "Pressione F1 para %s", (use_fst_)?"Trie":"FST");
        mvprintw(2,1, "Pressione F2 para %s o Levenshtein-Automata", (use_levenshtein_)?"desativar":"ativar");

        mvprintw(5, 1, "Estatisticas | FST           | Trie           |");
        mvprintw(6, 1, "Criação (ms) | %-11lld | %-14lld |", fst_stats.creation_time, trie_stats.creation_time);
        mvprintw(7, 1, "Memória (kb) | %-12zu | %-14zu |", fst_stats.memory/1000, trie_stats.memory/1000);
        mvprintw(8, 1, "  Busca (us) | %-13lld | %-14lld |", fst_stats.search_time / 1000, trie_stats.search_time / 1000);


		mvprintw(9,1, "Resultados: %zu %zu", trie_results.size(), fst_results.size());

        if (!use_fst_)
            for(long unsigned int i=0; i<trie_results.size(); i++) {
                mvprintw(10+i,1, "%s", trie_results[i].c_str());
            }
        else 
            for(long unsigned int i=0; i<fst_results.size(); i++) {
                mvprintw(10+i,1, "%s", fst_results[i].c_str());
            }

        mvprintw(3, 1, "User Input: %s", userInput);
        refresh();
    }

    void clearLine(int line) {
        int y, x;            
        getyx(stdscr, y, x); 
        move(line, 0);			
        clrtoeol();          
        move(y, x); 
    }

    void clearLines(std::vector<int> lines) {
        for (auto i : lines)
            clearLine(i);
    }

    void handleInput() {
        int ch = getch();

        switch (ch) {
            case KEY_F(1):
                use_levenshtein_ = !use_fst_;
                break;
            case KEY_F(2):
                use_levenshtein_ = !use_levenshtein_;
                break;
            case KEY_BACKSPACE:
            case 127:
                if (strlen(userInput) > 0)
                    userInput[strlen(userInput) - 1] = '\0';
                break;
            default:
                if (ch >= 32 && ch <= 126)
                    strncat(userInput, (char *)&ch, 1);
                break;
        }

        int runs = 25;
		int i=runs;
        
		trie_stats.search_time = 0; fst_stats.search_time = 0;

        SparseLevenshteinAutomaton automaton(userInput, 1);
		while (i--) {

            // Trie search
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
			trie_results = trie_.getWords(userInput);
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			trie_stats.search_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

            if(use_levenshtein_) 
                trie_results = automaton.filter(trie_results);
            
            // FST search
            begin = std::chrono::steady_clock::now();
            fst_results = fst_.dfs(fst_.initialState, userInput, 10, 0);
            end = std::chrono::steady_clock::now();
            fst_stats.search_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

            if(use_levenshtein_) 
                fst_results = automaton.filter(fst_results);

		}

		trie_stats.search_time /= runs; fst_stats.search_time /= runs;

    }
};

int main() {
    Interface interface;

    interface.run();

    return 0;
}
