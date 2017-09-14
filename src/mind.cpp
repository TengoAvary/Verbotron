//
//  Mind.cpp
//  Chess
//
//  Created by Jack Setford on 17/04/2017.
//  Copyright © 2017 Jack Setford. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <vector>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#include "chess.h"

Mind::Mind(Board &board)
: best_move {{0,0}, {0,0}, 5, false}
{
	
	// read openings from openings.txt
	std::ifstream openings_file("/Users/Jack/Documents/Verbotron/Verbotron/openings.txt");
	if (!openings_file.is_open()) {
		std::cout << "Openings table not found!\n";
	}
	else {
		Board new_board(board);
		std::string line;
		std::string delimiter = " >> ";
		while(getline(openings_file, line)) {
			std::string FEN = line.substr(0, line.find(delimiter));
			std::string move = line.substr(line.find(delimiter) + delimiter.length(), line.length());
			new_board.get_FEN(FEN);
			uint64_t board_hash = new_board.get_hash();
			auto it = openings.find(board_hash);
			if (it == openings.end()) {
				std::vector<Move> new_vector;
				new_vector.push_back(new_board.long_algebraic_to_move(move));
				openings.insert({board_hash, new_vector});
			}
			else {
				openings.at(board_hash).push_back(new_board.long_algebraic_to_move(move));
			}
		}
	}
	openings_file.close();
	
}

void Mind::store(Board &board, int depth, int value)
{
	
	uint64_t board_hash = board.get_hash();
	auto it = hash_table.find(board_hash);
	if (it == hash_table.end()) {
		hash_table.insert({board_hash, std::make_tuple(depth, value)});
	}
	else if (std::get<0>(hash_table[board_hash]) < depth) {
		hash_table[board_hash] = std::make_tuple(depth, value);
	}
	
}

int Mind::alpha_beta(Board &board, std::vector<Move> &moves, int depth, int main_depth, int alpha, int beta, bool side)
{
	
	// check for checkmate:
	if (moves.empty()) {
		store(board, depth, (side ? -1000 : 1000));
		return (side ? -1000 : 1000);
	}
	// check for stalemate:
	if (moves[0].get_move_type() == 5) {
		store(board, depth, 0);
		return 0;
	}
	if (depth == 0) {
		store(board, 0, board.get_value());
		return board.get_value();
	}
	
	Move best_move_inner = moves[0];
	
	if (side) {
		int v = -1000;
		for (unsigned int i = 0; i<moves.size(); i++) {
			Board new_board(board);
			new_board.make_move(moves[i]);
			std::vector<Move> new_moves = new_board.get_moves();
			int new_v = alpha_beta(new_board, new_moves, depth-1, main_depth, alpha, beta, false);
			if (new_v > v) {
				v = new_v;
				best_move_inner = moves[i];
			}
			alpha = std::max(alpha, v);
			if (beta <= alpha) {
				break;
			}
		}
		if (depth == main_depth) {
			best_move = best_move_inner;
		}
		// save board value to hash_table:
		store(board, depth, v);
		return v;
	}
	else {
		int v = 1000;
		for (unsigned int i = 0; i<moves.size(); i++) {
			Board new_board(board);
			new_board.make_move(moves[i]);
			std::vector<Move> new_moves = new_board.get_moves();
			int new_v = alpha_beta(new_board, new_moves, depth-1, main_depth, alpha, beta, true);
			if (new_v < v) {
				v = new_v;
				best_move_inner = moves[i];
			}
			beta = std::min(beta, v);
			if (beta <= alpha) {
				break;
			}
		}
		if (depth == main_depth) {
			best_move = best_move_inner;
		}
		// save board value to hash_table:
		store(board, depth, v);
		return v;
	}
	
}

