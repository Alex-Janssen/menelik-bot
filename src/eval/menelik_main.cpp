/// @brief Main method for API, however Menelik turns FEN to next move should be translated through this.
/// @param argc Number of args
/// @param argv Args
/// @return Next board state printed
#include <string>
#include "Eval_Operator.cpp"
#include <iostream>


int main(int argc, char *argv[]) { 
    if(argc > 2){
        std::string fen_string = argv[1];
        operate(fen_string);
    }
    else{
        std::cout<<"Fen string not identified.";
    }
}
static Board* identify_best_from_fen(std::string fen_string) { 
    return operate_return_board(fen_string);
}