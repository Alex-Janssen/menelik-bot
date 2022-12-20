#include "..\board-rep\board.hpp"

#ifndef MENELIK_EVAL_H
#define MENELIK_EVAL_H

class Eval{
    public:
        virtual Board* get_best_board(Board* board) = 0;
        virtual Move get_best_move(Board* board) = 0;
        virtual Board* get_best_board(std::string& fen_string) = 0;
        virtual Move get_best_move(std::string& fen_string) = 0;
};

#endif