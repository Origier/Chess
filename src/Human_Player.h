#ifndef CPLUSPLUS_CHESS_HUMAN_PLAYER
#define CPLUSPLUS_CHESS_HUMAN_PLAYER

#include "Player.h"
#include "Chess_API_vars.h"
#include <string>

namespace Chess_API {
    class Human_Player: public Player {
    public:
        // Default Constructor for the computer player
        Human_Player() : Player(DEFAULT_HUMAN_NAME) {}

        // Name based constructor for the computer player
        Human_Player(std::string name) : Player(name) {}

        std::pair<std::string, std::string> take_turn() const;
    };
}

#endif