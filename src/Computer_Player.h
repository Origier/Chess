#ifndef CPLUSPLUS_CHESS_COMPUTER_PLAYER
#define CPLUSPLUS_CHESS_COMPUTER_PLAYER

#include "Player.h"
#include "Game.h"

namespace Chess_API {
    class Computer_Player: public Player {
    private:
        const Game * game = nullptr; // A const reference to the game to make a determinination on what to do

    public:
        DIFFICULTY difficulty;    

        // Default Constructor for the computer player - computer must always have a game to reference to make descisions from
        Computer_Player(const Game * game_in) : game(game_in), Player(), difficulty(DEFAULT_COMPUTER_DIFFICULTY) {}

        // Constructor for determining the computers level of difficulty
        Computer_Player(const Game * game_in, GAME_PIECE_COLOR color_in, DIFFICULTY difficulty_in) : game(game_in), Player(DEFAULT_COMPUTER_NAME, color_in), difficulty(difficulty_in) {}

        // Sets a copy of the game to the computer players memory
        void set_internal_game(const Game * game_in) {game = game_in;}

        // Uses the provided game object to make a turn
        std::pair<std::string, std::string> take_turn() const;

    };
}

#endif