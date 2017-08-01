//
//  chess.hpp
//  Chess
//
//  Created by Jack Setford on 17/04/2017.
//  Copyright © 2017 Jack Setford. All rights reserved.
//

#ifndef chess_hpp
#define chess_hpp

#include <utility>

using pair = std::pair<int, int>;

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

#include "move.hpp"
#include "board.hpp"


#endif /* chess_hpp */
