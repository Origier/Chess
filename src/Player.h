#ifndef CPLUSPLUS_CHESS_PLAYER
#define CPLUSPLUS_CHESS_PLAYER

#include <tuple> // std::pair
#include <string>
#include "Chess_API_vars.h"

namespace Chess_API {
    class Player {
    protected:
        int player_id;                  // Player identifier
        GAME_PIECE_COLOR player_color;  // Piece color to help identifying the player further
        std::string name;               // Players name

        // Sets up the player id to destinguish different players
        void set_player_id() {
            static int next_player_id = 0; 
            ++next_player_id;
            player_id = next_player_id;
        }
    public:
        // Default constructor for the player object
        Player(): name(DEFAULT_PLAYER_NAME){
            set_player_id();
        }

        // Constructor for recieving the players name
        Player(std::string name_in, GAME_PIECE_COLOR color_in) : name(name_in), player_color(color_in) {
            set_player_id();
        }

        std::string get_name() const {return name;}

        int get_player_id() const {return player_id;}

        GAME_PIECE_COLOR get_player_color() const {return player_color;}

        bool operator==(const Player& other) const {return player_id == other.player_id;}
        
        // Prompts the player to take their turn
        virtual std::pair<std::string, std::string> take_turn() const = 0;
    };
}

#endif