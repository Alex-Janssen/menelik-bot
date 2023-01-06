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
    this->squares = new Square [64];
    //initialize to nobody's turn
    this->turn = colors::NONE;
    //initialize to all true
    this->castle_status = 0b1111;
}

Board::Board(Square* squares){
    //change board pointer to point to incoming board pointer
    this->squares = squares;
};


Board::Board(Square* squares, colors turn, unsigned int castle_status){
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
    return squares[8*row+col];
}

void Board::load_board(std::string& fen_string){
    std::string piece_string = fen_string.substr(0, fen_string.find(' '));
    char active_color = fen_string[piece_string.length()+1];
    std::string after_active_color = fen_string.substr(piece_string.length()+3);
    std::string castling_availability = after_active_color.substr(0, after_active_color.find(' '));
    std::string after_castling = after_active_color.substr(after_active_color.find(' ')+1);
    std::string en_passant_target = after_castling.substr(0, after_castling.find(' '));
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
        this->ep_row = -1;
        this->ep_col = -1;
    } else {
        this->ep_row = std::stoi(en_passant_target.substr(1))-1;
        this->ep_col = en_passant_target[0]-'a';
    }

    recalc_legal_moves();
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
            Square* to_set = &squares[ 8*(7-row) + column];//Adjusted because mirror set-up, thanks Theo...
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
    Square* next_squares;
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
    next_castle_status = this->castle_status & move.castle_change;

    //get references to the start and end squares
    Square* end_square = &next_squares[8*move.end_row + move.end_col];
    Square* start_square = &next_squares[8*move.start_row + move.start_col];

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
    if((move.end_row == ep_row && move.end_col == ep_col) && start_piece == pieces::PAWN){
        int row;
        //change row to get the associated pawn's location
        if(next_turn == colors::WHITE){
            row = move.end_row-1;
        }
        if(next_turn == colors::BLACK){
            row = move.end_row+1;
        }
        //get rid of the associated pawn
        next_squares[8*row + move.end_col].color = colors::NONE;
        next_squares[8*row + move.end_col].piece = pieces::NONE;
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
        
        //if we are castling (king move from e (col=4) to c (col=2) or g (col=6))
        if(move.start_col == 4 && (move.end_col == 2 || move.end_col == 6 )){

            //queenside castle (to c = col 2)
            if(move.end_col == 2){
                //remove rook from queen's side
                //row = move.start_row
                //col is 0
                next_squares[8 * move.start_row + 0].color = colors::NONE;
                next_squares[8 * move.start_row + 0].piece = pieces::NONE;
                
                //row = move.start_row
                //place rook at D (col = 3), with the color of the person who castled
                next_squares[8 * move.start_row + 3].color = this->turn;
                next_squares[8 * move.start_row + 3].piece = pieces::ROOK;
            }

            //kingside castle (to g = col 6)
            if(move.end_col == 6){
                //remove rook from king's side
                next_squares[8 * move.start_row + 7].color = colors::NONE;
                next_squares[8 * move.start_row + 7].piece = pieces::NONE;

                //place rook at F (x = 5), with the color of the person who castled
                next_squares[8 * move.start_row + 5].color = this->turn;
                next_squares[8 * move.start_row + 5].piece = pieces::ROOK;

            }
        }
    }
    
    //rook move check for castling rights update
    if(move.castle_change < 0b10000 && start_piece == pieces::ROOK ){
        //woooooo bitwise operand time
        bool K = move.start_col==7;
        bool W = move.start_row==0;

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
        if(move.start_row == 1 && move.end_row == 3){
            next_board->ep_col = move.end_col;
            next_board->ep_row = 2;
        }
        if(move.start_row == 6 && move.end_row == 4){
            next_board->ep_col = move.end_col;
            next_board->ep_row = 5;
        }
    }

    //return the updated board
    return next_board;
}

