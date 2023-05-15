#include <stdio.h>
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <unordered_map>

#include "thc.h"
#include "chessEngine.h"

using namespace std;

//string black = "b"; //depricated
//string white = "w";

//white pieces
const char wpawn = 'P';
const char wrook = 'R';
const char wknight = 'N';
const char wbischop = 'B';
const char wqueen = 'Q';
const char wking = 'K';

//black pieces
const char bpawn = 'p';
const char brook = 'r';
const char bknight = 'n';
const char bbischop = 'b';
const char bqueen = 'q';
const char bking = 'k';

//empty square
const char emptySQ = '.';

const map<char, int> piece_score_dic = {
    //white
    {wpawn, 100},
    {wrook, 500},
    {wknight, 300},
    {wbischop, 300},
    {wqueen, 900},
    {wking, 0},
    //black
    {bpawn, 100},
    {brook, 500},
    {bknight, 300},
    {bbischop, 300},
    {bqueen, 900},
    {bking, 0},
    //empty
    {emptySQ,0}
};

const int MATE_SCORE = 1000000000;
const int MATE_THRESHOLD = 999000000;

const int pawnEvalWhite[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
        5, 10, 10, -20, -20, 10, 10, 5,
        5, -5, -10, 0, 0, -10, -5, 5,
        0, 0, 0, 20, 20, 0, 0, 0,
        5, 5, 10, 25, 25, 10, 5, 5,
        10, 10, 20, 30, 30, 20, 10, 10,
        50, 50, 50, 50, 50, 50, 50, 50,
        0, 0, 0, 0, 0, 0, 0, 0
};

const int pawnEvalBlack[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5, 5, 10, 25, 25, 10, 5, 5,
    0, 0, 0, 20, 20, 0, 0, 0,
    5, -5, -10, 0, 0, -10, -5, 5,
    5, 10, 10, -20, -20, 10, 10, 5,
    0, 0, 0, 0, 0, 0, 0, 0};

const int knightEval[64] = {
    -50, -40, -30, -30, -30, -30, -40, -50,
        -40, -20, 0, 0, 0, 0, -20, -40,
        -30, 0, 10, 15, 15, 10, 0, -30,
        -30, 5, 15, 20, 20, 15, 5, -30,
        -30, 0, 15, 20, 20, 15, 0, -30,
        -30, 5, 10, 15, 15, 10, 5, -30,
        -40, -20, 0, 5, 5, 0, -20, -40,
        -50, -40, -30, -30, -30, -30, -40, -50
};

const int bishopEvalWhite[64] = {
    -20, -10, -10, -10, -10, -10, -10, -20,
        -10, 5, 0, 0, 0, 0, 5, -10,
        -10, 10, 10, 10, 10, 10, 10, -10,
        -10, 0, 10, 10, 10, 10, 0, -10,
        -10, 5, 5, 10, 10, 5, 5, -10,
        -10, 0, 5, 10, 10, 5, 0, -10,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -20, -10, -10, -10, -10, -10, -10, -20
};

const int bishopEvalBlack[64] = {
    -20, -10, -10, -10, -10, -10, -10, -20,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -10, 0, 5, 10, 10, 5, 0, -10,
    -10, 5, 5, 10, 10, 5, 5, -10,
    -10, 0, 10, 10, 10, 10, 0, -10,
    -10, 10, 10, 10, 10, 10, 10, -10,
    -10, 5, 0, 0, 0, 0, 5, -10,
    -20, -10, -10, -10, -10, -10, -10, -20
};

const int rookEvalWhite[64] = {
        0, 0, 0, 5, 5, 0, 0, 0,
       -5, 0, 0, 0, 0, 0, 0, -5,
       -5, 0, 0, 0, 0, 0, 0, -5,
       -5, 0, 0, 0, 0, 0, 0, -5,
       -5, 0, 0, 0, 0, 0, 0, -5,
       -5, 0, 0, 0, 0, 0, 0, -5,
        5, 10, 10, 10, 10, 10, 10, 5,
        0, 0, 0, 0, 0, 0, 0, 0
};

const int rookEvalBlack[64] = {
     0, 0, 0, 0, 0, 0, 0, 0,
     5, 10, 10, 10, 10, 10, 10, 5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
     0, 0, 0, 5, 5, 0, 0, 0
};

const int queenEval[64] = {
    -20, -10, -10, -5, -5, -10, -10, -20,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -10, 0, 5, 5, 5, 5, 0, -10,
        -5, 0, 5, 5, 5, 5, 0, -5,
        0, 0, 5, 5, 5, 5, 0, -5,
        -10, 5, 5, 5, 5, 5, 0, -10,
        -10, 0, 5, 0, 0, 0, 0, -10,
        -20, -10, -10, -5, -5, -10, -10, -20
};

