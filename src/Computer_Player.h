#ifndef CPLUSPLUS_CHESS_COMPUTER_PLAYER
#define CPLUSPLUS_CHESS_COMPUTER_PLAYER

#include "Player.h"
#include "Chess_API_vars.h"
#include "Game.h"

namespace Chess_API {
    class Computer_Player: public Player {
    private:
        const Game * game = nullptr; // A const reference to the game to make a determinination on what to do

    public:
        enum DIFFICULTY {
            VERY_EASY,
            EASY,
            MEDIUM,
            HARD,
            HARDEST,
            EXPERT
        };

        DIFFICULTY difficulty;    

        // Default Constructor for the computer player - computer must always have a game to reference to make descisions from
        Computer_Player(const Game * game_in) : game(game_in), Player(DEFAULT_COMPUTER_NAME), difficulty(DIFFICULTY::MEDIUM) {}

        // Constructor for determining the computers level of difficulty
        Computer_Player(const Game * game_in, DIFFICULTY difficulty_in) : game(game_in), Player(DEFAULT_COMPUTER_NAME), difficulty(difficulty_in) {}

        // Uses the provided game object to make a turn
        std::pair<std::string, std::string> take_turn() const;
    };
}

#endif