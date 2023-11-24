#include <ncurses.h>
#include <cstring>
#include <vector>
#include <chrono>
#include <fstream>
#include "lib/trie.hpp"

class Statistics {
public:
	uint64_t memory;
	int64_t creation;
	int64_t search;
};

class Interface {
public:
    Interface() : trie_(10) {

        std::ifstream file("data/american-english");

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		trie_stats.creation = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
		//trie_stats.memory = trie_.getMemoryUsage();

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

		mvprintw(3, 1, "Estatisticas |         FST |        Trie |");
		mvprintw(4, 1, "     Criação |");
		mvprintw(5, 1, "     Memória |");
		mvprintw(6, 1, "       Busca |");

		mvprintw(8,1, "Resultados");


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

		int i=5;
		trie_stats.search = 0;

		while (i--) {

			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
			trie_results = trie_.getWords(userInput);
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			trie_stats.search += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

		}

		trie_stats.search /= 5;

    }
};

int main() {
    Interface interface;

    interface.run();

    return 0;
}
