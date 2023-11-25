#ifndef CPLUSPLUS_CHESS_GAME
#define CPLUSPLUS_CHESS_GAME

#include "Board.h"

namespace Chess_API {
    class Game {
    private:
        Board game_board;   // Contains the game board which includes the abstraction of the pieces underneath

        // Determines if the provided starting and ending position are valid moves based on Chess ruling
        bool is_valid_move(std::pair<int, int> start_pos, std::pair<int, int> end_pos);
    public:
        // Default constructor creating an empty game
        Game();

        // Constructor that accepts a board object to build that game based on that
        Game(Board board_in);

        // Plays the given move placing the game piece from start_pos to end_pos
        bool play_move(std::pair<int, int> start_pos, std::pair<int, int> end_pos);

        // Shows the current state of the game board
        void show_board();
    };
}

#endif