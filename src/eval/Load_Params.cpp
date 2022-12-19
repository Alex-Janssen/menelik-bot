#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Load_Params.hpp"

using std::ifstream;


static std::map<char, int> read_piece_vals(){
    std::fstream p_vals;
    std::map<char, int> to_return;
    p_vals.open("./params/piece_vals.txt",std::ios::in); //read file
    if (p_vals.is_open()){ //if open
        std::string to_parse;
        while(getline(p_vals, to_parse)){ //read data from file object and put it into string.
            std::string piece_line = to_parse.substr(0, to_parse.find(':'));
            std::string point_line = to_parse.substr(to_parse.find(':')+1, to_parse.size());
            to_return.emplace(tolower(piece_line.at(0)), stoi(point_line));//Insert character at piece line
        }
        p_vals.close(); //close the file object.
    }
    return to_return;
}