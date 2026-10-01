#include <iostream>
#include "board.h"

using namespace std;



vector <uint64_t> Board::generate_legal_moves(int row, int column){
    /*
    Produces the legal move of the selected piece

    Input:
        int row: The row of the selected piece
        int column: The column of the selected piece
    
    Returns:
        {legal_moves, capture_moves}
        Legal moves is a uint64_t value which has a one where there is a legal move
        Capture move is a uint64_t value which has a one where there is a capturable move
    
    */
    uint64_t legal_moves = 0;
    uint64_t capture_moves = 0;

    if(find_type(row, column) == nullptr){
        return {0,0};
    }

    // =========== Pawn Logic =========== //
    //White pawn
    if(*find_type(row, column) == this->white_pawn_state){
        uint64_t pawn = find_bit_position(row, column);

        //If the pawn is on the starting sqaure it can take two steps forwards
        if(row == 1){
            if( !((pawn<<8) & this->board_state())){
                legal_moves |= pawn<< 8;
                
                if (!((pawn<<16) & this->board_state())){
                    legal_moves |= pawn << 16;
                }
            }
        }else if( !((pawn<<8) & this->board_state())){
            legal_moves |= pawn<< 8;
        }

        uint64_t capture_squares = (pawn << 7) | (pawn << 9);
        capture_moves = capture_moves |  capture_squares &this->black_board_state();

        legal_moves = ((this->white_board_state()) ^ legal_moves )  & legal_moves;
        return {legal_moves, capture_moves};
    }
    
    //Black pawn
    else if(*find_type(row, column) == this->black_pawn_state){
        uint64_t pawn = find_bit_position(row, column);

        //If the pawn is on the starting sqaure it can take two steps forwards
        if(row == 6){
            if( !((pawn>>8) & this->board_state())){
                legal_moves |= pawn>>8;
                
                if (!((pawn>>16) & this->board_state())){
                    legal_moves |= pawn >> 16;
                }
            }
        }
        else if( !((pawn>>8) & this->board_state())){
            legal_moves |= pawn>>8;
        }


        uint64_t capture_squares = (pawn >> 7) | (pawn >> 9);
        capture_moves = capture_moves |  capture_squares &this->white_board_state();

        legal_moves = ((this->black_board_state()) ^ legal_moves )  & legal_moves;
        return {legal_moves, capture_moves};
    }


    // =========== Horse Logic =========== //
    else if(*find_type(row, column) == this->white_horse_state || *find_type(row, column) == this->black_horse_state){
        uint64_t friendly_piece, enemy_piece;

        //Determinig friendly and enemy pieces
        if(find_bit_position(row, column) & (this->white_board_state())){
            friendly_piece = this->white_board_state();
            enemy_piece = this->black_board_state();
        }
        else{
            friendly_piece = this->black_board_state();
            enemy_piece = this->white_board_state();
        }

        //Horse Movement rules
        vector<int> row_increments = {2,2,1,1,-1,-1,-2,-2};
        vector<int> column_increaments = {1,-1,2,-2,2,-2,1,-1};
        int size_vector = row_increments.size();

        for(int i = 0 ; i < size_vector; i++){
            int changed_row = row+row_increments[i];
            int changed_column = column+column_increaments[i];

            //Bounds check for boundary
            if(changed_row >= 0 && changed_column >=0 && changed_row < 8 &&  changed_column < 8){
                legal_moves = legal_moves | find_bit_position(changed_row, changed_column);
            }
        }
        //Saving the overlapping moves
        capture_moves = capture_moves |  legal_moves & enemy_piece;

        //Making sure white horse can't jump to squares occupied with white pieces and the capture moves
        legal_moves = ((friendly_piece) ^ legal_moves)  & legal_moves;
        legal_moves = (capture_moves ^ legal_moves) & legal_moves;
        return {legal_moves, capture_moves};
    }


    // =========== Rook Logic =========== //
    else if(*find_type(row, column) == this->white_rook_state || *find_type(row, column) == this->black_rook_state){
        //Finding which piece is selected
        uint64_t selected_piece;
        if(find_bit_position(row, column) & this->white_board_state()){
            selected_piece = this->white_rook_state;
        }else{
            selected_piece = this->black_rook_state;
        }

        //Directions Rook can take
        vector<int> North = {1, 0};
        vector<int> South = {-1, 0};
        vector<int> East = {0, 1};
        vector<int> West = {0,-1};

        vector< vector<int>> Directions = {North, South, East, West};

        for( vector<int> orientation : Directions){
            vector<uint64_t>  result = movement_check(selected_piece, row, column, orientation);
            legal_moves = legal_moves | result[0];
            capture_moves = capture_moves | result[1];
        }
        return {legal_moves, capture_moves};
    }

    // =========== Bishop Logic =========== //
    else if(*find_type(row, column) == this->white_bishop_state || *find_type(row, column) == this->black_bishop_state){
        //Finding which piece is selected
        uint64_t selected_piece;
        if(find_bit_position(row, column) & this->white_board_state()){
            selected_piece = this->white_bishop_state;
        }else{
            selected_piece = this->black_bishop_state;
        }

        //Directions Bishop can take
        vector<vector<int>> Directions = {{1, -1}, {1, 1}, {-1, 1}, {-1,-1}};

        for( vector<int> orientation : Directions){
            vector<uint64_t>  result = movement_check(selected_piece, row, column, orientation);
            legal_moves = legal_moves | result[0];
            capture_moves = capture_moves | result[1];
        }
        return {legal_moves, capture_moves};
    }

    // =========== Queen Logic =========== //
    else if(*find_type(row, column) == this->white_queen_state || *find_type(row, column) == this->black_queen_state){
        //Finding which piece is selected
        uint64_t selected_piece;
        if(find_bit_position(row, column) & this->white_board_state()){
            selected_piece = this->white_queen_state;
        }else{
            selected_piece = this->black_queen_state;
        }

        //Possible directions
        vector<vector<int>> Directions = { {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1,-1}, {0,-1}, {1, -1}};

        //Looping through the directions
        for( vector<int> orientation : Directions){
            vector<uint64_t>  result = movement_check(selected_piece, row, column, orientation);            
            legal_moves = legal_moves |result[0];
            capture_moves = capture_moves | result[1];
        }
        return {legal_moves, capture_moves};
    }


    // White King
    else if(*find_type(row, column) == this->white_king_state || *find_type(row, column) == this->black_king_state){

        uint64_t *selected_king = nullptr;
        uint64_t *enemy_king = nullptr;
        uint64_t enemy_board = 0;
        uint64_t friendly_board = 0;

        if(find_bit_position(row, column) & this->white_king_state){
            selected_king = &(this->white_king_state);
            enemy_king = &(this->black_king_state);
            enemy_board = (this->black_board_state());

            friendly_board = (this->white_board_state());
        }else{
            selected_king = &(this->black_king_state);
            enemy_king = &(this->white_king_state);
            enemy_board = (this->white_board_state());
            friendly_board = (this->black_board_state());
        }

        vector<vector<int>> king_moves{
            //{increment_row, increment_column}
            {1, 0}, {1, 1}, {1, -1}, {-1, 0}, {-1, 1}, {-1, -1}, {0, 1}, {0, -1}
        };

        for(vector<int>&possible_moves : king_moves){
            int r = row;
            int c = column;

            r += possible_moves[0];
            c += possible_moves[1];

            bool check;
            uint64_t move;

            if(r >= 0 && c >= 0 && r < 8 && c< 8){
                move = find_bit_position(r,c);


                if(move & friendly_board){
                    continue;
                }
                
                uint64_t temp_king_pos = *selected_king;
                *selected_king = move;
                bool check = king_check(*selected_king);

                *selected_king = temp_king_pos;
                
                if(check){
                    continue;
                }

                //skipping the move if the location is occupied by the attacking squares of the king
                bool occupied_enemy_king = false;
                for(vector<int>&p_moves : king_moves){
                    int r2 = r;
                    int c2 = c;
                    r2 += p_moves[0];
                    c2 += p_moves[1];
                    if(r2 >= 0 && c2 >= 0 && r2 < 8 && c2< 8){
                        if(find_bit_position(r2, c2) & (*enemy_king)){
                            occupied_enemy_king = true;
                            break;
                        }
                    }
                }

                if(occupied_enemy_king){
                    continue;
                }


                

                legal_moves |= move;
                if(move & enemy_board){
                    capture_moves |= move;
                }
            }
        }

        //Adding legal moves for caslting
        bool white_king = (*selected_king & this->white_king_state);
        vector<bool> castling_side = {true, false};

        for(bool direction : castling_side){
            if(castling_check(white_king, direction)){
                uint64_t castle_move = 0;
                if(direction == true){
                    castle_move = find_bit_position(row, column + 2);
                    legal_moves |= castle_move;
                }
                else if (direction == false){
                    castle_move = find_bit_position(row, column - 2);
                    legal_moves |= castle_move;
                }
            }
        }
        
        return {legal_moves, capture_moves};
    }

    return {0,0};
}


