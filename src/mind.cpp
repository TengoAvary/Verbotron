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

#include "chess.h"

const int Mind::piece_value[12] {1, 3, 3, 5, 9, 1000, -1, -3, -3, -5, -9, -1000};

Mind::Mind()
: best_move {{0,0}, {0,0}, 5, false}
{

}

int Mind::board_value(Board &board)
{
	
	int result = 0;
	
	for (int rank = 0; rank<8; rank++) {
		for (int file = 0; file<8; file++) {
			if (board.is_piece_at(rank, file)) {
				result += piece_value[board.piece_at(rank, file)];
			}
		}
	}
	
	return result;
	
}

int Mind::alpha_beta(Board &board, std::vector<Move> &moves, int depth, int alpha, int beta, bool side)
{
	
	// check for checkmate:
	if (moves.empty()) {
		return (side ? -1000 : 1000);
	}
	// check for stalemate:
	if (moves[0].get_move_type() == 5) {
		return 0;
	}
	if (depth == 0) {
		return board_value(board);
	}
	
	Move best_move_inner = moves[0];
	
	if (side) {
		int v = -1000;
		for (int i = 0; i<moves.size(); i++) {
			Board new_board(board);
			new_board.make_move(moves[i]);
			std::vector<Move> new_moves = new_board.get_moves();
			int new_v = alpha_beta(new_board, new_moves, depth-1, alpha, beta, false);
			if (new_v > v) {
				v = new_v;
				best_move_inner = moves[i];
			}
			alpha = std::max(alpha, v);
			if (beta <= alpha) {
				break;
			}
		}
		best_move = best_move_inner;
		return v;
	}
	else {
		int v = 1000;
		for (int i = 0; i<moves.size(); i++) {
			Board new_board(board);
			new_board.make_move(moves[i]);
			std::vector<Move> new_moves = new_board.get_moves();
			int new_v = alpha_beta(new_board, new_moves, depth-1, alpha, beta, true);
			if (new_v < v) {
				v = new_v;
				best_move_inner = moves[i];
			}
			beta = std::min(beta, v);
			if (beta <= alpha) {
				break;
			}
		}
		best_move = best_move_inner;
		return v;
	}
	
}

Move Mind::best_move_alpha_beta(Board &board, int depth)
{
	
	std::vector<Move> moves = board.get_moves();
	alpha_beta(board, moves, depth, -1000, 1000, board.get_turn());
	return best_move;
	
}
