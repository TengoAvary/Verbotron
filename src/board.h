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
    
	int half_move_number;
	
    bool turn;
    
    bool en_passantable;
    // did a pawn move forward by two spaces on the previous turn
    
    Square en_passant_square;
    
    bool can_white_castle_kingside;
    
    bool can_white_castle_queenside;
    
    bool can_black_castle_kingside;
    
    bool can_black_castle_queenside;
    
	static bool on_board(int rank, int file);
	// is ('rank', 'file') a legitimate board square?
	
    static bool on_board(Square square);
    // is 'square' a legitimate board square?
    
	static int coor_to_bit_position(int r, int c);
	// turns coordinate into bit position
    
    static char type_to_char(int type);
    // prints the characters corresponding to piece 'type'
    
public:
    
    Board();
    // constructor
	
	static const std::vector<Square> move_vectors[12];
	// contains move vectors for all the pieces, if applicable, and the direction of the move (for knights this is -1, the 'null' direction).
	
	static const bool is_limited[12];
	// whether each piece can move arbitrarily far.
	
	static const bool is_attacking[12][8];
	// whether each piece attacks in a given direction.
	
	static const Square direction_to_vector[8];
	// vectors pointing in each of the 8 directions.
	
	static const char rank_to_char[8];
	// turns int rank into human-readable char
	
	static const char file_to_char[8];
	// turns int file into human-readable char.
	
    Square find_piece(Piece type);
	// scans along the bit string type and returns the position of the first piece it finds.
	// especially useful for locating the king, since there can be only one of them.
    
    Piece piece_at(Square square);
    // returns the piece type at 'square', and 12 if there is no piece there.
	
	Piece piece_at(int rank, int file);
	// returns the piece type at {'rank', 'file'} and 12 if there is no piece there.
    
    bool is_piece_at(Piece type, int rank, int file);
    // returns true if piece 'type' is at ('rank', 'file'), false otherwise.
    
	bool is_piece_at(int rank, int file);
	// returns true if any piece is at ('rank', 'file'), false otherwise.
	
	bool is_piece_at(Square square);
	// return true if any piece is at 'square', false otherwise.
	
    bool is_piece_at(Piece type, int64_t bit);
    // returns true if piece 'type' is at 'bit' position.
    
    void put_piece(Piece type, int rank, int file);
    // puts piece 'type' at ('rank', 'file').
    
	void put_piece(Piece type, Square square);
	// puts piece 'type' at 'square'.
	
    void remove_piece(int rank, int file);
    // removes piece (any type) from ('rank', 'file').
    
	void remove_piece(Square square);
	// remove piece (any type) from 'square'.
	
	bool piece_at_side(int rank, int file);
	// returns the side of the piece at ('rank', 'file').
	
	bool piece_at_side(Square square);
	// returns the side of the piece at 'square'.
	
    void print();
    // prints board.
	
	Square sq(int rank, int file);
	// returns the square ('rank', 'file').s
	
	bool is_in(Square square, std::vector<Square> vector);
	// just a shortcut for checking whether 'vector' contains 'square'.
	
	std::vector<Square> look_along(Square initial_square, int direction, int N);
	// returns a vector of all the squares in a certain 'direction' looking out from 'initial_square'.
	// stops when it reaches the 'N'th square with a piece on.
	
	std::vector<Square> attacking_squares(Square square, bool side, bool taking);
	// returns a list of all pieces belonging to 'side' that can move to 'square'.
	// if 'taking' is true, pieces move as if taking (difference is only for pawns).
	// includes direction from which the attack is coming, but not for knights and pawns.
	
	std::vector<Square> find_constrained_squares();
	// Contains the positions of pieces which cannot move without exposing the king to check.
	// Includes the direction from which the attack is coming.
	
	std::vector<Move> get_moves();
	// returns a vector containing all possible moves in the current position.
    
	void make_move(Move move);
	// makes 'move', updates board position.
	
	void get_FEN(std::string FEN);
	// Reads in FEN string and arranges the board accordingly.
	
	std::string move_to_str(Move move);
	// Returns the string for 'move' in ordinary notation.
	
};

#endif /* Board_h */