void Board::undo_move(const Undo& undo){
    //Restore piece bitboards
    *undo.initial_piece = undo.saved_initial_state;
    if(undo.destination_piece != nullptr){
        *undo.destination_piece = undo.saved_destination_state;
    }

    //Restore rook bitboards (covers castling's direct mutation)
    this->white_rook_state = undo.saved_white_rook_state;
    this->black_rook_state = undo.saved_black_rook_state;

    //Restore castling rights flags
    white_king_first_move = undo.saved_white_king_first_move;
    black_king_first_move = undo.saved_black_king_first_move;
    white_rook_a = undo.saved_white_rook_a;
    white_rook_h = undo.saved_white_rook_h;
    black_rook_a = undo.saved_black_rook_a;
    black_rook_h = undo.saved_black_rook_h;
}

bool Board:: can_make_move(int destination_row, int destination_column, int intial_row, int intial_column, bool white){
    uint64_t* initial_piece_type = find_type(intial_row, intial_column);
    uint64_t* destination_piece_type = find_type(destination_row, destination_column);
    uint64_t piece_clicked = find_bit_position(intial_row, intial_column);
    uint64_t destination_pos = find_bit_position(destination_row, destination_column);

    //Saving all the variables
    Undo undo;
    undo.initial_piece = initial_piece_type;
    undo.destination_piece = destination_piece_type;
    undo.saved_initial_state = *initial_piece_type;
    undo.saved_destination_state = (destination_piece_type != nullptr) ? *destination_piece_type : 0;
    undo.saved_white_rook_state = white_rook_state;
    undo.saved_black_rook_state = black_rook_state;
    undo.saved_white_king_first_move = white_king_first_move;
    undo.saved_black_king_first_move = black_king_first_move;
    undo.saved_white_rook_a = white_rook_a;
    undo.saved_white_rook_h = white_rook_h;
    undo.saved_black_rook_a = black_rook_a;
    undo.saved_black_rook_h = black_rook_h;

    //Make a temp move
    if(destination_piece_type != nullptr){
        *destination_piece_type &= ~destination_pos;
    }
    *initial_piece_type = (*initial_piece_type & (~piece_clicked)) | destination_pos;

    //Is king under check
    uint64_t king_state = white ? white_king_state : black_king_state;
    bool still_in_check = king_check(king_state);

    //Undo
    undo_move(undo);

    //True if move legal
    return !still_in_check;
}



