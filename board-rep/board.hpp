//
// Created by ajans on 16.12.2022.
//

#include <string>
#include <vector>

#ifndef MENELIK_BOARD_H
#define MENELIK_BOARD_H

struct Move {
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    bool castle = false;
};

enum class colors {BLACK, WHITE, NONE}; //Defines colors which are linked to a square
enum class pieces {KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, EP_GHOST, NONE}; //Defines piece on square.

struct Square {
    colors color = colors::NONE;
    pieces piece = pieces::NONE;
};


class Board {


    public:

        Square** the_board;
        int_fast8_t castle_status;
        colors turn = colors::NONE;

        Board();
        Board(Square** squares, colors turn, unsigned int castle_status);
        ~Board();
        void load_board(Board);
        void load_board(std::string&);
        std::vector<Move> get_legal_moves();
        Board* next_from_move(Move);

    private:

        Square** board_copy();

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


#endif //MENELIK_BOARD_H
