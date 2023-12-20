#ifndef CPLUSPLUS_CHESS_HUMAN_PLAYER
#define CPLUSPLUS_CHESS_HUMAN_PLAYER

#include "Player.h"
#include "Chess_API_vars.h"
#include <string>

namespace Chess_API {
    class Human_Player: public Player {
    public:
        // Default Constructor for the computer player
        Human_Player() : Player() {}

        // Name based constructor for the computer player
        Human_Player(std::string name, GAME_PIECE_COLOR color_in) : Player(name, color_in) {}

        std::pair<std::string, std::string> take_turn() const;

    };
}

#endif