#include "Eval.hpp"
#include "Load_Params.hpp"
#include <map>
#include <vector>

class Eli_Algorithm : public Eval{
    static const std::map<char, int> piece_vals; 
    static const std::map<std::string, int> eval_params;
    public: 
        static Board* get_best_board(Board* board);
        virtual Move get_best_move(Board* board);
        static Board* get_best_board(std::string& fen_string);
        virtual Move get_best_move(std::string& fen_string);
    private:
        static std::vector<Board*> spawn_children(Board* board);
        static float minimax(Board* board, int depth, float alpha, float beta, bool maximizing_player);
        static void identify_best_move(Board board);
};