int Mind::alpha_beta_ordering(Board &board, std::vector<std::tuple<int, Move>> &move_values, int depth, int main_depth, int alpha, int beta, bool side, std::atomic<bool> *flag)
{
	
	if (*flag) {
		// indicates the calculation was interrupted
		return 1000000;
	}
	
	// check for checkmate:
	if (move_values.empty()) {
		store(board, depth, (side ? -1000 : 1000));
		return (side ? -1000 : 1000);
	}
	// check for stalemate:
	if (std::get<1>(move_values[0]).get_move_type() == 5) {
		store(board, depth, 0);
		return 0;
	}
	if (depth == 0) {
		store(board, 0, board.get_value());
		return board.get_value();
	}
	
	// check position hasn't already been evaluated at a greater depth:
	uint64_t board_hash = board.get_hash();
	auto it = hash_table.find(board_hash);
	if (it != hash_table.end() && std::get<0>(hash_table[board_hash]) >= depth) {
		return std::get<1>(hash_table[board_hash]);
	}
	
	Move best_move_inner = std::get<1>(move_values[0]);
	
	// print move values
	//if (depth == main_depth) {
	//	for (int i = 0; i<move_values.size(); i++) {
	//		Board new_board(board);
	//		new_board.make_move(std::get<1>(move_values[i]));
	//		std::cout << std::get<0>(move_values[i]) << " " << board.move_to_str(std::get<1>(move_values[i]));
	//	}
	//}
	
	if (side) {
		int v = -1000;
		for (unsigned int i = 0; i<move_values.size(); i++) {
			Board board1(board);
			board1.make_move(std::get<1>(move_values[i]));
			std::vector<Move> moves1 = board1.get_moves();
			// sort possible moves by value stored in hash_table, or by their naive value
			std::vector<std::tuple<int, Move>> move_values1;
			for (unsigned int move = 0; move<moves1.size(); move++) {
				Board board2(board1);
				board2.make_move(moves1[move]);
				uint64_t board2_hash = board2.get_hash();
				auto it = hash_table.find(board2_hash);
				if (it != hash_table.end()) {
					move_values1.push_back(std::make_tuple(std::get<1>(hash_table[board2_hash]), moves1[move]));
				}
				else {
					move_values1.push_back(std::make_tuple(board2.get_value(), moves1[move]));
				}
			}
			std::sort(begin(move_values1), end(move_values1), [](std::tuple<int, Move> const &t1, std::tuple<int, Move> const &t2) {
				return std::get<0>(t1) < std::get<0>(t2);
			});
			int new_v = alpha_beta_ordering(board1, move_values1, depth-1, main_depth, alpha, beta, false, flag);
			std::get<0>(move_values[i]) = new_v;
			if (new_v == 1000000) {
				return 1000000;
			}
			if (new_v > v) {
				v = new_v;
				best_move_inner = std::get<1>(move_values[i]);
				if (depth == main_depth) {
					std::cout << board.move_to_str(best_move_inner);
				}
			}
			alpha = std::max(alpha, v);
			if (beta <= alpha) {
				break;
			}
		}
		// save board value to hash_table:
		store(board, depth, v);
		if (depth == main_depth) {
			best_move = best_move_inner;
		}
		return v;
	}
	else {
		int v = 1000;
		for (unsigned int i = 0; i<move_values.size(); i++) {
			Board board1(board);
			board1.make_move(std::get<1>(move_values[i]));
			std::vector<Move> moves1 = board1.get_moves();
			// sort possible moves by value stored in hash_table, or by their naive value
			std::vector<std::tuple<int, Move>> move_values1;
			for (unsigned int move = 0; move<moves1.size(); move++) {
				Board board2(board1);
				board2.make_move(moves1[move]);
				uint64_t board2_hash = board2.get_hash();
				auto it = hash_table.find(board2_hash);
				if (it != hash_table.end()) {
					move_values1.push_back(std::make_tuple(std::get<1>(hash_table[board2_hash]), moves1[move]));
				}
				else {
					move_values1.push_back(std::make_tuple(board2.get_value(), moves1[move]));
				}
			}
			std::sort(begin(move_values1), end(move_values1), [](std::tuple<int, Move> const &t1, std::tuple<int, Move> const &t2) {
				return std::get<0>(t1) > std::get<0>(t2);
			});
			int new_v = alpha_beta_ordering(board1, move_values1, depth-1, main_depth, alpha, beta, true, flag);
			std::get<0>(move_values[i]) = new_v;
			if (new_v == 1000000) {
				return 1000000;
			}
			if (new_v < v) {
				v = new_v;
				best_move_inner = std::get<1>(move_values[i]);
				if (depth == main_depth) {
					std::cout << board.move_to_str(best_move_inner);
				}
			}
			beta = std::min(beta, v);
			if (beta <= alpha) {
				break;
			}
		}
		// save board value to hash_table:
		store(board, depth, v);
		if (depth == main_depth) {
			best_move = best_move_inner;
		}
		return v;
	}
	
}

void Mind::best_move_deepening(Board &board, std::atomic<bool> *flag)
{
	
	hash_table.clear();
	std::vector<Move> moves = board.get_moves();
	// sort possible moves by value stored in hash_table, or by their naive value
	std::vector<std::tuple<int, Move>> move_values;
	for (unsigned int move = 0; move<moves.size(); move++) {
		Board new_board(board);
		new_board.make_move(moves[move]);
		uint64_t board_hash = new_board.get_hash();
		auto it = hash_table.find(board_hash);
		if (it != hash_table.end()) {
			move_values.push_back(std::make_tuple(std::get<1>(hash_table[board_hash]), moves[move]));
		}
		else {
			move_values.push_back(std::make_tuple(new_board.get_value(), moves[move]));
		}
	}
	
	int depth = 1;
	while (!*flag) {
		
		std::cout << "depth = " << depth << "\n";
		
		if (board.get_turn()) {
			std::sort(begin(move_values), end(move_values), [](std::tuple<int, Move> const &t1, std::tuple<int, Move> const &t2) {
				return std::get<0>(t1) > std::get<0>(t2);
			});
		}
		else {
			std::sort(begin(move_values), end(move_values), [](std::tuple<int, Move> const &t1, std::tuple<int, Move> const &t2) {
				return std::get<0>(t1) < std::get<0>(t2);
			});
		}
		
		alpha_beta_ordering(board, move_values, depth, depth, -1000, 1000, board.get_turn(), flag);
		
		depth++;
		
	}
	
}

Move &Mind::get_best_move()
{
	return best_move;
}

bool Mind::is_opening_position(Board &board)
{
	uint64_t board_hash = board.get_hash();
	auto it = openings.find(board_hash);
	return !(it == openings.end());
}

Move &Mind::best_move_from_openings(Board &board)
{
	srand (time(NULL));
	int index = rand() % openings.at(board.get_hash()).size();
	return openings.at(board.get_hash())[index];
}