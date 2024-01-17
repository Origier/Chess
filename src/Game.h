#ifndef CPLUSPLUS_CHESS_GAME
#define CPLUSPLUS_CHESS_GAME

#include <tuple>            // std::pair
#include <unordered_map>    // std::unordered_map for containing the key-value pair of game piece movesets
#include <stdexcept>        // std::runtime_error
#include <memory>           // std::shared_ptr
#include <iostream>         // std::cout, std::endl
#include <io.h>             // _setmode
#include <fcntl.h>          // _O_U16TEXT

#include "Player.h"
#include "Chess_API_vars.h"


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

        // The constantly defined movesets for each piece based on the ruling of chess
        // Each moveset is a normalized move - meaning that it is an x,y directional move rather than the limits of their move
        // Based on the struct definition - if the piece is restricted then it may only move by its moveset
        // Otherwise it may move freely along its moveset directional
        const std::unordered_map<GAME_PIECE_TYPE, std::vector<std::pair<int, int>>> PIECE_MOVESETS = {
            // Pawns can move forward one row at a time, may move diagonal to take a piece, or may move two rows on their first move
            {PAWN, {std::make_pair(1, 0), std::make_pair(1, 1), std::make_pair(1, -1), std::make_pair(2, 0),
                    std::make_pair(-1, 0), std::make_pair(-1, 1), std::make_pair(-1, -1), std::make_pair(-2, 0)}},

            // Knights can move in eight different directions but always in an L-shape of 2 x 1
            {KNIGHT, {std::make_pair(2, 1), std::make_pair(2, -1), std::make_pair(1, 2), std::make_pair(-1, 2),
                      std::make_pair(-2, 1), std::make_pair(-2, -1), std::make_pair(1, -2), std::make_pair(-1, -2)}},

            // Kings can move in any direction however they are restricted to moving only one unit in any direction
            // They may also move two columns on their first move in either direction if the circumstances are aligned to do so "castling"
            {KING, {std::make_pair(1, 1), std::make_pair(1, -1), std::make_pair(-1, -1), std::make_pair(-1, 1),
                    std::make_pair(1, 0), std::make_pair(0, 1), std::make_pair(-1, 0), std::make_pair(0, -1), std::make_pair(0, 2), std::make_pair(0, -2)}},

            // Queens have almost the exact same moveset as kings however they are not restricted and can move all they want in a single direction
            {QUEEN, {std::make_pair(1, 1), std::make_pair(1, -1), std::make_pair(-1, -1), std::make_pair(-1, 1),
                    std::make_pair(1, 0), std::make_pair(0, 1), std::make_pair(-1, 0), std::make_pair(0, -1)}},

            // Bishops can move along each diagonal but are not restricted
            {BISHOP, {std::make_pair(1, 1), std::make_pair(1, -1), std::make_pair(-1, -1), std::make_pair(-1, 1)}},

            // Rooks may move in the horizontal or vertical axis but they are not restricted
            {ROOK, {std::make_pair(1, 0), std::make_pair(0, 1), std::make_pair(-1, 0), std::make_pair(0, -1)}}
        };

        // Default constructor creating an empty game
        Game(const std::shared_ptr<Player> player1_in, const std::shared_ptr<Player> player2_in);

        // Copy constructor
        Game(const Game& copy_source);

        // Destructor for removing all board allocated memory
        ~Game();

        // Assignment operator - copies the current games data rather then acting as a reference
        Game& operator=(const Game& other);

        // Adds the given piece type to the game board at the provided location
        // Throws an error if attempting to place the piece outside the bounds
        void add_piece(const GAME_PIECE_TYPE type_in, const GAME_PIECE_COLOR color_in, const std::pair<int, int>& location);

        // Returns a game_piece copy for this location
        // Throws an error if attempting to pull a location beyond the scope of the board
        // Returns an invalid piece if there is no piece - type = NOTYPE, color = NOCOLOR
        game_piece get_location(const std::pair<int, int>& location) const;

        // Removes any pieces on the provided location - if there isn't a piece there then it does nothing
        void remove_piece(const std::pair<int, int>& location);

        // Plays the given move placing the game piece from start_pos to end_pos
        // Assumes that the move has been validated by is_valid_move - this function only validates that the move is within the bounds of the board
        // Simulate_move is a flag that will do all of the normal functionality with the expectation that the move will be undone
        // Therefore it does not update cached information (en_passant_position / king position)
        void play_move(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos, bool simulate_move = false);

        // Updates the internal game state based on chess ruling
        // Essentially determines if the game is in stalemate / check / checkmate / or normal play
        // Intented to be used after every call of play_move - must be manually called
        void update_game_state();

        // Returns the current state of the game
        GAME_STATE get_current_game_state() const;

        // Prints the game board to std::cout in a friendly manner
        void show_board() const;

        // Validates if the provided game piece is a valid piece or not
        bool validate_game_piece(const game_piece& piece) const;

        // Determines if the move described by the start and end positions is a legal move based on chess ruling
        // Notably - this only determines if this move is considered by chess ruling for the pieces
        // However, this does not take into consideration if this is a valid move given the board state
        // No checks are made to determine if the game will be in check based on this function
        bool is_legal_move(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos) const;

        // Determines if the move described by the start and end positions is a valid move based on chess ruling
        // Notably - this is different from is_legal_move because it ensures the move is not placing the player in check
        // This is the function that should be used to determine if a move is truly valid
        bool is_valid_move(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos);

        // Swaps which player is the current player
        void swap_current_player();

        // Returns a read-only version of the current player
        const std::shared_ptr<Player> get_current_player() const {return current_player;}

    private:
        // Prints the provided character the number of times provided - helper function for show board - assumes the CLI has been set to UTF-16 mode
        void print_wchar_times(const wchar_t wide_char, const int times) const;

        // Prints the board line separator - helper function for show board - assumes the CLI has been set to UTF-16 mode
        void print_board_line_separator() const;

        // Prints the board letters - helper function for show board - assumes the CLI has been set to UTF-16 mode
        void Game::print_board_letters() const;

        // validates that the position is a valid position on the board
        bool validate_position(const std::pair<int, int>& position) const;

        // Validates if the given piece can move described by move
        bool validate_piece_move_restricted(const GAME_PIECE_TYPE type, const std::pair<int, int>& move) const;

        // Validates if the given piece can move described by move - for unrestricted pieces (ensures the path is clear for the piece)
        bool validate_piece_move_unrestricted(const GAME_PIECE_TYPE type, const std::pair<int, int>& move, const std::pair<int, int>& start_pos, const std::pair<int, int> end_pos) const;

        // Determines is baseline delta move for the piece, finding the root move direction for unrestricted pieces
        std::pair<float, float> calculate_piece_delta_move(const game_piece& piece, const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos) const;

        // Validates that the move is an acceptable move for a pawn given the current state of the board - no consideration for checks
        bool validate_pawn_move(const game_piece& starting_piece, const game_piece& ending_piece, const std::pair<int, int>& move, const std::pair<int, int>& end_pos) const;

        // Validates that the move is an acceptable move for a king given the current state of the board - no consideration for checks
        bool validate_king_move(const game_piece& starting_piece, const std::pair<int, int> move, const std::pair<int, int>& start_pos) const;

        // Determines if the provided move would place the player in check - only checks the one move and not any moves between it
        bool simulate_move_for_check(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos);

        // Determines if the current player is in check
        bool is_in_check();

        // Determines if the game is currently in checkmate
        bool is_in_checkmate();

        // Determines if the game is currently in stalemate
        bool is_in_stalemate();

        std::shared_ptr<Player> player1;                                    // Player object that would have the "white" pieces
        std::shared_ptr<Player> player2;                                    // Player object that would have the "black" pieces
        std::shared_ptr<Player> current_player;                             // Reference to whomever is the current player object to take their turn

        game_piece *** game_board = nullptr;                                // The game boards - containing either pieces or nullptrs

        GAME_STATE current_game_state = NORMAL;                             // Tracks the games state - read only from API and used to determine the play state of the game

        std::pair<int, int> en_passant_position = std::make_pair(-1, -1);   // Tracks the position for the next available en passant move, updates every move played, defaults to negative values when there isn't a valid en passant move
        std::pair<int, int> player1_king_position = std::make_pair(-1, -1); // Tracks the current position of player 1's king - defaults to -1, -1 if not defined
        std::pair<int, int> player2_king_position = std::make_pair(-1, -1); // Tracks the current position of player 2's king - defaults to -1, -1 if not defined
   
    };
    
}

#endif