std::vector<Move> Board::get_legal_moves(){
    //create output vector
    std::vector<Move> legal_moves = std::vector<Move>();

    //iterate over all pieces
    for(int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            //only get the moves for the pieces whose turn it is
            if (this->squares[8*row + col].color != this -> turn){
                continue;
            }
            //get the type of the piece being observed
            pieces this_piece_type;
            this_piece_type = this->squares[8*row + col].piece;
            //get the moves this piece can make
            std::vector<Move> moves_this_piece;
            moves_this_piece = get_moves_from_position(row, col, this_piece_type, this->turn);
            //add this pieces possible moves to all possible moves
            legal_moves.insert(legal_moves.end(), moves_this_piece.begin(), moves_this_piece.end());
        }
    }
    return legal_moves;
}

void Board::recalc_legal_moves(){


    //iterate over all pieces
    for(int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            //get the type of the piece being observed
            pieces this_piece_type;
            this_piece_type = this->squares[8*row + col].piece;

            colors this_piece_color;
            this_piece_color = this->squares[8*row + col].color;
            //get the moves this piece can make
            std::vector<Move> moves_this_piece;
            moves_this_piece = get_moves_from_position(row, col, this_piece_type, this_piece_color);
            //this pieces moves are the moves_by_start of this piece's location
            moves_by_start[8*row + col] = moves_this_piece;
            //for each move here, add it to the moves_by_dest for that move's dest
            for(Move move : moves_this_piece){
                int dest_row = move.end_row;
                int dest_col = move.end_col;
                moves_by_dest[dest_row*8 + dest_col].push_back(move);
            }
        }
    }

    std::cout << "bye" << std::endl;

}

Square* Board::board_copy(){
    //create new squares for the copy
    Square* copy = new Square [64] ;
    for(int index = 0; index < 64; index++){
            //set by value the copied board
            copy[index] = squares[index];
    }
    return copy;
}

//when I first started writing this, I did not think that pawns would suck the most to write.
//considering the double-move, diagional for taking and forward for movement, en passant and promotion rules though it makes sense
std::vector<Move> Board::get_pawn_moves_from_pos(int row, int col, colors turn){
    std::vector<Move> out = std::vector<Move>();
    int dest_col = col;
    int dest_row;
    if(turn == colors::WHITE){
        dest_row = row+1;
        //if the pawn has not moved from the start, add a double move possiblity if unobstructed
        if(row == 1){
            if(!any_piece_here(dest_row + 1, dest_col) && !any_piece_here(dest_row, dest_col)){
                Move move = {.start_row = row, .start_col = col, .end_row = dest_row+1, .end_col = dest_col};
                out.push_back(move);
            }
        }
    }
    //like white, but y-1 for the basic move and also for the double move
    if(turn == colors::BLACK){
        dest_row = row - 1;
        //if the pawn has not moved from the start, add a double move possiblity if unobstructed
        if(row == 6){
            if(!any_piece_here(dest_row - 1, dest_col) && !any_piece_here(dest_row, dest_col)){
                Move move = {.start_row = row, .start_col = col, .end_row = dest_row-1, .end_col = dest_col};
                out.push_back(move);
            }
        }
    }
    //look in the 3 forward directions for movement
    for(dest_col = col - 1; dest_col <= col + 1; dest_col++){
        //bounds check
        if(dest_col < 0 || dest_col > 7){
            continue;
        }
        //only move if directly forwards (no change in x) and no piece there, or if diagonally (unequal x) and an enemy piece is there or en passant is there
        if((dest_col == col && !any_piece_here(dest_row, dest_col)) || (col != dest_col && (enemy_piece_here(dest_row, dest_col, turn) || (ep_row == dest_row && ep_col == dest_col)))){
            //if the move requires a promotion, add all possible promotions to the moves table
            if((dest_row == 7 && turn == colors::WHITE) || ((dest_row == 0 && turn == colors::BLACK))){
                Move move = {.start_row = row, .start_col = col, .end_row = dest_row, .end_col = dest_col};
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
                Move move = {.start_row = row, .start_col = col, .end_row = dest_row, .end_col = dest_col};
                out.push_back(move);
            }
        }
    }
    return out;
}