const int kingEvalWhite[64] = {
    20, 30, 10, 0, 0, 10, 30, 20,
        20, 20, 0, 0, 0, 0, 20, 20,
        -10, -20, -20, -20, -20, -20, -20, -10,
        20, -30, -30, -40, -40, -30, -30, -20,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30
};
const int kingEvalBlack[64] = {
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -20, -30, -30, -40, -40, -30, -30, 20,
    -10, -20, -20, -20, -20, -20, -20, -10,
    20, 20, 0, 0, 0, 0, 20, 20,
    20, 30, 10, 0, 0, 10, 30, 20
};

const int kingEvalEndGameWhite[64] = {
    50, -30, -30, -30, -30, -30, -30, -50,
        -30, -30, 0, 0, 0, 0, -30, -30,
        -30, -10, 20, 30, 30, 20, -10, -30,
        -30, -10, 30, 40, 40, 30, -10, -30,
        -30, -10, 30, 40, 40, 30, -10, -30,
        -30, -10, 20, 30, 30, 20, -10, -30,
        -30, -20, -10, 0, 0, -10, -20, -30,
        -50, -40, -30, -20, -20, -30, -40, -50
};

const int kingEvalEndGameBlack[64] = {
    -50, -40, -30, -20, -20, -30, -40, -50,
    -30, -20, -10, 0, 0, -10, -20, -30,
    -30, -10, 20, 30, 30, 20, -10, -30,
    -30, -10, 30, 40, 40, 30, -10, -30,
    -30, -10, 30, 40, 40, 30, -10, -30,
    -30, -10, 20, 30, 30, 20, -10, -30,
    -30, -30, 0, 0, 0, 0, -30, -30, -50,
    -30, -30, -30, -30, -30, -30, 50
};

std::string generateHashKey(thc::ChessRules* cr) {
    std::string fen_full = cr->ForsythPublish();
    std::string fen = fen_full.substr(0, fen_full.find(' '));
    return fen;
}


void toPositionString(thc::ChessRules* cr, std::string& bord) {
    std::string bord_det = cr->ToDebugStr();
    std::size_t first_newline = bord_det.find('\n');
    std::size_t second_newline = bord_det.find('\n', first_newline + 1);
    bord = bord_det.substr(second_newline + 1);
    bord.erase(std::remove(bord.begin(), bord.end(), '\n'), bord.end());
}


std::tuple<int, int> evaluate(const thc::ChessRules* cr) {
    int white_material = 0;
    int black_material = 0;
    std::string fen_full = cr -> ForsythPublish();
    std::string fen = fen_full.substr(0, fen_full.find(' '));
    //printf("chess fenn string: %s\n", fen.c_str());

    for (char c : fen) {
        if (std::islower(c)) {
            black_material += piece_score_dic.find(c)->second;
        }
        else if (std::isupper(c)) {
            white_material += piece_score_dic.find(c)->second;
        }
    }
    return std::make_tuple(white_material, black_material);
}

bool check_end_game(thc::ChessRules* cr) {
    /*
    *  Are we in the end game ?
    *  Per Michniewski :
    *  -Both sides have no queens or
    *  -Every side which has a queen has additionally no other pieces or one minorpiece maximum.
    */
    int queens = 0;
    int minors = 0;

    std::string fen_full = cr -> ForsythPublish();
    std::string fen = fen_full.substr(0, fen_full.find(' '));
    for (char c : fen) {
        if (c == wqueen || c == bqueen) {queens++; }
        else if (c == wbischop || c == bbischop || c == wknight || c == bknight) { minors++; }
    }
    return queens == 0 || (queens == 2 && minors <= 1);
}


int evaluate_capture(thc::ChessRules* cr, thc::Move* mv) {
    /*
    *   Given a capturing move, weight the trade being made.
    */

    std::string bord;
    toPositionString(cr, bord);

    char _to = bord[mv -> dst];
    char _from = bord[mv -> src];
    int val = 0;
    if (cr -> enpassant_target == _to) {
        return piece_score_dic.find(wpawn) -> second;
    }
    //cout << _to << endl;
    return piece_score_dic.find(_to)->second - piece_score_dic.find(_from)->second;
}

