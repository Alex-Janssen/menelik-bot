#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include "Menelik_Main.hpp"
#include "board-rep/board.hpp"
#include "Eli_Algorithm.cpp"

int main () {
    std::ofstream fingerprint;
    fingerprint.open ("fingerprint.txt");

    int mate_in_one_start = time(NULL);
    fingerprint<<write_result(mate_in_one_1(), "Mate in one 1");
    int mate_in_one_end = time(NULL);
    fingerprint<<write_time(mate_in_one_start, mate_in_one_end, "Mate in one 1");

    int fork_kn_start = time(NULL);
    fingerprint<<write_result(fork_king_knight_1(), "King-rook knight fork 1");
    int fork_kn_end = time(NULL);
    fingerprint<<write_time(fork_kn_start, fork_kn_end, "King-rook knight fork 1");

    int mate_in_three_start = time(NULL);
    fingerprint<<write_result(mate_in_three_1(), "Mate in Three 1");
    int mate_in_three_end = time(NULL);
    fingerprint<<write_time(mate_in_three_start, mate_in_three_end, "Mate in Three 1");

    fingerprint.close();
    return 0;
}

bool mate_in_one_1 (){
    std::string fen = "r1bqk1nr/pppp1ppp/2n5/4p3/1bB5/4PQ2/PPPP1PPP/RNB1K1NR w KQkq - 0 1"; //scholar's mate
    Board* result = Eli_Algorithm::get_best_board(fen);
    return piece_enum_to_name(result->at(5,6).piece) == "QUEEN" && color_enum_to_name(result->at(5,6).color)=="WHITE";
}

bool mate_in_three_1 (){
    std::string fen = "r5rk/5p1p/5R2/4B3/8/8/7P/7K w - - 0 1"; //mate in 3
    Board* result = Eli_Algorithm::get_best_board(fen);
    return piece_enum_to_name(result->at(0,5).piece) == "QUEEN" && color_enum_to_name(result->at(0,5).color)=="WHITE";
}

bool fork_king_knight_1 (){
    std::string fen = "rnb1k1nr/2pppppp/8/3N2qb/p7/1P6/P1PPPPPP/RNBQKB1R w KQkq - 0 1"; //forksnight
    Board* result = Eli_Algorithm::get_best_board(fen);
    return piece_enum_to_name(result->at(2,6).piece) == "KNIGHT" && color_enum_to_name(result->at(5,6).color)=="WHITE";
}

std::string write_result(bool res, std::string name){
    if(res){
        return "Passed test " +name; 
    }
    return "Failed test "+name;
}
std::string write_time(int start, int end, std::string name){
    return "Test " + name + ": " + std::to_string(end-start) + " seconds";
}