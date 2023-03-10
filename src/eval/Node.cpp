#include "../board-rep/Board.hpp"
#include "Node.hpp"
#include <functional>
#include <vector>
#include <iostream>

#include "Eval_Functions.hpp"
/*
/// @brief Spawns a candidate node as a root value. Do not pass parent, but have to specify max-depth.
/// @param board The boardstate whose next move is to be calculated
/// @param eval An evaluation function which evaluates the board value
/// @param max_depth Maximum depth of recursive generation of children
*/
Node::Node(Board* board, std::function<float(Board*)> eval, int max_depth){
    this->board = board;
    this->eval = eval;
    this->parent = nullptr;
    this->depth = 0;
    this->max_depth = max_depth;
    this->is_root = true;
    this->cur_board_val = eval(board);
    this->children = this->spawn_children();//Spawn children
    std::cout<<"Children spawned."<<"\n";
}
/// @brief Spawns a candidate node which is an inner node. Must specify a parent pointer.
/// @param board The boardstate whose next move is to be calculated
/// @param eval An evaluation function which evaluates the board value
/// @param parent The parent that spawned it.
Node::Node(Board* board, std::function<float(Board*)> eval, Node* parent, bool find_out){
    this->board = board;
    this->eval = eval;
    this->parent = parent;
    this->depth = parent->get_depth()+1;
    this->max_depth = parent->get_max_depth();
    this->is_root = find_out;
    if(this->depth < this->max_depth){
        this->children = this->spawn_children();
    }
    else{//No children if at max depth
        this->children = std::vector<Node*>();
    }
    this->cur_board_val = eval(board);//Evaluate using flexibible function.
}
Node::~Node(){
    delete board;
}
/// @brief Returns board encapsulated by node
/// @return Board.
Board* Node::get_board(){
    return this->board;
}
/// @brief Returns current depth of node.
/// @return Depth of node
int Node::get_depth(){
    return this->depth;
}
/// @brief Returns current max depth of node.
/// @return Max depth of node
int Node::get_max_depth(){
    return this->max_depth;
}
/// @brief Returns the propsective board val, assuming it is defined.
/// @return Prospective board val
float Node::get_prospective_board_val(){
    if (!this->is_prospective_calced()){
        this->set_prospective_board_val();
    }
    return this->prospective_board_val;
}
/// @brief Returns the current board val
/// @return Current board val
float Node::get_cur_board_val(){
    return this->cur_board_val;
}
/// @brief Returns true if the prospective val has been calculated on this node.
/// @return prospective_val_calculated
bool Node::is_prospective_calced(){
    return this-> prospective_val_calculated;
}
/// @brief Returns the optimal move suceeding this node, which would be the optimal opponent move in an inner node and optimal local move for the root.
/// @return Pointer to best move calculated.
Node* Node::get_best_successor(){
    if (!this->is_prospective_calced()){
        this->set_prospective_board_val();
    }    
    return this-> favorite_child;
}

/// @brief Initializes all candidate nodes encapsulating all moves from the board state
/// @return A vector of candidate nodes reachable from this position.
std::vector<Node*> Node::spawn_children(){
    std::vector<Node*> to_return;
    //iterate over all moves associated with this->board, create nodes, and then package in to_return.
    std::vector<Move> moves = this->board->get_legal_moves();
    for(Move move : moves){
        Board* associated_board = this->board->next_from_move(move);
        Node* new_child = new Node(associated_board, eval, this, false);
        to_return.push_back(new_child);
    }
    return to_return;
}
/// @brief Alters the board val to the best/worst of its children.
void Node::set_prospective_board_val(){//THIS HAS INBUILT CACHING, BUT WE NEED TO BE CAREFUL BECAUSE NEW NODES OF GREATER DEPTH WILL AFFECT RESULTS. FOR NOW, DESTROY THE CAND-NODES AND REGENERATE AFTER A MOVE IS MADE.
    Node* favorite;
    int prospective_val;
    if(abs(this->cur_board_val)>900){//A king has been taken, don't recurse.
        prospective_val = cur_board_val;
        favorite = nullptr; 
    }
    else{
        if(this-> depth < this->max_depth){//We recurse again
            if(this-> depth % 2 == 1){
                int min_option = 99999999999;//Arbitrarily large
                Node* best_opp_move = nullptr;
                for(Node* cand : this->children){//Iterate over every child board, checking if its prospective has been calced. If it has and it is larger, then replace the max with it.
                    if(cand->get_prospective_board_val() < min_option){
                        min_option = cand->get_prospective_board_val();
                        best_opp_move = cand;
                    }
                }
                prospective_val = min_option;
                favorite = best_opp_move;
            }
            else{
                int max_option = -99999999999;//Arbitrarily small
                Node* best_move = nullptr;
                for(Node* cand : this->children){//Iterate over every child board, checking if its prospective has been calced. If it has and it is larger, then replace the max with it.
                    if(cand->get_prospective_board_val() > max_option){
                        max_option = cand->get_prospective_board_val();
                        best_move = cand;
                    }
                }
                prospective_val = max_option;
                favorite = best_move;
            }
        }
        else{
            prospective_val = this->get_cur_board_val();
            favorite_child = nullptr;
        }
    }
    this->prospective_board_val = prospective_val;
    this->favorite_child = favorite;
    this->prospective_val_calculated = true;
}