std::vector<Move> Board::get_rook_moves_from_pos(int row, int col, colors turn){
    std::vector<Move> out = std::vector<Move>();
    //vertical moves
    for(int delta_row = -1; delta_row <= 1; delta_row+=2){
        Move move;
        int dest_row = row + delta_row;
        int dest_col = col;
        while(dest_row >= 0 && dest_row <= 7){
            if(any_piece_here(dest_row,dest_col)){
                break;
            }
            move = {.start_row = row, .start_col = col, .end_row = dest_row, .end_col = dest_col};
            out.push_back(move);
            dest_row += delta_row;
        }
        if (dest_row >= 0 && dest_row <= 7){
            if(enemy_piece_here(dest_row,dest_col,turn)){
                move = {.start_row = row, .start_col = col, .end_row = dest_row, .end_col = dest_col};
                out.push_back(move);
            }
        }
    }
    //horizontal moves
    for(int delta_col = -1; delta_col <= 1; delta_col+=2){
        Move move;
        int dest_row = row;
        int dest_col = col + delta_col;
        while(dest_col >= 0 && dest_col <= 7){
            if(any_piece_here(dest_row,dest_col)){
                break;
            }
            move = {.start_row = row, .start_col = col, .end_row = dest_row, .end_col = dest_col};
            out.push_back(move);
            dest_col += delta_col;
        }
        if (dest_col >= 0 && dest_col <= 7){
            if(enemy_piece_here(dest_row,dest_col,turn)){
                move = {.start_row = row, .start_col = col, .end_row = dest_row, .end_col = dest_col};
                out.push_back(move);
            }
        }
    }
    return out;
}

std::vector<Move> Board::get_bishop_moves_from_pos(int row, int col, colors turn){
    std::vector<Move> out = std::vector<Move>();
    //diagonals are thankfully straight lines but sideways
    for(int delta_row = -1; delta_row <= 1; delta_row+=2){
        for(int delta_col = -1; delta_col <= 1; delta_col+=2){
            Move move;
            int dest_row = row + delta_row;
            int dest_col = col + delta_col;
            while(dest_row >= 0 && dest_row <= 7 && dest_col >= 0 && dest_col <= 7){
                if(any_piece_here(dest_row,dest_col)){
                    break;
                }
                move = {.start_row = row, .start_col = col, .end_row = dest_row, .end_col = dest_col};
                out.push_back(move);
                dest_row += delta_row;
                dest_col += delta_col;
            }
            if(dest_row >= 0 && dest_row <= 7 && dest_col >= 0 && dest_col <= 7){
                if(enemy_piece_here(dest_row,dest_col,turn)){
                    move = {.start_row = row, .start_col = col, .end_row = dest_row, .end_col = dest_col};
                    out.push_back(move);
                }
            }
        }
    }
    return out;
}

std::vector<Move> Board::get_knight_moves_from_pos(int row, int col, colors turn){
    std::vector<Move> out = std::vector<Move>();
    //just a silly little algorithm to look at all 8 possible horsey moves
    for(int dest_row = row - 2; dest_row <= row + 2; dest_row += 1){
        //boundary check x
        if(dest_row < 0 || dest_row > 7 || dest_row == row){
            continue;
        }
        //the funky part, isnt this neat?
        int x_factor = abs(dest_row-row) % 2;
        for(int dest_col = col - 1 - x_factor; dest_col <= col + 1 + x_factor; dest_col += 1 + x_factor){
            //boundary check y
            if(dest_col < 0 || dest_col > 7 || dest_col == col){
                continue;
            }
            
            //horsey only moves to where an ally is not
            if(!ally_piece_here(dest_row,dest_col,turn)){
                Move move = {.start_row = row, .start_col = col, .end_row = dest_row, .end_col = dest_col};
                out.push_back(move);
            }

        }
    }
    return out;
}

