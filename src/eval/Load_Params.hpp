//
// Created by ajans on 19.12.2022.
//

#include "../board-rep/Board.hpp"
#include <map>

#ifndef MENELIK_LOAD_PARAMS_H
#define MENELIK_LOAD_PARAMS_H


class Load_Params {
    public:
        static std::map<char, int> read_piece_vals();
};


#endif //MENELIK_LOAD_PARAMS_H