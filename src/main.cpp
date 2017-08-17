#include <iostream>
#include <sys/types.h>
#include <vector>
#include <cassert>
#include <algorithm>

#include "chess.h"

int main(int argc, const char * argv[]) {
	
	Board board;
	Mind mind;
	
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
			std::cin >> move_choice;
			board.make_move(moves[move_choice-1]);
			board.print();
		}
		else if (user_input == "b") {
			Move best = mind.best_move_alpha_beta(board, 6);
			std::cout << board.move_to_str(best);
			board.make_move(best);
			board.print();
		}
		else if (user_input == "x") {
			return 0;
		}
	}
	
}
