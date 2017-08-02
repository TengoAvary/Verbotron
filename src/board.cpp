//
//  Board.cpp
//  Chess
//
//  Created by Jack Setford on 17/04/2017.
//  Copyright © 2017 Jack Setford. All rights reserved.
//

#include <iostream>
#include <sys/types.h>
#include <vector>
#include <cassert>

#include "chess.h"

// initialise move_vectors

const std::vector<Square> Board::move_vectors[12] {{}, 
													{{2,1},{2,-1},{1,2},{1,-2},{-2,1},{-2,-1},{-1,2},{-1,-2}}, 
													{{1,1},{1,-1},{-1,1},{-1,-1}}, 
													{{1,0},{-1,0},{0,1},{0,-1}}, 
													{{1,1},{1,-1},{-1,1},{-1,-1},{1,0},{-1,0},{0,1},{0,-1}}, 
													{{1,1},{1,-1},{-1,1},{-1,-1},{1,0},{-1,0},{0,1},{0,-1}}, 
													{}, 
													{{2,1},{2,-1},{1,2},{1,-2},{-2,1},{-2,-1},{-1,2},{-1,-2}}, 
													{{1,1},{1,-1},{-1,1},{-1,-1}}, 
													{{1,0},{-1,0},{0,1},{0,-1}}, 
													{{1,1},{1,-1},{-1,1},{-1,-1},{1,0},{-1,0},{0,1},{0,-1}}, 
													{{1,1},{1,-1},{-1,1},{-1,-1},{1,0},{-1,0},{0,1},{0,-1}}};

const bool Board::is_limited[12] {true, true, false, false, false, true, true, true, false, false, false, true};

const Square Board::direction_to_vector[8] {{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1-1}};

bool Board::on_board(Square square) {
    if (square.rank < 0 || square.rank >= 8 || square.file < 0 || square.file >= 8) {
        return false;
    }
    else {
        return true;
    }
}

Board::Board()
    : board {}, // set all bitboards to zero first
    move_number {1},
    turn {true},
    en_passantable {false},
    en_passant_square_rank {0},
    en_passant_square_file {0},
    can_white_castle_kingside {true},
    can_white_castle_queenside {true},
    can_black_castle_kingside {true},
    can_black_castle_queenside {true}
{
    // then set up the board
    put_piece(WHITE_ROOK, 0, 0);
    put_piece(WHITE_KNIGHT, 0, 1);
    put_piece(WHITE_BISHOP, 0, 2);
    put_piece(WHITE_QUEEN, 0, 3);
    put_piece(WHITE_KING, 0, 4);
    put_piece(WHITE_BISHOP, 0, 5);
    put_piece(WHITE_KNIGHT, 0, 6);
    put_piece(WHITE_ROOK, 0, 7);
    put_piece(BLACK_ROOK, 7, 0);
    put_piece(BLACK_KNIGHT, 7, 1);
    put_piece(BLACK_BISHOP, 7, 2);
    put_piece(BLACK_QUEEN, 7, 3);
    put_piece(BLACK_KING, 7, 4);
    put_piece(BLACK_BISHOP, 7, 5);
    put_piece(BLACK_KNIGHT, 7, 6);
    put_piece(BLACK_ROOK, 7, 7);
    for (int file = 0; file<8; file++) {
        put_piece(WHITE_PAWN, 1, file);
        put_piece(BLACK_PAWN, 6, file);
    }
}

int Board::coor_to_bit_position(int r, int c)
{
    
    return 8*r + c;
    
}

Square Board::find_piece(Piece type)
{

	uint64_t test_bit = uint64_t(1);
	for (int bit_position = 0; bit_position<64; bit_position++) {
		if (test_bit & board[type]) {
			Square result {bit_position/8, bit_position%8};
			return result;
		}
		else {
			test_bit <<= 1;
		}
	}
	
	Square nullresult {-1,-1};
	return nullresult;
	
}

char Board::type_to_char(int type)
{
    
    switch (type) {
        case 0 : return 'P';
        case 1 : return 'N';
        case 2 : return 'B';
        case 3 : return 'R';
        case 4 : return 'Q';
        case 5 : return 'K';
        case 6 : return 'p';
        case 7 : return 'n';
        case 8 : return 'b';
        case 9 : return 'r';
        case 10 : return 'q';
        case 11 : return 'k';
        case 12 : return 'O';
        default : return '!';
    }
    
}

int Board::piece_at(Square square)
{
    
    uint64_t bit = uint64_t(1) << coor_to_bit_position(square.rank, square.file);
    for (int i = 0; i<NO_OF_TYPES; i++) {
        if (board[i] & bit) {
            return i;
        }
    }
    return 12;
}

int Board::piece_at(int rank, int file)
{
    
    uint64_t bit = uint64_t(1) << coor_to_bit_position(rank, file);
    for (int i = 0; i<NO_OF_TYPES; i++) {
        if (board[i] & bit) {
            return i;
        }
    }
    return 12;
}

bool Board::is_piece_at(Piece type, int rank, int file)
{
    
    uint64_t bit = uint64_t(1) << coor_to_bit_position(rank, file);
    return (board[type] & bit);
    
}

bool Board::is_piece_at(Square square) {
	
	uint64_t bit = uint64_t(1) << coor_to_bit_position(square.rank, square.file);
    for (int i = 0; i<NO_OF_TYPES; i++) {
        if (board[i] & bit) {
            return true;
        }
    }
    return false;
	
}

bool Board::is_piece_at(Piece type, int64_t bit)
{
    
    return (board[type] & bit);
    
}

void Board::put_piece(Piece type, int rank, int file)
{
    
    board[type] |= uint64_t(1) << coor_to_bit_position(rank, file);
    
}

void Board::remove_piece(int rank, int file)
{
    
    for(int i = 0; i<NO_OF_TYPES; i++) {
        board[i] -= board[i] & (uint64_t(1)<<coor_to_bit_position(rank,file));
    }
    
}

void Board::print()
{
    
    for (int rank = 7; rank>=0; rank--) {
        for (int file = 0; file<8; file++) {
            std::cout << type_to_char(piece_at(rank, file));
        }
        std::cout << '\n';
    }
    
}

std::vector<Square> Board::look_along(Square initial_square, int direction, int N)
{
	
	std::vector<Square> result;
	
	Square new_square = initial_square;
	Square move_vector = direction_to_vector[direction];
	
	for (int n = 1; n<8; n++) {
		new_square.rank += move_vector.rank;
		new_square.file += move_vector.file;
		if (on_board(new_square)) {
			result.push_back(new_square);
			if (is_piece_at(new_square)) {
				N--;
				if (N == 0) {
					return result;
				}
			}
		}
	}
	return result;
	
}

std::vector<Move> Board::get_moves()
{
	
	std::vector<Move> result;
	
	std::vector<Square> constrained_squares;
	// Will contain the positions of pieces which cannot move without exposing the king to check.
	// Includes the direction from which the attack is coming.
	
	// ** CALCULATING CONSTRAINED_SQUARES **
	
	// First we find the position of the king.
	Square king_square = find_piece(turn ? WHITE_KING : BLACK_KING);
	// Check that we found the king.
	assert(king_square.rank != -1 && "King not found.");
    
	
	return result;
	
}