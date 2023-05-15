#pragma once

#include <iostream>
#include <tuple>
#include <unordered_map>

struct TranspositionTableEntry {
    int score;
    int depth;
    thc::Move* bestMove;
};

class TranspositionTable {
public:
    void store(const std::string key, const int score, const int depth, thc::Move* bestMove) {
        table[key] = { score, depth, bestMove };
    }

    TranspositionTableEntry* lookup(const std::string key) {
        auto it = table.find(key);
        if (it != table.end()) {
            return &(it->second);
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string, TranspositionTableEntry> table;
};

void toPositionString(thc::ChessRules* cr, std::string& bord);
std::tuple<int, int> evaluate(const thc::ChessRules* cr);
bool check_end_game(thc::ChessRules* cr);
int evaluate_capture(thc::ChessRules* cr, thc::Move* mv);
double move_value(thc::ChessRules* cr, thc::Move* mv, bool endgame);
int evaluate_piece(char piece, int square, bool endgame);
thc::MOVELIST get_orderd_moves(thc::ChessRules* cr);
thc::Move minimax_root(thc::ChessRules* cr, int depth, bool maximize, TranspositionTable* transpositionTable);