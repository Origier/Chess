#include "Computer_Player.h"

namespace Chess_API {
    // Prompts the computer to come up with their move
    std::pair<std::string, std::string> Computer_Player::take_turn() const {
        // TODO - Implement
        return std::make_pair("a2", "a3");
    }
}