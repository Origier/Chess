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

        if (!validate_position(location)) {
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

        if (!validate_position(location)) {
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
        if (!validate_position(location)) {
            throw std::runtime_error("Cannot remove pieces outside the bounds of the board");
        }

        // As long as there is a piece on this position then remove it and ensure that this position is set back to a nullptr
        if (game_board[x][y] != nullptr) {
            delete game_board[x][y];
            game_board[x][y] = nullptr;
        }
    }

    // Plays the given move placing the game piece from start_pos to end_pos
    // Assumes that the move has been validated by is_valid_move - this function only validates that the move is within the bounds of the board
    void Game::play_move(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos) {
        int start_x = std::get<0>(start_pos);
        int start_y = std::get<1>(start_pos);
        int end_x = std::get<0>(end_pos);
        int end_y = std::get<1>(end_pos);

        // Validate all of the positions to avoid access violations
        if (!validate_position(start_pos)) {
            throw std::runtime_error("You may not move a piece outside the bounds of the board");
        }

        if (!validate_position(end_pos)) {
            throw std::runtime_error("You may not move a piece outside the bounds of the board");
        }

        game_piece * start_piece_ptr = game_board[start_x][start_y];
        game_piece * end_piece_ptr = game_board[end_x][end_y];

        // If the end spot is an empty location - simply transfer ownership of the memory from one position to the other
        // Otherwise first deallocate the memory for the end location
        if (end_piece_ptr != nullptr) {
            delete game_board[end_x][end_y];
        }

        game_board[end_x][end_y] = start_piece_ptr;
        game_board[start_x][start_y] = nullptr;

        // Considerations for pawns - if a pawn moved 2 vertically, then the position behind it becomes a en passant valid position
        if (start_piece_ptr->type == GAME_PIECE_TYPE::PAWN) {
            int delta_x = end_x - start_x;

            // Setting the en passant position accordingly - one space behind where the pawn went
            if (abs(delta_x) == 2) {
                int en_passant_x_negation = delta_x < 0 ? 1 : -1;
                en_passant_position = std::make_pair(start_x - en_passant_x_negation, start_y);
            } else {
                en_passant_position = std::make_pair(-1, -1);
            }

        // Otherwise the en passant position become invalid
        } else {
            en_passant_position = std::make_pair(-1, -1);
        }
    }

    // Updates the internal game state based on chess ruling
    // Essentially determines if the game is in stalemate / check / checkmate / or normal play
    // Intented to be used after every call of play_move - must be manually called
    void Game::update_game_state() {
        // Run through each function to check which state the game is in - check first since checkmate implies check
        // Stalemate last because stalemate is separate from both
        if (is_in_check()) {
            current_game_state = GAME_STATE::CHECK;
        }

        if (is_in_checkmate()) {
            current_game_state = GAME_STATE::CHECKMATE;
        }

        if (is_in_stalemate()) {
            current_game_state = GAME_STATE::STALEMATE;
        }

    }

    // Returns the current state of the game
    Game::GAME_STATE Game::get_current_game_state() const {
        return current_game_state;
    }

    // Prints the game board to std::cout in a friendly manner
    void Game::show_board() const {
        // TODO - Implement
    }

    // Validates that the position is a valid position on the board
    bool Game::validate_position(const std::pair<int, int>& position) const {
        int x = std::get<0>(position);
        int y = std::get<1>(position);

        // If attempting to remove outside the bounds then simply do nothing
        if (x >= DEFAULT_CHESS_BOARD_SIZE || y >= DEFAULT_CHESS_BOARD_SIZE || x < 0 || y < 0) {
            return false;
        } else {
            return true;
        }
    }

    // Validates if the provided game piece is a valid piece or not
    bool Game::validate_game_piece(const game_piece& piece) const {
        if (piece.color == GAME_PIECE_COLOR::NOCOLOR || piece.type == GAME_PIECE_TYPE::NOTYPE) {
            return false;
        } else {
            return true;
        }
    }

    // Validates if the given piece can move described by move - for restricted pieces
    bool Game::validate_piece_move_restricted(const GAME_PIECE_TYPE type, const std::pair<int, int>& move) const {
        std::vector<std::pair<int, int>> moveset = PIECE_MOVESETS.at(type);

        auto move_found = std::find(moveset.cbegin(), moveset.cend(), move);

        return move_found != moveset.cend();
    }

    // Validates if the given piece can move described by move - for unrestricted pieces (ensures the path is clear for the piece)
    bool Game::validate_piece_move_unrestricted(const GAME_PIECE_TYPE type, const std::pair<int, int>& move, const std::pair<int, int>& start_pos, const std::pair<int, int> end_pos) const {
        // First ensure the move is within the moveset
        if (!validate_piece_move_restricted(type, move)) {
            return false;
        }

        // Next - map out the path from the start pos to the end pos ensuring there are no pieces in the way
        int x_start = std::get<0>(start_pos) + std::get<0>(move);
        int y_start = std::get<1>(start_pos) + std::get<1>(move);

        int x_end = std::get<0>(end_pos);
        int y_end = std::get<1>(end_pos);

        // If there are any valid pieces along the path then return false
        while (x_start != x_end && y_start != y_end) {
            game_piece piece = get_location(std::make_pair(x_start, y_start));

            if (validate_game_piece(piece)) {
                return false;
            }

            x_start += std::get<0>(move);
            y_start += std::get<1>(move);
        }
        
        // If we make it here then the path should be clear
        return true;
    }

    // Determines is baseline delta move for the piece, finding the root move direction for unrestricted pieces
    std::pair<float, float> Game::calculate_piece_delta_move(const game_piece& piece, const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos) const {
        // Calculate the overall move
        int delta_x = std::get<0>(end_pos) - std::get<0>(start_pos);
        int delta_y = std::get<1>(end_pos) - std::get<1>(start_pos);
        float base_delta_x = 0;
        float base_delta_y = 0;

        // For unrestricted pieces - we must first calculate the rise over run to 'normalize' the movement
        if (!piece.is_restricted) {
            // Ensuring no division by 0
            if (delta_y == 0) {
                // If no change in y then delta_x is just divided by its own absolute value
                base_delta_x = delta_x / abs(delta_x);
            } else if (delta_x == 0) {
                // If no change in x then delta_y is just divided by its own absolute value
                base_delta_y = delta_y / abs(delta_y);
            } else {
                // Determine the smallests delta value as the value to divide both by to get a base value
                if (delta_y < delta_x) {
                    base_delta_x = static_cast<float>(delta_x) / static_cast<float>(abs(delta_y));
                    base_delta_y = static_cast<float>(delta_y) / static_cast<float>(abs(delta_y));
                } else {
                    base_delta_x = static_cast<float>(delta_x) / static_cast<float>(abs(delta_x));
                    base_delta_y = static_cast<float>(delta_y) / static_cast<float>(abs(delta_x));
                }
            }
        }

        std::pair<float, float> delta_pair = std::make_pair(base_delta_x, base_delta_y);

        return delta_pair;
    } 

    // Validates that the move is an acceptable move for a pawn given the current state of the board - no consideration for checks
    bool Game::validate_pawn_move(const game_piece& starting_piece, const game_piece& ending_piece, const std::pair<int, int>& move, const std::pair<int, int>& end_pos) const {
        std::vector<std::pair<int, int>> moveset = PIECE_MOVESETS.at(GAME_PIECE_TYPE::PAWN);
        // Pawns may only move in a set x-direction, therefore change the moveset for the pawn accordingly
        if (!starting_piece.pawn_move_positive_x) {
            // Flip all of the x-type moves to be negative before determining if the move is valid
            for (int i = 0; i < moveset.size(); ++i) {
                moveset[i] = std::make_pair(std::get<0>(moveset[i]) * -1, std::get<1>(moveset[i]));
            }
        }

        // Based on any changes to moveset - now determine if the move is within the set
        auto move_found = std::find(moveset.cbegin(), moveset.cend(), move);
        if (move_found == moveset.cend()) {
            return false;
        }

        // Now that the move is validated - we must further consider if the move is the double move
        if (abs(std::get<0>(move)) == 2) {
            // As long as this is the pawns first move then this is valid
            if (starting_piece.moves_made == 0) {
                return true;
            } else {
                return false;
            }
        }

        // Further consider if this is an en passant move or a diagonal capture
        if (abs(std::get<0>(move)) / abs(std::get<1>(move)) == 1) {
            // First consider if this is a normal capture - the capture piece must be of a different color but not an invalid color
            if (ending_piece.color != starting_piece.color && ending_piece.color != GAME_PIECE_COLOR::NOCOLOR) {
                return true;
            
            // Otherwise consider if this is an en passant move
            } else {
                // Compares the end position to the current valid en passant position that is determined every call of play_move
                if (end_pos == en_passant_position) {
                    return true;
                } else {
                    return false;
                }
            }

        // If it is not a diagonal move then it must be an empty spot otherwise it is an invalid move
        } else {
            if (ending_piece.color != GAME_PIECE_COLOR::NOCOLOR || ending_piece.type != GAME_PIECE_TYPE::NOTYPE) {
                return false;
            } else {
                return true;
            }
        }
    }

    // Validates that the move is an acceptable move for a king given the current state of the board - no consideration for checks
    bool Game::validate_king_move(const game_piece& starting_piece, const std::pair<int, int> move, const std::pair<int, int>& start_pos) const {
        // First determine if the move is valid in the set
        if (!validate_piece_move_restricted(GAME_PIECE_TYPE::KING, move)) {
            return false;
        }

        // Determine if the move is attempting to castle
        if (abs(std::get<1>(move)) == 2) {
            // Determine if the king has moved
            if (starting_piece.moves_made != 0) {
                return false;
            }

            // Select the respective rook to ensure it also hasn't moved
            game_piece castle_rook;
            if (std::get<1>(move) < 0) {
                castle_rook = get_location(std::make_pair(std::get<0>(move), 0));
            } else {
                castle_rook = get_location(std::make_pair(std::get<0>(move), 7));
            }

            if (castle_rook.moves_made != 0) {
                return false;
            }

            // Ensure that the game state is not currently in check
            if (current_game_state == GAME_STATE::CHECK) {
                return false;
            }

            // Ensure the path between the rook and king is clear
            int x = std::get<0>(start_pos);
            int y = std::get<1>(start_pos);
            int end_y = std::get<1>(move) < 0 ? 0 : 7;
            if (end_y > y) {
                ++y;
            } else {
                --y;
            }
            while (y != end_y) {
                game_piece piece = get_location(std::make_pair(x, y));

                // At any point if there is a valid piece - return false
                if (validate_game_piece(piece)) {
                    return false;
                }
                
                if (end_y > y) {
                    ++y;
                } else {
                    --y;
                }
            }
        // If it isn't a castle attempt then the move is valid
        } else {
            return true;
        }
    }

    // Determines if the move described by the start and end positions is a legal move based on chess ruling
    // Notably - this only determines if this move is considered by chess ruling for the pieces
    // However, this does not take into consideration if this is a valid move given the board state
    // No checks are made to determine if the game will be in check based on this function
    bool Game::is_legal_move(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos) const {
        // First - validate that the moves are within the bounds of the board
        if (!validate_position(start_pos)) {
            return false;
        }

        if (!validate_position(end_pos)) {
            return false;
        }

        // For a move to be valid there must first be a piece at the start_pos and there must not be a piece at the end_pos of the same color
        game_piece starting_piece = get_location(start_pos);
        game_piece ending_piece = get_location(end_pos);

        // Validate that there exists a piece at start_pos
        if (!validate_game_piece(starting_piece)) {
            return false;
        }

        // Validate that the ending piece is either empty or a valid piece of a different color
        if (validate_game_piece(ending_piece) && ending_piece.color == starting_piece.color) {
            return false;
        } 

        // Determine the baseline movement - unrestricted pieces will have the movement become base 1
        std::pair<float, float> delta_pair_float = calculate_piece_delta_move(starting_piece, start_pos, end_pos);

        // Ensuring the delta pair are whole numbers
        if (ceil(std::get<0>(delta_pair_float)) != floor(std::get<0>(delta_pair_float)) || ceil(std::get<1>(delta_pair_float)) != floor(std::get<1>(delta_pair_float))) {
            return false;
        }

        std::pair<int, int> delta_pair = std::make_pair(std::get<0>(delta_pair_float), std::get<1>(delta_pair_float));
        
        // Determine if the delta_pair is a valid move for each piece given the game board
        // Pawns have special ruling on how they move
        if (starting_piece.type == GAME_PIECE_TYPE::PAWN) {
            return validate_pawn_move(starting_piece, ending_piece, delta_pair, end_pos);

        // Special consideration should also be taken for kings for castling
        } else if (starting_piece.type == GAME_PIECE_TYPE::KING) {
            return validate_king_move(starting_piece, delta_pair, start_pos);

        // For unrestricted pieces - there must be a clear path to the end_pos
        } else if (!starting_piece.is_restricted) {
            return validate_piece_move_unrestricted(starting_piece.type, delta_pair, start_pos, end_pos);
        
        // Lastly for unrestricted pieces its a simple look-up
        } else {
            return validate_piece_move_restricted(starting_piece.type, delta_pair);
        }
    }

    // Determines if the move described by the start and end positions is a valid move based on chess ruling
    // Notably - this is different from is_legal_move because it ensures the move is not placing the player in check
    // This is the function that should be used to determine if a move is truly valid
    bool Game::is_valid_move(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos) const {
        if (!is_legal_move(start_pos, end_pos)) {
            return false;
        }

        // TODO - Implement check for if this move places the player in check
        return true;
    }

    // Determines if the game is currently in check
    bool Game::is_in_check() {
        // TODO - Implement
        return false;
    }

    // Determines if the game is currently in checkmate
    bool Game::is_in_checkmate() {
        // TODO - Implement
        return false;
    }

    // Determines if the game is currently in stalemate
    bool Game::is_in_stalemate() {
        // TODO - Implement
        return false;
    }
}

