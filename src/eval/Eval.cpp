//
// Created by ajans on 19.12.2022.
//

#include "Eval.hpp"
#include "../board-rep/Board.hpp"
#include <map>

Eval::Eval(Board to_evaluate){
    this->to_evaluate = to_evaluate;
}

float Eval::evaluate(){
    return 0.1f;
}