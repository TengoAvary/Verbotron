#ifndef hashtable_h
#define hashtable_h

#include "chess.h"

class Hashtable {
	
private:
	
	std::vector<std::tuple<uint64_t, int, int>> table[1000000];
	
public:
	
	Hashtable();
	
	void update(uint64_t key, int depth, int value);
	
	int value(Board &board);
	
};

#endif /* hashtable_h */