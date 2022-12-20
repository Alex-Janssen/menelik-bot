//
// Created by ajans on 16.12.2022.
//

#include "Board.hpp"
#include <vector>
#include <ctype.h>
#include <string.h>
#include <iostream>

Board::Board(){
    //initalize empty board
    this->squares = new Square * [8] {new Square[8], new Square[8], new Square[8], new Square[8], new Square[8], new Square[8], new Square[8], new Square[8]};
    //initialize to nobody's turn
    this->turn = colors::NONE;
    //initialize to all true
    this->castle_status = 0b1111;
    this->turns_until_draw = 50;
}

Board::Board(Square** squares){
    //change board pointer to point to incoming board pointer
    this->squares = squares;
};


Board::Board(Square** squares, colors turn, unsigned int castle_status){
    //change board pointer to point to incoming board pointer
    this->squares = squares;

    //also load turn and castle status (enum class and int types)
    this->turn = turn;
    this->castle_status = castle_status;
};

Board::~Board() {
    delete[] squares;
}

Square Board::at(int row, int col){
    return squares[row][col];
}

void Board::load_board(Board other){
    this->squares = other.squares;
    this->turn = other.turn;
}

void Board::load_board(std::string& fen_string){
    std::string piece_string = fen_string.substr(0, fen_string.find(' '));
    char active_color = fen_string[piece_string.length()+1];
    std::string castling_availability = fen_string.substr(piece_string.length()+3, fen_string.substr(piece_string.length()+3).find(' '));
    std::string en_passant_target = fen_string.substr(fen_string.substr(piece_string.length()+3).find(' ')+1, fen_string.substr(fen_string.substr(piece_string.length()+3).find(' ')+1).find(' '));
    load_board_pieces(piece_string);
    auto contains = [] (std::string str, char target) -> bool {
        if(str.find(target) == -1){
            return false;
        } else {
            return true;
        }
    };
    //set the turn
    if(active_color == 'w'){
        this->turn = colors::WHITE;
    } else {
        this->turn = colors::BLACK;
    }
    //set castling bits
    if (castling_availability == "-"){
        this->castle_status = 0b0000;
    } else {
        bool Wk = contains(castling_availability, 'K');
        bool Wq = contains(castling_availability, 'Q');
        bool Bk = contains(castling_availability, 'k');
        bool Bq = contains(castling_availability, 'q');
        this->castle_status = 0b1000 * Wq + 0b0100 * Wk + 0b0010 * Bq + 0b0001 * Bk;
    }
    //set en passant
    if (en_passant_target == "-"){
        this->ep_x = -1;
        this->ep_y = -1;
    } else {
        this->ep_x = en_passant_target[0]-'a';
        this->ep_y = std::stoi(en_passant_target.substr(1))-1;
    }
}

void Board::load_board_pieces(std::string& fen_string){
    int column = 0;
    int row = 0;
    int fen_scan = 0;
    while (column+row*8 < 64 && fen_scan < fen_string.length()){
        if (fen_string.at(fen_scan) == '/'){
            row ++;
            column = 0;
        }
        else if (fen_string.at(fen_scan) > char('0') && fen_string.at(fen_scan) < char('9')){ //number for empty, can't be nine
            column += fen_string.at(fen_scan)-char('0');//Casts to int by subtracting 0 character value.
        }
        else{
            Square* to_set = &squares[column][7-row];//Adjusted because mirror set-up, thanks Theo...
            if(islower(fen_string.at(fen_scan))){//Sets color
                to_set->color = colors::BLACK;
            }
            else{
                to_set->color = colors::WHITE;
            }
            switch (tolower(fen_string.at(fen_scan))){//TO REFACTOR AS SEPERATE FUNCTION, GETS LOWER CASE VALUE
                case 'p':
                    to_set->piece = pieces::PAWN;
                    break;
                case 'r':
                    to_set->piece = pieces::ROOK;
                    break;     
                case 'k':
                    to_set->piece = pieces::KING;
                    break;      
                case 'n':
                    to_set->piece = pieces::KNIGHT;
                    break;                 
                case 'q':
                    to_set->piece = pieces::QUEEN;
                    break;  
                case 'b':
                    to_set->piece = pieces::BISHOP;
                    break;                     
            }
            column++;//increments square
        }
        fen_scan++;  //always go through a fen string          
    }
}

