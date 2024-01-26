#ifndef CPLUSPLUS_CHESS
#define CPLUSPLUS_CHESS

#include <vector>
#include <string>
#include <algorithm>    // transform
#include <stdexcept>    // runtime_error
#include <memory>       // shared_ptr     

#include "Game.h"
#include "Chess_API_vars.h"
#include "Human_Player.h"
#include "Computer_Player.h"

namespace Chess_API {
    class Chess {
    private:
        Game * game = nullptr;                                          // Object containing the game details such as board and game pieces
        std::vector<std::pair<std::string, std::string>> played_moves;  // Past moves played - allows for showing a play-by-play of how the game has gone as well as serializing the game to play later

        /* Determines if the provided input is a valid player input or not - used in conjunction with the player class to get player input
        *  The correct format for player input should be a string pair, indicating the starting position and the ending postion
        *  Examples include: {a1, b2}, {f7, e5}, {h8, a1}...
        *  This functions determines if the correct formatting was used for the player input
        */
        bool is_valid_player_input(std::pair<std::string, std::string>) const;
    public:
        // Default constructor to start a blank new game with a human vs a computer of default difficulty
        Chess();

        // Main constructor for taking the players name as well as the difficulty to play on 
        Chess(std::string player_name, DIFFICULTY difficulty);

        // Multiplayer constructor to play with two human players
        Chess(std::string player_name1, std::string player_name2);

        // Destructor that deletes all game data
        ~Chess();

        // Constructor that can regenerate the game from the game_in and past_moves
        Chess(Game game_in, std::vector<std::pair<std::string, std::string>> past_moves);

        /*  Plays the next turn of the game - allows for the next determined player to input their moves into the game
        *   Plays out the moves, assuming they are valid, and changes the game state accordingly
        */ 
        void play_turn();

        // Returns the games current state - used to determine if the game is in stalemate, checkmate, check or normal play
        Game::GAME_STATE get_current_game_state();

        // Returns the current player
        const std::shared_ptr<Player> get_current_player() const;

        // Displays the board to the console
        void show_board() const;
    };
}
    
#endif