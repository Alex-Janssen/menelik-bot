#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Load_Params.hpp"

using std::ifstream;

/// @brief Takes file called piece_vals.txt and parses each value. Must be of format 'piece':intval
/// @return Map of 'piece' to intval.
const std::map<char, int> Load_Params::read_piece_vals(){
    std::fstream p_vals;
    std::map<char, int> to_return;
    p_vals.open("src/eval/params/piece_vals.txt",std::ios::in); //read file
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
/// @brief Takes file called eval_params.txt and parses it. Must be of format "param":int
/// @return Map from string to int.
const std::map<std::string, int> Load_Params::read_eval_params(){
    std::fstream p_vals;
    std::map<std::string, int> to_return;
    p_vals.open("src/eval/params/eval_params.txt",std::ios::in); //read file
    if (p_vals.is_open()){ //if open
        std::string to_parse;
        while(getline(p_vals, to_parse)){ //read data from file object and put it into string.
            std::string param_name = to_parse.substr(0, to_parse.find(':'));
            std::string val = to_parse.substr(to_parse.find(':')+1, to_parse.size());
            to_return.emplace(param_name, stoi(val));//Insert character at piece line
        }
        p_vals.close(); //close the file object.
    }
    return to_return;
}