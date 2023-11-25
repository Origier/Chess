#include "..\include\Chess\Chess.h"

namespace Chess_API {
    bool Chess::is_valid_player_input(std::pair<std::string, std::string>) {
        // TODO - Implement
        return false;
    }
    
    // Default constructor to start a blank new game
    Chess::Chess() {
        // TODO - Implement
    }

    // Constructor that can use serialized data to generate the game up to the current point based on past moves
    Chess::Chess(Game game_in, std::vector<std::pair<std::string, std::string>> past_moves) {
        // TODO - Implement
    }

    /*  Plays the next turn of the game - allows for the next determined player to input their moves into the game
    *   Plays out the moves, assuming they are valid, and changes the game state accordingly
    */ 
    void Chess::play_turn() {
        // TODO - Implement
    }

    // Determines if the game is currently in a state of check
    bool Chess::is_in_check() {
        // TODO - Implement
        return false;
    }

    // Determines if the game is currently in a state of stalemate
    bool Chess::is_stalemate() {
        // TODO - Implement
        return false;
    }

    // Determines if the game is currently in a check mate state - indicating the game is over
    bool Chess::is_in_check_mate() {
        // TODO - Implement
        return true;
    }

    // Returns the current player
    Player Chess::get_current_player() {
        // TODO - Implement
        return current_player;
    }

    // Displays the board to the console
    void Chess::show_board() {
        // TODO - Implement
    }
}