double move_value(thc::ChessRules* cr, thc::Move* mv, bool endgame) {
    /*
    * How good is a move?
    * A promotion is great.
    * A weaker piece taking a stronger piece is good.
    * A stronger piece taking a weaker piece is bad.
    * Also consider the position change via piece-square table.
    */
    std::string bord;
    toPositionString(cr, bord);

    if (mv -> special == thc::SPECIAL_PROMOTION_QUEEN || mv -> special == thc::SPECIAL_PROMOTION_ROOK || mv -> special == thc::SPECIAL_PROMOTION_BISHOP || mv -> special == thc::SPECIAL_PROMOTION_KNIGHT ) {
        if (cr -> WhiteToPlay()) {
            return INFINITY;
        }
        else {
            return -INFINITY;
        }
    }

    char _piece = bord[mv -> src];

    //int _from_value = evaluate_piece(_piece, mv -> src, endgame);
    //int _to_value = evaluate_piece(_piece, mv -> dst, endgame);
    //int position_change = _to_value - _from_value;
    int position_change = evaluate_piece(_piece, mv->dst, endgame) - evaluate_piece(_piece, mv->src, endgame);
    int capture_value = 0.0;
    if (mv -> capture != ' ') {
        capture_value = evaluate_capture(cr, mv);
    }
    int current_move_value = capture_value + position_change;
    if (!cr -> WhiteToPlay()) {
        current_move_value = -current_move_value;
    }
    //cout << current_move_value << endl;
    return current_move_value;
}

int evaluate_piece(char piece, int square, bool endgame) {
    switch (piece)
    {
    case wpawn:
        return pawnEvalWhite[square];
        break;
    case bpawn:
        return pawnEvalBlack[square];
        break;
    case wknight:
        return knightEval[square];
        break;
    case bknight:
        return knightEval[square];
        break;
    case wbischop:
        return bishopEvalWhite[square];
        break;
    case bbischop:
        return bishopEvalBlack[square];
        break;
    case wrook:
        return rookEvalWhite[square];
        break;
    case brook:
        return rookEvalBlack[square];
        break;
    case wqueen:
        return queenEval[square];
        break;
    case bqueen:
        return queenEval[square];
        break;
    case wking:
        if (endgame) {
            return kingEvalEndGameWhite[square];
        }
        else {
            return kingEvalWhite[square];
        }
        break;
    case bking:
        if (endgame) {
            return kingEvalEndGameBlack[square];
        }
        else {
            return kingEvalBlack[square];
        }
        break;
    default:
        return 0;
        break;
    }
}

thc::Move* reverseArray(const thc::Move* arr, const std::size_t size) {
    thc::Move* reversed = new thc::Move[size];
    for (std::size_t i = 0; i < size; i++) {
        reversed[i] = arr[size - 1 - i];
    }
    return reversed;
}

thc::MOVELIST get_orderd_moves(thc::ChessRules* cr) {
   /*
   * Get legal moves.
   * Attempt to sort moves by best to worst.
   * Use piece values (and positional gains/losses) to weight captures.
   */
    thc::MOVELIST legalMoves;
    bool end_game = check_end_game(cr);
    cr -> GenLegalMoveList(&legalMoves);
    int size = legalMoves.count;
    thc::Move* moves = legalMoves.moves;

    // Define a lambda function to compare moves by their values
    auto compareMoves = [&](thc::Move a, thc::Move b) {
        int value1 = move_value(cr, &a, end_game);
        int value2 = move_value(cr, &b, end_game);
        return value1 < value2;
    };

    // Sort the moves array using the comparator function
    std::stable_sort(moves, moves + size, compareMoves);
    
    if (cr -> WhiteToPlay()) {
        moves = reverseArray(moves, size);
    }

    // Create a new MOVELIST and copy the sorted moves into it
    thc::MOVELIST orderedMoves;
    orderedMoves.count = size;
    std::copy(moves, moves + size, orderedMoves.moves);
    return orderedMoves;
}

thc::Move find_random_move(thc::ChessRules* cr) {
    thc::MOVELIST temp = get_orderd_moves(cr);
    thc::Move* moves = temp.moves;
    srand(time(NULL));

    // Generate a random index within the range of the array
    int random_index = rand() % temp.count;

    // Return the element at the random index
    return moves[random_index];
}

