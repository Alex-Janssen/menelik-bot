//
// Created by ajans on 19.12.2022.
//

#include "../board-rep/Board.hpp"
#include <map>

#ifndef MENELIK_MAIN_H
#define MENELIK_MAIN_H

class Menelik_Main {
    public:
        int main(int argc, char *argv[]);
        static Board* identify_best_from_fen(std::string fen_string);
};


#endif //MENELIK_LOAD_PARAMS_H
