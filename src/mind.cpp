//
//  Mind.cpp
//  Chess
//
//  Created by Jack Setford on 17/04/2017.
//  Copyright © 2017 Jack Setford. All rights reserved.
//

#include <iostream>
#include <sys/types.h>
#include <vector>
#include <algorithm>
#include <cassert>
#include <unordered_map>

#include "chess.h"

Mind::Mind()
: best_move {{0,0}, {0,0}, 5, false}
{

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

int Mind::alpha_beta_ordering(Board &board, std::vector<Move> &moves, int depth, int main_depth, int alpha, int beta, bool side)
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
	
	uint64_t board_hash = board.get_hash();
	auto it = hash_table.find(board_hash);
	if (it != hash_table.end() && std::get<0>(hash_table[board_hash]) >= depth) {
		return std::get<1>(hash_table[board_hash]);
	}
	
	Move best_move_inner = moves[0];
	
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
	if (side) {
		std::sort(begin(move_values), end(move_values), [](std::tuple<int, Move> const &t1, std::tuple<int, Move> const &t2) {
			return std::get<0>(t1) > std::get<0>(t2);
		});
	}
	else {
		std::sort(begin(move_values), end(move_values), [](std::tuple<int, Move> const &t1, std::tuple<int, Move> const &t2) {
			return std::get<0>(t1) < std::get<0>(t2);
		});
	}
	
	//for(int i = 0; i<move_values.size(); i++) {
	//	std::cout << std::get<0>(move_values[i]) << " " << board.move_to_str(std::get<1>(move_values[i])) << "\n";
	//}
	
	if (depth == main_depth) {
		std::cout << "considering " << std::get<0>(move_values[0]) << " " << board.move_to_str(std::get<1>(move_values[0]));
	}
	
	if (side) {
		int v = -1000;
		for (unsigned int i = 0; i<move_values.size(); i++) {
			Board new_board(board);
			new_board.make_move(std::get<1>(move_values[i]));
			std::vector<Move> new_moves = new_board.get_moves();
			int new_v = alpha_beta(new_board, new_moves, depth-1, main_depth, alpha, beta, false);
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
		if (depth == main_depth) {
			best_move = best_move_inner;
		}
		// save board value to hash_table:
		store(board, depth, v);
		return v;
	}
	else {
		int v = 1000;
		for (unsigned int i = 0; i<move_values.size(); i++) {
			Board new_board(board);
			new_board.make_move(std::get<1>(move_values[i]));
			std::vector<Move> new_moves = new_board.get_moves();
			int new_v = alpha_beta(new_board, new_moves, depth-1, main_depth, alpha, beta, true);
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
		if (depth == main_depth) {
			best_move = best_move_inner;
		}
		// save board value to hash_table:
		store(board, depth, v);
		return v;
	}
	
}

Move Mind::best_move_alpha_beta(Board &board, int depth)
{
	
	std::vector<Move> moves = board.get_moves();
	alpha_beta_ordering(board, moves, depth, depth, -1000, 1000, board.get_turn());
	return best_move;
	
}

void Mind::best_move_deepening(Board &board)
{
	
	std::vector<Move> moves = board.get_moves();
	
	int depth = 1;
	while (true) {
		
		std::cout << "depth = " << depth << "\n";
		
		alpha_beta_ordering(board, moves, depth, depth, -1000, 1000, board.get_turn());
		
		depth++;
		
	}
	
}