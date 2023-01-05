#include "Candidate_Node.hpp"
#include "Load_Params.hpp"
#include "Eli_Algorithm.hpp"
#include "../board-rep/board.hpp"
#include "Eval_Functions.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


Eli_Algorithm::Eli_Algorithm(){
}

Eli_Algorithm::~Eli_Algorithm(){
}

std::vector<Board*> Eli_Algorithm::spawn_children(Board* board){ //generates a list of pointers to possible future boards
    std::vector<Board*> to_return;
    std::vector<Move> moves = board->get_legal_moves();
    for(Move move : moves){
        Board* new_board = board->next_from_move(move);
        to_return.push_back(new_board);
    }
    return to_return;
}
float Eli_Algorithm::minimax(Board* board, int depth, float alpha, float beta, bool maximizing_player){ //returns mininmal loss value
    float eval;
    if(depth == 0 || abs(Eval_Functions::evaluate_board_naive(board, piece_vals)) > 39){
        return Eval_Functions::evaluate_board_naive(board, piece_vals)+depth*0.1;
    }

    if(maximizing_player){
        float max_eval = -99999999;
        std::vector<Board*> children = spawn_children(board);
        //delete
        for(Board* child : children){
            eval = minimax(child,depth-1,alpha,beta,false);
            max_eval = std::max(max_eval,eval);
            alpha = std::max(alpha,eval);
            if(beta <= alpha) {break;}
        }
        std::for_each(children.begin(), children.end(), delete_children);
        return max_eval;
    } else {
        float min_eval = 99999999;
        std::vector<Board*> children = spawn_children(board);
        for(Board* child : children){
            eval = minimax(child,depth-1,alpha,beta,true);
            min_eval = std::min(min_eval,eval);
            beta = std::min(beta,eval);
            if(beta <= alpha) {break;}
        }
        std::for_each(children.begin(), children.end(), delete_children);
        return min_eval;
    }
}

void Eli_Algorithm::identify_best_move(Board board){
    float eval;
    float best_eval;
    int max_depth = (*eval_params.find("maxdepth")).second;
    std::vector<Board*> moves = spawn_children(&board);
    Board* best_board = moves[0];

    if(board.turn==colors::WHITE){ //max
        best_eval = -99999999;
        auto it = moves.begin();
        while(it != moves.end()){
            eval = (minimax(*it,max_depth,-99999999,99999999,board.turn!=colors::WHITE));
            if(best_eval < eval){
                best_board = *it;
                best_eval = eval;
                it++;
                if(eval > 103){break;}
            }
            else{
                it = moves.erase(it);
            }
        }
    } else { //min
        best_eval = 99999999;
        auto it = moves.begin();
        while(it != moves.end()){
            eval = (minimax(*it,max_depth,-99999999,99999999,board.turn!=colors::WHITE));
            if(best_eval > eval){
                best_board = *it;
                best_eval = eval;
                it++;
                if(eval < -103){break;}
            }
            else{
                it = moves.erase(it);
            }
        }
    }
    std::cout << best_board->to_string() << std::endl; //top move
}

Board* Eli_Algorithm::get_best_board(Board* board){
    float eval;
    float best_eval;
    int max_depth = (*eval_params.find("maxdepth")).second;
    std::vector<Board*> moves = spawn_children(board);
    Board* best_board = moves[0];

    if(board->turn==colors::WHITE){ //max
        best_eval = -99999999;
        auto it = moves.begin();
        while(it != moves.end()){
            eval = (minimax(*it,max_depth,-99999999,99999999,board->turn!=colors::WHITE));
            if(best_eval < eval){
                best_board = *it;
                best_eval = eval;
                it++;
                if(eval > 103){break;}
            }
            else{
                it = moves.erase(it);
            }
        }
    } else { //min
        best_eval = 99999999;
        auto it = moves.begin();
        while(it != moves.end()){
            eval = (minimax(*it,max_depth,-99999999,99999999,board->turn!=colors::WHITE));
            if(best_eval > eval){
                best_board = *it;
                best_eval = eval;
                it++;
                if(eval < -103){break;}
            }
            else{
                it = moves.erase(it);
            }
        }
    }
    return best_board;
}

Board* Eli_Algorithm::get_best_board(std::string& fen_string){
    Board* game = new Board();
    game->load_board(fen_string);
    return get_best_board(game);
}

Move Eli_Algorithm::get_best_move(Board* board){
    //TODO
    return Move();
}

Move Eli_Algorithm::get_best_move(std::string& fen_string){
    //TODO
    return Move();
}