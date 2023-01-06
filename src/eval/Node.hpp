//
// Created by ajans on 19.12.2022.
//
#ifndef MENELIK_CAND_NODE
#define MENELIK_CAND_NODE

#include "../board-rep/Board.hpp"
#include <functional>
#include <vector>

class Node {
    public:
        Node(Board* board, std::function<float(Board*)> eval, int max_depth);
        Node(Board* board, std::function<float(Board*)> eval, Node* parent, bool);
        float get_cur_board_val();
        float get_prospective_board_val();
        int get_depth();
        int get_max_depth();
        Board* get_board();
        bool is_prospective_calced();
        Node* get_best_successor();
        ~Node();
    private:
        float cur_board_val;
        float prospective_board_val;
        bool prospective_val_calculated = false;
        bool is_root;
        Board* board;
        Node* parent;
        std::vector<Node*> children;
        Node* favorite_child;//best node
        int depth;
        int max_depth;
        std::vector<Node*> spawn_children();
        void set_prospective_board_val();
        std::function<float (Board *)> eval;

};

#endif //MENELIK_CAND_NODE
