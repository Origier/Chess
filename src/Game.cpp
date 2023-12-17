#include "Game.h"

namespace Chess_API {

    // Determines if the provided starting and ending position are valid moves based on Chess ruling
    bool Game::is_valid_move(std::pair<int, int> start_pos, std::pair<int, int> end_pos) const {
        // TODO - Implement
        return true;
    }

    // Default constructor creating an empty game
    Game::Game() {
        // Building out the game board
        game_board = new game_piece ** [DEFAULT_CHESS_BOARD_SIZE];

        // Setting default values for each 'cell' on the chess board
        for (int i = 0; i < DEFAULT_CHESS_BOARD_SIZE; ++i) {
            game_board[i] = new game_piece * [8];
            for (int j = 0; j < DEFAULT_CHESS_BOARD_SIZE; ++j) {
                game_board[i][j] = nullptr;
            }
        }
    }

    // Copy constructor
    Game::Game(const Game& copy) {
        // TODO - Implement
    }

    // Destructor for removing all of the board allocated memory
    Game::~Game() {
        // Setting default values for each 'cell' on the chess board
        for (int i = 0; i < DEFAULT_CHESS_BOARD_SIZE; ++i) {
            for (int j = 0; j < DEFAULT_CHESS_BOARD_SIZE; ++j) {
                // Deallocating any pieces that remain in memory
                if (game_board[i][j] != nullptr) {
                    remove_piece(std::make_pair(i, j));
                }
            }

            // Deallocating this row of memory
            delete[] game_board[i];
        }

        // Deleting the remainder of the game board
        delete[] game_board;
    }

    // Adds the given piece type to the game board at the provided location
    // Throws an error if attempting to place the piece outside the bounds
    void Game::add_piece(GAME_PIECE_TYPE type_in, GAME_PIECE_COLOR color_in, std::pair<int, int> location) {
        int x = std::get<0>(location);
        int y = std::get<1>(location);

        if (x >= DEFAULT_CHESS_BOARD_SIZE || y >= DEFAULT_CHESS_BOARD_SIZE || x < 0 || y < 0) {
            throw std::runtime_error("You cannot place a piece outside the bounds of the board");
        }

        if (game_board[x][y] != nullptr) {
            throw std::runtime_error("There is already a piece on that spot of the board");
        }

        game_board[x][y] = new game_piece(type_in, color_in);
    }

    // Returns the game_piece pointer for the provided location
    // Throws an error if attempting to pull a location beyond the scope of the board
    // Simply returns a nullptr if there isn't anything there
    Game::game_piece Game::get_location(std::pair<int, int> location) {
        int x = std::get<0>(location);
        int y = std::get<1>(location);

        if (x >= DEFAULT_CHESS_BOARD_SIZE || y >= DEFAULT_CHESS_BOARD_SIZE || x < 0 || y < 0) {
            throw std::runtime_error("You cannot select outside the bounds of the board");
        }

        // Returning a copy of the game_piece for memory safety
        if (game_board[x][y] == nullptr) {
            return Game::game_piece();
        }

        return *game_board[x][y];
    }

    // Removes any pieces on the provided location - if there isn't a piece there then it does nothing
    void Game::remove_piece(std::pair<int, int> location) {
        int x = std::get<0>(location);
        int y = std::get<1>(location);

        // If attempting to remove outside the bounds then simply do nothing
        if (x >= DEFAULT_CHESS_BOARD_SIZE || y >= DEFAULT_CHESS_BOARD_SIZE || x < 0 || y < 0) {
            throw std::runtime_error("Cannot remove pieces outside the bounds of the board");
        }

        // As long as there is a piece on this position then remove it and ensure that this position is set back to a nullptr
        if (game_board[x][y] != nullptr) {
            delete game_board[x][y];
            game_board[x][y] = nullptr;
        }
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