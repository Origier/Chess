#include "Game.h"

namespace Chess_API {
    // Default constructor creating an empty game
    Game::Game() {
        // Building out the game board
        game_board = new game_piece ** [DEFAULT_CHESS_BOARD_SIZE];

        // Setting default values for each 'cell' on the chess board
        for (int i = 0; i < DEFAULT_CHESS_BOARD_SIZE; ++i) {
            game_board[i] = new game_piece * [DEFAULT_CHESS_BOARD_SIZE];
            for (int j = 0; j < DEFAULT_CHESS_BOARD_SIZE; ++j) {
                game_board[i][j] = nullptr;
            }
        }
    }

    // Copy constructor - creates a copied version of the board from the copy_source
    Game::Game(const Game& copy_source) {
        // Building out the game board
        game_board = new game_piece ** [DEFAULT_CHESS_BOARD_SIZE];

        // Setting default values for each 'cell' on the chess board
        for (int i = 0; i < DEFAULT_CHESS_BOARD_SIZE; ++i) {
            game_board[i] = new game_piece * [DEFAULT_CHESS_BOARD_SIZE];

            for (int j = 0; j < DEFAULT_CHESS_BOARD_SIZE; ++j) {
                // Adding the game pieces that exist on the copy source
                if (copy_source.game_board[i][j] != nullptr) {
                    game_board[i][j] = new game_piece(copy_source.game_board[i][j]->type, copy_source.game_board[i][j]->color);
                } else {
                    game_board[i][j] = nullptr;
                }   
            }
        }
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

    // Assignment operator - copies the current games data rather then acting as a reference
    Game& Game::operator=(const Game& other) {
        // Building out the game board
        game_board = new game_piece ** [DEFAULT_CHESS_BOARD_SIZE];

        // Setting default values for each 'cell' on the chess board
        for (int i = 0; i < DEFAULT_CHESS_BOARD_SIZE; ++i) {
            game_board[i] = new game_piece * [DEFAULT_CHESS_BOARD_SIZE];

            for (int j = 0; j < DEFAULT_CHESS_BOARD_SIZE; ++j) {
                // Adding the game pieces that exist on the copy source
                if (other.game_board[i][j] != nullptr) {
                    game_board[i][j] = new game_piece(other.game_board[i][j]->type, other.game_board[i][j]->color);
                } else {
                    game_board[i][j] = nullptr;
                }   
            }
        }

        return *this;
    }

    // Adds the given piece type to the game board at the provided location
    // Throws an error if attempting to place the piece outside the bounds
    void Game::add_piece(const GAME_PIECE_TYPE type_in, const GAME_PIECE_COLOR color_in, const std::pair<int, int>& location) {
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
    // Simply returns an invalid piece if there isn't anything there
    game_piece Game::get_location(const std::pair<int, int>& location) const {
        int x = std::get<0>(location);
        int y = std::get<1>(location);

        if (x >= DEFAULT_CHESS_BOARD_SIZE || y >= DEFAULT_CHESS_BOARD_SIZE || x < 0 || y < 0) {
            throw std::runtime_error("You cannot select outside the bounds of the board");
        }

        // Returning a copy of the game_piece for memory safety
        if (game_board[x][y] == nullptr) {
            return game_piece();
        }

        return *game_board[x][y];
    }

    // Removes any pieces on the provided location - if there isn't a piece there then it does nothing
    void Game::remove_piece(const std::pair<int, int>& location) {
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
    void Game::play_move(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos) {
        // TODO - Implement
    }

    // Returns the current state of the game
    Game::GAME_STATE Game::get_current_game_state() const {
        return current_game_state;
    }

    // Prints the game board to std::cout in a friendly manner
    void Game::show_board() const {
        // TODO - Implement
    }

    // Determines if the provided starting and ending position are valid moves based on Chess ruling
    bool Game::is_valid_move(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos) const {
        // For a move to be valid there must first be a piece at the start_pos and there must not be a piece at the end_pos of the same color
        game_piece starting_piece = get_location(start_pos);
        game_piece ending_piece = get_location(end_pos);

        // Validate that there exists a piece at start_pos
        if (starting_piece.color == GAME_PIECE_COLOR::NOCOLOR || starting_piece.type == GAME_PIECE_TYPE::NOTYPE) {
            return false;
        }

        // Validate that there is NO piece at end_pos or that the piece at the end_pos is a different color
        if (ending_piece.color != GAME_PIECE_COLOR::NOCOLOR && ending_piece.type != GAME_PIECE_TYPE::NOTYPE) {
            if (ending_piece.color == starting_piece.color) {
                return false;
            }
        }

        // Next - get the moveset for the specific piece and determine if the given move is within the pieces moveset
        std::vector<std::pair<int, int>> moveset = PIECE_MOVESETS.at(starting_piece.type);

        int delta_x = std::get<0>(start_pos) - std::get<0>(end_pos);
        int delta_y = std::get<1>(start_pos) - std::get<1>(end_pos);

        // For unrestricted pieces - we must first calculate the rise over run to 'normalize' the movement
        if (!starting_piece.is_restricted) {
            // Ensuring no division by 0
            if (delta_y == 0) {
                // If no change in y then delta_x is just divided by its own absolute value
                delta_x = delta_x / abs(delta_x);
            } else {
                float rise_over_run = (static_cast<float>(delta_x) / static_cast<float>(delta_y));
                // We know that rise_over_run should be a whole number for any of the unrestricted pieces - if it is not then return false
                int big_num = rise_over_run * 1000;
                big_num = big_num / 1000;
                
                delta_x = ceil(rise_over_run);
                
                if (delta_x != big_num) {
                    return false;
                } else {
                    delta_y = 1;
                }
            }
        }

        // Finally - determine if the movement is within the moveset
        std::pair<int, int> delta_pair = std::make_pair(delta_x, delta_y);
        auto move_found = std::find(moveset.cbegin(), moveset.cend(), delta_pair);
        if (move_found != moveset.cend()) {
            return true;
        } else {
            return false;
        }


        // TODO: Consider special moves as well such as en passant, castling, pawn double move and pawn angular capture
    }
}