/// @brief copys itself and moves a move
/// @pre Must be someone's turn.
/// @param move the move to be made
/// @return a reference to a board with the next move in it
Board* Board::next_from_move(Move move){
    colors next_turn;
    Square** next_squares;
    int_fast8_t next_castle_status;
    colors next_victory = colors::NONE;

    //set next_turn to the color who has their turn next
    switch (this->turn){
        case colors::BLACK:
            next_turn = colors::WHITE;
            break;
        case colors::WHITE:
            next_turn = colors::BLACK;
            break;
        case colors::NONE:
            return this;
            break;
    }

    //make a copy of the board to update with the move (and retrun refernce to)
    next_squares = board_copy();
    next_castle_status = this->castle_status;

    //get references to the start and end squares
    Square* end_square = &next_squares[move.end_x][move.end_y];
    Square* start_square = &next_squares[move.start_x][move.start_y];

    //if the move is an enemy king capture, set victory
    //(this will count stalemates and checkmates both as a win, TODO?)
    if(end_square->piece == pieces::KING){
        next_victory = this->turn;
    }

    //store values of the piece information of the starting square
    colors start_color = start_square->color;
    pieces start_piece = start_square->piece;

    //remove piece from starting square
    start_square->color = colors::NONE;
    start_square->piece = pieces::NONE;

    //if we are taking en passant with our pawn
    if((move.end_x == ep_x && move.end_y == ep_y) && start_piece == pieces::PAWN){
        int y;
        //change y to get the associated pawn's location
        if(next_turn == colors::WHITE){
            y = move.end_y-1;
        }
        if(next_turn == colors::BLACK){
            y = move.end_y+1;
        }
        //get rid of the associated pawn
        next_squares[move.end_x][y].color = colors::NONE;
        next_squares[move.end_x][y].piece = pieces::NONE;
    }

    //king move check for castling
    if(start_piece == pieces::KING ){
        
        //update castling rights:
        if(this->turn == colors::WHITE){
            next_castle_status &= 0b0011;
        }
        if(this->turn == colors::BLACK){
            next_castle_status &= 0b1100;
        }
        
    //if we are castling (king move from e (x=4) to c (x=2) or g (x=6))
        if(move.start_x == 4 && (move.end_x == 2 || move.end_x == 6 )){

        //queenside castle
            if(move.end_x == 2){
                //remove rook from queen's side
                next_squares[0][move.start_y].color = colors::NONE;
                next_squares[0][move.start_y].piece = pieces::NONE;

                //place rook at F (x = 5), with the color of the person who castled
                next_squares[5][move.start_y].color = this->turn;
                next_squares[5][move.start_y].piece = pieces::ROOK;
            }

            //kingside castle
            if(move.end_x == 6){
                //remove rook from king's side
                next_squares[7][move.start_y].color = colors::NONE;
                next_squares[7][move.start_y].piece = pieces::NONE;

                //place rook at D (x = 3), with the color of the person who castled
                next_squares[3][move.start_y].color = this->turn;
                next_squares[3][move.start_y].piece = pieces::ROOK;

            }
        }
    }
    
    //rook move check for castling rights update
    if(start_piece == pieces::ROOK ){
        //woooooo bitwise operand time
        bool K = move.start_x==7;
        bool W = move.start_y==0;

        //castle status: White Queenside | White Kingside | Black Queenside | Black Kingside

        int_fast8_t result = 0b1000 * (!K && W) + 0b0100 * (K && W) + 0b0010 * (!K && !W) + 0b0001 * (K && !W);
        next_castle_status &= result;
    }
    

    //place the moving piece at its destination, possibly overwriting a piece that is there
    end_square->color = start_color;
    end_square->piece = start_piece;

    //if the move is a pawn promotion, replace the end_square piece with the promoted type
    if(move.promote_target != pieces::NONE){
        end_square->piece = move.promote_target;
    }

    Board* next_board = new Board(next_squares, next_turn, next_castle_status);

    next_board->victory = next_victory;
    
    //if the move is a pawn first move double, add the en passant possibility
    if(start_piece == pieces::PAWN){
        if(move.start_y == 1 && move.end_y == 3){
            next_board->ep_x = move.end_x;
            next_board->ep_y = 2;
        }
        if(move.start_y == 6 && move.end_y == 4){
            next_board->ep_x = move.end_x;
            next_board->ep_y = 5;
        }
    }

    //return the updated board
    return next_board;
}

std::vector<Move> Board::get_legal_moves(){
    //create output vector
    std::vector<Move> legal_moves = std::vector<Move>();

    //iterate over all pieces
    for(int pos_x = 0; pos_x < 8; pos_x++){
        for (int pos_y = 0; pos_y < 8; pos_y++){
            //get the type of the piece being observed
            pieces this_piece_type;
            this_piece_type = this->squares[pos_x][pos_y].piece;
            //get the moves this piece can make
            std::vector<Move> moves_this_piece;
            moves_this_piece = get_moves_from_position(pos_x, pos_y, this_piece_type, this->turn);\
            //add this pieces possible moves to all possible moves
            legal_moves.insert(legal_moves.end(), moves_this_piece.begin(), moves_this_piece.end());
        }
    }
    return legal_moves;
}

