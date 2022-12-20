
#ifndef MENELIK_ELIALGO_H
#define MENELIK_ELIALGO_H

#include "Eval.hpp"
#include "Load_Params.hpp"
#include <map>
#include <vector>


class Eli_Algorithm : public Eval{
    const std::map<char, int> piece_vals = Load_Params::read_piece_vals(); 
    const std::map<std::string, int> eval_params = Load_Params::read_eval_params();
    public: 
        Board* get_best_board(Board* board);
        virtual Move get_best_move(Board* board);
        Board* get_best_board(std::string& fen_string);
        virtual Move get_best_move(std::string& fen_string);
        Eli_Algorithm();
        ~Eli_Algorithm();
    private:
        std::vector<Board*> spawn_children(Board* board);
        float minimax(Board* board, int depth, float alpha, float beta, bool maximizing_player);
        void identify_best_move(Board board);
};

#endif