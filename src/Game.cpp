#include "Game.h"

namespace Chess_API {
    // Default constructor creating an empty game
    Game::Game(const std::shared_ptr<Player> player1_in, const std::shared_ptr<Player> player2_in) {
        // Building out the game board
        game_board = new game_piece ** [DEFAULT_CHESS_BOARD_SIZE];

        // Setting default values for each 'cell' on the chess board
        for (int i = 0; i < DEFAULT_CHESS_BOARD_SIZE; ++i) {
            game_board[i] = new game_piece * [DEFAULT_CHESS_BOARD_SIZE];
            for (int j = 0; j < DEFAULT_CHESS_BOARD_SIZE; ++j) {
                game_board[i][j] = nullptr;
            }
        }

        // Setting up the players
        player1 = player1_in;
        player2 = player2_in;

        current_player = player1;

        if (player1->get_player_color() == player2->get_player_color()) {
            throw std::runtime_error("The player colors must be different");
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

        player1 = copy_source.player1;
        player2 = copy_source.player2;
        current_player = copy_source.current_player;
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

        player1 = other.player1;
        player2 = other.player2;
        current_player = other.current_player;

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

        if (color_in != player1->get_player_color() && color_in != player2->get_player_color()) {
            throw std::runtime_error("The piece colors need to match one of the players");
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
    // Simulate_move is a flag that will do all of the normal functionality with the expectation that the move will be undone
    // Therefore it does not update cached information (en_passant_position / king position)
    void Game::play_move(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos, bool simulate_move) {
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
        if (start_piece_ptr->type == GAME_PIECE_TYPE::PAWN && !simulate_move) {
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





        // TODO: Implement captures for en passant and implement castling move and detection
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

    // Prints the provided character the number of times provided - helper function for show board - assumes the CLI has been set to UTF-16 mode
    void Game::print_wchar_times(const wchar_t wide_char, const int times) const {
        for (int c = 0; c < times; ++c) {
            std::wcout << wide_char;
        }
    }

    // Prints the board line separator - helper function for show board - assumes the CLI has been set to UTF-16 mode
    void Game::print_board_line_separator() const {
        // Printing the line separators - printing space on either side to match the grid fully
        print_wchar_times(CHESS_BOARD_SPACE_CHAR, (SPACING_BETWEEN_PIECES_FOR_DISPLAY * 2) + 1);
        print_wchar_times(CHESS_BOARD_LINE_CHAR, 33);
        print_wchar_times(CHESS_BOARD_SPACE_CHAR, (SPACING_BETWEEN_PIECES_FOR_DISPLAY * 2) + 1);
        std::wcout << std::endl;
    }

    // Prints the board letters - helper function for show board - assumes the CLI has been set to UTF-16 mode
    void Game::print_board_letters() const {
        print_wchar_times(CHESS_BOARD_SPACE_CHAR, (SPACING_BETWEEN_PIECES_FOR_DISPLAY * 2) + 1);
        for (int i = 0; i < VALID_CHARS.size(); ++i) {
            print_wchar_times(CHESS_BOARD_SPACE_CHAR, SPACING_BETWEEN_PIECES_FOR_DISPLAY * 2);

            std::wcout << VALID_CHARS.at(i);

            print_wchar_times(CHESS_BOARD_SPACE_CHAR, SPACING_BETWEEN_PIECES_FOR_DISPLAY);
        }
    }

    // Prints the game board to std::cout in a friendly manner
    void Game::show_board() const {
        // Preps the CLI for unicode output - platform dependent (Windows)
        int previous_mode = _setmode(_fileno(stdout), _O_U16TEXT);
        // First displaying the letters of the board
        print_board_letters();

        std::wcout << std::endl;

        print_board_line_separator();

        // Displaying each line of the board - for each line count down from board size
        for (int i = DEFAULT_CHESS_BOARD_SIZE; i > 0; --i) {
            
            print_wchar_times(CHESS_BOARD_SPACE_CHAR, SPACING_BETWEEN_PIECES_FOR_DISPLAY);
            std::wcout << i;

            print_wchar_times(CHESS_BOARD_SPACE_CHAR, SPACING_BETWEEN_PIECES_FOR_DISPLAY);
            std::wcout << CHESS_BOARD_SEPERATOR_CHAR;

            for (int j = 0; j < DEFAULT_CHESS_BOARD_SIZE; ++j) {
                game_piece piece = get_location(std::make_pair(i - 1, j));

                print_wchar_times(CHESS_BOARD_SPACE_CHAR, SPACING_BETWEEN_PIECES_FOR_DISPLAY);

                // If this is a valid piece then print the pieces symbol
                if (validate_game_piece(piece)) {
                    if (piece.color == GAME_PIECE_COLOR::WHITE) {
                        std::wcout << WHITE_GAME_PIECE_SYMBOLS.at(piece.type);
                    } else {
                        std::wcout << BLACK_GAME_PIECE_SYMBOLS.at(piece.type);
                    }
                // Otherwise print a blank
                } else {
                    std::wcout << CHESS_BOARD_SPACE_CHAR;
                }

                print_wchar_times(CHESS_BOARD_SPACE_CHAR, SPACING_BETWEEN_PIECES_FOR_DISPLAY);
                std::wcout << CHESS_BOARD_SEPERATOR_CHAR;
            }

            print_wchar_times(CHESS_BOARD_SPACE_CHAR, SPACING_BETWEEN_PIECES_FOR_DISPLAY);
            std::wcout << i << std::endl;
            
            print_board_line_separator();
        }

        // Display the letters yet again
        print_board_letters();

        std::wcout << std::endl;

        // Finally resetting the CLI to default mode
        _setmode(_fileno(stdout), previous_mode);
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
        // Pawns may only move in a set x-direction, therefore check to ensure the move aligns with this pawns direction
        if (starting_piece.pawn_move_positive_x) {
            if (std::get<0>(move) < 0) {
                return false;
            }
        } else {
            if (std::get<0>(move) > 0) {
                return false;
            }
        }

        // Determine if the move is within the set
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

            // Finally if there are no pieces in the way then this is a valid castle move
            return true;

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

        // Next validate that the current player has control of the target piece
        if (starting_piece.color != current_player->get_player_color()) {
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

    // Determines if the provided move would place the player in check - only checks the one move and not any moves between it
    bool Game::simulate_move_for_check(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos) {
        // Setup the variables to simulate playing the incremental move
        game_piece start_piece = get_location(start_pos);
        game_piece end_piece = get_location(end_pos);
        play_move(start_pos, end_pos, true);

        bool return_val = is_in_check();

        // Clearing all potential positions where a piece could still be at before re-adding all of the pieces
        remove_piece(end_pos);
        remove_piece(start_pos);

        if (validate_game_piece(start_piece)) {
            add_piece(start_piece.type, start_piece.color, start_pos);
        }
        
        if (validate_game_piece(end_piece)) {
            add_piece(end_piece.type, end_piece.color, end_pos);
        }

        return return_val;
    }

    // Determines if the move described by the start and end positions is a valid move based on chess ruling
    // Notably - this is different from is_legal_move because it ensures the move is not placing the player in check
    // This is the function that should be used to determine if a move is truly valid before playing the move
    bool Game::is_valid_move(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos) {
        if (!is_legal_move(start_pos, end_pos)) {
            return false;
        }

        // For the move to be valid - it must not place the current player in check
        // Simulate the move and then determine if the current player is in check
        game_piece end_piece = get_location(end_pos);
        game_piece start_piece = get_location(start_pos);

        int delta_x = abs(std::get<0>(end_pos) - std::get<0>(start_pos));
        int delta_y = abs(std::get<1>(end_pos) - std::get<1>(start_pos));

        // For moves greater than 1 unit we must simulate each move to ensure that no move along the route places the player in check
        // Knights are the exception - they move straight to their end position
        if ((delta_x > 1 || delta_y > 1) && start_piece.type != GAME_PIECE_TYPE::KNIGHT) {
            std::pair<float, float> move_delta = calculate_piece_delta_move(start_piece, start_pos, end_pos);
            int move_delta_x = round(std::get<0>(move_delta));
            int move_delta_y = round(std::get<1>(move_delta));
            int start_x = std::get<0>(start_pos);
            int start_y = std::get<1>(start_pos);
            
            std::pair<int, int> next_move;
            do {
                start_x += move_delta_x;
                start_y += move_delta_y;
                next_move = std::make_pair(start_x, start_y);
                
                // Simulate the move and determine if it places the player in check
                if (simulate_move_for_check(start_pos, next_move)) {
                    return false;
                }
            } while (next_move != end_pos);

            // Making it out of the do-while loop means that all of the moves are valid and the move can be accomplished
            return true;
        
        // A restricted piece can just make the move - simply ensure that the move won't place the player in check
        } else {
            return !simulate_move_for_check(start_pos, end_pos);
        }
    }

    // Swaps which player is the current player
    void Game::swap_current_player() {
        if (current_player == player1) {
            current_player = player2;
        } else {
            current_player = player1;
        }
    }

    // Determines if the current player is in check
    bool Game::is_in_check() {
        // Get the current players king position
        std::pair<int, int> king_pos = current_player == player1 ? player1_king_position : player2_king_position;

        // Ensure that the king position has been defined
        if (std::get<0>(king_pos) == -1 || std::get<1>(king_pos) == -1) {
            return false;
        }

        game_piece king = get_location(king_pos);

        // Check every piece around the king - if they are the same color then the king is safe from all pieces except knights
        std::vector<std::pair<int, int>> area_around_king = {std::make_pair(1, 0), std::make_pair(-1, 0), std::make_pair(0, 1), std::make_pair(0, -1),
                                                             std::make_pair(1, 1), std::make_pair(-1, 1), std::make_pair(1, -1), std::make_pair(-1, -1)};
        game_piece piece;

        for(int i = 0; i < area_around_king.size(); ++i) {
            std::pair<int, int> piece_delta = area_around_king[i];
            int piece_x = std::get<0>(piece_delta) + std::get<0>(king_pos);
            int piece_y = std::get<1>(piece_delta) + std::get<1>(king_pos);

            try {
                piece = get_location(std::make_pair(piece_x, piece_y));

                // If this piece is a different color and a valid game piece then check if that piece can make a legal move to the king
                if (piece.color != king.color && validate_game_piece(piece)) {
                    // Temporarily swap the current player to see if the enemy piece can make the legal move
                    swap_current_player();

                    // If this move is legal then the current player is in check
                    if(is_legal_move(std::make_pair(piece_x, piece_y), king_pos)) {
                        swap_current_player();
                        return true;
                    // Otherwise move on to the next piece
                    } else {
                        swap_current_player();
                    }
                // In the case where the square is empty then continue expanding out in that direction until we hit the edge of the board or a valid piece
                } else if (!validate_game_piece(piece)) {
                    // Loop to scale up the direction and get the next piece
                    for(int j = 2; j < DEFAULT_CHESS_BOARD_SIZE; ++j) {
                        int piece_x = (std::get<0>(piece_delta) * j) + std::get<0>(king_pos);
                        int piece_y = (std::get<1>(piece_delta) * j) + std::get<1>(king_pos);

                        // If we hit a wall then this will throw an error that we catch below
                        piece = get_location(std::make_pair(piece_x, piece_y));

                        // If we find a valid piece check if its the same color first
                        if (validate_game_piece(piece)) {
                            // This direction is safe, move on to the next one
                            if (piece.color == king.color) {
                                break;
                            // Otherwise determine if the piece has a legal move to the king
                            } else {
                                // Temporarily swap the current player to see if the enemy piece can make the legal move
                                swap_current_player();

                                // If this move is legal then the current player is in check
                                if(is_legal_move(std::make_pair(piece_x, piece_y), king_pos)) {
                                    swap_current_player();
                                    return true;
                                // Otherwise this direction is safe, move on to the next direction
                                } else {
                                    swap_current_player();
                                    break;
                                }
                            }
                        }
                    }
                }

            // Expected error if this exceeds the bounds - this position doesn't matter since the king is next to a wall
            } catch (std::runtime_error) {
                continue;
            }

        }

        // Check for a knight of a different color in every knight moveset away from the king to ensure there isn't any knights there
        std::vector<std::pair<int, int>> knight_moveset = PIECE_MOVESETS.at(GAME_PIECE_TYPE::KNIGHT);

        for (int i = 0; i < knight_moveset.size(); ++i) {
            std::pair<int, int> piece_delta = knight_moveset[i];
            int piece_x = std::get<0>(piece_delta) + std::get<0>(king_pos);
            int piece_y = std::get<1>(piece_delta) + std::get<1>(king_pos);

            try {
                piece = get_location(std::make_pair(piece_x, piece_y));
                
                // If the piece is a different colored knight then the current player is in check
                if (piece.color != king.color && validate_game_piece(piece) && piece.type == GAME_PIECE_TYPE::KNIGHT) {
                    return true;
                }

            // Expected error if we hit a wall
            } catch (std::runtime_error) {
                continue;
            }

        }

        // Finally if all of the above did not return anything then the current player is not in check
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

