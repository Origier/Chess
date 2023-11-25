#include "..\include\Chess\Chess.h"

namespace Chess_API {
    /* Determines if the provided input is a valid player input or not - used in conjunction with the player class to get player input
    *  The correct format for player input should be a string pair, indicating the starting position and the ending postion
    *  Examples include: {a1, b2}, {f7, e5}, {h8, a1}...
    *  This functions determines if the correct formatting was used for the player input
    */
    bool Chess::is_valid_player_input(std::pair<std::string, std::string> player_move) {
        // Create a copy of each string
        std::string start = std::get<0>(player_move);
        std::string end = std::get<1>(player_move);

        // If either strings are not exactly two characters then the format is incorrect
        if (start.size() != 2 || end.size() != 2) {
            return false;
        }

        // Ensure that each string is not case sensitive
        std::transform(start.cbegin(), start.cend(), start.begin(),
        [](unsigned char c) {std::tolower(c);});

        std::transform(end.cbegin(), end.cend(), end.begin(),
        [](unsigned char c) {std::tolower(c);});

        // Ensure that the first character of each string is a valid character
        auto i_start_char = std::find(valid_chars.cbegin(), valid_chars.cend(), start[0]);
        auto i_end_char = std::find(valid_chars.cbegin(), valid_chars.cend(), end[0]);

        // Ensure that the second character of each string is a valid number
        auto i_start_num = std::find(valid_nums.cbegin(), valid_nums.cend(), start[1]);
        auto i_end_num = std::find(valid_nums.cbegin(), valid_nums.cend(), end[1]);

        // Lastly - determining if the input is valid - for each case the iterator should not be pointing to the end iterator for the valid input strings
        bool valid_input = (i_start_char != valid_chars.end()) && (i_end_char != valid_chars.end()) && (i_start_num != valid_nums.end()) && (i_end_num != valid_nums.end());

        return valid_input;
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
        return current_player;
    }

    // Displays the board to the console
    void Chess::show_board() {
        game.show_board();
    }
}