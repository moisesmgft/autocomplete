#include <ncurses.h>
#include <cstring>
#include <vector>
#include <chrono>
#include <fstream>
#include "lib/trie.hpp"

class Statistics {
public:
	size_t memory;
	long long creation_time, search_time;
};

class Interface {
public:
    Interface() : trie_(10) {

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
	Statistics fst_stats, trie_stats;
	std::vector<std::string> trie_results;

    char userInput[100];
    void (*currentFunction)();

    void initNcurses() {
        initscr();
        raw();
        keypad(stdscr, TRUE);
        noecho();
    }

    void drawInterface() {

        mvprintw(3, 1, "Estatisticas | FST           | Trie           |");
        mvprintw(4, 1, "Criação (ms) | %-11lld | %-14lld |", fst_stats.creation_time, trie_stats.creation_time);
        mvprintw(5, 1, "Memória (kb) | %-12zu | %-14zu |", fst_stats.memory, trie_stats.memory/1000);
        mvprintw(6, 1, "  Busca (us) | %-13lld | %-14lld |", fst_stats.search_time / 1000, trie_stats.search_time / 1000);


		mvprintw(8,1, "Resultados");

        clearLines({9,10,11,12,13,14,15,16,17,18});
        for(long unsigned int i=0; i<trie_results.size(); i++) {
            mvprintw(9+i,1, "%s", trie_results[i].c_str());
        }


        clearLine(1);
        mvprintw(1, 1, "User Input: %s", userInput);
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


		int i=25;
		trie_stats.search_time = 0;

		while (i--) {

            // Trie search
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
			trie_results = trie_.getWords(userInput);
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			trie_stats.search_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();

		}

		trie_stats.search_time /= 25;

    }
};

int main() {
    Interface interface;

    interface.run();

    return 0;
}
