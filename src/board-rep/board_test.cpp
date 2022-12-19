#include "Board.cpp"
#include "Board_test.hpp"
#include <iostream>

using std::cout;
using std::endl;

int main() {
	Board* test_board = new Board();
	cout << "Test Constructor: " << endl << endl;
	Board_Test::print_board_state(test_board);

	std::string fen_string = "2b1kb2/p2pp1pp/1r1q1r2/p1n4n/PpPQ2p1/1N2P3/P2PKPPP/R1B2BNR w HA - 0 1";

	cout << "Test Loading from FEN: " << endl << endl;
	cout << "Input FEN string = " << fen_string << endl;

	test_board->load_board(fen_string);
	Board_Test::print_board_state(test_board);

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