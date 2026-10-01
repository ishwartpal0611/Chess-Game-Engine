#include <iostream>
#include "board.h"

using namespace std;

//Helper functions for checking if caslting is possible and updating castling variables
bool Board::castling_check(bool white_king, bool king_side){
    /*
    A function which checks if castling can be done for both colours

    Input:
        bool white_king: If the castling piece is white or not
        bool King_side: If the castling direction is on the king side or not
    
    Return:
            False if castling can not occur
            True if castling can occur
    */


    //Saving the state of the king
    uint64_t original_king_state = 0;
    uint64_t *reference_to_king = nullptr;

    if(white_king){
        original_king_state = this->white_king_state;
        reference_to_king = &(this->white_king_state);
        
        // if the king already made first move then no castling
        if(!white_king_first_move){return false;}

        //If the rook in the direction of castling already moved, no castling
        if(king_side && !(this->white_rook_h)){ return false; }
        else if(!king_side && !(this->white_rook_a)){ return false;}

        //If the rook isn't actually still sitting on its corner square, no castling
        if(king_side && !(this->white_rook_state & intial_wr_state_h)){ return false; }
        else if(!king_side && !(this->white_rook_state & intial_wr_state_a)){ return false; }

    }
    else{
        original_king_state = this->black_king_state;
        reference_to_king = &(this->black_king_state);

        if(!black_king_first_move){return false;}
        if(king_side && !(this->black_rook_h)){ return false; }
        else if(!king_side && !(this->black_rook_a)){ return false; }


        if(king_side && !(this->black_rook_state & intial_br_state_h)){ return false; }
        else if(!king_side && !(this->black_rook_state & intial_br_state_a)){ return false; }
    }

    if(king_check(original_king_state)){
        return false; // can't castle out of check
    }

    //Extracting the location of the king
    vector<int> co_ordinates = find_coordinates(original_king_state);
    int row = co_ordinates[0];
    int column = co_ordinates[1];


    //Selecting Directions
    int direction;
    if(king_side){
        direction = 1;
    }else{
        direction = -1;
    }

    uint64_t new_position = 0;

    for(int i = 1; i < 3; i++){
        new_position = find_bit_position(row, column + (i * direction));

        if(new_position & this->board_state()){
            *reference_to_king = original_king_state; 
            return false;
        }


        *reference_to_king = new_position;

        if(king_check(*reference_to_king)){
            *reference_to_king = original_king_state;
            return false;
        }
    }

    *reference_to_king = original_king_state;

    return true;
}

void Board:: update_castling_var(uint64_t *initial_piece_type, uint64_t destination_pos){
    //If king is moved then its not its first move anymore
    if((*initial_piece_type & this->white_king_state) && white_king_first_move){
        white_king_first_move = false;
    }
    else if((*initial_piece_type & this->black_king_state) && black_king_first_move){
        black_king_first_move = false;
    }

    //If rook is moved then its not it's first move anymore
    if((*initial_piece_type & this->white_rook_state)){
        if(intial_wr_state_a & (*initial_piece_type)){
            white_rook_a = false;
        }
        else if(intial_wr_state_h & (*initial_piece_type)){
            white_rook_h = false;
        }
    }

    if((*initial_piece_type & this->black_rook_state)){
        if(intial_br_state_a & (*initial_piece_type)){
            black_rook_a = false;
        }
        else if(intial_br_state_h & (*initial_piece_type)){
            black_rook_h = false;
        }
    }

    // Capturing rook
    if(destination_pos & intial_wr_state_a){
        white_rook_a = false;
    }
    if(destination_pos & intial_wr_state_h){
        white_rook_h = false;
    }
    if(destination_pos & intial_br_state_a){
        black_rook_a = false;
    }
    if(destination_pos & intial_br_state_h){
        black_rook_h = false;
    }
}


//Making the castling move
void Board:: make_castle_move(int destination_row, int destination_column, int intial_row, int intial_column, uint64_t* initial_piece_type, uint64_t piece_clicked){
    uint64_t destination_pos = find_bit_position(destination_row, destination_column);
    uint64_t rook_desintation = 0;

    if(*find_type(intial_row, intial_column) == this->white_king_state){
        if(intial_column - destination_column == -2 && castling_check(true, true)){
            rook_desintation = find_bit_position(destination_row, destination_column - 1);
            this->white_rook_state = (this->white_rook_state & (~intial_wr_state_h)) | rook_desintation;

            *initial_piece_type = (*initial_piece_type & (~piece_clicked)) | destination_pos;

            white_king_first_move = false;
            white_rook_h = false;
        }

        if(intial_column - destination_column == 2 && castling_check(true, false)){
            rook_desintation = find_bit_position(destination_row, destination_column + 1);
            this->white_rook_state = (this->white_rook_state & (~intial_wr_state_a)) | rook_desintation;

            *initial_piece_type = (*initial_piece_type & (~piece_clicked)) | destination_pos;

            white_king_first_move = false;
            white_rook_a = false;
        }
    }
    else if(*find_type(intial_row, intial_column) == this->black_king_state){
        if(intial_column - destination_column == -2 && castling_check(false, true)){
            rook_desintation = find_bit_position(destination_row, destination_column - 1);
            this->black_rook_state = (this->black_rook_state & (~intial_br_state_h)) | rook_desintation;
            
            *initial_piece_type = (*initial_piece_type & (~piece_clicked)) | destination_pos;

            black_king_first_move = false;
            black_rook_h = false;
        }

        if(intial_column - destination_column == 2 && castling_check(false, false)){
            rook_desintation = find_bit_position(destination_row, destination_column + 1);
            this->black_rook_state = (this->black_rook_state & (~intial_br_state_a)) | rook_desintation;

            *initial_piece_type = (*initial_piece_type & (~piece_clicked)) | destination_pos;

            black_king_first_move = false;
            black_rook_a = false;
        }
    }
}
