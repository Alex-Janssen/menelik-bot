#include "Board.cpp"
#include "board_test.hpp"
#include <iostream>

using std::cout;
using std::endl;

int main() {
	Board* test_board = new Board();
	cout << "Test Constructor: " << endl << endl;
	Board_Test::print_board_state(test_board);

	cout << "Test Loading from FEN: " << endl << endl;

	return 0;
}

void Board_Test::print_board_state(Board* board){
	cout << "Board Colors: " << endl;
	for(int y= 0; y < 8; y++){
		for(int x = 0; x < 8; x++){
			cout << color_enum_to_name(board->squares[x][y].color) << '\t';
		}
		cout << endl;
	}
	cout << endl;
	cout << "Board Pieces: " << endl;
	for(int y= 0; y < 8; y++){
		for(int x = 0; x < 8; x++){
			cout << piece_enum_to_name(board->squares[x][y].piece) << '\t';
		}
		cout << endl;
	}
	cout << endl;
}