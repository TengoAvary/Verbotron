//
//  Commander.cpp
//  Chess
//
//  Created by Jack Setford on 07/09/2017.
//  Copyright © 2017 Jack Setford. All rights reserved.
//

#include "chess.h"

Commander::Commander() 
	: command {"null"},
	uci_mode {false}
{

}

void Commander::get_command() {
	std::cin >> command;
}

void Commander::print_command() {
	std::cout << command << "\n";
}

void Commander::process() {
	
	if (!uci_mode) {
		
		if (command == "null") {
			
		}
		else if (command == "uci") {
			uci_mode = true;
			std::cout << "[[ uci mode true ]]\n";
		}
		
	}
	else {
		
		
		
	}
	
}