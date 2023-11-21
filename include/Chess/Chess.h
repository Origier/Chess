#ifndef CPLUSPLUS_CHESS
#define CPLUSPLUS_CHESS

#include <vector>
#include <string>

#include ".\Game.h"
#include ".\Player.h"

namespace Chess {
    class Chess {
    private:
        Game game;                                                      // Object containing the game details such as board and game pieces
        Player player1;                                                 // Player object that would have the "white" pieces
        Player player2;                                                 // Player object that would have the "black" pieces
        std::vector<std::pair<std::string, std::string>> played_moves;  // Past moves played - allows for showing a play-by-play of how the game has gone as well as serializing the game to play later
        bool player1_played_last = false;                               // Flag to determine if the first player went previously - determines who is next to go

        // Determines if the provided input is a valid player input or not - used in conjunction with the player class to get player input
        bool is_valid_player_input(std::pair<std::string, std::string>);
    public:
        // Default constructor to start a blank new game
        Chess();

        // Constructor that can use serialized data to generate the game up to the current point based on past moves
        Chess(Game game_in, std::vector<std::pair<std::string, std::string>> past_moves);

        /*  Plays the next turn of the game - allows for the next determined player to input their moves into the game
        *   Plays out the moves, assuming they are valid, and changes the game state accordingly
        */ 
        void play_turn();

        // Determines if the game is currently in a state of check
        bool is_in_check();

        // Determines if the game is currently in a state of stalemate
        bool is_stalemate();

        // Determines if the game is currently in a check mate state - indicating the game is over
        bool is_in_check_mate();
    };
}
    
#endif