double minimax(thc::ChessRules* cr, double alpha, double beta, int depth, bool maximizing_player, bool whiteVraagteken, int* counter, TranspositionTable* transpositionTable) {
    std::string key = generateHashKey(cr);

    // Lookup the position in the Transposition Table
    TranspositionTableEntry* ttEntry = transpositionTable->lookup(key);
    if (ttEntry != nullptr && ttEntry->depth >= depth) {
        // Return the stored evaluation score if depth is sufficient
        return ttEntry->score;
    }

    thc::TERMINAL eval_final_position;
    bool legal = cr -> Evaluate(eval_final_position);
    bool matew = (eval_final_position == thc::TERMINAL_WCHECKMATE);
    bool mateb = (eval_final_position == thc::TERMINAL_BCHECKMATE);
    if (matew && !whiteVraagteken ||mateb && whiteVraagteken) { 
        //cout << cr -> ToDebugStr() << "\n this is mate mate" << endl; //TODO remove
        return maximizing_player ? -MATE_SCORE : MATE_SCORE; }
    
    if (depth == 0) {
        std::tuple<int,int> material_scores = evaluate(cr);
        if (whiteVraagteken) { 
            return std::get<0>(material_scores);
        }
        else { 
            return std::get<1>(material_scores);
        }
    }

    if (maximizing_player) {
        double best_move = -INFINITY;
        thc::MOVELIST movesList = get_orderd_moves(cr);
        int size = movesList.count;
        thc::Move* moves = movesList.moves;
        for (int i = 0; i < size; i++) {
            *counter += 1;
            thc::Move move = moves[i];
            cr -> PushMove(move);
            double curr_move = minimax(cr, alpha, beta, depth-1, !maximizing_player, !whiteVraagteken, counter, transpositionTable);
            
            // Each ply after a checkmate is slower, so they get ranked slightly less
            // We want the fastest mate!
            if (curr_move > MATE_THRESHOLD) {
                curr_move -= 1;
            }
            else if (curr_move < -MATE_THRESHOLD) {
                curr_move += 1;
            }
            best_move = (best_move > curr_move) ? best_move : curr_move;
            cr -> PopMove(move);
            alpha = (alpha > best_move) ? alpha : best_move;
            if (beta <= alpha) {
                transpositionTable->store(key, best_move, depth, &move);
                return best_move;
            }
        }
        return best_move;
    }else {
        double best_move = INFINITY;
        thc::MOVELIST movesList = get_orderd_moves(cr);
        int size = movesList.count;
        thc::Move* moves = movesList.moves;
        for (int i = 0; i < size; i++) {
            *counter += 1;
            thc::Move move = moves[i];
            cr -> PushMove(move);
            double curr_move = minimax(cr, alpha, beta, depth - 1, !maximizing_player, !whiteVraagteken, counter, transpositionTable);
            // Each ply after a checkmate is slower, so they get ranked slightly less
            // We want the fastest mate!
            if (curr_move > MATE_THRESHOLD) {
                curr_move -= 1;
            }
            else if (curr_move < -MATE_THRESHOLD) {
                curr_move += 1;
            }
            best_move = (best_move < curr_move) ? best_move : curr_move;
            cr -> PopMove(move);
            beta = (beta < best_move) ? beta : best_move;
            if (beta <= alpha) {
                transpositionTable->store(key, best_move, depth, &move);
                return best_move;
            }
        }
        return best_move;
    }
}

thc::Move minimax_root(thc::ChessRules* cr, int depth, bool maximize, TranspositionTable* transpositionTable) {
    /*
    * What is the highest value move per our evaluation function?
    */
    std::string key = generateHashKey(cr);

    // Lookup the position in the Transposition Table
    TranspositionTableEntry* ttEntry = transpositionTable -> lookup(key);
    if (ttEntry != nullptr && ttEntry->depth >= depth) {
        // Return the stored evaluation score if depth is sufficient
        return *(ttEntry->bestMove);
    }

    double best_move = maximize ? -INFINITY : INFINITY;
    thc::MOVELIST movesList = get_orderd_moves(cr);
    int size = movesList.count;
    thc::Move* moves = movesList.moves;
    thc::Move best_move_found = moves[0];

    bool end_game = check_end_game(cr);
    for (int i = 0; i < size; i++) {
        thc::Move move = moves[i];
        cr -> PushMove(move);
        thc::DRAWTYPE drawtype;
        double value = 0.0;
        int branchCount = 0;
        int* branchCounter = &branchCount;
        if (!cr -> IsDraw(cr -> WhiteToPlay(), drawtype)) {
            value = minimax(cr, -INFINITY, INFINITY, depth - 1, !maximize, cr -> WhiteToPlay(), branchCounter, transpositionTable);
        }
        cr -> PopMove(move);
        cout << "move: " << move.NaturalOut(cr) << " " << branchCount << endl;
        if (maximize && value > best_move) {
            best_move = value;
            best_move_found = move;
        }
        else if (!maximize && value < best_move) {
            best_move = value;
            best_move_found = move;
        }
    }
    transpositionTable -> store(key, best_move, depth, &best_move_found);
    return best_move_found;
}