//
// Created by ajans on 19.12.2022.
//

#include "../board-rep/Board.hpp"

#ifndef MENELIK_EVAL_H
#define MENELIK_EVAL_H


class Eval {
    public:
        Board to_evaluate;
        Eval(Board to_evaluate);
        float evaluate();
};


#endif //MENELIK_EVAL_H
