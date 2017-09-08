#include <iostream>
#include <sys/types.h>
#include <vector>
#include <cassert>
#include <algorithm>
#include <thread>
#include <atomic>
#include <chrono>

#include "chess.h"

int main(int argc, const char * argv[]) {
	
	Board board;
	Mind mind;
	Commander commander;
	
	//commander.get_command();
	//commander.process();
	
	board.print();

	std::string user_input;
	
	while (true) {
		std::getline(std::cin, user_input);
		if (user_input == "l") {
			std::vector<Move> moves = board.get_moves();
			for (unsigned int i = 0; i<moves.size(); i++) {
				std::cout << "(" << i+1 << ") --" << (i<9 ? "  " : " ") << board.move_to_str(moves[i]);
			}
		}
		else if (user_input == "f") {
			std::string FEN;
			std::cout << "Enter FEN:\n";
			std::getline(std::cin, FEN);
			board.get_FEN(FEN);
			board.print();
		}
		else if (user_input == "p") {
			board.print();
		}
		else if (user_input == "m") {
			std::vector<Move> moves = board.get_moves();
			int move_choice;
			char sure;
			std::cin >> move_choice;
			std::cout << "Your move = " << board.move_to_str(moves[move_choice-1]) << "Are you sure?\n";
			std::cin >> sure;
			if (sure == 'y') {
				board.make_move(moves[move_choice-1]);
				board.print();
			}
			else {
				std::vector<Move> moves = board.get_moves();
				for (unsigned int i = 0; i<moves.size(); i++) {
					std::cout << "(" << i+1 << ") --" << (i<9 ? "  " : " ") << board.move_to_str(moves[i]);
				}
			}
		}
		else if (user_input == "t") {
			Mind *mind_ptr = &mind;
			std::atomic<bool> stop (false);
			std::atomic<bool> *flag = &stop;
			std::thread thinking(&Mind::best_move_deepening, mind_ptr, std::ref(board), flag);
			std::this_thread::sleep_for(std::chrono::seconds(60));
			stop = true;
			thinking.join();
			std::cout << "BEST MOVE = " << board.move_to_str(mind.get_best_move());
			board.make_move(mind.get_best_move());
			board.print();
		}
		else if (user_input == "c") {
			std::string move_input;
			std::cin >> move_input;
			Move inmove = board.long_algebraic_to_move(move_input);
			std::cout << board.move_to_str(inmove);
		}
		else if (user_input == "x") {
			return 0;
		}
	}
	
}
