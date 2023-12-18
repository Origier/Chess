#ifndef CPLUSPLUS_CHESS_GAME
#define CPLUSPLUS_CHESS_GAME

#include <tuple>            // std::pair
#include <unordered_map>    // std::unordered_map for containing the key-value pair of game piece movesets
#include <stdexcept>
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
            {PAWN, {std::make_pair(1, 0), std::make_pair(1, 1), std::make_pair(1, -1), std::make_pair(2, 0)}},

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
        Game();

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
        void play_move(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos);

        // Returns the current state of the game
        GAME_STATE get_current_game_state() const;

        // Prints the game board to std::cout in a friendly manner
        void show_board() const;

        // Determines if the provided starting and ending position are valid moves based on Chess ruling
        bool is_valid_move(const std::pair<int, int>& start_pos, const std::pair<int, int>& end_pos) const;

    private:

        game_piece *** game_board = nullptr;   // The game boards - containing either pieces or nullptrs

        GAME_STATE current_game_state = NORMAL;
   
    };
    
}

#endif