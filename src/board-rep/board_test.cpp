#include "Board.cpp"
#include "Board_test.hpp"
#include <iostream>

using std::cout;
using std::endl;

int main() {
	Board* test_board = new Board();
	cout << "Test Constructor: " << endl << endl;
	Board_Test::print_board(test_board);

	std::string fen_string = "2b1kb2/p2pp1pp/1r1q1r2/p1n4n/PpPQ2p1/1N2P3/P2PKPPP/R1B2BNR w HA - 0 1";

	cout << "Test Loading from FEN: " << endl << endl;
	cout << "Input FEN string = " << fen_string << endl;

	test_board->load_board(fen_string);
	Board_Test::print_board(test_board);

	cout << "Test next from a legal move: " << endl << endl;
	cout << "Move = (3,3) to (4,4): " << endl;
	test_board->turn = colors::WHITE;
	Move move = {.start_x = 3, .start_y = 3, .end_x = 4, .end_y = 4};
	Board* test_next_board = test_board->next_from_move(move);

	cout << "Original board: " << endl;
	Board_Test::print_board(test_board);
	
	cout << "Result board: " << endl;
	Board_Test::print_board(test_next_board);


	return 0;
}

void Board_Test::print_board(Board* board){
	for(int y = 7; y >= 0; y--){
		for(int x = 0; x < 8; x++){
			cout << piece_to_char(board->at(x,y).piece, board->at(x,y).color) << ' ';
		}
		cout << endl;
	}
}

void Board_Test::print_board_state(Board* board){
	cout << "Board Colors: " << endl;
	for(int y = 7; y >= 0; y--){
		for(int x = 0; x < 8; x++){
			cout << color_enum_to_name(board->at(x,y).color) << '\t';
		}
		cout << endl;
	}
	cout << endl;
	cout << "Board Pieces: " << endl;
	for(int y= 7; y >= 0; y--){
		for(int x = 0; x < 8; x++){
			cout << piece_enum_to_name(board->at(x,y).piece) << '\t';
		}
		cout << endl;
	}
	cout << endl;
}