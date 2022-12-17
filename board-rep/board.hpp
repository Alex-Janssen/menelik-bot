//
// Created by ajans on 16.12.2022.
//

#include <string>
#include <vector>

#ifndef MENELIK_BOARD_H
#define MENELIK_BOARD_H


enum class colors {BLACK, WHITE, NONE}; //Defines colors which are linked to a square
enum class pieces {KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, EP_GHOST, NONE}; //Defines piece on square.

struct Square {
    colors color = colors::NONE;
    pieces piece = pieces::NONE;
};

struct Move {
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    bool castle = false;
};

class Board {

    Square** the_board;
    int_fast8_t castle_status;
    colors turn = colors::NONE;

    public:

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


#endif //MENELIK_BOARD_H
