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

    // Different types of game pieces for chess
    enum GAME_PIECE_TYPE {
        PAWN = 1,
        KNIGHT,
        ROOK,
        BISHOP,
        KING,
        QUEEN,
        NOTYPE,  // Void value to indicate an invalid piece
        TYPEMIN = PAWN,
        TYPEMAX = QUEEN
    };

    // Different colors for game pieces for chess
    enum GAME_PIECE_COLOR {
        WHITE = 1,
        BLACK,
        NOCOLOR, // Void value to indicate an invalid piece
        COLORMIN = WHITE,
        COLORMAX = BLACK
    };

    // Representation of a game piece - simple mechanics to set some rules for the piece
    struct game_piece {
        // Default values to indicate an invalidly defined piece
        GAME_PIECE_TYPE type = NOTYPE;      
        GAME_PIECE_COLOR color = NOCOLOR;
        int moves_made = 0;
        bool is_restricted;

        // Default constructor to allow sending pieces that indicate an invalid piece
        game_piece() {}

        // Simply put - rooks, queens and bishops have freer movemen than the other pieces
        game_piece(const GAME_PIECE_TYPE type_in, const GAME_PIECE_COLOR color_in) : type(type_in), color(color_in) {
            if (type_in == ROOK || type_in == QUEEN || type_in == BISHOP) {
                is_restricted = false;
            } else {
                is_restricted = true;
            }
        }
    };

}

#endif