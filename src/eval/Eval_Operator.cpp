#include "Candidate_Node.cpp"
#include "Eval_Functions.cpp"
#include "Load_Params.cpp"
#include "../board-rep/board.cpp"
#include <functional>
#include <map>
#include <iostream>
#include <string>

void identify_move(Board board){
    std::function<float (Board *)> func = std::function<float (Board*)>(Eval_Functions::evaluate_board_naive);
    int max_depth = Load_Params::read_eval_params()["maxdepth"];
    Candidate_Node root = Candidate_Node(&board, func, max_depth);
    std::cout << root.get_best_successor()->get_board()->to_string();
}

int main(){
    auto joker = Board();
    std::string jokerfen = "7k/1R6/PB3p2/8/6n1/5pp1/3n4/2K5 w - - 0 1";
    joker.load_board(jokerfen);
    identify_move(joker);
}