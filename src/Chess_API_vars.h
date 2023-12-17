#ifndef CPLUSPLUS_CHESS_API_VARS
#define CPLUSPLUS_CHESS_API_VARS

#include <string>
#include <tuple>

namespace Chess_API {
    const std::string VALID_CHARS = "abcdefgh";                             // Valid characters referring to the places on the chess board
    const std::string VALID_NUMS = "12345678";                              // Valid numbers referring to the places on the chess board
    const std::string DEFAULT_COMPUTER_NAME = "Computer";                   // Default value for the computer player
    const std::string DEFAULT_PLAYER_NAME = "Default";                      // Default name for the player class
    const std::string DEFAULT_HUMAN_NAME = "Human";                         // Default name for the human player
    const std::pair<std::string, std::string> DEFAULT_STRING_PAIR("", "");  // Default value for returns on string pairs - used for virtual classes
    const short DEFAULT_CHESS_BOARD_SIZE = 8;                               // Default value for the chess' board size

    // Error message for typing in the wrong input in the game
    const std::string INVALID_INPUT_ERROR_MSG = "That isn't valid input, type your move in {{char}{num} {char}{num}} format using \"" + VALID_CHARS + "\" as the valid characters and \"" + VALID_NUMS + "\" as the valid numbers";

    // Error message for typing in an invalid move
    const std::string INVALID_MOVE_ERROR_MSG = "This isn't a valid move - ensure that the move you are typing is feasible for the piece you are using.";
}

#endif