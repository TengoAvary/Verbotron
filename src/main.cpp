#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <vector>
#include <cassert>
#include <algorithm>
#include <thread>
#include <atomic>
#include <chrono>
#include <sstream>
#include <iterator>
#include <stdlib.h>

#include "chess.h"

int main(int argc, const char * argv[]) {
	
	Board board;
	Mind mind(board);
	Commander commander;
	
	int think_time = 120;
	
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
			if (mind.is_opening_position(board)) {
				Move best_move = mind.best_move_from_openings(board);
				std::cout << "BEST MOVE = " << board.move_to_str(best_move);
				board.make_move(best_move);
				board.print();
			}
			else {
				Mind *mind_ptr = &mind;
				std::atomic<bool> stop (false);
				std::atomic<bool> *flag = &stop;
				std::thread thinking(&Mind::best_move_deepening, mind_ptr, std::ref(board), flag);
				std::this_thread::sleep_for(std::chrono::seconds(think_time));
				stop = true;
				thinking.join();
				std::cout << "BEST MOVE = " << board.move_to_str(mind.get_best_move());
				board.make_move(mind.get_best_move());
				board.print();
			}
		}
		else if (user_input == "q") {
			// "quick", useful for blitz games
			std::vector<Move> moves = board.get_moves();
			int move_choice;
			char sure;
			std::cin >> move_choice;
			board.make_move(moves[move_choice-1]);
			if (mind.is_opening_position(board)) {
				Move best_move = mind.best_move_from_openings(board);
				std::string move_string = board.move_to_str(best_move);
				board.make_move(best_move);
				moves = board.get_moves();
				for (unsigned int i = 0; i<moves.size(); i++) {
					std::cout << "(" << i+1 << ") --" << (i<9 ? "  " : " ") << board.move_to_str(moves[i]);
				}
				std::cout << "BEST MOVE = " << move_string;
			}
			else {
				Mind *mind_ptr = &mind;
				std::atomic<bool> stop (false);
				std::atomic<bool> *flag = &stop;
				std::thread thinking(&Mind::best_move_deepening, mind_ptr, std::ref(board), flag);
				std::this_thread::sleep_for(std::chrono::seconds(5));
				stop = true;
				thinking.join();
				Move best_move = mind.get_best_move();
				std::string move_string = board.move_to_str(best_move);
				board.make_move(best_move);
				moves = board.get_moves();
				for (unsigned int i = 0; i<moves.size(); i++) {
					std::cout << "(" << i+1 << ") --" << (i<9 ? "  " : " ") << board.move_to_str(moves[i]);
				}
				std::cout << "BEST MOVE = " << move_string;
			}
		}
		else if (user_input == "fen") {
			std::cout << board.export_FEN() << '\n';
		}
		else if (user_input =="long") {
			std::string long_move;
			std::cin >> long_move;
			Move move = board.long_algebraic_to_move(long_move);
			std::cout << board.move_to_str(move);
		}
		else if (user_input == "generate_opening_book") {
			
			std::ifstream openings_file("/home/jack/Dropbox/ecoe_out.txt");
			if (!openings_file.is_open()) {
				std::cout << "Openings table not found!\n";
			}
			else {
				std::ofstream output("/home/jack/Dropbox/openings_out.txt");
				std::string line;
				while(getline(openings_file, line)) {
					std::istringstream buf(line);
					std::istream_iterator<std::string> beg(buf), end;
					std::vector<std::string> tokens(beg, end);
					Board test_board;
					std::vector<Move> moves = test_board.get_moves();
					std::vector<std::string> move_list;
					for (int i = 0; i<moves.size(); i++) {
						move_list.push_back(test_board.move_to_str_raw(moves[i]));
					}
					for (int token = 0; token<tokens.size(); token++) {
						if (tokens[token].back() != '.') {
							int move = 0;
							std::cout << tokens[token] << '\n';
							while (move < move_list.size()) {
								if (move_list[move] == tokens[token]) {
									break;
								}
								move++;
							}
							if (move == move_list.size()) {
								std::cout << "move not found!";
								std::cout << line << '\n';
								return 0;
							}
							output << test_board.export_FEN() << " >> " << test_board.move_to_long_algebraic(moves[move]) << '\n';
							test_board.make_move(moves[move]);
							moves = test_board.get_moves();
							move_list.clear();
							for (int i = 0; i<moves.size(); i++) {
								move_list.push_back(test_board.move_to_str_raw(moves[i]));
							}
						}
					}
				}
				output.close();
			}
			
			openings_file.close();
			
		}
		else if (user_input == "x") {
			return 0;
		}
	}
	
}
