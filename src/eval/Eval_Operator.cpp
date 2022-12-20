#include "Candidate_Node.cpp"
#include "Eval_Functions.cpp"
#include "Load_Params.cpp"
#include "../board-rep/board.cpp"
#include <functional>
#include <map>
#include <iostream>
#include <string>

void identify_move(Board board){
    std::cout << "identifying move" << std::endl;
    std::function<float (Board)> func = std::function<float (Board)>(Eval_Functions::evaluate_board_naive);
    std::cout << "identifying move 2" << std::endl;
    int max_depth = Load_Params::read_eval_params()["maxdepth"];
    std::cout << "identifying move 3" << std::endl;
    Candidate_Node root = Candidate_Node(&board, func, max_depth);
    std::cout << "identifying move 4" << std::endl;
    std::cout << root.get_best_successor()->get_board()->to_string();
}

int main(){
    auto joker = Board();
    std::string jokerfen = "7k/1R6/8/8/8/8/8/R2K4 w - - 0 1";
    joker.load_board(jokerfen);
    identify_move(joker);
}