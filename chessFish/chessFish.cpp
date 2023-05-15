// chessFish.cpp : Defines the entry point for the application.
//

#include <stdio.h>
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <chrono>

#include "thc.h"

#include "chessFish.h"
#include "chessEngine.h"

using namespace std;
const map<std::string, double> expectedVal1 = {
    //white
    {"Nf3", 50.0},
    {"Nc3", 50.0},
    {"Nh3", 10.0},
    {"Na3", 10.0},
    {"e3", -20.0},
    {"d3", -20.0},
    {"e4", -25.0},
    {"d4", -25.0},
    {"f3", -30.0},
    {"c3", -30.0},
    {"h3", -40.0},
    {"g3", -40.0},
    {"b3", -40.0},
    {"a3", -40.0},
    {"f4", -40.0},
    {"c4", -40.0},
    {"h4", -45.0},
    {"g4", -45.0},
    {"b4", -45.0},
    {"a4", -45.0}
};
const map<std::string, double> expectedVal2 = {
    {"Rxf4", 900},
    {"Rxg7", 500},
    {"Rxb7", 300},
    {"Nxd4", 110},
    {"Ne5", 10.0},
    {"Kh1", 10.0},
    {"Rf8", 0.0},
    {"Re7", 0.0},
    {"Rd7", 0.0},
    {"Rc7+", 0.0},
    {"Rf6", 0.0},
    {"Rf5", 0.0},
    {"Kf1", 0.0},
    {"Rf1", 0.0},
    {"Re1", 0.0},
    {"Rd1", 0.0},
    {"Rc1", 0.0},
    {"Rb1", 0.0},
    {"Ng5", -5.0},
    {"Nd2", -5.0},
    {"c3" , -30.0},
    {"Nh4", -40.0},
    {"Ne1", -40.0},
    {"h3" , -40.0},
    {"g3" , -40.0},
    {"b3" , -40.0},
    {"a3" , -40.0},
    {"c4" , -40.0},
    {"h4" , -45.0},
    {"g4" , -45.0},
    {"b4" , -45.0},
    {"a4" , -45.0}
};

void display_position(thc::ChessRules* cr, const std::string& description)
{
    std::string fen = cr -> ForsythPublish();
    std::string s = cr -> ToDebugStr();
    printf("%s\n", description.c_str());
    printf("FEN (Forsyth Edwards Notation) = %s\n", fen.c_str());
    printf("Position = %s\n", s.c_str());
}

