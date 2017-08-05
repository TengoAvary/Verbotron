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
	
    int move_type;  
	// 0 - normal move, 1 - promotion, 2 - castle kingside, 3 - castle queenside, 4 en passant
	
	bool piece_taken;
	
    Piece promotion_piece;
	// piece type that pawn is promoted to
	
    Piece moving_piece;
	// piece which is moving
	
	Piece taken_piece;
	// piece which is taken
    
public:
	
 	Move(Square initial_position_in, Square final_position_in, int move_type_in, 
	bool piece_taken_in, Piece moving_piece_in, Piece taken_piece = NO_OF_TYPES, Piece promotion_piece_in = NO_OF_TYPES);
	// constructor -- taken piece and promotion piece have default values since they might not be used
	
	Piece get_piece();
	// returns 'moving_piece'
	
	Square get_initial_position();
	// returns 'initial_position'
	
	Square get_final_position();
	// returns 'final_position'
	
	int get_move_type();
	// returns 'move_type'
	
	bool is_piece_taken();
	// returns 'piece_taken'
	
};

#endif /* move_h */