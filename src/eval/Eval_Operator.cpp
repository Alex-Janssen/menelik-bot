#include "Candidate_Node.cpp"
#include "Load_Params.cpp"
#include "Eval_Functions.cpp"
#include "../board-rep/board.cpp"
//#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::map<char, int> piece_vals = Load_Params::read_piece_vals();

std::vector<Board*> spawn_children(Board* board){ //generates a list of pointers to possible future boards
    std::vector<Board*> to_return;
    std::vector<Move> moves = board->get_legal_moves();
    for(Move move : moves){
        Board* new_board = board->next_from_move(move);
        to_return.push_back(new_board);
    }
    return to_return;
}


float minimax(Board* board, int depth, float alpha, float beta, bool maximizing_player){ //returns mininmal loss value
    float eval;
    if(depth == 0 || abs(Eval_Functions::evaluate_board_naive(board, piece_vals)) > 39){
        return Eval_Functions::evaluate_board_naive(board, piece_vals)+depth*0.1;
    }

    if(maximizing_player){
        float max_eval = -99999999;
        std::vector<Board*> children = spawn_children(board);
        for(Board* child : children){
            eval = minimax(child,depth-1,alpha,beta,false);
            max_eval = std::max(max_eval,eval);
            alpha = std::max(alpha,eval);
            if(beta <= alpha) {break;}
        }
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
        return min_eval;
    }
}

void identify_move(Board board){
    int max_depth = Load_Params::read_eval_params()["maxdepth"];
    std::vector<Board*> moves = spawn_children(&board);
    std::vector<float> scores;
    for(Board* move : moves){
        scores.push_back(minimax(move,max_depth,-99999999,99999999,board.turn!=colors::WHITE));
    }

    auto it = std::minmax_element(scores.begin(), scores.end());
    int idx = 0;
    if(board.turn==colors::WHITE){ //max index
        idx = std::distance(scores.begin(), it.second);
    } else { //min index
        idx = std::distance(scores.begin(), it.first);
    }
    //for(int i = 0; i < moves.size(); i++){ //prints all possible first moves and predicted loss
        //std::cout << moves[i]->to_string() << std::endl;
        //std::cout << "score = " << scores[i] << std::endl;
    //}
    std::cout << moves[idx]->to_string() << std::endl; //top move
}

Board* identify_move_board(Board board){
    int max_depth = Load_Params::read_eval_params()["maxdepth"];
    std::vector<Board*> moves = spawn_children(&board);
    std::vector<float> scores;
    for(Board* move : moves){
        scores.push_back(minimax(move,max_depth,-99999999,99999999,board.turn!=colors::WHITE));
    }

    auto it = std::minmax_element(scores.begin(), scores.end());
    int idx = 0;
    if(board.turn==colors::WHITE){ //max index
        idx = std::distance(scores.begin(), it.second);
    } else { //min index
        idx = std::distance(scores.begin(), it.first);
    }
    //for(int i = 0; i < moves.size(); i++){ //prints all possible first moves and predicted loss
        //std::cout << moves[i]->to_string() << std::endl;
        //std::cout << "score = " << scores[i] << std::endl;
    //}
    return moves[idx];
}

static void operate(std::string fen){
    auto game = Board();
    game.load_board(fen);
    identify_move(game);
}

static Board* operate_return_board(std::string fen){
    auto game = Board();
    game.load_board(fen);
    return identify_move_board(game);    
}