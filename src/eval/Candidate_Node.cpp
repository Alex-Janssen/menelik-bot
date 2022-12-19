#include "../board-rep/Board.hpp"
#include "Candidate_Node.hpp"
#include <functional>
#include <vector>

Candidate_Node::Candidate_Node(Board* board, std::function<float(Board)> eval, int max_depth){
    this->board = board;
    this->eval = eval;
    this->parent = nullptr;
    this->children = this->spawn_children();
    this->depth = 0;
    this->max_depth = max_depth;
    this->cur_board_val = eval(*(board));
    this->is_root = true;
}
Candidate_Node::Candidate_Node(Board* board, std::function<float(Board)> eval, Candidate_Node* parent){
    this->board = board;
    this->eval = eval;
    this->parent = parent;
    this->depth = parent->get_depth()+1;
    this->max_depth = parent->get_max_depth();
    if(this->depth < this->max_depth){
        this->children = this->spawn_children();
    }
    else{//No children if at max depth
        this->children = std::vector<Candidate_Node*>();
    }
    this->cur_board_val = eval(*(board));//Evaluate using flexibible function.
    this->is_root = false;
}

int Candidate_Node::get_depth(){
    return this->depth;
}
int Candidate_Node::get_max_depth(){
    return this->max_depth;
}
float Candidate_Node::get_prospective_board_val(){
    return this->prospective_board_val;
}
float Candidate_Node::get_cur_board_val(){
    return this->cur_board_val;
}
bool Candidate_Node::is_prospective_calced(){
    return this-> prospective_val_calculated;
}
Candidate_Node* Candidate_Node::get_best_successor(){
    return this-> favorite_child;
}

std::vector<Candidate_Node*> Candidate_Node::spawn_children(){
    std::vector<Candidate_Node*> to_return;
    //TODO iterate over all moves associated with this->board, create nodes, and then package in to_return.
    for(Move move : this->board->get_legal_moves()){
        Board* associated_board = this->board->next_from_move(move);
        Candidate_Node new_child = Candidate_Node(associated_board, eval, this);
        to_return.push_back(&new_child);
    }
    return to_return;
}

void Candidate_Node::set_prospective_board_val(){//THIS HAS INBUILT CACHING, BUT WE NEED TO BE CAREFUL BECAUSE NEW NODES OF GREATER DEPTH WILL AFFECT RESULTS. FOR NOW, DESTROY THE CAND-NODES AND REGENERATE AFTER A MOVE IS MADE.
    int prospective_val;
    Candidate_Node* favorite;
    if(this->is_root){
        int max_option = -99999999999;//Arbitrarily small
        Candidate_Node* best_move = nullptr;
        for(Candidate_Node* cand : this->children){//Iterate over every child board, checking if its prospective has been calced. If it has and it is larger, then replace the max with it.
            if(cand->is_prospective_calced()){
                if(cand->get_prospective_board_val() > max_option){
                    max_option = cand->get_prospective_board_val();
                    best_move = cand;
                }
            }
            else{
                cand->set_prospective_board_val();
                if(cand->get_prospective_board_val() > max_option){
                    max_option = cand->get_prospective_board_val();
                    best_move = cand;
                }
            }
        }
        prospective_val = max_option;
        favorite = best_move;
    }
    else{
        if(this-> depth < this->max_depth){//We recurse again
            int min_option = 99999999999;//Arbitrarily small
            Candidate_Node* best_opp_move = nullptr;
            for(Candidate_Node* cand : this->children){//Iterate over every child board, checking if its prospective has been calced. If it has and it is larger, then replace the max with it.
                if(cand->is_prospective_calced()){
                    if(cand->get_prospective_board_val() < min_option){
                        min_option = cand->get_prospective_board_val();
                        best_opp_move = cand;
                    }
                }
                else{
                    cand->set_prospective_board_val();
                    if(cand->get_prospective_board_val() < min_option){
                        min_option = cand->get_prospective_board_val();
                        best_opp_move = cand;
                    }
                }
            }
            prospective_val = min_option;
            favorite = best_opp_move;
        }
        else{
            prospective_val = this->get_cur_board_val();
            favorite_child = nullptr;
        }
    }
    this->prospective_val_calculated = true;
    this->prospective_board_val = prospective_val;
    this->favorite_child = favorite;
}