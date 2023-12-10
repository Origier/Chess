#ifndef CPLUSPLUS_CHESS_GAME
#define CPLUSPLUS_CHESS_GAME

#include "Board.h"
#include <tuple>  // std::pair

namespace Chess_API {
    class Game {
    public:
        // States that the game could be in - used to determine where the game is currently at based on Chess ruling
        enum GAME_STATE {
            NORMAL,
            CHECK,
            STALEMATE,
            CHECKMATE
        };

        // Default constructor creating an empty game
        Game();

        // Copy constructor
        Game(const Game& copy);

        // Constructor that accepts a board object to build that game based on that
        Game(Board board_in);

        // Plays the given move placing the game piece from start_pos to end_pos
        void play_move(std::pair<int, int> start_pos, std::pair<int, int> end_pos);

        // Returns the current state of the game
        GAME_STATE get_current_game_state() const;

        // Shows the current state of the game board
        void show_board() const;

        // Determines if the provided starting and ending position are valid moves based on Chess ruling
        bool is_valid_move(std::pair<int, int> start_pos, std::pair<int, int> end_pos) const;

    private:
        Board game_board;   // Contains the game board which includes the abstraction of the pieces underneath

        GAME_STATE current_game_state = NORMAL;
   
    };
    
}

#endif