#include "Game.h"

namespace Chess_API {

    // Determines if the provided starting and ending position are valid moves based on Chess ruling
    bool Game::is_valid_move(std::pair<int, int> start_pos, std::pair<int, int> end_pos) const {
        // TODO - Implement
        return true;
    }

    // Default constructor creating an empty game
    Game::Game() {
        // TODO - Implement
    }

    // Copy constructor
    Game::Game(const Game& copy) {
        // TODO - Implement
    }

    // Constructor that accepts a board object to build that game based on that
    Game::Game(Board board_in) {
        // TODO - Implement
    }

    // Plays the given move placing the game piece from start_pos to end_pos
    void Game::play_move(std::pair<int, int> start_pos, std::pair<int, int> end_pos) {
        // TODO - Implement
    }

    Game::GAME_STATE Game::get_current_game_state() const {
        return current_game_state;
    }

    // Shows the current state of the game board
    void Game::show_board() const {
        // TODO - Implement
    }
}