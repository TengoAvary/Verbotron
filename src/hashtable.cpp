//
//  move.cpp
//  Chess
//
//  Created by Jack Setford on 17/04/2017.
//  Copyright © 2017 Jack Setford. All rights reserved.
//

#include "chess.h"

Hashtable::Hashtable()
{
	
}

void Hashtable::update(uint64_t key, int depth, int value)
{
	
	int index = key%1000000;
	
	for(std::tuple<uint64_t, int, int>&tup : table[index]) {
		if (std::get<0>(tup) == key && std::get<1>(tup) < depth) {
			std::get<1>(tup) = depth;
			std::get<2>(tup) = value;
			return;
		}
	}
	
	table[index].push_back(std::make_tuple(key, depth, value));
	
}

int Hashtable::value(Board& board) {
	
	int key = board.get_hash();
	int index = key%1000000;
	
	for(std::tuple<uint64_t, int, int>&tup : table[index]) {
		if (std::get<0>(tup) == key) {
			return std::get<2>(tup);
		}
	}
	
	return board.get_value();
	
}