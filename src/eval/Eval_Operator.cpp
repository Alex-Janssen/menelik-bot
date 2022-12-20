#include "Candidate_Node.hpp"
#include "Eval_Functions.hpp"
#include "Load_Params.hpp"
#include "../board-rep/board.hpp"
#include <functional>
#include <map>
#include <iostream>
#include <string>

void identify_move(Board board){
    std::function func = std::function(Eval_Functions::evaluate_board_naive);
    int max_depth = Load_Params::read_eval_params()["maxdepth"];
    Candidate_Node root = Candidate_Node(&board, func, max_depth);
    std::cout << root.get_best_successor()->get_board()->to_string();
}

int main(){
    auto joker = Board();
    std::string jokerfen = "7k/1R6/8/8/8/8/8/R2K4 w - - 0 1";
    joker.load_board(jokerfen);
    identify_move(joker);
}