//
// Created by ajans on 16.12.2022.
//

#include "board.h"
#include <vector>

class Board{
    Square[8][8] the_board;

    Board(){
        this.the_board = Square[8][8];
        this.turn = colors.WHITE;
    };

    Board(Square[8][8] squares, turns turn){
        this.the_board = squares;
        this.turn = turn;
    };

    ~Board() {
        delete the_board;
    }

    void load_board(Board other){
        this.the_board = other.the_board;
        this.turn = other.turn;
    }

    void load_board(std::string& fen_string){
        int square_pos = 0;
        int fen_scan = 0;
        while (tick < 64 && fen_scan < std::strlen(fen_string)){

        }
    }

    Board next_from_move(Move move){
        turns next_turn;
        Square[8][8] next_board;

        switch (this.turn){
            case colors.WHITE:
                next_turn = colors.BLACK;
                break;
            case colors.BLACK:
                next_turn = colors.WHITE;
                break;
        }

        next_board = board_copy();

        Square* end_square = &next_board[move.end_x][move.end_y];
        Square* start_square = &next_board[move.start_x][move.start_y];

        colors start_color = start_square->color;
        pieces start_piece = start_square->piece;

        start_square->color = colors.NONE;
        start_square->piece = pieces.NONE;
        start_square->can_castle = false;

        if(end_square->piece == pieces.EP_GHOST && end_square->color == next_turn){
            int y;
            if(next_turn == colors.WHITE){
                y = end_y+1;
            }
            if(next_turn == colors.BLACK){
                y = end_y-1;
            }
            next_board[move.end_x][y].color = colors.NONE;
            next_board[move.end_x][y].piece = pieces.NONE;
            next_board[move.end_x][y].can_castle = false;
        }

        end_square->color = start_color;
        end_square->piece = start_piece;
        end_square->can_castle = false;

        return Board(next_board, next_turn);
    }

    std::vector<Move> get_legal_moves(){
        std::vector<Move> legal_moves;
        for(int pos_x = 0; pos_x < 8; pos_y++){
            for (int pos_y = 0; pos_y < 8; pos_x++){
                Move move;
                switch (the_board[pos_x][pos_y].piece){
                    case pieces.NONE:
                        break;
                    case pieces.PAWN:
                        break;
                    case pieces.ROOK:
                        break;
                    case pieces.KNIGHT:
                        break;
                    case pieces.BISHOP:
                        break;
                    case pieces.QUEEN:
                        break;
                    case pieces.KING:
                        break;
                }
                if(move != null){
                    legal_moves.push_back(move);
                }
            }
        }
        return legal_moves;
    }

    Square[8][8] board_copy(){
        Square[8][8] copy = Square[8][8];
        for(int x = 0; x < 8; x++){
            for(int y = 0; y < 8; y++){
                copy[x][y] = the_board[x][y];
            }
        }
    }
};
