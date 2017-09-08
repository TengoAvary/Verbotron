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
#include <set>
#include <cassert>
#include <algorithm>
#include <cstdlib>


#include "chess.h"

// initialise move_vectors

const std::vector<Square> Board::move_vectors[12] {{}, 
													{{2,1,-1},{2,-1,-1},{1,2,-1},{1,-2,-1},{-2,1,-1},{-2,-1,-1},{-1,2,-1},{-1,-2,-1}}, 
													{{1,1,1},{-1,1,3},{-1,-1,5},{1,-1,7}}, 
													{{1,0,0},{0,1,2},{-1,0,4},{0,-1,6}}, 
													{{1,0,0},{1,1,1},{0,1,2},{-1,1,3},{-1,0,4},{-1,-1,5},{0,-1,6},{1,-1,7}}, 
													{{1,0,0},{1,1,1},{0,1,2},{-1,1,3},{-1,0,4},{-1,-1,5},{0,-1,6},{1,-1,7}}, 
													{}, 
													{{2,1,-1},{2,-1,-1},{1,2,-1},{1,-2,-1},{-2,1,-1},{-2,-1,-1},{-1,2,-1},{-1,-2,-1}}, 
													{{1,1,1},{-1,1,3},{-1,-1,5},{1,-1,7}}, 
													{{1,0,0},{0,1,2},{-1,0,4},{0,-1,6}}, 
													{{1,0,0},{1,1,1},{0,1,2},{-1,1,3},{-1,0,4},{-1,-1,5},{0,-1,6},{1,-1,7}}, 
													{{1,0,0},{1,1,1},{0,1,2},{-1,1,3},{-1,0,4},{-1,-1,5},{0,-1,6},{1,-1,7}}};

const bool Board::is_limited[12] {true, true, false, false, false, true, true, true, false, false, false, true};