std::vector<Move> Board::get_king_moves_from_pos(int row, int col, colors turn){
    std::vector<Move> out = std::vector<Move>();
    //regular 3x3 movement block of king
    for(int dest_row = row - 1; dest_row <= row + 1; dest_row ++){
        //boundary check row
        if(dest_row < 0 || dest_row > 7){
            continue;
        }
        for(int dest_col = col - 1; dest_col <= col + 1; dest_col ++){
            //boundary check col
            if(dest_col < 0 || dest_col > 7){
                continue;
            }
            //king can go to any square without an ally there 
            //(no check consideration; that is more expensive than seeing victory by king capture; TODO?)
            if(!ally_piece_here(dest_row, dest_col, turn)){
                Move move = {.start_row = row, .start_col = col, .end_row = dest_row, .end_col = dest_col};
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
        if(!any_piece_here(row, 5) && !any_piece_here(row, 6)){
            Move move = {.start_row = row, .start_col = col, .end_row = row, .end_col = 6};
            out.push_back(move);
        }
    }

    if(can_castle_QS){
        if(!any_piece_here(row, 1) && !any_piece_here(row, 2) && !any_piece_here(row, 3)){
            Move move = {.start_row = row, .start_col = col, .end_row = row, .end_col = 2};
            out.push_back(move);
        }
    }
    return out;
}


std::vector<Move> Board::get_queen_moves_from_pos(int row, int col, colors turn){
    std::vector<Move> out = std::vector<Move>();
    std::vector<Move> bishopMoves = get_bishop_moves_from_pos(row,col,turn);
    std::vector<Move> rookMoves = get_rook_moves_from_pos(row,col,turn);
    out = bishopMoves;
    out.insert(out.end(), rookMoves.begin(), rookMoves.end());
    return out;
}

std::vector<Move> Board::get_moves_from_position(int row, int col, pieces piece_type, colors turn){
    std::vector<Move> out = std::vector<Move>();
    if(at(row,col).color == turn){
        switch (piece_type){
            case pieces::NONE:
                break;

            case pieces::PAWN:{
                out = get_pawn_moves_from_pos(row,col,turn);
                break;
            }

            case pieces::ROOK:{
                out = get_rook_moves_from_pos(row,col,turn);
                break;
            }

            case pieces::KNIGHT:{
                out = get_knight_moves_from_pos(row,col,turn);
                break;
            }

            case pieces::BISHOP:{
                out = get_bishop_moves_from_pos(row,col,turn);
                break;
            }

            case pieces::QUEEN:{
                out = get_queen_moves_from_pos(row, col, turn);
                break;
            }

            case pieces::KING:{
                out = get_king_moves_from_pos(row, col, turn);
                break;
            }
            default:{
                std::cout << "doing undefined things" << std::endl;
                break;
            }
        }
    }
    return out;
}

std::string Board::to_string(){
    std::string out = "";
	for(int row = 7; row >= 0; row--){
		for(int col = 0; col < 8; col++){;
            out += piece_to_char(at(row,col).piece, at(row,col).color);
		}
		out += '\n';
	}
    return out;
}

bool Board::any_piece_here(int row, int col){
    return squares[8*row + col].piece != pieces::NONE;
}
bool Board::ally_piece_here(int row, int col, colors team){
    return squares[8*row + col].piece != pieces::NONE && squares[8*row + col].color == team;
}
bool Board::enemy_piece_here(int row, int col, colors team){
    colors enemy_team;
    if(team==colors::WHITE){
        enemy_team = colors::BLACK;
    }
    if(team==colors::BLACK){
        enemy_team = colors::WHITE;
    }
    return squares[8*row + col].piece != pieces::NONE && squares[8*row + col].color == enemy_team;
}