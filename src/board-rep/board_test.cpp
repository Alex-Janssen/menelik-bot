#include "Board.cpp"
#include "Board_test.hpp"
#include <iostream>

using std::cout;
using std::endl;

int main() {
	Board* test_board = new Board();
	cout << "Test Constructor: " << endl << endl;
	Board_Test::print_board(test_board);

	std::string fen_string = "2b1kb2/p2pp1pp/1r1q1r2/p1n4n/PpPQ2p1/1N2P3/P2PKPPP/R1B2BNR w Kq f6 0 1";

	cout << "Test Loading from FEN: " << endl << endl;
	cout << "Input FEN string = " << fen_string << endl;

	test_board->load_board(fen_string);
	test_board->turn = colors::WHITE;
	Board_Test::print_board(test_board);
	cout << "EP target at " << test_board->ep_x << ' ' << test_board->ep_y << endl;
	cout << "Castling availability " << (int)test_board->castle_status << endl;

	cout << endl << endl;
	cout << "Test next from a legal move: " << endl << endl;
	cout << "Move = (d4) to (e5): " << endl << endl;
	Move move = {.start_x = 3, .start_y = 3, .end_x = 4, .end_y = 4};
	Board* test_next_board = test_board->next_from_move(move);

	cout << "Original board: " << endl;
	Board_Test::print_board(test_board);
	cout << endl;

	cout << "Result board: " << endl;
	Board_Test::print_board(test_next_board);

	cout << endl << endl;
	cout << "Test getting legal moves: " << endl << endl;
	fen_string = "rnbqkbn1/pppppp1r/6p1/6Bp/8/3P1N2/PPP1PPPP/RN1QKB1R w KQq - 0 1";
	test_board = new Board();
	test_board->load_board(fen_string);
	test_board->turn = colors::WHITE;
	cout << "Board: " << endl;
	Board_Test::print_board(test_board);
	std::vector<Move> legal_moves = test_board->get_legal_moves();
	cout << "Got legal moves" << endl;
	for(Move move : legal_moves){
		cout << piece_enum_to_name(test_board->at(move.start_x, move.start_y).piece) <<  " at ";
		cout << '(' << (char) ('a'+ move.start_x) << move.start_y+1 << ')';
		cout << " to ";
		cout << '(' << (char) ('a'+ move.end_x) << move.end_y+1 << ')';
		cout << endl;
	}



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