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

class Mind {
	
private:
	
	// relative values of the pieces.
	static const int piece_value[12];
	
	Move best_move;
	
public:

	Mind();
	
	// returns the value of 'board'.
	int board_value(Board &board);
	
	// returns the alpha-beta score of 'board' at search depth 'depth'.
	int alpha_beta(Board &board, std::vector<Move> &moves, int depth, int alpha, int beta, bool side);
	
	// uses basic alpha beta pruning to 'depth' to find the best move
	Move best_move_alpha_beta(Board &board, int depth);
	
};

#endif /* mind_h */