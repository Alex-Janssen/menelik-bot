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
    int_fast8_t start_row;
    int_fast8_t start_col;
    int_fast8_t end_row;
    int_fast8_t end_col;
    int_fast8_t castle_change = 0b1111;
    pieces promote_target = pieces::NONE;
};

struct Pos {
    int_fast8_t row;
    int_fast8_t col;
};

class Board {

    public:

        colors turn = colors::NONE;
        colors victory = colors::NONE;
        int_fast8_t ep_row = -1;
        int_fast8_t ep_col = -1;
        /// @brief castle status: White Queenside | White Kingside | Black Queenside | Black Kingside
        int_fast8_t castle_status;
        Board();
        Board(Square* squares);
        Board(Square* squares, colors turn, unsigned int castle_status);
        ~Board();
        void load_board(std::string& fen_string);
        std::vector<Move> get_legal_moves();
        Board* next_from_move(Move move);
        Square at(int row, int col);
        std::string to_string();

        int white_threat_at(int row, int col);
        int black_threat_at(int row, int col);

    private:

        Square* squares;
        std::vector<Move> get_moves_from_position(int pos_x, int pos_y, pieces piece_type, colors turn);
        std::vector<Move> get_pawn_moves_from_pos(int pos_x, int pos_y, colors turn);
        std::vector<Move> get_knight_moves_from_pos(int pos_x, int pos_y, colors turn);
        std::vector<Move> get_king_moves_from_pos(int pos_x, int pos_y, colors turn);
        std::vector<Move> get_queen_moves_from_pos(int pos_x, int pos_y, colors turn);
        std::vector<Move> get_bishop_moves_from_pos(int pos_x, int pos_y, colors turn);
        std::vector<Move> get_rook_moves_from_pos(int pos_x, int pos_y, colors turn);
        Square* board_copy();
        void recalc_legal_moves();
        void load_board_pieces(std::string& fen_string);
        bool any_piece_here(int pos_x, int pos_y);
        bool ally_piece_here(int pos_x, int pos_y, colors team);
        bool enemy_piece_here(int pos_x, int pos_y, colors team);

        /// @brief list of all currently possible moves by start
        std::vector<Move> moves_by_start [64];
        /// @brief list of all currently possible moves by destination
        std::vector<Move> moves_by_dest [64];


        /// @brief starting squares which need moves recalculating iff an ally is present on a square
        std::vector<Pos> blocking [64] ;

        /// @brief ending squares which need moves recalculating iff an ally is present on a square
        std::vector<Pos> end_threat_removed_ally [64] ;

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

Move move_from_text(std::string& move_text){

    int start_col = move_text[0] - 'a';
    int start_row = move_text[1] - '1';
    int end_col = move_text[2] - 'a';
    int end_row = move_text[3] - '1';
    pieces promote_target;

    switch (std::tolower(move_text[4])){
        case 'r':{promote_target = pieces::ROOK; break;}
        case 'n':{promote_target = pieces::KNIGHT; break;}
        case 'b':{promote_target = pieces::BISHOP; break;}
        case 'q':{promote_target = pieces::QUEEN; break;}
        default:{promote_target = pieces::NONE; break;}
    }

    Move to_move = Move {.start_row=start_row, .start_col=start_col, 
                    .end_row=end_row, .end_col=end_col,
                    .castle_change=0b11111, .promote_target=promote_target};
    
    return to_move;

}


#endif //MENELIK_BOARD_H
