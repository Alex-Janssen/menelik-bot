//
// Created by ajans on 16.12.2022.
//

#include "board.hpp"
#include <vector>

    Board::Board(){
        this->the_board = new Square*[] {new Square[8], new Square[8], new Square[8], new Square[8], new Square[8], new Square[8], new Square[8], new Square[8]};
        this->turn = colors::WHITE;
        this->castle_status = 0b1111;
    }

    Board::Board(Square** squares, colors turn, unsigned int castle_status){
        this->the_board = squares;
        this->turn = turn;
        this->castle_status = castle_status;
    };

    Board::~Board() {
        delete the_board;
    }

    void Board::load_board(Board other){
        this->the_board = other.the_board;
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
        Square** next_board;

        switch (this->turn){
            case colors::WHITE:
                next_turn = colors::BLACK;
                break;
            case colors::BLACK:
                next_turn = colors::WHITE;
                break;
        }

        next_board = board_copy();
        int_fast8_t next_castle_status;
        next_castle_status = this->castle_status;

        Square* end_square = &next_board[move.end_x][move.end_y];
        Square* start_square = &next_board[move.start_x][move.start_y];

        colors start_color = start_square->color;
        pieces start_piece = start_square->piece;

        start_square->color = colors::NONE;
        start_square->piece = pieces::NONE;

        if(end_square->piece == pieces::EP_GHOST && end_square->color == next_turn){
            int y;
            if(next_turn == colors::WHITE){
                y = move.end_y-1;
            }
            if(next_turn == colors::BLACK){
                y = move.end_y+1;
            }
            next_board[move.end_x][y].color = colors::NONE;
            next_board[move.end_x][y].piece = pieces::NONE;
        }

        end_square->color = start_color;
        end_square->piece = start_piece;

        return new Board(next_board, next_turn, next_castle_status);
    }

    std::vector<Move> Board::get_legal_moves(){
        std::vector<Move> legal_moves;
        for(int pos_x = 0; pos_x < 8; pos_x++){
            for (int pos_y = 0; pos_y < 8; pos_y++){
                Move* move = nullptr;
                switch (this->the_board[pos_x][pos_y].piece){
                    case pieces::NONE:
                        break;
                    case pieces::PAWN:
                        break;
                    case pieces::ROOK:
                        break;
                    case pieces::KNIGHT:
                        break;
                    case pieces::BISHOP:
                        break;
                    case pieces::QUEEN:
                        break;
                    case pieces::KING:
                        break;
                }
                if(move != nullptr){
                    legal_moves.push_back(*move);
                }
            }
        }
        return legal_moves;
    }

    Square** Board::board_copy(){
        Square** copy = new Square * [] {new Square[8], new Square[8], new Square[8], new Square[8], new Square[8], new Square[8], new Square[8], new Square[8]};
        for(int x = 0; x < 8; x++){
            for(int y = 0; y < 8; y++){
                copy[x][y] = the_board[x][y];
            }
        }
        return copy;
    }
