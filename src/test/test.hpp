#ifndef MENELIK_TEST
#define MENELIK_TEST

#include "..\eval\Eval.hpp"
#include <string>

class Test{
    Eval* algorithm;
    public:
        Test();
        bool mate_in_one_1();
        bool fork_king_knight_1();
        bool mate_in_three_1();
        bool mate_in_three_2();

};

std::string write_result(bool res, std::string name){
    if(res){
        return "Passed test " +name+"\n"; 
    }
    return "Failed test "+name+"\n";
}
std::string write_time(int start, int end, std::string name){
    return "Test " + name + ": " + std::to_string(end-start) + " seconds \n";
}

#endif