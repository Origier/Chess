#include "Human_Player.h"

namespace Chess_API {
    // Prompts the player to come up with their move
    std::pair<std::string, std::string> Human_Player::take_turn() const {
        // TODO - Implement
        return std::make_pair("b2", "b3");
    }
}