int main(){
    thc::ChessRules crObj;
    thc::ChessRules* cr = &crObj;
    display_position(cr, "initial position");
    //thc::Move move;
    //move.NaturalIn(&cr, "Nf3");
    //cout << "Nf3 value: " << move_value(cr, move, false) << endl;

    /*
    // test for evaluate piece
    for (char p : "NBQPRKnbqprk") {
        cout << p << ": " << evaluate_piece(p, thc::a1, false) << endl;
    }
    */
    /*
    thc::MOVELIST movelist = get_orderd_moves(cr);
    size_t size = movelist.count;
    thc::Move* moves = movelist.moves;
    for (int i = 0; i < size; i++) {
        thc::Move move = moves[i];
        cout << i << ") " << move.NaturalOut(&cr) << " " << move_value(cr, move, false)  << " correct ? " << (move_value(cr, move, false) == expectedVal1.find(move.NaturalOut(&cr)) -> second) << endl;
    }
    */
    
    ///*
    // Start the timer
    auto start = std::chrono::high_resolution_clock::now();
    TranspositionTable transpositionTable;
    thc::Move mv = minimax_root(cr, 7, true, &transpositionTable);
    // Stop the timer
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    std::chrono::duration<double> duration = end - start;

    // Print the duration in seconds
    std::cout << "Elapsed time: " << duration.count() << " seconds" << std::endl;
    cr -> PushMove(mv);
    display_position(cr, "after first bot move");
    //*/

    /*
    cout << "second test: " << endl;
    cr.Forsyth("2kr4/pb3Rrp/8/1p1p4/3p1q2/3P1N2/PPP2PPP/R5K1 w - - 0 20");
    movelist = get_orderd_moves(cr);
    size = movelist.count;
    moves = movelist.moves;
    for (int i = 0; i < size; i++) {
        thc::Move move = moves[i];
        cout << i << ") " << move.NaturalOut(&cr) << " " << move_value(cr, move, false) << " correct ? " << (move_value(cr, move, false) == expectedVal2.find(move.NaturalOut(&cr))->second) << endl;
    }
    */

    /*
    // Example 1, Play a few good moves from the initial position
    thc::ChessRules cr;
    //cr.Forsyth("rnb1kbnr/p3pppp/2p5/8/8/8/PPPP1PPP/RNB1KBNR w KQkq - 0 6");
    display_position(cr, "Initial position");
    thc::Move mv;
    mv.NaturalIn(&cr, "e4");

    // Call evaluate and get the material scores
    auto material_scores = evaluate(cr);
    int white_material = std::get<0>(material_scores);
    int black_material = std::get<1>(material_scores);

    // Print the material scores to the console
    std::cout << "White material score: " << white_material << std::endl;
    std::cout << "Black material score: " << black_material << std::endl;

    cout << "endgame?: " << check_end_game(cr) << endl;

    cout << "eval capture: " << evaluate_capture(cr, mv) << endl;

    thc::MOVELIST ordered_moves = get_orderd_moves(cr);
    cout << "move list: " << ordered_moves.moves << " size: " << ordered_moves.count << endl;

    // Loop through the ordered moves and print each move
    for (int i = 0; i < ordered_moves.count; i++) {
        mv = ordered_moves.moves[i];
        std::string mv_txt = mv.NaturalOut(&cr);
        printf("4.%s\n", mv_txt.c_str());
    }

    cr.PlayMove(mv);
    mv.NaturalIn(&cr, "e5");
    cr.PlayMove(mv);
    mv.NaturalIn(&cr, "Nf3");
    cr.PlayMove(mv);
    mv.NaturalIn(&cr, "Nc6");
    cr.PlayMove(mv);
    mv.NaturalIn(&cr, "Bc4");
    cr.PlayMove(mv);
    mv.NaturalIn(&cr, "Bc5");
    cr.PlayMove(mv);
    display_position(cr, "Starting position of Italian opening, after 1.e4 e5 2.Nf3 Nc6 3.Bc4 Bc5");

    
    printf("List of all legal moves in the current position\n");
    std::vector<thc::Move> moves;
    std::vector<bool> check;
    std::vector<bool> mate;
    std::vector<bool> stalemate;
    cr.GenLegalMoveList(moves, check, mate, stalemate);
    unsigned int len = moves.size();
    for (unsigned int i = 0; i < len; i++)
    {
        mv = moves[i];
        std::string mv_txt = mv.NaturalOut(&cr);
        const char* suffix = "";
        if (check[i])
            suffix = " (note '+' indicates check)";
        else if (mate[i])
            suffix = " (note '#' indicates mate)";
        printf("4.%s%s\n", mv_txt.c_str(), suffix);
    }

    // Example 2, The shortest game leading to mate
    printf("\n");
    thc::ChessRules cr2;
    cr = cr2;
    display_position(cr, "Initial position");
    mv.TerseIn(&cr, "g2g4");
    std::string description = "Position after 1." + mv.NaturalOut(&cr);
    cr.PlayMove(mv);
    display_position(cr, description);
    mv.TerseIn(&cr, "e7e5");
    description = "Position after 1..." + mv.NaturalOut(&cr);
    cr.PlayMove(mv);
    display_position(cr, description);
    mv.TerseIn(&cr, "f2f4");
    description = "Position after 2." + mv.NaturalOut(&cr);
    cr.PlayMove(mv);
    thc::TERMINAL eval_penultimate_position;
    bool legal1 = cr.Evaluate(eval_penultimate_position);
    bool normal1 = (eval_penultimate_position == thc::NOT_TERMINAL);
    display_position(cr, description);
    mv.TerseIn(&cr, "d8h4");
    description = "Position after 2..." + mv.NaturalOut(&cr);
    cr.PlayMove(mv);
    display_position(cr, description);
    thc::TERMINAL eval_final_position;
    bool legal2 = cr.Evaluate(eval_final_position);
    bool mate2 = (eval_final_position == thc::TERMINAL_WCHECKMATE);
    printf("legal1=%s, normal1=%s, legal2=%s, mate2=%s\n",
        legal1 ? "true" : "false",
        normal1 ? "true" : "false",
        legal2 ? "true" : "false",
        mate2 ? "true" : "false");
    if (legal1 && normal1 && legal2 && mate2)
        printf("As expected, all flags true, so both penultimate and final positions are legal, in the final position White is mated\n");
    else
        printf("Strange(?!), we expected all flags true, meaning both penultimate and final positions are legal, in the final position White is mated\n");
*/
}
