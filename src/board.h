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
    
	// bit boards, one for each piece type
    uint64_t board[NO_OF_TYPES];

    int move_number;
    
	int half_move_number;
	
    bool turn;
    
	// did a pawn move forward by two spaces on the previous turn
    bool en_passantable;
    
    Square en_passant_square;
    
    bool can_white_castle_kingside;
    
    bool can_white_castle_queenside;
    
    bool can_black_castle_kingside;
    
    bool can_black_castle_queenside;
    
	// is ('rank', 'file') a legitimate board square?
	static bool on_board(int rank, int file);
	
	// is 'square' a legitimate board square?
    static bool on_board(Square square);
    
	// turns coordinate into bit position
	static int coor_to_bit_position(int r, int c);
    
	// prints the characters corresponding to piece 'type'
    static char type_to_char(int type);
	
	// STUFF FOR HASHING:
	// when a new board is initialised a new set of random longs is generated.
	// when implenting a search, child boards will inherit these from parents, so hashes will not change.
	// if the Mind needs to use the hashes, it can extract them from the current board.
	
	uint64_t hash;
	
	uint64_t hash_pieces[8][8][12];
	uint64_t hash_to_move;
	uint64_t hash_castling[4];
    
public:
    
	// constructor
    Board();
	
	// contains move vectors for all the pieces, if applicable, and the direction of the move (for knights this is -1, the 'null' direction).
	static const std::vector<Square> move_vectors[12];
	
	// whether each piece can move arbitrarily far.
	static const bool is_limited[12];
	
	// whether each piece attacks in a given direction.
	static const bool is_attacking[12][8];
	
	// vectors pointing in each of the 8 directions.
	static const Square direction_to_vector[8];
	
	// turns int rank into human-readable char.
	static const char rank_to_char[8];
	
	// turns int file into human-readable char.
	static const char file_to_char[8];
	
	// returns a random long (used for hashing).
	static uint64_t rand64();
	
	// returns the current turn.
	bool get_turn();
	
	// scans along the bit string type and returns the position of the first piece it finds.
	// especially useful for locating the king, since there can be only one of them.
    Square find_piece(Piece type);
    
	// returns the piece type at 'square', and 12 if there is no piece there.
    Piece piece_at(Square square);
	
	// returns the piece type at ('rank', 'file') and 12 if there is no piece there.
	Piece piece_at(int rank, int file);
    
	// returns true if piece 'type' is at ('rank', 'file'), false otherwise.
    bool is_piece_at(Piece type, int rank, int file);
    
	// returns true if any piece is at ('rank', 'file'), false otherwise.
	bool is_piece_at(int rank, int file);
	
	// return true if any piece is at 'square', false otherwise.
	bool is_piece_at(Square square);
	
	// returns true if piece 'type' is at 'bit' position.
    bool is_piece_at(Piece type, int64_t bit);
    
	// toggles piece 'type' at ('rank', 'file') -- updates hash.
    void toggle_piece(Piece type, int rank, int file);
	
	// put piece 'type' at ('rank', 'file').
	void put_piece(Piece type, int rank, int file);
	
	// put piece 'type' at 'square'
	void put_piece(Piece type, Square square);
	
	// remove piece at ('rank', 'file').
	void remove_piece(int rank, int file);
	
	// remove piece at 'square'.
	void remove_piece(Square square);
	
	// returns the side of the piece at ('rank', 'file').
	bool piece_at_side(int rank, int file);
	
	// returns the side of the piece at 'square'.
	bool piece_at_side(Square square);
	
	// prints board.
    void print();
	
	// returns the square ('rank', 'file').
	Square sq(int rank, int file);
	
	// just a shortcut for checking whether 'vector' contains 'square'.
	bool is_in(Square square, const std::vector<Square> &vector);
	
	// returns a vector of all the squares in a certain 'direction' looking out from 'initial_square'.
	// stops when it reaches the 'N'th square with a piece on.
	std::vector<Square> look_along(Square initial_square, int direction, int N);
	
	// returns a list of all pieces belonging to 'side' that can move to 'square'.
	// if 'taking' is true, pieces move as if taking (difference is only for pawns).
	// includes direction from which the attack is coming, but not for knights and pawns.
	std::vector<Square> attacking_squares(Square square, bool side, bool taking);
	
	// Contains the positions of pieces which cannot move without exposing the king to check.
	// Includes the direction from which the attack is coming.
	std::vector<Square> find_constrained_squares();
	
	// returns a vector containing all possible moves in the current position.
	std::vector<Move> get_moves();
    
	// makes 'move', updates board position and hash.
	void make_move(Move &move);
	
	// Reads in FEN string and arranges the board accordingly.
	void get_FEN(std::string FEN);
	
	// Returns the string for 'move' in ordinary notation.
	std::string move_to_str(Move &move);
	
	// HASHING FUNCTIONS
	
	// generate the hash for the current board. should only be used for new boards.
	uint64_t generate_hash();
	
	// returns the board's current hash.
	uint64_t get_hash();
	
};

#endif /* Board_h */