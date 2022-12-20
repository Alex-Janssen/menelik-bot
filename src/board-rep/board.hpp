//
// Created by ajans on 16.12.2022.
//

#ifndef MENELIK_BOARD_H
#define MENELIK_BOARD_H

#include <string>
#include <vector>

enum class colors {BLACK, WHITE, NONE}; //Defines colors which are linked to a square
enum class pieces {KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, NONE}; //Defines piece on square.

struct Square {
    colors color = colors::NONE;
    pieces piece = pieces::NONE;
};

struct Move {
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    pieces promote_target = pieces::NONE;
};

class Board {

    public:

        colors turn = colors::NONE;
        colors victory = colors::NONE;
        Board();
        Board(Square** squares);
        Board(Square** squares, colors turn, unsigned int castle_status);
        ~Board();
        void load_board(Board other);
        void load_board(std::string& fen_string);
        std::vector<Move> get_legal_moves();
        Board* next_from_move(Move move);
        Square at(int x, int y);
        std::string to_string();

    private:

        Square** squares;
        int ep_x = -1;
        int ep_y = -1;
        /// @brief castle status: White Queenside | White Kingside | Black Queenside | Black Kingside
        int_fast8_t castle_status;
        int turns_until_draw = 50;
        std::vector<Move> get_moves_from_position(int pos_x, int pos_y, pieces piece_type, colors turn);
        Square** board_copy();
        void load_board_pieces(std::string& fen_string);
        bool any_piece_here(int pos_x, int pos_y);
        bool ally_piece_here(int pos_x, int pos_y, colors team);
        bool enemy_piece_here(int pos_x, int pos_y, colors team);

};

std::string color_enum_to_name(colors color){
    switch (color)
    {
    case colors::BLACK:
        return "BLACK";
        break;
    case colors::WHITE:
        return "WHITE";
        break;
    case colors::NONE:
        return "NONE";
        break;
    default:
        return "ERROR";
        break;
    }
}

std::string piece_enum_to_name(pieces piece){
    switch (piece)
    {
    case pieces::KING:
        return "KING";
        break;
    case pieces::BISHOP:
        return "BISHOP";
        break;
    case pieces::QUEEN:
        return "QUEEN";
        break;
    case pieces::ROOK:
        return "ROOK";
        break;
    case pieces::KNIGHT:
        return "KNIGHT";
        break;
    case pieces::PAWN:
        return "PAWN";
        break;
    case pieces::NONE:
        return "NONE";
        break;
    default:
        return "ERROR";
        break;
    }
}

char piece_to_char(pieces piece, colors color){
    switch (color)
    {
    case colors::BLACK:
        switch(piece){
            case pieces::KING:
                return 'k';
            case pieces::QUEEN:
                return 'q';
            case pieces::ROOK:
                return 'r';
            case pieces::KNIGHT:
                return 'n';
            case pieces::BISHOP:
                return 'b';
            case pieces::PAWN:
                return 'p';
            default:{
                return 'L';
            }
        }
        break;
    case colors::WHITE:
        switch(piece){
            case pieces::KING:
                return 'K';
            case pieces::QUEEN:
                return 'Q';
            case pieces::ROOK:
                return 'R';
            case pieces::KNIGHT:
                return 'N';
            case pieces::BISHOP:
                return 'B';
            case pieces::PAWN:
                return 'P';
            default:{
                return 'W';
            }
        }
        break;
    case colors::NONE:
        return '_';
    default:{
        return 'A';
        }
    }
}


#endif //MENELIK_BOARD_H
