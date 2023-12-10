#include "Chess.h"
#include <iostream>

using namespace std;
using namespace Chess_API;

int main() {
    // Expected usage of the Chess object
    Chess test_game;

    while (true) {
        // Print the game board
        test_game.show_board();

        // Play the next players turn
        test_game.play_turn();

        // If the game is in a check state then give the warning
        if (test_game.is_in_check()) {
            cout << test_game.get_current_player()->get_name() << " is in check!" << endl;
        }

        // If the game is in stalemate then end the game
        if (test_game.is_stalemate()) {
            cout << "The game has stalemated! Nobody wins..." << endl;
            break;
        }

        // If the game is in check mate then end the game
        if (test_game.is_in_check_mate()) {
            cout << "Check mate! " << test_game.get_current_player()->get_name() << " wins!" << endl;
            break;
        }
    }
}