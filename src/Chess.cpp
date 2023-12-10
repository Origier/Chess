#include "Chess.h"

namespace Chess_API {
    /* Determines if the provided input is a valid player input or not - used in conjunction with the player class to get player input
    *  The correct format for player input should be a string pair, indicating the starting position and the ending postion
    *  Examples include: {a1, b2}, {f7, e5}, {h8, a1}...
    *  This functions determines if the correct formatting was used for the player input
    */
    bool Chess::is_valid_player_input(std::pair<std::string, std::string> player_move) const {
        // Create a copy of each string
        std::string start = std::get<0>(player_move);
        std::string end = std::get<1>(player_move);

        // If either strings are not exactly two characters then the format is incorrect
        if (start.size() != 2 || end.size() != 2) {
            return false;
        }

        // Ensure that each string is not case sensitive
        std::transform(start.cbegin(), start.cend(), start.begin(),
        [](unsigned char c) {return std::tolower(c);});

        std::transform(end.cbegin(), end.cend(), end.begin(),
        [](unsigned char c) {return std::tolower(c);});

        // Ensure that the first character of each string is a valid character
        auto i_start_char = std::find(VALID_CHARS.cbegin(), VALID_CHARS.cend(), start[0]);
        auto i_end_char = std::find(VALID_CHARS.cbegin(), VALID_CHARS.cend(), end[0]);

        // Ensure that the second character of each string is a valid number
        auto i_start_num = std::find(VALID_NUMS.cbegin(), VALID_NUMS.cend(), start[1]);
        auto i_end_num = std::find(VALID_NUMS.cbegin(), VALID_NUMS.cend(), end[1]);

        // Lastly - determining if the input is valid - for each case the iterator should not be pointing to the end iterator for the valid input strings
        bool valid_input = (i_start_char != VALID_CHARS.end()) && (i_end_char != VALID_CHARS.end()) && (i_start_num != VALID_NUMS.end()) && (i_end_num != VALID_NUMS.end());

        return valid_input;
    }
    
    // Default constructor to start a blank new game against a computer player
    Chess::Chess() {
        // Default game, computer player and player
        game = new Game();
        player1 = new Human_Player();
        player2 = new Computer_Player(game);
        current_player = player1;
    }

    // Main constructor for taking the players name as well as the difficulty to play on 
    Chess::Chess(std::string player_name, Computer_Player::DIFFICULTY difficulty) {
        // Default game, computer player and player
        game = new Game();
        player1 = new Human_Player(player_name);
        player2 = new Computer_Player(game, difficulty);
        current_player = player1;
    }

    // Multiplayer constructor to play with two human players
    Chess::Chess(std::string player_name1, std::string player_name2) {
        // Default game, computer player and player
        game = new Game();
        player1 = new Human_Player(player_name1);
        player2 = new Human_Player(player_name2);
        current_player = player1;
    }

    // Constructor that can use serialized data to generate the game up to the current point based on past moves
    Chess::Chess(Game game_in, std::vector<std::pair<std::string, std::string>> past_moves, std::string player_name1, std::string player_name2, Computer_Player::DIFFICULTY difficulty) {
        // Copies all of the serialized data to recreate the game as it was
        game = new Game(game_in);
        played_moves = past_moves;
        player1 = new Human_Player(player_name1);

        // Currently utilizes the name of player 2 to determine if the saved game was against a player or computer
        if (player_name2 == DEFAULT_COMPUTER_NAME) {
            player2 = new Computer_Player(game, difficulty);
        } else {
            player2 = new Human_Player(player_name2);
        }

        // If the number of past moves is odd then it is currently players 2's turn
        if (played_moves.size() % 2 == 1) {
            current_player = player2;
        } else {
            current_player = player1;
        }
    }

    // Destructor that deletes all game data
    Chess::~Chess() {
        delete game;
        delete player1;
        delete player2;
    }

    /*  Plays the next turn of the game - allows for the next determined player to input their moves into the game
    *   Plays out the moves, assuming they are valid, and changes the game state accordingly
    */ 
    void Chess::play_turn() {
        // Determining that the move is valid per the chess board
        std::pair<std::string, std::string> new_move = current_player->take_turn();

        if (!is_valid_player_input(new_move)) {
            throw std::runtime_error(INVALID_INPUT_ERROR_MSG);
        }

        // Parsing the move into usable int values to determine what the start and end locations are on the chess board
        // calculating the index of the 2D grid based on the position in the valid chars array
        int start_y = std::distance(VALID_CHARS.cbegin(),
                                    std::find(VALID_CHARS.cbegin(), VALID_CHARS.cend(), 
                                              std::get<0>(new_move).at(0)));
        int start_x = std::distance(VALID_NUMS.cbegin(),
                                    std::find(VALID_NUMS.cbegin(), VALID_NUMS.cend(), 
                                              std::get<0>(new_move).at(1)));
        
        // Repeating calculations for the end position
        int end_y = std::distance(VALID_CHARS.cbegin(),
                                    std::find(VALID_CHARS.cbegin(), VALID_CHARS.cend(), 
                                              std::get<1>(new_move).at(0)));
        int end_x = std::distance(VALID_NUMS.cbegin(),
                                    std::find(VALID_NUMS.cbegin(), VALID_NUMS.cend(), 
                                              std::get<1>(new_move).at(1)));
        
        // Creating the move pairs to indicate which position on the grid to start in and which position to end in
        std::pair<int, int> start_pos(start_x, start_y);
        std::pair<int, int> end_pos(end_x, end_y);

        // Determining if the move provided is within the confines of chess ruling
        if (!game->is_valid_move(start_pos, end_pos)) {
            throw std::runtime_error(INVALID_MOVE_ERROR_MSG);
        }

        // Finally - if this is a valid move to be played per the input parameters and is a valid chess move then the move is recorded and played
        played_moves.push_back(new_move);
        game->play_move(start_pos, end_pos);

        // Switch which player is the new current player
        if (current_player == player1) {
            current_player = player2;
        } else {
            current_player = player1;
        }
    }

    // Determines if the game is currently in a state of check
    bool Chess::is_in_check() const {
        if (game->get_current_game_state() == Game::CHECK) {
            return true;
        } else {
            return false;
        }
    }

    // Determines if the game is currently in a state of stalemate
    bool Chess::is_stalemate() const {
        if (game->get_current_game_state() == Game::STALEMATE) {
            return true;
        } else {
            return false;
        }
    }

    // Determines if the game is currently in a check mate state - indicating the game is over
    bool Chess::is_in_check_mate() const {
        if (game->get_current_game_state() == Game::CHECKMATE) {
            return true;
        } else {
            return false;
        }
    }

    // Returns the current player
    const Player * Chess::get_current_player() const {
        return current_player;
    }

    // Displays the board to the console
    void Chess::show_board() const {
        game->show_board();
    }
}