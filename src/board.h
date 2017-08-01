//
//  Board.h
//  Chess
//
//  Created by Jack Setford on 17/04/2017.
//  Copyright © 2017 Jack Setford. All rights reserved.
//

#ifndef Board_h
#define Board_h

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
    
    static bool on_board(int rank, int file);
    // is ('rank', 'file') a legitimate board square?
    
	static int coor_to_bit_position(int r, int c);
	// turns coordinate into bit position
    
    static char type_to_char(int type);
    // prints the characters corresponding to piece 'type'
    
public:
    
    Board();
    // constructor
	
	static const std::vector<Square> move_vectors[12];
	// contains move vectors for all the pieces, if applicable.
	
	static const bool is_limited[12];
	
    Square find_piece(Piece type);
	// scans along the bit string type and returns the positions of the first piece it finds.
	// especially useful for locating the king, since there can be only one of them.
    
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
    
    static int direction_to_vector(int direction, bool rank_or_file);
    // takes direction, which is a number between 0 and 7, and returns a vector
	
	std::vector<Square> look_along(Square initial_square, int direction, int N);
	// returns a vector of all the squares in a certain 'direction' looking out from 'initial_square'.
	// stops when it reaches the 'N'th square with a piece on.
	
	std::vector<Move> get_moves();
	// returns a vector containing all possible moves in the current position
    
};

#endif /* Board_h */