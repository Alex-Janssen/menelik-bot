//
// Created by ajans on 19.12.2022.
//
#ifndef MENELIK_EVALFUNC_H
#define MENELIK_EVALFUNC_H

#include "../board-rep/Board.hpp"
#include "Load_Params.hpp"
#include <map>



class Eval_Functions {
    public:
        static float evaluate_board_naive(Board* to_evaluate, std::map<char, int> piece_vals);
};


#endif //MENELIK_EVAL_H
