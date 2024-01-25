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
        int x = location.first;
        int y = location.second;

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

    // Sets up the game with the default chess board state
    void Game::setup_default_board_state() {
        
        // Placing white pieces
        for (auto white_it = WHITE_DEFAULT_GAME_PIECE_POS.begin(); white_it != WHITE_DEFAULT_GAME_PIECE_POS.end(); ++white_it) {
            std::vector<std::pair<int, int>> piece_locations = white_it->second;
            for (int i = 0; i < piece_locations.size(); ++i) {
                add_piece(white_it->first, GAME_PIECE_COLOR::WHITE, piece_locations.at(i));
            }
        }
        
        // Placing black pieces
        for (auto black_it = BLACK_DEFAULT_GAME_PIECE_POS.begin(); black_it != BLACK_DEFAULT_GAME_PIECE_POS.end(); ++black_it) {
            std::vector<std::pair<int, int>> piece_locations = black_it->second;
            for (int i = 0; i < piece_locations.size(); ++i) {
                add_piece(black_it->first, GAME_PIECE_COLOR::BLACK, piece_locations.at(i));
            }
        }

        // Setting up players king positions
        player1_king_position = WHITE_DEFAULT_GAME_PIECE_POS.at(KING).at(0);
        player2_king_position = BLACK_DEFAULT_GAME_PIECE_POS.at(KING).at(0);
    }

    // Returns the game_piece pointer for the provided location
    // Throws an error if attempting to pull a location beyond the scope of the board
    // Simply returns an invalid piece if there isn't anything there
    game_piece Game::get_location(const std::pair<int, int>& location) const {
        int x = location.first;
        int y = location.second;

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
        int x = location.first;
        int y = location.second;

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
    // Warning - ensure the move has been validated by is_valid_move - this can have memory violations otherwise
    // Simulate_move is a flag that will do all of the normal functionality with the expectation that the move will be undone
    // Therefore it does not update cached information (en_passant_position / king position)
    // Returns the game piece and location of the game piece captured - returns an invalid game piece if no piece was captured
    std::pair<game_piece, std::pair<int, int>> Game::play_move(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos, bool simulate_move) {
        int start_x = start_pos.first;
        int start_y = start_pos.second;
        int end_x = end_pos.first;
        int end_y = end_pos.second;

        // Validate all of the positions to avoid access violations
        if (!validate_position(start_pos)) {
            throw std::runtime_error("You may not move a piece outside the bounds of the board");
        }

        if (!validate_position(end_pos)) {
            throw std::runtime_error("You may not move a piece outside the bounds of the board");
        }

        game_piece * start_piece_ptr = game_board[start_x][start_y];
        game_piece * end_piece_ptr = nullptr;
        game_piece return_piece;
        std::pair<int, int> return_loc;
        bool normal_move = false;

        if (!simulate_move) {
            ++start_piece_ptr->moves_made;
        }

        // Consideration for en passant captures
        if (start_piece_ptr->type == GAME_PIECE_TYPE::PAWN) {
            int delta_x = end_x - start_x;

            if (end_pos == en_passant_position) {
                return_piece = get_location(std::make_pair(end_x - delta_x, end_y));
                return_loc = std::make_pair(end_x - delta_x, end_y);
                delete game_board[end_x - delta_x][end_y];
                game_board[end_x - delta_x][end_y] = nullptr;
                game_board[end_x][end_y] = start_piece_ptr;
                game_board[start_x][start_y] = nullptr;
            } else {
                normal_move = true;
            }
        //  Consideration for castling kings
        } else if (start_piece_ptr->type == GAME_PIECE_TYPE::KING) {
            int delta_y = end_y - start_y;

            // Castling situation - assumes no need to deallocate memory as there should not be any pieces being removed
            if (abs(delta_y) == 2) {
                int rook_y = delta_y > 0 ? DEFAULT_CHESS_BOARD_SIZE - 1 : 0;
                game_piece * rook_ptr = game_board[start_x][rook_y];

                // Move the king
                game_board[end_x][end_y] = start_piece_ptr;
                game_board[start_x][start_y] = nullptr;

                // Move the rook
                if (!simulate_move) {
                    ++rook_ptr->moves_made;
                }
                int end_rook_y = delta_y > 0 ? DEFAULT_CHESS_BOARD_SIZE - 3 : 3;
                game_board[end_x][end_rook_y] = rook_ptr;
                game_board[start_x][rook_y] = nullptr;

            } else {
                normal_move = true;
            }
            
            if (current_player == player1) {
                player1_king_position = std::make_pair(end_x, end_y);
            } else {
                player2_king_position = std::make_pair(end_x, end_y);
            }
        } else {
            normal_move = true;
        }

        // Normal considerations - simply move the start piece to the end position
        if (normal_move) {
            end_piece_ptr = game_board[end_x][end_y];
            return_piece = get_location(std::make_pair(end_x, end_y));
            return_loc = std::make_pair(end_x, end_y);

            // If the end spot is an empty location - simply transfer ownership of the memory from one position to the other
            // Otherwise first deallocate the memory for the end location
            if (end_piece_ptr != nullptr) {
                delete game_board[end_x][end_y];
            }

            game_board[end_x][end_y] = start_piece_ptr;
            game_board[start_x][start_y] = nullptr;
        }

        // Considerations for pawns - if a pawn moved 2 vertically, then the position behind it becomes an en passant valid position
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
        } else if (!simulate_move) {
            en_passant_position = std::make_pair(-1, -1);
        }

        return std::make_pair(return_piece, return_loc);
    }

    // Updates the internal game state based on chess ruling
    // Essentially determines if the game is in stalemate / check / checkmate / or normal play
    // Intented to be used after every call of play_move - must be manually called
    void Game::update_game_state() {
        // Run through each function to check which state the game is in - check first since checkmate implies check
        // Stalemate last because stalemate is separate from both
        if (is_in_checkmate()) {
            current_game_state = GAME_STATE::CHECKMATE;
        } else if (is_in_stalemate()) {
            current_game_state = GAME_STATE::STALEMATE;
        } else if (is_in_check()) {
            current_game_state = GAME_STATE::CHECK;
        } else {
            current_game_state = GAME_STATE::NORMAL;
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
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
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
                        // Intense black shapes with white background
                        SetConsoleTextAttribute(hStdout, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | FOREGROUND_INTENSITY);
                        std::wcout << GAME_PIECE_SYMBOLS.at(piece.type);
                    } else {
                        // Intense white shapes with black background
                        SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
                        std::wcout << GAME_PIECE_SYMBOLS.at(piece.type);
                    }
                    // Reset to default
                    SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
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
        int x = position.first;
        int y = position.second;

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
    Game::MOVE_ERROR_CODE Game::validate_piece_move_restricted(const GAME_PIECE_TYPE type, const std::pair<int, int>& move) const {
        std::vector<std::pair<int, int>> moveset = PIECE_MOVESETS.at(type);

        auto move_found = std::find(moveset.cbegin(), moveset.cend(), move);

        if(move_found != moveset.cend()) {
            return VALID_MOVE;
        } else {
            return ILLEGAL_MOVE;
        }
    }

    // Validates if the given piece can move described by move - for unrestricted pieces (ensures the path is clear for the piece)
    Game::MOVE_ERROR_CODE Game::validate_piece_move_unrestricted(const GAME_PIECE_TYPE type, const std::pair<int, int>& move, const std::pair<int, int>& start_pos, const std::pair<int, int> end_pos) const {
        // First ensure the move is within the moveset
        if (validate_piece_move_restricted(type, move) != VALID_MOVE) {
            return ILLEGAL_MOVE;
        }

        // Next - map out the path from the start pos to the end pos ensuring there are no pieces in the way
        int x_start = start_pos.first + move.first;
        int y_start = start_pos.second + move.second;

        int x_end = end_pos.first;
        int y_end = end_pos.second;

        // If there are any valid pieces along the path then return BLOCKED
        while (x_start != x_end || y_start != y_end) {
            game_piece piece = get_location(std::make_pair(x_start, y_start));

            if (validate_game_piece(piece)) {
                return BLOCKED_MOVE;
            }

            x_start += move.first;
            y_start += move.second;
        }
        
        // If we make it here then the path should be clear
        return VALID_MOVE;
    }

    // Determines is baseline delta move for the piece, finding the root move direction for unrestricted pieces
    std::pair<float, float> Game::calculate_piece_delta_move(const game_piece& piece, const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos) const {
        // Calculate the overall move
        int delta_x = end_pos.first - start_pos.first;
        int delta_y = end_pos.second - start_pos.second;
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
        } else {
            base_delta_x = delta_x;
            base_delta_y = delta_y;
        }

        std::pair<float, float> delta_pair = std::make_pair(base_delta_x, base_delta_y);

        return delta_pair;
    } 

    // Validates that the move is an acceptable move for a pawn given the current state of the board - no consideration for checks
    Game::MOVE_ERROR_CODE Game::validate_pawn_move(const game_piece& starting_piece, const game_piece& ending_piece, const std::pair<int, int>& move, const std::pair<int, int>& end_pos) const {
        std::vector<std::pair<int, int>> moveset = PIECE_MOVESETS.at(GAME_PIECE_TYPE::PAWN);
        // Pawns may only move in a set x-direction, therefore check to ensure the move aligns with this pawns direction
        if (starting_piece.pawn_move_positive_x) {
            if (move.first < 0) {
                return ILLEGAL_MOVE;
            }
        } else {
            if (move.first > 0) {
                return ILLEGAL_MOVE;
            }
        }

        // Determine if the move is within the set
        auto move_found = std::find(moveset.cbegin(), moveset.cend(), move);
        if (move_found == moveset.cend()) {
            return ILLEGAL_MOVE;
        }

        // Now that the move is validated - we must further consider if the move is the double move
        if (abs(move.first) == 2) {
            // As long as this is the pawns first move and there isn't a piece in the way then this is valid
            if (starting_piece.moves_made == 0 && !validate_game_piece(ending_piece)) {
                // We need to ensure the whole path is clear for the pawn to make this move
                int delta_x = move.first > 0 ? -1 : 1;
                int other_x = end_pos.first + delta_x;
                if(!validate_game_piece(get_location(std::make_pair(other_x, end_pos.second)))) {
                    return VALID_MOVE;
                } else {
                    return BLOCKED_MOVE;
                }
            } else {
                return ILLEGAL_MOVE;
            }
        }

        // Further consider if this is an en passant move or a diagonal capture
        if (move.second != 0) {
            // First consider if this is a normal capture - the capture piece must be of a different color but not an invalid color
            if (ending_piece.color != starting_piece.color && ending_piece.color != GAME_PIECE_COLOR::NOCOLOR) {
                return VALID_MOVE;
            
            // Otherwise consider if this is an en passant move
            } else {
                // Compares the end position to the current valid en passant position that is determined every call of play_move
                if (end_pos == en_passant_position) {
                    return VALID_MOVE;
                } else {
                    return ILLEGAL_MOVE;
                }
            }
        
        // Simplest normal move for the pawn - ensure there is no piece in the end position
        } else {
            if(!validate_game_piece(ending_piece)) {
                return VALID_MOVE;
            } else {
                return BLOCKED_MOVE;
            }
        }
    }

    // Validates that the move is an acceptable move for a king given the current state of the board - no consideration for checks
    Game::MOVE_ERROR_CODE Game::validate_king_move(const game_piece& starting_piece, const std::pair<int, int> move, const std::pair<int, int>& start_pos) const {
        // First determine if the move is valid in the set
        if (validate_piece_move_restricted(GAME_PIECE_TYPE::KING, move) != VALID_MOVE) {
            return ILLEGAL_MOVE;
        }

        // Determine if the move is attempting to castle
        if (abs(move.second) == 2) {
            // Determine if the king has moved
            if (starting_piece.moves_made != 0) {
                return ILLEGAL_MOVE;
            }

            // Select the respective rook to ensure it also hasn't moved
            game_piece castle_rook;
            if (move.second < 0) {
                castle_rook = get_location(std::make_pair(start_pos.first, 0));
            } else {
                castle_rook = get_location(std::make_pair(start_pos.first, 7));
            }

            if (castle_rook.moves_made != 0) {
                return ILLEGAL_MOVE;
            }

            // Ensure that the game state is not currently in check
            if (current_game_state == GAME_STATE::CHECK) {
                return ILLEGAL_MOVE;
            }

            // Ensure the path between the rook and king is clear
            int x = start_pos.first;
            int y = start_pos.second;
            int end_y = move.second < 0 ? 0 : 7;
            if (end_y > y) {
                ++y;
            } else {
                --y;
            }
            while (y != end_y) {
                game_piece piece = get_location(std::make_pair(x, y));

                // At any point if there is a valid piece - return BLOCKED
                if (validate_game_piece(piece)) {
                    return BLOCKED_MOVE;
                }
                
                if (end_y > y) {
                    ++y;
                } else {
                    --y;
                }
            }

            // Finally if there are no pieces in the way then this is a valid castle move
            return VALID_MOVE;

        // If it isn't a castle attempt then the move is valid
        } else {
            return VALID_MOVE;
        }
    }

    // Determines if the move described by the start and end positions is a legal move based on chess ruling
    // Notably - this only determines if this move is considered by chess ruling for the pieces
    // However, this does not take into consideration if this is a valid move given the board state
    // No checks are made to determine if the game will be in check based on this function
    // Returns an error code to describe the state of the move
    Game::MOVE_ERROR_CODE Game::is_legal_move(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos) const {
        // First - validate that the moves are within the bounds of the board
        if (!validate_position(start_pos)) {
            return OUT_OF_BOUNDS_MOVE;
        }

        if (!validate_position(end_pos)) {
            return OUT_OF_BOUNDS_MOVE;
        }

        // For a move to be valid there must first be a piece at the start_pos and there must not be a piece at the end_pos of the same color
        game_piece starting_piece = get_location(start_pos);
        game_piece ending_piece = get_location(end_pos);

        // Validate that there exists a piece at start_pos
        if (!validate_game_piece(starting_piece)) {
            return NO_PIECE;
        }

        // Validate that the ending piece cannot be the same color
        if (validate_game_piece(ending_piece) && ending_piece.color == starting_piece.color) {
            return BLOCKED_MOVE;
        } 

        // Next validate that the current player has control of the target piece
        if (starting_piece.color != current_player->get_player_color()) {
            return WRONG_PLAYER;
        }

        // Determine the baseline movement - unrestricted pieces will have the movement become base 1
        std::pair<float, float> delta_pair_float = calculate_piece_delta_move(starting_piece, start_pos, end_pos);
        
        // Ensuring the delta pair are whole numbers
        if (ceil(delta_pair_float.first) != floor(delta_pair_float.first) || ceil(delta_pair_float.second) != floor(delta_pair_float.second)) {
            return ILLEGAL_MOVE;
        }

        std::pair<int, int> delta_pair = std::make_pair(delta_pair_float.first, delta_pair_float.second);
        
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
        
        // Lastly for restricted pieces its a simple look-up
        } else {
            return validate_piece_move_restricted(starting_piece.type, delta_pair);
        }
    }

    // Determines if the provided move would place the player in check - only checks the one move and not any moves between it
    bool Game::simulate_move_for_check(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos) {
        // Setup the variables to simulate playing the incremental move
        game_piece start_piece = get_location(start_pos);
        std::pair<game_piece, std::pair<int, int>> removed_data = play_move(start_pos, end_pos, true);
        game_piece end_piece = removed_data.first;

        bool return_val = is_in_check();

        // Case where the king castled - we need to replace where both the king and rook were
        if (start_piece.type == GAME_PIECE_TYPE::KING && abs(start_pos.second - end_pos.second) == 2) {
            int delta_y = end_pos.second - start_pos.second;
            // Getting the rooks new position relative to the king
            int rook_y = delta_y > 0 ? end_pos.second - 1 : end_pos.second + 1;
            game_piece * rook_ptr = game_board[start_pos.first][rook_y];
            game_piece * king_ptr = game_board[end_pos.first][end_pos.second];

            // Replacing the king
            game_board[start_pos.first][start_pos.second] = king_ptr;
            game_board[end_pos.first][end_pos.second] = nullptr;

            // Replacing the rook
            if (delta_y > 0) {
                game_board[start_pos.first][DEFAULT_CHESS_BOARD_SIZE - 1] = rook_ptr;
            } else {
                game_board[start_pos.first][0] = rook_ptr;
            }

            game_board[start_pos.first][rook_y] = nullptr;
            
        } else {
            // Clearing all potential positions where a piece could still be at before re-adding all of the pieces
            remove_piece(removed_data.second);
            remove_piece(start_pos);

            // Readding pieces with moves made to ensure the piece is exactly the same as the previous version
            if (validate_game_piece(start_piece)) {
                game_board[start_pos.first][start_pos.second] = new game_piece(start_piece.type, start_piece.color);
                game_board[start_pos.first][start_pos.second]->moves_made = start_piece.moves_made;
            }

            if (validate_game_piece(end_piece)) {
                game_board[removed_data.second.first][removed_data.second.second] = new game_piece(end_piece.type, end_piece.color);
                game_board[removed_data.second.first][removed_data.second.second]->moves_made = end_piece.moves_made;
            }
        }

        return return_val;
    }

    // Determines if the move described by the start and end positions is a valid move based on chess ruling
    // Notably - this is different from is_legal_move because it ensures the move is not placing the player in check
    // This is the function that should be used to determine if a move is truly valid before playing the move
    Game::MOVE_ERROR_CODE Game::is_valid_move(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos) {
        MOVE_ERROR_CODE legality = is_legal_move(start_pos, end_pos);
        if (legality != VALID_MOVE) {
            return legality;
        }

        // For the move to be valid - it must not place the current player in check
        // Simulate the move and then determine if the current player is in check
        game_piece end_piece = get_location(end_pos);
        game_piece start_piece = get_location(start_pos);

        int delta_x = abs(end_pos.first - start_pos.first);
        int delta_y = abs(end_pos.second - start_pos.second);

        // For moves greater than 1 unit we must simulate each move to ensure that no move along the route places the player in check
        // Knights are the exception - they move straight to their end position
        if ((delta_x > 1 || delta_y > 1) && start_piece.type != GAME_PIECE_TYPE::KNIGHT) {
            std::pair<float, float> move_delta = calculate_piece_delta_move(start_piece, start_pos, end_pos);
            int move_delta_x = round(move_delta.first);
            int move_delta_y = round(move_delta.second);
            int start_x = start_pos.first;
            int start_y = start_pos.second;
            
            std::pair<int, int> next_move;
            do {
                start_x += move_delta_x;
                start_y += move_delta_y;
                next_move = std::make_pair(start_x, start_y);
                
                // Simulate the move and determine if it places the player in check
                if (simulate_move_for_check(start_pos, next_move)) {
                    return CHECK_MOVE;
                }
            } while (next_move != end_pos);

            // Making it out of the do-while loop means that all of the moves are valid and the move can be accomplished
            return VALID_MOVE;
        
        // A restricted piece can just make the move - simply ensure that the move won't place the player in check
        } else {
            if (simulate_move_for_check(start_pos, end_pos)) {
                return CHECK_MOVE;
            } else {
                return VALID_MOVE;
            }
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
        if (king_pos.first == -1 || king_pos.second == -1) {
            return false;
        }

        game_piece king = get_location(king_pos);

        // Check every piece around the king - if they are the same color then the king is safe from all pieces except knights
        std::vector<std::pair<int, int>> area_around_king = {std::make_pair(1, 0), std::make_pair(-1, 0), std::make_pair(0, 1), std::make_pair(0, -1),
                                                             std::make_pair(1, 1), std::make_pair(-1, 1), std::make_pair(1, -1), std::make_pair(-1, -1)};
        game_piece piece;

        for(int i = 0; i < area_around_king.size(); ++i) {
            std::pair<int, int> piece_delta = area_around_king.at(i);
            int piece_x = piece_delta.first + king_pos.first;
            int piece_y = piece_delta.second + king_pos.second;


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
                        int piece_x = (piece_delta.first * j) + king_pos.first;
                        int piece_y = (piece_delta.second * j) + king_pos.second;

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
                                if(is_legal_move(std::make_pair(piece_x, piece_y), king_pos) == VALID_MOVE) {
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
            int piece_x = piece_delta.first + king_pos.first;
            int piece_y = piece_delta.second + king_pos.second;

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

    // Determines if the current player is in checkmate
    // Checkmate is defined as currently being in check but also being unable to make a move to take you out of check
    bool Game::is_in_checkmate() {
        // Loop through every position on the board - check if the piece matches the current players color and see if that piece has a valid move - aka doesn't leave the player in check
        game_piece player_piece;
        std::vector<std::pair<int, int>> piece_moves;
        std::pair<int, int> move;
        std::pair<int, int> start_pos;
        std::pair<int, int> end_pos;
        for (int i = 0; i < DEFAULT_CHESS_BOARD_SIZE; ++i) {
            for (int j = 0; j < DEFAULT_CHESS_BOARD_SIZE; ++j) {
                player_piece = get_location(std::make_pair(i, j));
                if (validate_game_piece(player_piece) && (current_player->get_player_color() == player_piece.color)) {
                    piece_moves = PIECE_MOVESETS.at(player_piece.type);
                    for (int k = 0; k < piece_moves.size(); ++k) {
                        move = piece_moves.at(k);
                        start_pos = std::make_pair(i, j);
                        if (player_piece.is_restricted) {
                            end_pos = std::make_pair(i + move.first, j + move.second);
                            if (is_valid_move(start_pos, end_pos) == VALID_MOVE) {
                                return false;
                            }
                        } else {
                            for (int l = 1; (((l * move.first) + i < DEFAULT_CHESS_BOARD_SIZE) && ((l * move.first) + i >= 0)) \
                            && (((l * move.second) + j < DEFAULT_CHESS_BOARD_SIZE) && ((l * move.second) + j >= 0)); ++l) {
                                end_pos = std::make_pair(l * move.first + i, l * move.second + j);
                                if (is_valid_move(start_pos, end_pos) == VALID_MOVE) {
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }
        return true;
    }

    // Determines if the game is currently in stalemate
    bool Game::is_in_stalemate() {
        // TODO - Implement
        return false;
    }
}

