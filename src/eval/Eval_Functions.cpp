//
// Created by ajans on 19.12.2022.
//

#include "Eval_Functions.hpp"
//TODO IMMEDIATELY FIGURE THIS OUT
#ifndef MENELIK_LOADPARAM_CPP
#define MENELIK_LOADPARAM_CPP
#include "Load_Params.cpp"
#endif
#include "../board-rep/Board.hpp"
#include <map>
//#include <iostream>


float Eval_Functions::evaluate_board_naive(Board* to_evaluate, std::map<char, int> piece_vals){
    float eval_result = 0;
    if(to_evaluate->victory!=colors::NONE)
    {
        return (to_evaluate->victory==colors::WHITE)*(999999)-(to_evaluate->victory==colors::BLACK)*(999999);
    }
    for(int row = 0; row < 8; row++){//Iterate over every square
        for(int col = 0; col < 8; col++){
            Square check = to_evaluate->at(col, row);
            int sign;
            switch (check.color){
                case colors::WHITE:
                    sign = 1;
                    break;
                case colors::BLACK:
                    sign = -1;
                    break;
                case colors::NONE:
                    sign = 0;
                    break;
            }
            if(sign != 0){
                switch (check.piece){
                    case pieces::PAWN:
                        eval_result += sign*piece_vals['p'];
                        break;
                    case pieces::ROOK:
                        eval_result += sign*piece_vals['r'];
                        break;
                    case pieces::BISHOP:
                        eval_result += sign*piece_vals['b'];
                        break;
                    case pieces::KNIGHT:
                        eval_result += sign*piece_vals['n'];
                        break;
                    case pieces::QUEEN:
                        eval_result += sign*piece_vals['q'];
                        break;
                    case pieces::KING:
                        eval_result += sign*piece_vals['k'];
                        break;
                }
            }

        }
    }
    return eval_result;
}