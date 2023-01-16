#include "Board.cpp"
#include "Board_test.hpp"
#include <iostream>

using std::cout;
using std::endl;


void print_move(Move move, Board* board){
	cout << piece_enum_to_name(board->at(move.start_row, move.start_col).piece) <<  " at ";
	cout << '(' << (char) ('a'+ move.start_col) << move.start_row+1 << ')';
	cout << " to ";
	cout << '(' << (char) ('a'+ move.end_col) << move.end_row+1 << ')';
	cout << endl;
}

Move set_move(int start_col, int start_row, int end_col, int end_row){
	Move move;
	move.start_col = start_col;
	move.start_row = start_row;
	move.end_col = end_col;
	move.end_row = end_row;
	return move;
}

void print_blocking(Board* board){
	for(int row = 0; row < 8; row++){
		for(int col = 0; col < 8; col++){
			for(Pos loc : board->blocking[8*row+col]){
				cout << (char)('a'+col) << (char)('1'+row) << " blocking " << (char)('a'+loc.col) << (char)('1'+loc.row) << std::endl;
			}
		}
	}
}

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
	cout << "EP target at " << (int)test_board->ep_col << ' ' << (int)test_board->ep_row << endl;
	cout << "Castling availability " << (int)test_board->castle_status << endl;

	cout << endl << endl;
	cout << "Test next from a legal move: " << endl << endl;
	cout << "Move = (d4) to (e5): " << endl << endl;
	Move move = {.start_row = 3, .start_col = 3, .end_row = 4, .end_col = 4};
	Board* test_next_board = test_board->next_from_move(move);
    std::cout << "got out" << std::endl;

	cout << "Original board: " << endl;
	cout << test_board->to_string();
	cout << endl;

	cout << "Result board: " << endl;
	cout << test_next_board->to_string();

	cout << endl << endl;
	cout << "Test getting legal moves: " << endl << endl;
	fen_string = "2b1kb2/p2pp1pp/1r1q1r2/p1n4n/PpPQ2p1/1N2P3/P2PKPPP/R1B2BNR w - - 0 1";
	test_board = new Board();
	test_board->load_board(fen_string);
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

	cout << "Original board: " << endl;
	cout << test_board->to_string();

	cout << piece_enum_to_name(test_board->at(move.start_row, move.start_col).piece) <<  " at ";
	cout << '(' << (char) ('a'+ move.start_col) << move.start_row+1 << ')';
	cout << " to ";
	cout << '(' << (char) ('a'+ move.end_col) << move.end_row+1 << ')';
	cout << endl;

	cout << "Result board: " << endl;
	cout << next_board->to_string();

	std::vector<Move> next_legal_moves = next_board->get_legal_moves();
	cout << endl;
	cout << "Got legal moves" << endl;
	for(Move move : next_legal_moves){
		cout << piece_enum_to_name(next_board->at(move.start_row, move.start_col).piece) <<  " at ";
		cout << '(' << (char) ('a'+ move.start_col) << move.start_row+1 << ')';
		cout << " to ";
		cout << '(' << (char) ('a'+ move.end_col) << move.end_row+1 << ')';
		cout << endl;
	}

	
	cout << endl << endl;
	cout << "Test a very weird thing: " << endl << endl;
	test_board = new Board();
	fen_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	test_board->load_board(fen_string);
	//e2e4
	move = set_move(4,1,4,3);
	print_move(move, test_board);
	test_board = test_board->next_from_move(move);
	cout << "Legal move count " << test_board->get_legal_moves().size() << endl;
	//a7a5
	move = set_move(0,6,0,4);
	print_move(move, test_board);
	test_board = test_board->next_from_move(move);
	cout << "Legal move count " << test_board->get_legal_moves().size() << endl;
	//d2d4
	move = set_move(3,1,3,3);
	print_move(move, test_board);
	test_board = test_board->next_from_move(move);
	cout << "Legal move count " << test_board->get_legal_moves().size() << endl;
	//b7b6
	move = set_move(1,6,1,5);
	print_move(move, test_board);
	test_board = test_board->next_from_move(move);
	cout << "Legal move count " << test_board->get_legal_moves().size() << endl;
	//e4e5
	move = set_move(4,3,4,4);
	print_move(move, test_board);
	test_board = test_board->next_from_move(move);
	cout << "Legal move count " << test_board->get_legal_moves().size() << endl;
	//c7c5
	move = set_move(2,6,2,4);
	print_move(move, test_board);
	test_board = test_board->next_from_move(move);
	cout << "Legal move count " << test_board->get_legal_moves().size() << endl;
	//d4d5
	move = set_move(3,3,3,4);
	print_move(move, test_board);
	test_board = test_board->next_from_move(move);
	cout << "Legal move count " << test_board->get_legal_moves().size() << endl;
	//h7h6
	move = set_move(7,6,7,5);
	print_move(move, test_board);
	test_board = test_board->next_from_move(move);
	cout << "Legal move count " << test_board->get_legal_moves().size() << endl;
	//f1b5
	move = set_move(5,0,1,4);
	print_move(move, test_board);
	test_board = test_board->next_from_move(move);
	cout << "Legal move count " << test_board->get_legal_moves().size() << endl;
	//f7f5
	move = set_move(5,6,5,4);
	print_move(move, test_board);
	test_board = test_board->next_from_move(move);
	cout << "Legal move count " << test_board->get_legal_moves().size() << endl;
	cout << "Legal moves: " << std::endl;
	for(Move leg_mov : test_board->get_legal_moves()){
		print_move(leg_mov, test_board);
	}
	//d1f3
	move = set_move(3,0,5,2);
	print_move(move, test_board);
	test_board = test_board->next_from_move(move);
	cout << "Legal move count " << test_board->get_legal_moves().size() << endl;
	cout << "Legal moves: " << std::endl;
	for(Move leg_mov : test_board->get_legal_moves()){
		print_move(leg_mov, test_board);
	}


	return 0;
}