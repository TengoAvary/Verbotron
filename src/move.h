//
//  move.h
//  Chess
//
//  Created by Jack Setford on 17/04/2017.
//  Copyright © 2017 Jack Setford. All rights reserved.
//

#ifndef move_h
#define move_h

#include "chess.h"

class Move {
    
private:

    Square initial_position;
	
    Square final_position;
	
	// 0 - normal move, 1 - promotion, 2 - castle kingside, 3 - castle queenside, 4 en passant, 5 null (used to signify stalemate)
    int move_type;
	
	bool piece_taken;
	
	// piece type that pawn is promoted to
    Piece promotion_piece;
	
    
public:
	
	// constructor -- taken piece and promotion piece have default values since they might not be used
 	Move(Square initial_position_in, Square final_position_in, int move_type_in, 
	bool piece_taken_in, Piece promotion_piece_in = NO_OF_TYPES);
	
	// returns 'initial_position'
	Square get_initial_position();
	
	// returns 'final_position'
	Square get_final_position();
	
	// returns 'move_type'
	int get_move_type();
	
	// returns 'piece_taken'
	bool is_piece_taken();
	
	// returns 'promotion_piece'
	Piece get_promotion_piece();
	
};

#endif /* move_h */