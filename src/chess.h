//
//  chess.h
//  Chess
//
//  Created by Jack Setford on 17/04/2017.
//  Copyright © 2017 Jack Setford. All rights reserved.
//

#ifndef chess_h
#define chess_h

#include <utility>

enum Piece {
    
    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING,
    NO_OF_TYPES
    
};

struct Square {
	
	int rank;
	int file;
	int direction = 0; // used in get_moves function
	
};

#include "move.h"
#include "board.h"


#endif /* chess_h */