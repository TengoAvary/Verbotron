//
//  move.hpp
//  Chess
//
//  Created by Jack Setford on 17/04/2017.
//  Copyright © 2017 Jack Setford. All rights reserved.
//

#ifndef move_hpp
#define move_hpp

class Move {
    
private:
    int initial_position_rank;
    int initial_position_file;
    int final_position_rank;
    int final_position_file;
    int moveType;  // 0 - normal move, 1 - promotion, 2 - castle kingside, 3 - castle queenside, 4 en passant
    bool piece_taken;
    bool side;
    int promotionType;  // piece type that pawn is promoted to
    int pieceType;
    int takenPieceType;
    
};

#endif /* move_hpp */
