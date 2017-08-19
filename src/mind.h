//
//  Mind.h
//  Chess
//
//  Created by Jack Setford on 17/04/2017.
//  Copyright © 2017 Jack Setford. All rights reserved.
//

#ifndef mind_h
#define mind_h

#include <iostream>
#include <sys/types.h>
#include <vector>
#include <unordered_map>

class Mind {
	
private:
	
	// relative values of the pieces.
	static const int piece_value[12];
	
	// contains scores of boards already analysed.
	// The key is the board hash, and the vaule is the tuple <depth, value>.
	std::unordered_map<uint64_t, std::tuple<int, int>> hash_table;
	
	// Mind's current opinion on what the best move is.
	Move best_move;

public:

	Mind();
	
	// returns the value of 'board'.
	int board_value(Board &board);
	
	// stores the 'value' of 'board' if and only if another value found at a deeper 'depth' is not already stored
	void store(Board &board, int depth, int value);
	
	// returns the alpha-beta score of 'board' at search 'depth'.
	int alpha_beta(Board &board, std::vector<Move> &moves, int depth, int main_depth, int alpha, int beta, bool side);
	
	// uses basic alpha beta pruning to 'depth' to find the best move
	Move best_move_alpha_beta(Board &board, int depth);
	
};

#endif /* mind_h */