Square** Board::board_copy(){
    //create new squares for the copy
    Square** copy = new Square * [8] {new Square[8], new Square[8], new Square[8], new Square[8], new Square[8], new Square[8], new Square[8], new Square[8]};
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            //set by value the copied board
            copy[x][y] = squares[x][y];
        }
    }
    return copy;
}

std::vector<Move> Board::get_moves_from_position(int pos_x, int pos_y, pieces piece_type, colors turn){
    std::vector<Move> out = std::vector<Move>();
    if(squares[pos_x][pos_y].color == turn){
        switch (piece_type){
            case pieces::NONE:
                break;

            //when I first started writing this, I did not think that pawns would suck the most to write.
            //considering the double-move, diagional for taking and forward for movement, en passant and promotion rules though it makes sense
            case pieces::PAWN:{
                int dest_x = pos_x;
                int dest_y;
                if(turn == colors::WHITE){
                    dest_y = pos_y+1;
                    //if the pawn has not moved from the start, add a double move possiblity if unobstructed
                    if(pos_y == 1){
                        if(!any_piece_here(dest_x, dest_y+1) && !any_piece_here(dest_x, dest_y)){
                            Move move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y+1};
                            out.push_back(move);
                        }
                    }
                }
                //like white, but y-1 for the basic move and also for the double move
                if(turn == colors::BLACK){
                    dest_y = pos_y-1;
                    //if the pawn has not moved from the start, add a double move possiblity if unobstructed
                    if(pos_y == 1){
                        if(!any_piece_here(dest_x, dest_y-1) && !any_piece_here(dest_x, dest_y)){
                            Move move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y-1};
                            out.push_back(move);
                        }
                    }
                }
                //look in the 3 forward directions for movement
                for(dest_x = pos_x - 1; dest_x <= pos_x + 1; dest_x++){
                    //bounds check
                    if(dest_x < 0 || dest_x > 7){
                        continue;
                    }
                    //only move if directly forwards (no change in x) and no piece there, or if diagonally (unequal x) and an enemy piece is there or en passant is there
                    if((dest_x == pos_x && !any_piece_here(dest_x, dest_y)) || (pos_x != dest_x && (enemy_piece_here(dest_x, dest_y, turn) || (ep_x == dest_x && ep_y == dest_y)))){
                        //if the move requires a promotion, add all possible promotions to the moves table
                        if(dest_y == 7){
                            Move move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y};
                            move.promote_target = pieces::QUEEN;
                            out.push_back(move);
                            move.promote_target = pieces::BISHOP;
                            out.push_back(move);
                            move.promote_target = pieces::KNIGHT;
                            out.push_back(move);
                            move.promote_target = pieces::ROOK;
                            out.push_back(move);
                        }
                        //no promotion, simple move
                        else{
                            Move move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y};
                            out.push_back(move);
                        }
                    }
                }
                break;}

            case pieces::ROOK:{
                //horizontal moves
                for(int dx = -1; dx <= 1; dx+=2){
                    Move move;
                    int dest_x = pos_x + dx;
                    int dest_y = pos_y;
                    while(!any_piece_here(dest_x,dest_y)){
                        if(dest_x < 0 || dest_x > 7){
                            break;
                        }
                        move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y};
                        out.push_back(move);
                        dest_x += dx;
                    }
                    if(enemy_piece_here(dest_x,dest_y,turn)){
                        move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y};
                        out.push_back(move);
                    }
                }
                //vertical moves
                for(int dy = -1; dy <= 1; dy+=2){
                    Move move;
                    int dest_x = pos_x;
                    int dest_y = pos_y + dy;
                    while(!any_piece_here(dest_x,dest_y)){
                        if(dest_y < 0 || dest_y > 7){
                            break;
                        }
                        move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y};
                        out.push_back(move);
                        dest_y += dy;
                    }
                    if(enemy_piece_here(dest_x,dest_y,turn)){
                        move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y};
                        out.push_back(move);
                    }
                }
                break;}

            case pieces::KNIGHT:{
                //just a silly little algorithm to look at all 8 possible horsey moves
                for(int dest_x = pos_x - 2; dest_x <= pos_x + 2; dest_x += 1){
                    //boundary check x
                    if(dest_x < 0 || dest_x > 7 || dest_x == pos_x){
                        continue;
                    }
                    int x_factor = abs(dest_x-pos_x) % 2;
                    for(int dest_y = pos_y - 1 - x_factor; dest_y <= pos_y + 1 + x_factor; dest_y += 1 + x_factor){
                        //boundary check y
                        if(dest_y < 0 || dest_y > 7 || dest_y == pos_y){
                            continue;
                        }
                        
                        //horsey only moves to where an ally is not
                        if(!ally_piece_here(dest_x,dest_y,turn)){
                            Move move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y};
                            out.push_back(move);
                        }

                    }
                }
                break;}

            case pieces::BISHOP:{
                //diagonals are thankfully straight lines but sideways
                for(int dx = -1; dx <= 1; dx+=2){
                    for(int dy = -1; dy <= 1; dy+=2){
                        Move move;
                        int dest_x = pos_x + dx;
                        int dest_y = pos_y + dy;
                        while(!any_piece_here(dest_x,dest_y)){
                            dest_x += dx;
                            dest_y += dy;
                            if(dest_x < 0 || dest_x > 7 || dest_y < 0 || dest_y > 7){
                                break;
                            }
                            move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y};
                            out.push_back(move);
                        }
                        if(enemy_piece_here(dest_x,dest_y,turn)){
                            move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y};
                            out.push_back(move);
                        }
                    }
                }
                break;}

            case pieces::QUEEN:{
                //horizontal moves
                for(int dx = -1; dx <= 1; dx+=2){
                    Move move;
                    int dest_x = pos_x + dx;
                    int dest_y = pos_y;
                    while(!any_piece_here(dest_x,dest_y)){
                        if(dest_x < 0 || dest_x > 7){
                            break;
                        }
                        move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y};
                        out.push_back(move);
                        dest_x += dx;
                    }
                    if(enemy_piece_here(dest_x,dest_y,turn)){
                        move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y};
                        out.push_back(move);
                    }
                }
                //vertical moves
                for(int dy = -1; dy <= 1; dy+=2){
                    Move move;
                    int dest_x = pos_x;
                    int dest_y = pos_y + dy;
                    while(!any_piece_here(dest_x,dest_y)){
                        if(dest_y < 0 || dest_y > 7){
                            break;
                        }
                        move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y};
                        out.push_back(move);
                        dest_y += dy;
                    }
                    if(enemy_piece_here(dest_x,dest_y,turn)){
                        move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y};
                        out.push_back(move);
                    }
                }
                //diagonals moves
                for(int dx = -1; dx <= 1; dx+=2){
                    for(int dy = -1; dy <= 1; dy+=2){
                        Move move;
                        int dest_x = pos_x + dx;
                        int dest_y = pos_y + dy;
                        while(!any_piece_here(dest_x,dest_y)){
                            dest_x += dx;
                            dest_y += dy;
                            if(dest_x < 0 || dest_x > 7 || dest_y < 0 || dest_y > 7){
                                break;
                            }
                            move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y};
                            out.push_back(move);
                        }
                        if(enemy_piece_here(dest_x,dest_y,turn)){
                            move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y};
                            out.push_back(move);
                        }
                    }
                }
                break;}

            case pieces::KING:{
                //regular 3x3 movement block of king
                for(int dest_x = pos_x - 1; dest_x <= pos_x + 1; dest_x ++){
                    //boundary check x
                    if(dest_x < 0 || dest_x > 7){
                        continue;
                    }
                    for(int dest_y = pos_y - 1; dest_y <= pos_y + 1; pos_y ++){
                        //boundary check y
                        if(dest_y < 0 || dest_y > 7){
                            continue;
                        }
                        //king can go to any square without an ally there 
                        //(no check consideration; that is more expensive than seeing victory by king capture; TODO?)
                        if(!ally_piece_here(pos_x, pos_y, turn)){
                            Move move = {.start_x = pos_x, .start_y = pos_y, .end_x = dest_x, .end_y = dest_y};
                            out.push_back(move);
                        }
                    }
                }

                //castling options
                bool can_castle_KS;
                bool can_castle_QS;

                can_castle_KS = (((turn == colors::WHITE) * 0b0100 + (turn == colors::BLACK) * 0b0001) & castle_status) != 0;
                can_castle_QS = (((turn == colors::WHITE) * 0b1000 + (turn == colors::BLACK) * 0b0010) & castle_status) != 0;

                if(can_castle_KS){
                    Move move = {.start_x = pos_x, .start_y = pos_y, .end_x = 6, .end_y = pos_y};
                    out.push_back(move);
                }

                if(can_castle_QS){
                    Move move = {.start_x = pos_x, .start_y = pos_y, .end_x = 2, .end_y = pos_y};
                    out.push_back(move);
                }

                break;}
        }
    }
    return out;
}

bool Board::any_piece_here(int x,int y){
    return squares[x][y].piece != pieces::NONE;
}
bool Board::ally_piece_here(int x, int y, colors team){
    return squares[x][y].piece != pieces::NONE && squares[x][y].color == team;
}
bool Board::enemy_piece_here(int x, int y, colors team){
    colors enemy_team;
    if(team==colors::WHITE){
        enemy_team = colors::BLACK;
    }
    if(team==colors::BLACK){
        enemy_team = colors::WHITE;
    }
    return squares[x][y].piece != pieces::NONE && squares[x][y].color == enemy_team;
}