#ifndef CPLUSPLUS_CHESS_API_VARS
#define CPLUSPLUS_CHESS_API_VARS

#include <string>
#include <tuple>
#include <unordered_map>
#include <map>

namespace Chess_API {
    const std::string VALID_CHARS = "abcdefgh";                                                             // Valid characters referring to the places on the chess board
    const std::string VALID_NUMS = "12345678";                                                              // Valid numbers referring to the places on the chess board
    const std::string DEFAULT_COMPUTER_NAME = "Computer";                                                   // Default value for the computer player
    const std::string DEFAULT_PLAYER_NAME = "Default";                                                      // Default name for the player class
    const std::string DEFAULT_HUMAN_NAME = "Human";                                                         // Default name for the human player
    const short DEFAULT_CHESS_BOARD_SIZE = 8;                                                               // Default value for the chess' board size
    const short SPACING_BETWEEN_PIECES_FOR_DISPLAY = 1;                                                     // Default amount of spacing to separate each board element when displaying the board in the CLI
    const wchar_t CHESS_BOARD_SEPERATOR_CHAR = '|';                                                         // Default seperator between each element on the boards
    const wchar_t CHESS_BOARD_SPACE_CHAR = ' ';                                                             // Default char denoting spaces on the chess board
    const wchar_t CHESS_BOARD_LINE_CHAR = *L"\u2500";                                                       // Default char to separate lines on the chess board

    // Difficulties for the computer players
    enum DIFFICULTY {
        VERY_EASY,
        EASY,
        MEDIUM,
        HARD,
        HARDEST,
        EXPERT
    };

    const DIFFICULTY DEFAULT_COMPUTER_DIFFICULTY = DIFFICULTY::MEDIUM;       

    // Error message for typing in the wrong input in the game
    const std::string INVALID_INPUT_ERROR_MSG = "That isn't valid input, type your move in {{char}{num} {char}{num}} format using \"" + VALID_CHARS + "\" as the valid characters and \"" + VALID_NUMS + "\" as the valid numbers";

    // Error messages for each issue that may arise throughout the chess game
    const std::string BLOCKED_MOVE_ERROR_MSG = "There is a piece blocking your path!";
    const std::string ILLEGAL_MOVE_ERROR_MSG = "That piece cannot perform that move!";
    const std::string CHECK_MOVE_ERROR_MSG = "That move would place you in check!";
    const std::string NO_PIECE_MOVE_ERROR_MSG = "There is no piece at your starting position!";
    const std::string OUT_OF_BOUNDS_MOVE_ERROR_MSG = "That move would take you out of bounds!";
    const std::string WRONG_COLOR_MOVE_ERROR_MSG = "That isn't your piece!";

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

        int moves_made = 0;         // The number of moves the piece has made - useful for special moves
        bool is_restricted;         // Determines if the piece is limited to just the predefined movesets
        bool pawn_move_positive_x;  // If the piece is a pawn - determines which x-direction the piece is allowed to move in (positive / negative)

        // Default constructor to allow sending pieces that indicate an invalid piece
        game_piece() {}

        // Simply put - rooks, queens and bishops have freer movemen than the other pieces
        game_piece(const GAME_PIECE_TYPE type_in, const GAME_PIECE_COLOR color_in) : type(type_in), color(color_in) {
            if (type_in == ROOK || type_in == QUEEN || type_in == BISHOP) {
                is_restricted = false;
            } else {
                is_restricted = true;
            }
            // Default value to avoid errors in case this is used
            if (type_in == PAWN && color_in == WHITE) {
                pawn_move_positive_x = true;
            } else {
                pawn_move_positive_x = false;
            }
        }
    };

    // Symbols for each game piece when printing to the console
    const std::unordered_map<GAME_PIECE_TYPE, wchar_t> GAME_PIECE_SYMBOLS = {
        {PAWN, *L"\u2659"},
        {ROOK, *L"\u2656"},
        {KNIGHT, *L"\u2658"},
        {BISHOP, *L"\u2657"},
        {KING, *L"\u2654"},
        {QUEEN, *L"\u2655"}
    };

    // Default positions for each piece in a game of chess
    const std::map<GAME_PIECE_TYPE, std::vector<std::pair<int, int>>> WHITE_DEFAULT_GAME_PIECE_POS = {
        {PAWN, {std::make_pair(1, 0), std::make_pair(1, 1), std::make_pair(1, 2), std::make_pair(1, 3), std::make_pair(1, 4), std::make_pair(1, 5), std::make_pair(1, 6), std::make_pair(1, 7)}},
        {ROOK, {std::make_pair(0, 0), std::make_pair(0, 7)}},
        {KNIGHT, {std::make_pair(0, 1), std::make_pair(0, 6)}},
        {BISHOP, {std::make_pair(0, 2), std::make_pair(0, 5)}},
        {QUEEN, {std::make_pair(0, 3)}},
        {KING, {std::make_pair(0, 4)}}
    };

    const std::map<GAME_PIECE_TYPE, std::vector<std::pair<int, int>>> BLACK_DEFAULT_GAME_PIECE_POS = {
        {PAWN, {std::make_pair(6, 0), std::make_pair(6, 1), std::make_pair(6, 2), std::make_pair(6, 3), std::make_pair(6, 4), std::make_pair(6, 5), std::make_pair(6, 6), std::make_pair(6, 7)}},
        {ROOK, {std::make_pair(7, 0), std::make_pair(7, 7)}},
        {KNIGHT, {std::make_pair(7, 1), std::make_pair(7, 6)}},
        {BISHOP, {std::make_pair(7, 2), std::make_pair(7, 5)}},
        {QUEEN, {std::make_pair(7, 3)}},
        {KING, {std::make_pair(7, 4)}}
    };

}

#endif