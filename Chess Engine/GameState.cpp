#include <iostream>
#include "board.h"

using namespace std;

//General State of pieces
uint64_t Board:: white_board_state(){
    /*
    Is a collections of all white pieces

    Input:
        None

    Output:
        uint64_t number where there is a one in the bit position occupied by white pieces
    
    */

    return this->white_bishop_state | this->white_rook_state | this->white_king_state | this->white_pawn_state | 
    this->white_queen_state | this->white_horse_state;
}

uint64_t Board:: black_board_state(){
    /*
    Is a collections of all black pieces

    Input:
        None

    Output:
        uint64_t number where there is a one in the bit position occupied by black pieces
    
    */
    return this->black_bishop_state | this->black_horse_state | this->black_king_state | this->black_pawn_state | 
    this->black_pawn_state | this->black_queen_state | this->black_rook_state;
}

uint64_t Board:: board_state(){
    /*
    Is a collections of all pieces

    Input:
        None

    Output:
        uint64_t number where there is a one in the bit position occupied by all pieces
    
    */
    return this->black_board_state() | this->white_board_state();
}


//Helper functions for checkmate and statelmate
bool Board:: king_check(uint64_t king_state){
    /*
    A function which checks if the king is in check

    Input:
        uint64_t king_state: The state of the king black or white
    
    Output:
        True if king is in check
        False if king not in check
    
    
    */
    uint64_t move = 0;

    int row = (this->find_coordinates(king_state))[0];
    int column = (this->find_coordinates(king_state))[1];

    uint64_t enemy_bishop, enemy_queen, enemy_pawn, enemy_rook, enemy_horse, all_enemy_piece, friendly_pieces;

    //Assiging Enemy pieces
    if(king_state == (this->white_king_state)){
        enemy_bishop = this->black_bishop_state;
        enemy_queen = this->black_queen_state;
        enemy_pawn = this->black_pawn_state;
        enemy_rook = this->black_rook_state;
        enemy_horse = this->black_horse_state;
        all_enemy_piece = this->black_board_state();
        friendly_pieces = this->white_board_state();
    }
    else if(king_state == this->black_king_state){
        enemy_bishop = this->white_bishop_state;
        enemy_queen = this->white_queen_state;
        enemy_pawn = this->white_pawn_state;
        enemy_rook = this->white_rook_state;
        enemy_horse = this->white_horse_state;
        all_enemy_piece = this->white_board_state();
        friendly_pieces = this->black_board_state();
    }

    vector<vector<int>> horizontal_directions = {
        //{row, column}
        {0 , 1},    //horizontal +ve
        {0, -1},    //horizontal -ve
        {1, 0},     //veritcal +ve
        {-1, 0},    //veritcal -ve
        {1, 1},     //NE
        {1, -1},    //SE
        {-1, 1},    //NW
        {-1, -1}    //SW
    };


    //Checking for long range checks
    for(vector<int> &directions : horizontal_directions){
        int r = row, c = column;
        
        //Change in the directions
        int delta_r = directions[0];
        int delta_c = directions[1];

        r += delta_r;
        c += delta_c;

        //Checking Non-Diagonal directions
        if(delta_c == 0 || delta_r == 0){
            while(r >= 0 && c >= 0 && r <8 && c < 8){
                move = this->find_bit_position(r, c);
                
                //If the direction is blocked by a friendly piece check another direction
                if(move & friendly_pieces){
                    break;
                }

                //Checking horizontals for Queens or Rooks.
                if((move & enemy_rook) || (move & enemy_queen)){
                    return true;
                }
                if(move & all_enemy_piece){
                    break;
                }

                r += directions[0];
                c += directions[1]; 
            }
        }
        //Checking Diagonal directions
        else if(delta_r != 0 && delta_c != 0){
            while(r >= 0 && c >= 0 && r <8 && c < 8){
                move = this->find_bit_position(r, c);

                if(move & friendly_pieces){
                    break;
                }                

                //Checking Diagonals for Bishops and Queens.
                if((move & enemy_bishop) || (move & enemy_queen)){
                    return true;
                }
                
                if(move & all_enemy_piece){
                    break;
                }

                r += directions[0];
                c += directions[1]; 
            }
        }
    }


    //Checking for horse checks
    vector<vector<int>>possible_horse_locations = {
        //{row_increment, column_increment}
        {2, 1},
        {2, -1},
        {1, 2},
        {1, -2},
        {-2, 1}, 
        {-2, -1},
        {-1, 2}, 
        {-1, -2}
    };
    
    for(vector<int> &locations : possible_horse_locations){
        int r = row, c = column;

        r += locations[0];
        c += locations[1];

        if(r >= 0 && c >= 0 && r < 8 && c < 8){
            move = this->find_bit_position(r, c);

            if(move & enemy_horse){
                return true;
            }
        }
    }

    //Pawns
    int pawn_attack = 0;
    if(king_state == this->white_king_state){
        pawn_attack = 1;  // black pawns attack from king_row + 1
    }else{
        pawn_attack = -1; // white pawns attack from king_row - 1
    }

    int pawn_row = row + pawn_attack;
    vector<int> attacking_columns = {-1,1};

    for(int attack_c : attacking_columns){
        int pawn_column = column + attack_c;

        if((pawn_row) >= 0 && pawn_row < 8 && pawn_column >= 0 && pawn_column < 8){
            if((this->find_bit_position(pawn_row, pawn_column)) & (enemy_pawn)){
                return true;
            }
        }
    }


    return false;
}

bool Board:: check_legal_moves(string colour){
    bool white_side = (colour == "WHITE");

    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            uint64_t piece_pos = find_bit_position(row, col);
            uint64_t* piece_type = find_type(row, col);

            if(piece_type == nullptr){
                continue;
            }

            bool is_white_piece = (piece_pos & white_board_state()) != 0;
            if(is_white_piece != white_side){
                continue;
            }

            vector<uint64_t> moves = generate_legal_moves(row, col);
            uint64_t destinations = moves[0] | moves[1];

            for(int d_row = 0; d_row < 8; d_row++){
                for(int d_col = 0; d_col < 8; d_col++){
                    uint64_t dest_pos = find_bit_position(d_row, d_col);
                    if(!(destinations & dest_pos)) continue;

                    if(can_make_move(d_row, d_col, row, col, white_side)){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


//Functions for checking checkmate and stale mate
bool Board::is_checkmate(string colour){
    uint64_t king_state;

    if(colour == "WHITE"){
        king_state = white_king_state;
    }
    else{
        king_state = black_king_state;
    }

    if(!king_check(king_state)) return false;
    return !check_legal_moves(colour);
}

bool Board::is_stalemate(string colour){
    uint64_t king_state;

    if(colour == "WHITE"){
        king_state = white_king_state;
    }
    else{
        king_state = black_king_state;
    }

    if(king_check(king_state)) return false;
    return !check_legal_moves(colour);
}
