//
// Created by ajans on 19.12.2022.
//

#include "../board-rep/Board.hpp"
#include "Load_Params.cpp"
#include <map>

#ifndef MENELIK_EVAL_H
#define MENELIK_EVAL_H


class Eval_Functions {
    public:
        static float evaluate_board_naive(Board* to_evaluate, std::map<char, int> piece_vals);
};


#endif //MENELIK_EVAL_H
