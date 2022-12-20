//
// Created by ajans on 19.12.2022.
//

#include "../board-rep/Board.hpp"
#include <functional>
#include <vector>

#ifndef MENELIK_CAND_NODE
#define MENELIK_CAND_NODE


class Candidate_Node {
    public:
        Candidate_Node(Board* board, std::function<float(Board*)> eval, int max_depth);
        Candidate_Node(Board* board, std::function<float(Board*)> eval, Candidate_Node* parent, bool);
        float get_cur_board_val();
        float get_prospective_board_val();
        int get_depth();
        int get_max_depth();
        Board* get_board();
        bool is_prospective_calced();
        Candidate_Node* get_best_successor();
        ~Candidate_Node();
    private:
        float cur_board_val;
        float prospective_board_val;
        bool prospective_val_calculated = false;
        bool is_root;
        Board* board;
        Candidate_Node* parent;
        std::vector<Candidate_Node*> children;
        Candidate_Node* favorite_child;//best node
        int depth;
        int max_depth;
        std::vector<Candidate_Node*> spawn_children();
        void set_prospective_board_val();
        std::function<float (Board *)> eval;

};

#endif //MENELIK_CAND_NODE