const Square Board::direction_to_vector[8] {{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};

const bool Board::is_attacking[12][8] {{false, false, false, false, false, false, false, false},
										{false, false, false, false, false, false, false ,false},
										{false, true, false, true, false, true, false, true},
										{true, false, true, false, true, false, true, false},
										{true, true, true, true, true, true, true, true},
										{false, false, false, false, false, false, false, false},
										{false, false, false, false, false, false, false, false},
										{false, false, false, false, false, false, false ,false},
										{false, true, false, true, false, true, false, true},
										{true, false, true, false, true, false, true, false},
										{true, true, true, true, true, true, true, true},
										{false, false, false, false, false, false, false, false}};

const char Board::rank_to_char[8] {'1', '2', '3', '4', '5', '6', '7', '8'};

const char Board::file_to_char[8] {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

const int Board::piece_value[12] {1, 3, 3, 5, 9, 1000, -1, -3, -3, -5, -9, -1000};

bool Board::on_board(int rank, int file)
{
	return !(rank < 0 || rank >= 8 || file < 0 || file >= 8);
}

bool Board::on_board(Square square)
{
    return !(square.rank < 0 || square.rank >= 8 || square.file < 0 || square.file >= 8);
}

Board::Board()
    : board {}, // set all bitboards to zero first
    move_number {1},
	half_move_number {0},
    turn {true},
    en_passantable {false},
    en_passant_square {0,0},
    can_white_castle_kingside {true},
    can_white_castle_queenside {true},
    can_black_castle_kingside {true},
    can_black_castle_queenside {true},
	board_value {0}
{
	
	// initialise hash longs
	for (int rank = 0; rank<8; rank++) {
		for (int file = 0; file<8; file++) {
			for (int type = 0; type<12; type++) {
				hash_pieces[rank][file][type] = rand64();
			}
		}
	}
	for (int i = 0; i<4; i++) {
		hash_castling[i] = rand64();
	}
	hash_to_move = rand64();
	
    // then set up the board
    toggle_piece(WHITE_ROOK, 0, 0);
    toggle_piece(WHITE_KNIGHT, 0, 1);
    toggle_piece(WHITE_BISHOP, 0, 2);
    toggle_piece(WHITE_QUEEN, 0, 3);
    toggle_piece(WHITE_KING, 0, 4);
    toggle_piece(WHITE_BISHOP, 0, 5);
    toggle_piece(WHITE_KNIGHT, 0, 6);
    toggle_piece(WHITE_ROOK, 0, 7);
    toggle_piece(BLACK_ROOK, 7, 0);
    toggle_piece(BLACK_KNIGHT, 7, 1);
    toggle_piece(BLACK_BISHOP, 7, 2);
    toggle_piece(BLACK_QUEEN, 7, 3);
    toggle_piece(BLACK_KING, 7, 4);
    toggle_piece(BLACK_BISHOP, 7, 5);
    toggle_piece(BLACK_KNIGHT, 7, 6);
    toggle_piece(BLACK_ROOK, 7, 7);
    for (int file = 0; file<8; file++) {
        toggle_piece(WHITE_PAWN, 1, file);
        toggle_piece(BLACK_PAWN, 6, file);
    }
	
	hash = generate_hash();
	
}

uint64_t Board::rand64()
{
        return (static_cast<uint64_t>(rand()) << 32) | rand();

}

int Board::coor_to_bit_position(int r, int c)
{
    
    return 8*r + c;
    
}

bool Board::get_turn()
{
	return turn;
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

Piece Board::piece_at(Square square)
{
    
    uint64_t bit = uint64_t(1) << coor_to_bit_position(square.rank, square.file);
    for (int i = 0; i<NO_OF_TYPES; i++) {
        if (board[i] & bit) {
            return static_cast<Piece>(i);
        }
    }
    return static_cast<Piece>(12);
}

Piece Board::piece_at(int rank, int file)
{
    
    uint64_t bit = uint64_t(1) << coor_to_bit_position(rank, file);
    for (int i = 0; i<NO_OF_TYPES; i++) {
        if (board[i] & bit) {
            return static_cast<Piece>(i);
        }
    }
    return static_cast<Piece>(12);
}

bool Board::is_piece_at(Piece type, int rank, int file)
{
    
    uint64_t bit = uint64_t(1) << coor_to_bit_position(rank, file);
    return (board[type] & bit);
    
}

bool Board::is_piece_at(int rank, int file)
{
	
	uint64_t bit = uint64_t(1) << coor_to_bit_position(rank, file);
    for (int i = 0; i<NO_OF_TYPES; i++) {
        if (board[i] & bit) {
            return true;
        }
    }
    return false;
	
}

bool Board::is_piece_at(Square square)
{
	
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

void Board::toggle_piece(Piece type, int rank, int file)
{
    
    board[type] ^= uint64_t(1) << coor_to_bit_position(rank, file);
	hash ^= hash_pieces[rank][file][type];
    
}

void Board::put_piece(Piece type, int rank, int file)
{
	toggle_piece(type, rank, file);
}

void Board::put_piece(Piece type, Square square)
{
	toggle_piece(type, square.rank, square.file);
}

void Board::remove_piece(int rank, int file)
{
	toggle_piece(piece_at(rank, file), rank, file);
}

void Board::remove_piece(Square square)
{
	toggle_piece(piece_at(square), square.rank, square.file);
}

bool Board::piece_at_side(int rank, int file)
{
	
	int type = piece_at(rank, file);
	assert(type >=0 && type<12 && "No piece at square");
	if (type < 6) {
		return true;
	}
	return false;
	
}

bool Board::piece_at_side(Square square)
{
	
	int type = piece_at(square);
	assert(type >=0 && type<12 && "No piece at square");
	if (type < 6) {
		return true;
	}
	return false;
	
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

int Board::get_value()
{
	return board_value;
}

Square Board::sq(int rank, int file)
{
	
	Square result {rank, file};
	return result;
	
}

bool Board::is_in(Square square, const std::vector<Square> &vector)
{
	return (std::find(vector.begin(), vector.end(), square) != vector.end());
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
		else {
			break;
		}
	}
	return result;
	
}

std::vector<Square> Board::attacking_squares(Square square, bool side, bool taking)
{
	
	std::vector<Square> result;
	
	// look in each direction out from square:
	for (int direction = 0; direction<8; direction++) {
		std::vector<Square> look = look_along(square, direction, 1);
		if (!look.empty()) {
			Square attacking_square = look.back();
			if (is_piece_at(attacking_square) && piece_at_side(attacking_square) == side && is_attacking[piece_at(attacking_square)][direction]) {
				result.push_back({attacking_square.rank, attacking_square.file, direction});
			}
		}
	}
	
	// check for knights:
	for (int i = 0; i<8; i++) {
		Square attacking_square {square.rank + move_vectors[WHITE_KNIGHT][i].rank, square.file + move_vectors[WHITE_KNIGHT][i].file};
		if (on_board(attacking_square) && piece_at(attacking_square) == (side ? WHITE_KNIGHT : BLACK_KNIGHT)) {
			result.push_back(attacking_square);
		}
	}
	
	// check for pawns:
	if (taking) {
		int rank {side ? -1 : 1};
		for (int file = -1; file<2; file+=2) {
			Square attacking_square {square.rank + rank, square.file + file};
			if (on_board(attacking_square) && piece_at(attacking_square) == (side ? WHITE_PAWN : BLACK_PAWN)) {
				result.push_back(attacking_square);
			}
		}
	}
	else {
		// initial pawn advance:
		if (side && square.rank == 3) {
			Square attacking_square {1, square.file};
			if (piece_at(attacking_square) == WHITE_PAWN) {
				result.push_back(attacking_square);
			}
		}
		else if (!side && square.rank == 4) {
			Square attacking_square {6, square.file};
			if (piece_at(attacking_square) == BLACK_PAWN) {
				result.push_back(attacking_square);
			}
		}
		// normal pawn move:
		Square attacking_square {square.rank + (side ? -1 : 1), square.file};
		if (on_board(attacking_square) && piece_at(attacking_square) == (side ? WHITE_PAWN : BLACK_PAWN)) {
			result.push_back(attacking_square);
		}
	}
	
	return result;
	
}

std::vector<Square> Board::find_constrained_squares()
{
	
	std::vector<Square> result;
	
	// First we find the position of the king:
	Square king_square = find_piece(turn ? WHITE_KING : BLACK_KING);
	// Check that we found the king:
	assert(king_square.rank != -1 && "King not found.");
	// Look along each direction from the king:
	for (int direction = 0; direction<8; direction++) {
		std::vector<Square> king_line = look_along(king_square, direction, 2);
		// check attacking piece is on the opposite side:
		if (!king_line.empty() && is_piece_at(king_line.back()) && piece_at_side(king_line.back()) != turn) {
			Square attacking_square = king_line.back();
			// Find blocking piece:
			Square blocking_square;
			bool blocking_piece_is_side = false;
			for (unsigned int n = 0; n<king_line.size()-1; n++) {
				if (is_piece_at(king_line[n]) && piece_at_side(king_line[n]) == turn) {
					blocking_square = king_line[n];
					blocking_piece_is_side = true;
					break;
				}
			}
			if (blocking_piece_is_side && is_attacking[piece_at(attacking_square)][direction]) {
				result.push_back({blocking_square.rank, blocking_square.file, direction});
			}	
		}
	}
	
	return result;
	
}

std::vector<Move> Board::get_moves()
{
	
	std::vector<Move> result;
	
	/////////////// CALCULATE USEFUL THINGS \\\\\\\\\\\\\\\
	
	Piece king = turn ? WHITE_KING : BLACK_KING;
	Square king_square = find_piece(king);
	assert(king_square.rank != -1 && "King not found");
	Square enemy_king_square = find_piece(turn ? BLACK_KING : WHITE_KING);
	assert(enemy_king_square.rank != -1 && "Enemy king not found");
	
	// Get constrained squares:
	std::vector<Square> constrained_squares = find_constrained_squares();
	
	// Check whether the king is in check:
	std::vector<Square> king_attackers	= attacking_squares(king_square, !turn, true);
	bool in_check = !king_attackers.empty();
	
	/////////////// IF KING IS IN CHECK \\\\\\\\\\\\\\\
	
	if(in_check) {
		
		// king moves:
		remove_piece(king_square);
		for (int i = 0; i<8; i++) {
			Square new_square {king_square.rank + move_vectors[king][i].rank, king_square.file + move_vectors[king][i].file};
			if (on_board(new_square) && (!is_piece_at(new_square) || piece_at_side(new_square) != turn)) {
				// if not next to enemy king:
				if (!(abs(new_square.rank-enemy_king_square.rank)<=1 && abs(new_square.file-enemy_king_square.file)<=1)) {
					// if square not controlled:
					if (attacking_squares(new_square, !turn, true).empty()) {
						if (!is_piece_at(new_square)) {
							Move new_move(king_square, new_square, 0, false);
							result.push_back(new_move);
						}
						else {
							Move new_move(king_square, new_square, 0, true);
							result.push_back(new_move);
						}
					}
				}
			}
		}
		put_piece(king, king_square);
		
		if (king_attackers.size() == 1) {
			// take the attacking piece:
			Square king_attacker = king_attackers[0];
			std::vector<Square> king_attacker_attackers = attacking_squares(king_attacker, turn, true);
			for (unsigned int i = 0; i<king_attacker_attackers.size(); i++) {
				if (!is_in(king_attacker_attackers[i], constrained_squares)) {
					Move new_move(king_attacker_attackers[i], king_attacker, 0, true);
					result.push_back(new_move);
				}
			}
			// take the attacking piece en passant:
			if (piece_at(king_attacker) == (turn ? BLACK_PAWN : WHITE_PAWN) && en_passantable) {
				if (en_passant_square.rank == king_attacker.rank + (turn ? 1 : -1) && en_passant_square.file == king_attacker.file) {
					for (int file = -1; file<3; file++) {
						Square taking_square {king_attacker.rank, king_attacker.file + file};
						if (on_board(taking_square) && piece_at(taking_square) == (turn ? WHITE_PAWN : BLACK_PAWN)) {
							Move new_move(taking_square, en_passant_square, 4, true);
							result.push_back(new_move);
						}
					}
				}
			}
			// blocking:
			if (piece_at(king_attacker) != (turn ? BLACK_PAWN : WHITE_PAWN) && piece_at(king_attacker) != (turn ? BLACK_KNIGHT : WHITE_KNIGHT)) {
				std::vector<Square> look = look_along(king_square, king_attacker.direction, 1);
				// remove the last square -- should be king_attacker:
				look.pop_back();
				for (unsigned int i = 0; i<look.size(); i++) {
					std::vector<Square> blockers = attacking_squares(look[i], turn, false);
					for (unsigned int j = 0; j<blockers.size(); j++) {
						if (!is_in(blockers[j], constrained_squares)) {
							Move new_move(blockers[j], look[i], 0, false);
							result.push_back(new_move);
						}
					}
				}
			}
		}
		
	}
	/////////////// IF KING IS NOT IN CHECK \\\\\\\\\\\\\\\
	
	else {
		
		for (int rank=0; rank<8; rank++) {
			for (int file=0; file<8; file++) {
				if (on_board(rank, file) && is_piece_at(rank, file) && piece_at_side(rank, file) == turn) {
					Square square {rank, file};
					Piece moving_piece = piece_at(square);
					bool constrained = is_in(square, constrained_squares);
					int constrained_direction = -1;
					// if constrained, find the direction the attack is coming from:
					if (constrained) {
						constrained_direction = (*std::find(constrained_squares.begin(), constrained_squares.end(), square)).direction;
					}
					// pawns:
					if (moving_piece%6 == 0) {
						// pawn moving forward 2 on its first move:
						if (rank == (turn ? 1 : 6)) {
							Square target_square {(turn ? 3 : 4), file};
							Square potential_block {(turn ? 2 : 5), file};
							if (!is_piece_at(target_square) && !is_piece_at(potential_block)) {
								if (!constrained || constrained_direction == (turn ? 0 : 4)) {
									Move new_move(square, target_square, 0, false);
									result.push_back(new_move);
								}
							}
						}
						// moving forward 1
						Square target_square {(turn ? rank+1 : rank-1), file};
						if (!is_piece_at(target_square)) {
							if (!constrained || constrained_direction == (turn ? 0 : 4)) {
								if (turn ? (rank < 6) : (rank > 1)) {
									Move new_move(square, target_square, 0, false);
									result.push_back(new_move);
								}
								// promotion:
								else {
									Move new_move0(square, target_square, 1, false, turn ? WHITE_KNIGHT : BLACK_KNIGHT);
									Move new_move1(square, target_square, 1, false, turn ? WHITE_BISHOP : BLACK_BISHOP);
									Move new_move2(square, target_square, 1, false, turn ? WHITE_ROOK : BLACK_ROOK);
									Move new_move3(square, target_square, 1, false, turn ? WHITE_QUEEN : BLACK_QUEEN);
									result.push_back(new_move0);
									result.push_back(new_move1);
									result.push_back(new_move2);
									result.push_back(new_move3);
								}
							}
						}
						// taking:
						for (int d_file = -1; d_file<3; d_file+=2) {
							target_square = {(turn ? rank+1 : rank-1), file + d_file};
							if (on_board(target_square) && is_piece_at(target_square) && piece_at_side(target_square) != turn) {
								if (!constrained || (target_square.rank == rank+direction_to_vector[constrained_direction].rank && target_square.file == file+direction_to_vector[constrained_direction].file)) {
									if (turn ? (rank < 6) : (rank > 1)) {
										Move new_move(square, target_square, 0, true);
										result.push_back(new_move);
									}
									// taking with promotion:
									else {
										Move new_move0(square, target_square, 1, true, turn ? WHITE_KNIGHT : BLACK_KNIGHT);
										Move new_move1(square, target_square, 1, true, turn ? WHITE_BISHOP : BLACK_BISHOP);
										Move new_move2(square, target_square, 1, true, turn ? WHITE_ROOK : BLACK_ROOK);
										Move new_move3(square, target_square, 1, true, turn ? WHITE_QUEEN : BLACK_QUEEN);
										result.push_back(new_move0);
										result.push_back(new_move1);
										result.push_back(new_move2);
										result.push_back(new_move3);
									}
								}
							}
						}
						// en passant:
						if (en_passantable && en_passant_square.rank == rank + (turn ? +1 : -1) && (en_passant_square.file == file+1 || en_passant_square.file == file-1)) {
							if (!constrained || (en_passant_square.rank == rank+direction_to_vector[constrained_direction].rank && en_passant_square.file == file+direction_to_vector[constrained_direction].file)) {
								Move new_move (square, en_passant_square, 4, true);
								result.push_back(new_move);
							}
						}
					}
					// all other pieces except the king:
					else if (moving_piece%6 != 5) {
						for (unsigned int mv = 0; mv<move_vectors[moving_piece].size(); mv++) {
							Square move_vector = move_vectors[moving_piece][mv];
							for (int n = 1; n<8; n++) {
								if (is_limited[moving_piece] && n>1) {
									break;
								}
								Square target_square {rank + n*move_vector.rank, file + n*move_vector.file};
								if (!on_board(target_square)) {
									break;
								}
								if (!constrained || move_vector.direction == constrained_direction) {
									if (is_piece_at(target_square)) {
										// blocked by own piece:
										if (piece_at_side(target_square) == turn) {
											break;
										}
										// enemy piece:
										else {
											Move new_move(square, target_square, 0, true);
											result.push_back(new_move);
											break;
										}
									}
									// square empty:
									else {
										Move new_move(square, target_square, 0, false);
										result.push_back(new_move);
									}
								}
							}
						}
					}
					// king moves:
					else {
						for (int mv = 0; mv<8; mv++) {
							Square move_vector = move_vectors[WHITE_KING][mv];
							Square target_square {square.rank + move_vector.rank, square.file + move_vector.file};
							if (on_board(target_square) && (!is_piece_at(target_square) || piece_at_side(target_square) != turn)) {
								if (!(abs(target_square.rank-enemy_king_square.rank)<=1 && abs(target_square.file-enemy_king_square.file)<=1)) {
									if (attacking_squares(target_square, !turn, true).empty()) {
										Move new_move (square, target_square, 0, is_piece_at(target_square));
										result.push_back(new_move);
									}
								}
							}
						}
					}
				}
			}
		}
		
		/////////////// CASTLING \\\\\\\\\\\\\\\
		
		// castle_kingside
		if ((turn ? can_white_castle_kingside : can_black_castle_kingside)) {
			int rank = (turn ? 0 : 7);
			if (!is_piece_at({rank, 5}) && !is_piece_at({rank, 6})) {
				if (attacking_squares({rank, 5}, !turn, true).empty() && attacking_squares({rank, 6}, !turn, true).empty()) {
					Move new_move (king_square, {rank, 6}, 2, false);
					result.push_back(new_move);
				}
			}
		}
		
		// castle_queenside
		if ((turn ? can_white_castle_queenside : can_black_castle_queenside)) {
			int rank = (turn ? 0 : 7);
			if (!is_piece_at({rank, 1}) && !is_piece_at({rank, 2}) && !is_piece_at({rank, 3})) {
				if (attacking_squares({rank, 1}, !turn, true).empty() && attacking_squares({rank, 2}, !turn, true).empty() && attacking_squares({rank, 3}, !turn, true).empty()) {
					Move new_move (king_square, {rank, 2}, 3, false);
					result.push_back(new_move);
				}
			}
		}
		
	}
	
	if (result.empty() && !in_check) {
		Move null_move({0,0}, {0,0}, 5, false);
		result.push_back(null_move);
	}
	
	return result;
	
}

void Board::make_move(Move &move)
{
	
	Square initial_position = move.get_initial_position();
	Square final_position = move.get_final_position();
	
	// check king is not taken:
	assert(piece_at(final_position)%6 != 5 && "King taken!");
	
	// update halfmove number:
	if (piece_at(initial_position)%6 == 0 || move.is_piece_taken()) {
		half_move_number = 0;
	}
	else {
		half_move_number++;
	}
	
	// update castling rights if king or rooks are moved (also will work when the move itself is castling):
	bool &kingside = (turn ? can_white_castle_kingside : can_black_castle_kingside);
	bool &queenside = (turn ? can_white_castle_queenside : can_black_castle_queenside);
	uint64_t &hash_kingside = (turn ? hash_castling[0] : hash_castling[2]);
	uint64_t &hash_queenside = (turn ? hash_castling[1] : hash_castling[3]);
	int rank = (turn ? 0 : 7);
	if (initial_position == sq(rank, 4) && piece_at(initial_position)%6 == 5 && (kingside || queenside)) {
		kingside = false;
		queenside = false;
		hash ^= hash_kingside;
		hash ^= hash_queenside;
	}
	else if (move.get_initial_position() == sq(rank, 0) && piece_at(move.get_initial_position())%6 == 3 && queenside) {
		queenside = false;
		hash ^= hash_queenside;
	}
	else if (initial_position == sq(rank, 7) && piece_at(initial_position)%6 == 3 && kingside) {
		kingside = false;
		hash ^= hash_kingside;
	}
	
	en_passantable = false;
	
	// ordinary move:
	if (move.get_move_type() == 0) {
		if (move.is_piece_taken()) {
			board_value -= piece_value[piece_at(final_position)];
			remove_piece(final_position);
		}
		put_piece(piece_at(initial_position), final_position);
		remove_piece(initial_position);
		// check whether en passant will be possible next turn:
		if (piece_at(final_position)%6 == 0) {
			if (final_position.rank == final_position.rank + (turn ? 2 : -2)) {
				en_passantable = true;
				en_passant_square = {final_position.rank + (turn ? -1 : 1), final_position.file};
			}
		}
	}
	
	// pawn promotion:
	else if (move.get_move_type() == 1) {
		if (move.is_piece_taken()) {
			remove_piece(final_position);
			board_value -= piece_value[piece_at(final_position)];
		}
		put_piece(move.get_promotion_piece(), final_position);
		board_value -= piece_value[turn ? WHITE_PAWN : BLACK_PAWN];
		board_value += piece_value[piece_at(final_position)];
		remove_piece(initial_position);
	}
	
	// castle kingside:
	else if (move.get_move_type() == 2) {
		put_piece((turn ? WHITE_KING : BLACK_KING), {rank, 6});
		remove_piece({rank, 4});
		put_piece((turn ? WHITE_ROOK : BLACK_ROOK), {rank, 5});
		remove_piece({rank, 7});
	}
	
	// castle queenside:
	else if (move.get_move_type() == 3) {
		put_piece((turn ? WHITE_KING : BLACK_KING), {rank, 2});
		remove_piece({rank, 4});
		put_piece((turn ? WHITE_ROOK : BLACK_ROOK), {rank, 3});
		remove_piece({rank, 0});
	}
	
	// en passant:
	else if (move.get_move_type() == 4) {
		put_piece(piece_at(initial_position), final_position);
		remove_piece(initial_position);
		board_value -= piece_value[turn ? BLACK_PAWN : WHITE_PAWN];
		remove_piece({(turn ? 4 : 4), final_position.file});
	}
	
	turn = !turn;
	hash ^= hash_to_move;
	if (turn) {
		move_number++;
	}
	
}

void Board::get_FEN(std::string FEN)
{
	
	FEN += ' ';
	
	// set bitboards to zero:
	for (int i = 0; i < NO_OF_TYPES; i++) {
		board[i] = uint64_t(0);
	}
	can_black_castle_kingside = false;
	can_black_castle_queenside = false;
	can_white_castle_kingside = false;
	can_white_castle_queenside = false;
	en_passantable = false;
	
	char x;
	std::string number;
	
	// piece placement
	int file = 0;
	int rank = 7;
	int stage = 0;
	
	for (unsigned int i = 0; i<FEN.length(); i++) {
		
		x = FEN.at(i);
		
		if (stage == 0) {
			switch(x) {
				case ' ': stage++; break;
				case 'p': put_piece(BLACK_PAWN, rank, file); file++; break;
				case 'P': put_piece(WHITE_PAWN, rank, file); file++; break;
				case 'n': put_piece(BLACK_KNIGHT, rank, file); file++; break;
				case 'N': put_piece(WHITE_KNIGHT, rank, file); file++; break;
				case 'b': put_piece(BLACK_BISHOP, rank, file); file++; break;
				case 'B': put_piece(WHITE_BISHOP, rank, file); file++; break;
				case 'r': put_piece(BLACK_ROOK, rank, file); file++; break;
				case 'R': put_piece(WHITE_ROOK, rank, file); file++; break;
				case 'q': put_piece(BLACK_QUEEN, rank, file); file++; break;
				case 'Q': put_piece(WHITE_QUEEN, rank, file); file++; break;
				case 'k': put_piece(BLACK_KING, rank, file); file++; break;
				case 'K': put_piece(WHITE_KING, rank, file); file++; break;
				case '/': rank--; file = 0; break;
				case '1': file++; break;
				case '2': file+=2; break;
				case '3': file+=3; break;
				case '4': file+=4; break;
				case '5': file+=5; break;
				case '6': file+=6; break;
				case '7': file+=7; break;
				case '8': file+=8; break;
			}
		}
		else if (stage == 1) {
			switch(x) {
				case 'w': turn=true; break;
				case 'b': turn=false; break;
				case ' ': stage++; break;
			}
		}
		else if (stage == 2) {
			switch(x) {
				case 'k': can_black_castle_kingside = true; break;
				case 'K': can_white_castle_kingside = true; break;
				case 'q': can_black_castle_queenside = true; break;
				case 'Q': can_white_castle_queenside = true; break;
				case ' ': stage++;
			}
		}
		else if (stage == 3) {
			switch(x) {
				case 'a': file = 0; break;
				case 'b': file = 1; break;
				case 'c': file = 2; break;
				case 'd': file = 3; break;
				case 'e': file = 4; break;
				case 'f': file = 5; break;
				case 'g': file = 6; break;
				case 'h': file = 7; break;
				case '3': rank = 2; en_passantable = true; en_passant_square = {rank, file}; break;
				case '6': rank = 5; en_passantable = true; en_passant_square = {rank, file}; break;
				case ' ': stage++; break;
			}
		}
		else if (stage == 4) {
			if (x != ' ') {
				number += x;
			}
			else {
				half_move_number = atoi(number.c_str());
				stage++;
				number.clear();
			}
		}
		else if (stage == 5) {
			if (x != ' ') {
				number += x;
			}
			else {
				move_number = atoi(number.c_str());
				stage++;
			}
		}
		
	}
	
	hash = generate_hash();
	
}

std::string Board::move_to_str(Move &move)
{
	
	std::string result;
	Square initial_square = move.get_initial_position();
	Square final_square = move.get_final_position();
	Piece moving_piece = piece_at(initial_square);
	int move_type = move.get_move_type();
	
	result += (std::to_string(move_number) + ". ");
	if (!piece_at_side(initial_square)) {
		result += "...";
	}
	
	if (move_type == 0 || move_type == 1 || move_type == 4) {
		if (moving_piece%6 == 0 && move.is_piece_taken()) {
			result += file_to_char[initial_square.file];
		}
		if (moving_piece%6 != 0) {
			result += type_to_char(moving_piece%6);
		}
		// check if move is degenerate in knights:
		if (moving_piece%6 == 1) {
			for (int i = 0; i < 8; i++) {
				Square possible_knight_position = {final_square.rank + move_vectors[1][i].rank, final_square.file + move_vectors[1][i].file};
				if (!(possible_knight_position.rank == initial_square.rank && possible_knight_position.file == initial_square.file)) {
					if (on_board(possible_knight_position) && piece_at(possible_knight_position)%6 == 1 && piece_at_side(possible_knight_position) == piece_at_side(initial_square)) {
						if (possible_knight_position.file == initial_square.file) {
							result += rank_to_char[initial_square.rank];
							break;
						}
						else {
							result += file_to_char[initial_square.file];
							break;
						}
					}
				}
			}
		}
		// check if move is degenerate in rooks:
		if (moving_piece%6 == 3) {
			for (int direction = 0; direction<8; direction+=2) {
				std::vector<Square> look = look_along(final_square, direction, 1);
				if (!look.empty()) {
					Square possible_rook_position = look.back();
					if (!(possible_rook_position.rank == initial_square.rank && possible_rook_position.file == initial_square.file)) {
						if (piece_at(possible_rook_position)%6 == 3 && piece_at_side(possible_rook_position) == piece_at_side(initial_square)) {
							if (possible_rook_position.file == initial_square.file) {
								result += rank_to_char[initial_square.rank];
							}
							else {
								result += file_to_char[initial_square.file];
							}
						}
					}
				}
			}
		}
		if (move.is_piece_taken()) {
			result += 'x';
		}
		result += file_to_char[final_square.file];
		result += rank_to_char[final_square.rank];
		if (move_type == 1) {
			result += '=';
			result += type_to_char(move.get_promotion_piece()%6);
		}
	}
	else if (move_type == 2) {
		result += "O-O";
	}
	else if (move_type == 3) {
		result += "O-O-O";
	}
	
	else if (move_type == 5) {
		result += "NULL_MOVE";
	}
	
	result += "\n";
	
	return result;
	
}

// HASHING FUNCTIONS

uint64_t Board::generate_hash()
{
	
	uint64_t result = 0;
	
	for (int rank = 0; rank<8; rank++) {
		for (int file = 0; file<8; file++) {
			Piece piece_type = piece_at(rank, file);
			if (piece_type != NO_OF_TYPES) {
				result ^= hash_pieces[rank][file][piece_type];
			}
		}
	}
	if (can_white_castle_kingside) {
		result ^= hash_castling[0];
	}
	if (can_white_castle_queenside) {
		result ^= hash_castling[1];
	}
	if (can_black_castle_kingside) {
		result ^= hash_castling[2];
	}
	if (can_black_castle_queenside) {
		result ^= hash_castling[3];
	}
	if (!turn) {
		result ^= hash_to_move;
	}
	
	return result;
	
}

uint64_t Board::get_hash()
{
	return hash;
}

Move Board::long_algebraic_to_move(std::string move_str)
{
	
	Square initial_position;
	Square final_position;
	int move_type = 0;
	bool piece_taken = false;
	Piece promotion_piece = NO_OF_TYPES;
	
	unsigned int pos = 0;
	switch(move_str.at(pos)) {
		case 'a': initial_position.file = 0; break;
		case 'b': initial_position.file = 1; break;
		case 'c': initial_position.file = 2; break;
		case 'd': initial_position.file = 3; break;
		case 'e': initial_position.file = 4; break;
		case 'f': initial_position.file = 5; break;
		case 'g': initial_position.file = 6; break;
		case 'h': initial_position.file = 7; break;
	}
	pos++;
	initial_position.rank = (int)move_str.at(pos) - 49;
	pos++;
	switch(move_str.at(pos)) {
		case 'a': final_position.file = 0; break;
		case 'b': final_position.file = 1; break;
		case 'c': final_position.file = 2; break;
		case 'd': final_position.file = 3; break;
		case 'e': final_position.file = 4; break;
		case 'f': final_position.file = 5; break;
		case 'g': final_position.file = 6; break;
		case 'h': final_position.file = 7; break;
	}
	pos++;
	final_position.rank = (int)move_str.at(pos) - 49;
	pos++;
	bool side = piece_at_side(initial_position);
	if (move_str.length() > 4) {
		switch(move_str.at(pos)) {
			case 'n': promotion_piece = side ? WHITE_KNIGHT : BLACK_KNIGHT; break;
			case 'b': promotion_piece = side ? WHITE_BISHOP : BLACK_BISHOP; break;
			case 'r': promotion_piece = side ? WHITE_ROOK : BLACK_ROOK; break;
			case 'q': promotion_piece = side ? WHITE_QUEEN : BLACK_QUEEN; break;
			case 'k': promotion_piece = side ? WHITE_KING : BLACK_KING; break;
		}
	}
	// check for castling:
	if (piece_at(initial_position) == side ? WHITE_KING : BLACK_KING) {
		if (initial_position.file - final_position.file == -2) {
			move_type = 2;
		}
		else if (initial_position.file - final_position.file == 2) {
			move_type = 3;
		}
	}
	// check for en passant:
	if (piece_at(initial_position) == side ? WHITE_PAWN : BLACK_PAWN) {
		if (abs(initial_position.file - final_position.file) == 1 && !is_piece_at(final_position)) {
			move_type = 4;
			piece_taken = true;
		}
	}
	// check for piece promotion
	if (promotion_piece != NO_OF_TYPES) {
		move_type = 1;
	}
	// check if piece is taken
	if (is_piece_at(final_position)) {
		piece_taken = true;
	}
	
	Move result(initial_position, final_position, move_type, piece_taken, promotion_piece);
	
	return result;
	
}