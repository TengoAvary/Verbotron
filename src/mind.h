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
	
	// contains scores of boards already analysed.
	// The key is the board hash, and the vaule is the tuple <depth, value>.
	std::unordered_map<uint64_t, std::tuple<int, int>> hash_table;
	
	// opening book
	std::unordered_map<uint64_t, Move> openings;
	
	// Mind's current opinion on what the best move is.
	Move best_move;
	
	// List of moves, ordered in preference of Mind's evaulation so far.
	std::vector<Move> move_list;
	
	

public:

	Mind();

	// stores the 'value' of 'board' if and only if another value found at a deeper 'depth' is not already stored
	void store(Board &board, int depth, int value);
	
	// returns the alpha-beta score of 'board' at search 'depth'.
	int alpha_beta(Board &board, std::vector<Move> &moves, int depth, int main_depth, int alpha, int beta, bool side);
	
	// returns the alpha-beta score of 'board' at search 'depth', ordering moves based on previous analyses
	int alpha_beta_ordering(Board &board, std::vector<std::tuple<int, Move>> &move_value, int depth, int main_depth, int alpha, int beta, bool side, std::atomic<bool> *flag);
	
	// uses iterative deepening with move sorting to find the best move. Keeps thinking forever.
	void best_move_deepening(Board &board, std::atomic<bool> *flag);
	
	// returns best_move, as stored in memory (doesn't calculate anything!)
	Move &get_best_move();
	
};

#endif /* mind_h */