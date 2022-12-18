//
// Created by ajans on 16.12.2022.
//

#include "Board.hpp"
#include <vector>

Board::Board(){
    //initalize empty board
    this->squares = new Square * [8] {new Square[8], new Square[8], new Square[8], new Square[8], new Square[8], new Square[8], new Square[8], new Square[8]};
    //initialize to nobody's turn
    this->turn = colors::NONE;
    //castle status: White Queenside | White Kingside | Black Queenside | Black Kingside
    //initialize to all true
    this->castle_status = 0b1111;
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

void Board::load_board(Board other){
    this->squares = other.squares;
    this->turn = other.turn;
}

void Board::load_board(std::string& fen_string){
    int square_pos = 0;
    int fen_scan = 0;
    /*while (tick < 64 && fen_scan < std::strlen(fen_string)){

    }*/
}

Board* Board::next_from_move(Move move){
    colors next_turn;
    Square** next_squares;
    int_fast8_t next_castle_status;
    Board* next_board = new Board(next_squares, next_turn, next_castle_status);

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
        next_board->victory = this->turn;
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
            next_board->castle_status &= 0b0011;
        }
        if(this->turn == colors::BLACK){
            next_board->castle_status &= 0b1100;
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
                break;}

            case pieces::KNIGHT:{
                break;}

            case pieces::BISHOP:{
                break;}

            case pieces::QUEEN:{
                break;}

            case pieces::KING:{
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