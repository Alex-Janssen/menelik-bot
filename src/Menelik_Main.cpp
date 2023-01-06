#include <string>
#include "eval/Eli_Algorithm.cpp"
#include "eval/Eval_Functions.cpp"
#include "eval/Eval.hpp"
#include "board-rep/board.cpp"
#include <iostream>

/// @brief Main method for API, however Menelik turns FEN to next move should be translated through this.
/// @param argc Number of args
/// @param argv Args
/// @return Next board state printed
int main(int argc, char *argv[]) { 
    if(argc > 2){
        Eli_Algorithm* algo = new  Eli_Algorithm();
        std::cout << argv << std::endl;
        std::string fen_string = argv[1];
        Board* best_board = algo->get_best_board(fen_string);
        std::cout << best_board->to_string();
    }
    else{
        Eli_Algorithm* algo = new  Eli_Algorithm();
        Board* current_board = new Board();
        std::string default_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        current_board->load_board(default_fen);

        std::cout << "Current board state is:\n" << current_board->to_string() << std::endl;

        std::string opponent_move_str;
        while (current_board->victory == colors::NONE){
            std::cout << "Move: ";
            std::cin >> opponent_move_str;
            std::cout << std::endl;

            Move to_move = move_from_text(opponent_move_str);
            
            std::cout << "You played: " <<
                 (char)(to_move.start_col+'a') <<
                 (char)(to_move.start_row+'1') <<
                 (char)(to_move.end_col+'a') <<
                 (char)(to_move.end_row+'1') << std::endl;


            current_board = current_board->next_from_move(to_move);

            std::cout << "Next board state is:\n" << current_board->to_string() << std::endl;

            Move best_move = algo->get_best_move(current_board);

            std::cout << "Bot plays: " <<
                 (char)(best_move.start_col+'a') <<
                 (char)(best_move.start_row+'1') <<
                 (char)(best_move.end_col+'a') <<
                 (char)(best_move.end_row+'1') << std::endl;

            current_board = current_board->next_from_move(best_move);
            
            std::cout << "Next board state is:\n" << current_board->to_string() << std::endl;
        }

    }
}