#ifndef CPLUSPLUS_CHESS
#define CPLUSPLUS_CHESS

#include <vector>
#include <string>
#include <algorithm> // transform
#include <stdexcept> // runtime_error

#include "Game.h"
#include "Chess_API_vars.h"
#include "Human_Player.h"
#include "Computer_Player.h"

namespace Chess_API {
    class Chess {
    private:
        Game * game = nullptr;                                          // Object containing the game details such as board and game pieces
        Player * player1 = nullptr;                                     // Player object that would have the "white" pieces
        Player * player2 = nullptr;                                     // Player object that would have the "black" pieces
        std::vector<std::pair<std::string, std::string>> played_moves;  // Past moves played - allows for showing a play-by-play of how the game has gone as well as serializing the game to play later
        const Player * current_player = player1;                        // Reference to whomever is the current player object to take their turn

        /* Determines if the provided input is a valid player input or not - used in conjunction with the player class to get player input
        *  The correct format for player input should be a string pair, indicating the starting position and the ending postion
        *  Examples include: {a1, b2}, {f7, e5}, {h8, a1}...
        *  This functions determines if the correct formatting was used for the player input
        */
        bool is_valid_player_input(std::pair<std::string, std::string>) const;
    public:
        

        // Default constructor to start a blank new game
        Chess();

        // Main constructor for taking the players name as well as the difficulty to play on 
        Chess(std::string player_name, Computer_Player::DIFFICULTY difficulty);

        // Multiplayer constructor to play with two human players
        Chess(std::string player_name1, std::string player_name2);

        // Destructor that deletes all game data
        ~Chess();

        // Constructor that can use serialized data to generate the game up to the current point based on past moves
        Chess::Chess(Game game_in, std::vector<std::pair<std::string, std::string>> past_moves, std::string player_name1, std::string player_name2, Computer_Player::DIFFICULTY difficulty);

        /*  Plays the next turn of the game - allows for the next determined player to input their moves into the game
        *   Plays out the moves, assuming they are valid, and changes the game state accordingly
        */ 
        void play_turn();

        // Determines if the game is currently in a state of check
        bool is_in_check() const;

        // Determines if the game is currently in a state of stalemate
        bool is_stalemate() const;

        // Determines if the game is currently in a check mate state - indicating the game is over
        bool is_in_check_mate() const;

        // Returns the current player
        const Player * get_current_player() const;

        // Displays the board to the console
        void show_board() const;
    };
}
    
#endif