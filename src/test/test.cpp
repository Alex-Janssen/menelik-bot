#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include "test.hpp"
#include "../board-rep/board.cpp"
#include "../eval/Eval_Functions.cpp"
#include "../eval/Eli_Algorithm.cpp"

Test::Test(){
    algorithm = new Eli_Algorithm();
}

Test::~Test(){
    delete algorithm;
}

bool Test::mate_in_one_1 (){
    std::string fen = "r1bqk1nr/pppp1ppp/2n5/4p3/1bB5/4PQ2/PPPP1PPP/RNB1K1NR w KQkq - 0 1"; //scholar's mate
    Board* result = algorithm->get_best_board(fen);
    return piece_enum_to_name(result->at(5,6).piece) == "QUEEN" && color_enum_to_name(result->at(5,6).color)=="WHITE";
}

bool Test::mate_in_one_2 (){
    std::string fen = "rnb1k2r/pppppppp/7n/6B1/3q1b2/4Q3/PPPPPPPP/RN2KBNR w KQkq - 0 1"; //queen checkmate
    Board* result = algorithm->get_best_board(fen);
    return piece_enum_to_name(result->at(4,6).piece) == "QUEEN" && color_enum_to_name(result->at(4,6).color)=="WHITE";
}


bool Test::fork_king_knight_1 (){
    std::string fen = "rnb1k1nr/2pppppp/8/3N2qb/p7/1P6/P1PPPPPP/RNBQKB1R w KQkq - 0 1"; //forksnight
    Board* result = algorithm->get_best_board(fen);
    return piece_enum_to_name(result->at(2,6).piece) == "KNIGHT" && color_enum_to_name(result->at(5,6).color)=="WHITE";
}

bool Test::fork_king_knight_2 (){
    std::string fen = "2q1k3/8/n7/8/2N5/3B4/2PPPPPP/4K3 w - - 0 1"; //forkknight
    Board* result = algorithm->get_best_board(fen);
    return piece_enum_to_name(result->at(3,5).piece) == "KNIGHT" && color_enum_to_name(result->at(3,5).color)=="WHITE";
}

bool Test::mate_in_two_1 (){
    std::string fen = "2bqkbn1/2pppp2/np2N3/r3P1p1/p2N2B1/5Q2/PPPPKPP1/RNB2r2 w - - 0 1"; //mate in 2
    Board* result = algorithm->get_best_board(fen);
    return piece_enum_to_name(result->at(5,6).piece) == "QUEEN" && color_enum_to_name(result->at(5,6).color)=="WHITE";
}

bool Test::mate_in_three_1 (){
    std::string fen = "r5rk/5p1p/5R2/4B3/8/8/7P/7K w - - 0 1"; //mate in 3
    Board* result = algorithm->get_best_board(fen);
    return piece_enum_to_name(result->at(0,5).piece) == "ROOK" && color_enum_to_name(result->at(0,5).color)=="WHITE";
}

bool Test::mate_in_three_2 (){
    std::string fen = "8/R7/4kPP1/3ppp2/3B1P2/1K1P1P2/8/8 w - - 0 1"; //mate in 3
    Board* result = algorithm->get_best_board(fen);
    return piece_enum_to_name(result->at(5,6).piece) == "PAWN" && color_enum_to_name(result->at(5,6).color)=="WHITE";
}

int main () {
    Test test = Test();
    std::ofstream fingerprint;
    fingerprint.open ("fingerprint.txt");

    long int mate_in_one_1_start = time(NULL);
    fingerprint<<write_result(test.mate_in_one_1(), "Mate in one 1");
    long int mate_in_one_1_end = time(NULL);
    fingerprint<<write_time(mate_in_one_1_start, mate_in_one_1_end, "Mate in one 1");

    long int mate_in_one_2_start = time(NULL);
    fingerprint<<write_result(test.mate_in_one_2(), "Mate in one 2");
    long int mate_in_one_2_end = time(NULL);
    fingerprint<<write_time(mate_in_one_2_start, mate_in_one_2_end, "Mate in one 2");

    long int capture_1_start = time(NULL);
    fingerprint<<write_result(test.capture_1(), "Fork Capture 1");
    long int capture_1_end = time(NULL);
    fingerprint<<write_time(capture_1_start, capture_1_end, "Mate in one 2");

    long int fork_kn_1_start = time(NULL);
    fingerprint<<write_result(test.fork_king_knight_1(), "King-rook knight fork 1");
    long int fork_kn_1_end = time(NULL);
    fingerprint<<write_time(fork_kn_1_start, fork_kn_1_end, "King-rook knight fork 1");

    long int fork_kn_2_start = time(NULL);
    fingerprint<<write_result(test.fork_king_knight_2(), "King-rook knight fork 2");
    long int fork_kn_2_end = time(NULL);
    fingerprint<<write_time(fork_kn_2_start, fork_kn_2_end, "King-rook knight fork 2");

    long int mate_in_two_1_start = time(NULL);
    fingerprint<<write_result(test.mate_in_two_1(), "Mate in Two 1");
    long int mate_in_two_1_end = time(NULL);
    fingerprint<<write_time(mate_in_two_1_start, mate_in_two_1_end, "Mate in Two 1");

    long int mate_in_three_1_start = time(NULL);
    fingerprint<<write_result(test.mate_in_three_1(), "Mate in Three 1");
    long int mate_in_three__1_end = time(NULL);
    fingerprint<<write_time(mate_in_three_1_start, mate_in_three__1_end, "Mate in Three 1");

    long int mate_in_three_2_start = time(NULL);
    fingerprint<<write_result(test.mate_in_three_2(), "Mate in Three 2 (promotion)");
    long int mate_in_three__2_end = time(NULL);
    fingerprint<<write_time(mate_in_three_2_start, mate_in_three__2_end, "Mate in Three 2 (promotion)");

    fingerprint.close();
    return 0;
}