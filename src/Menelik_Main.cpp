#include <string>
#include "Eli_Algorithm.cpp"
#include "Eval.hpp"
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
        std::cout<<"no arguments";
    }
}