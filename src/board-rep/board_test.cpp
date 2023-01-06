#include "Board.cpp"
#include "Board_test.hpp"
#include <iostream>

using std::cout;
using std::endl;

int main() {
	Board* test_board = new Board();
	cout << "Test Constructor: " << endl << endl;
	cout << test_board->to_string();

	std::string fen_string = "2b1kb2/p2pp1pp/1r1q1r2/p1n4n/PpPQ2p1/1N2P3/P2PKPPP/R1B2BNR w Kq b5 0 1";

	cout << "Test Loading from FEN: " << endl << endl;
	cout << "Input FEN string = " << fen_string << endl;

	test_board->load_board(fen_string);
	test_board->turn = colors::WHITE;
	cout << test_board->to_string();
	cout << "EP target at " << test_board->ep_col << ' ' << test_board->ep_row << endl;
	cout << "Castling availability " << (int)test_board->castle_status << endl;

	cout << endl << endl;
	cout << "Test next from a legal move: " << endl << endl;
	cout << "Move = (d4) to (e5): " << endl << endl;
	Move move = {.start_row = 3, .start_col = 3, .end_row = 4, .end_col = 4};
	Board* test_next_board = test_board->next_from_move(move);

	cout << "Original board: " << endl;
	cout << test_board->to_string();
	cout << endl;

	cout << "Result board: " << endl;
	cout << test_board->to_string();

	cout << endl << endl;
	cout << "Test getting legal moves: " << endl << endl;
	fen_string = "2b1kb2/p2pp1pp/1r1q1r2/p1n4n/PpPQ2p1/1N2P3/P2PKPPP/R1B2BNR w - - 0 1";
	test_board = new Board();
	test_board->load_board(fen_string);
	test_board->turn = colors::WHITE;
	cout << "Board: " << endl;
	cout << test_board->to_string();
	std::vector<Move> legal_moves = test_board->get_legal_moves();
	cout << "Got legal moves" << endl;
	for(Move move : legal_moves){
		cout << piece_enum_to_name(test_board->at(move.start_row, move.start_col).piece) <<  " at ";
		cout << '(' << (char) ('a'+ move.start_col) << move.start_row+1 << ')';
		cout << " to ";
		cout << '(' << (char) ('a'+ move.end_col) << move.end_row+1 << ')';
		cout << endl;
	}

	
	cout << endl << endl;
	cout << "Test making a legal move: " << endl << endl;
	move = legal_moves[0];
	Board* next_board = test_board->next_from_move(move);

    std::cout << "piece at location is: " << std::endl;
    std::cout << piece_enum_to_name(next_board->at(move.start_row, move.start_col).piece) << std::endl;

	cout << "Original board: " << endl;
	cout << test_board->to_string();

	cout << piece_enum_to_name(test_board->at(move.start_row, move.start_col).piece) <<  " at ";
	cout << '(' << (char) ('a'+ move.start_col) << move.start_row+1 << ')';
	cout << " to ";
	cout << '(' << (char) ('a'+ move.end_col) << move.end_row+1 << ')';
	cout << endl;

	cout << "Result board: " << endl;
	cout << next_board->to_string();

	return 0;
}