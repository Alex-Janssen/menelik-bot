//
// Created by ajans on 16.12.2022.
//

#ifndef MENELIK_BOARD_H
#define MENELIK_BOARD_H


enum colors {BLACK, WHITE, NONE}; //Defines colors which are linked to a square
enum pieces {KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, EP_GHOST, NONE}; //Defines piece on square.

struct Square {
    colors color = colors.NONE;
    pieces piece = pieces.NONE;
    bool can_castle = false;
};

struct Move {
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    bool castle = false;
};

class Board {

    Square[8][8] the_board;

    public:

        Board();
        Board(Square[8][8] squares, colors turn);
        ~Board();
        void load_board(Board);
        void load_board(std::string&);
        Move[] get_legal_moves();
        Board next_from_move(Move);
        std::vector<Move> get_legal_moves();

    private:

        Square[8][8] board_copy();

};


#endif //MENELIK_BOARD_H
