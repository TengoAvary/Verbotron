#include <iostream>
#include <sys/types.h>
#include <vector>
#include <cassert>
#include <algorithm>

#include "chess.h"

int main(int argc, const char * argv[]) {
	
	Board board;
	Mind mind;
	
	board.get_FEN("1n4nr/1pq2k1p/5p2/5N2/2P1Q1p1/rP2P1P1/P4P1K/R4R2 b - - 0 26");
	
	board.print();
	
	std::string user_input;
	
	while (true) {
		std::getline(std::cin, user_input);
		if (user_input == "l") {
			std::vector<Move> moves = board.get_moves();
			for (int i = 0; i<moves.size(); i++) {
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