//Makes the move
void Board::make_move(int destination_row, int destination_column, int intial_row, int intial_column){
    bool successful_move;
    vector<uint64_t> moves = generate_legal_moves(intial_row, intial_column);

    //Find piece_being_moved, and possible moves from the board state
    uint64_t destination_pos = find_bit_position(destination_row, destination_column);
    uint64_t *destination_piece_type = this->find_type(destination_row, destination_column);

    uint64_t *initial_piece_type = this->find_type(intial_row, intial_column); 
    uint64_t piece_clicked = this->find_bit_position(intial_row, intial_column);

    uint64_t legal_moves = moves[0];
    uint64_t capture_moves = moves[1];

    if (initial_piece_type == nullptr) {
        return;
    }

    //If incorrect colour move then don't register
    if((this->white_move) && !(piece_clicked & this->white_board_state()) || !(this->white_move) && (piece_clicked & this->white_board_state())  ){
        return;
    }

    //Saving all the states
    Undo undo;
    undo.initial_piece = initial_piece_type;
    undo.destination_piece = destination_piece_type;
    undo.saved_initial_state = *initial_piece_type;
    undo.saved_destination_state = (destination_piece_type != nullptr) ? *destination_piece_type : 0;

    undo.saved_white_rook_state = this->white_rook_state;
    undo.saved_black_rook_state = this->black_rook_state;

    undo.saved_white_king_first_move = white_king_first_move;
    undo.saved_black_king_first_move = black_king_first_move;
    undo.saved_white_rook_a = white_rook_a;
    undo.saved_white_rook_h = white_rook_h;
    undo.saved_black_rook_a = black_rook_a;
    undo.saved_black_rook_h = black_rook_h;


    
    // If the destination position & capture piece then remove the destination piece type
    // Else if destination piece type == null ptr then move the piece there
    if(capture_moves & destination_pos && (destination_piece_type != nullptr)){        
        //Updating casitling variables
        update_castling_var(initial_piece_type, destination_pos);

        *initial_piece_type = (*initial_piece_type & (~piece_clicked)) | destination_pos;
        *destination_piece_type &= ~destination_pos;
        successful_move = true;
    }
    else if( (legal_moves & destination_pos) && (destination_piece_type == nullptr)){

        //Making castle move
        bool is_castle_attempt = (initial_piece_type == &this->white_king_state || initial_piece_type == &this->black_king_state) && abs(destination_column - intial_column) == 2;

        if(is_castle_attempt){
            //Castle
            make_castle_move( destination_row ,destination_column ,intial_row ,intial_column, initial_piece_type, piece_clicked);
        }
        else{
        //Updating castling variables
        update_castling_var(initial_piece_type, destination_pos);

        //Normal moves
        *initial_piece_type = (*initial_piece_type & (~piece_clicked)) | destination_pos;
        }
        
        successful_move = true;
    }


    else{
        //if move not made then continue
        return;
    }

    //If a piece moves and leaves its own king in check reverse the move
    bool is_king_check = false;
    if(white_move){
        is_king_check = king_check(this->white_king_state);
    }
    else{
        is_king_check = king_check(this->black_king_state);
    }

    if(is_king_check){
        undo_move(undo);
        successful_move = false;
    }

    //Check if a pawn can promote
    can_promote(destination_row, destination_column);
    if(black_promotion || white_promotion){
        char selected_piece;
        cout<<"Input Q, R, B or K"<< endl;
        cin>>selected_piece;
        pawn_promote(selected_piece);
    }

    //If a successful move has made then it is the opposite colour turn
    if(successful_move){
        white_move = !white_move;
        
    }
}
