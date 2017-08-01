//
//  Board.hpp
//  Chess
//
//  Created by Jack Setford on 17/04/2017.
//  Copyright © 2017 Jack Setford. All rights reserved.
//

#ifndef Board_hpp
#define Board_hpp

#include <iostream>
#include <sys/types.h>
#include <vector>

class Board {
    
private:
    
    uint64_t board[NO_OF_TYPES];
    // bit boards, one for each piece type
    
    int move_number;
    
    bool turn;
    
    bool en_passantable;
    // did a pawn move forward by two spaces on the previous turn
    
    int en_passant_square_rank;
    
    int en_passant_square_file;
    
    bool can_white_castle_kingside;
    
    bool can_white_castle_queenside;
    
    bool can_black_castle_kingside;
    
    bool can_black_castle_queenside;
    
    bool on_board(int rank, int file);
    // is ('rank', 'file') a legitimate board square?
    
    int coor_to_bit(int rank, int file);
    // converts ('rank', 'file') into an int in the range (0,63) (bit position)
    
    int bit_to_rank(int b);
    // converts bit position to rank
    
    int bit_to_file(int b);
    // converts bit position to file
    
    char type_to_char(int type);
    // prints the characters corresponding to piece 'type'
    
public:
    
    Board();
    // constructor
    
    int piece_at(int rank, int file);
    // returns the piece type at square ('rank', 'file'), and 12 if there is no piece there.
    
    bool piece_at(Piece type, int rank, int file);
    // returns true if piece 'type' is at ('rank', 'file'), false otherwise
    
    bool piece_at(Piece type, int64_t bit);
    // returns true if piece 'type' is at 'bit' position
    
    void put_piece(Piece type, int rank, int file);
    // puts piece 'type' at ('rank', 'file')
    
    void remove_piece(int rank, int file);
    // removes piece (any type) from ('rank', 'file')
    
    void print();
    // prints board
    
    int direction_to_vector(int direction, bool rank_or_file);
    // takes direction, which is a number between 0 and 7, and returns a vector
    
};

#endif /* Board_hpp */
