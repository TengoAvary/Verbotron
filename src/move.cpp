//
//  move.cpp
//  Chess
//
//  Created by Jack Setford on 17/04/2017.
//  Copyright © 2017 Jack Setford. All rights reserved.
//

#include "chess.h"

Move::Move(Square initial_position_in, Square final_position_in, int move_type_in, 
	bool piece_taken_in, Piece moving_piece_in, Piece promotion_piece_in,
	Piece taken_piece)
	// initializer list
	: initial_position {initial_position_in},
	final_position {final_position_in},
	move_type {move_type_in},
	piece_taken {piece_taken_in},
	moving_piece {moving_piece_in},
	promotion_piece {promotion_piece_in}
{
		
}