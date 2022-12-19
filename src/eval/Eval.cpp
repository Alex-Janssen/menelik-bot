//
// Created by ajans on 19.12.2022.
//

#include "Eval.hpp"
#include "Load_Params.hpp"
#include "../board-rep/Board.hpp"
#include <map>

Eval::Eval(Board to_evaluate){
    this->to_evaluate = to_evaluate;
    this->piece_vals = Load_Params::read_piece_vals();
}

float Eval::evaluate_board_naive(){
    float eval_result;
    for(int row = 0; row < 8; row++){//Iterate over every square
        for(int col = 0; col < 8; col++){
            Square check = to_evaluate.at(col, row);
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