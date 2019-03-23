//******************************************
// File: ship.h
// Author: Jake Cirino
// Purpose: Ship class
//******************************************
#ifndef SHIP_H
#define SHIP_H
#include <vector>
using namespace std;

class ship{

public:
ship(int r = 0, int c = 0, int rot_row = 0, int rot_col = 1, int size = 3){
    row = r;
    column = c;
    row_rotation = rot_row;
    column_rotation = rot_col;
    ship_size = size;
}

int get_row(){
    return row;
}

int get_column(){
    return column;
}

int get_row_rotation(){
    return row_rotation;
}

int get_column_rotation(){
    return column_rotation;
}

void set_position(int r, int c){
    row = r;
    column = c;
}

void move(int r, int c){
    set_position(row + r, column + c);
}

void set_rotation(int r, int c){
    row_rotation = r;
    column_rotation = c;
}

void rotate_counterclockwise(){
    if(row_rotation == 0 && column_rotation == 1){
        //was facing east
        row_rotation = -1;
    }else if(row_rotation == -1 && column_rotation == 1){
        //was facing northeast
        column_rotation = 0;
    }else if(row_rotation == -1 && column_rotation == 0){
        //was facing north
        column_rotation = -1;
    }else if(row_rotation == -1 && column_rotation == -1){
        //was facing northwest
        row_rotation = 0;
    }else if(row_rotation == 0 && column_rotation == -1){
        //was facing west
        row_rotation = 1;
    }else if(row_rotation == 1 && column_rotation == -1){
        //was facing southwest
        column_rotation = 0;
    }else if(row_rotation == 1 && column_rotation == 0){
        //was facing south
        column_rotation = 1;
    }else if(row_rotation == 1 && column_rotation == 1){
        //was facing southeast
        row_rotation = 0;
    }
}

void rotate_clockwise(){
    if(row_rotation == 0 && column_rotation == 1){
        //was facing east
        row_rotation = 1;
    }else if(row_rotation == -1 && column_rotation == 1){
        //was facing northeast
        row_rotation = 0;
    }else if(row_rotation == -1 && column_rotation == 0){
        //was facing north
        column_rotation = 1;
    }else if(row_rotation == -1 && column_rotation == -1){
        //was facing northwest
        column_rotation = 0;
    }else if(row_rotation == 0 && column_rotation == -1){
        //was facing west
        row_rotation = -1;
    }else if(row_rotation == 1 && column_rotation == -1){
        //was facing southwest
        row_rotation = 0;
    }else if(row_rotation == 1 && column_rotation == 0){
        //was facing south
        column_rotation = -1;
    }else if(row_rotation == 1 && column_rotation == 1){
        //was facing southeast
        column_rotation = 0;
    }
}

bool ship_inbounds(vector<vector<int> > &board){
    int row_check = row + ((ship_size-1)*row_rotation);
    int col_check = column + ((ship_size-1)*column_rotation);
    return row_check >= 0 && row_check < board.size() && col_check >= 0 && col_check < board.size();
}

/**
 * Properly rotates ship to fit on the board in its current location
 */
void fit_ship_clockwise(vector<vector<int> > &board){
    do{
        rotate_clockwise();
    }while(!ship_inbounds(board));
}

void fit_ship_counterclockwise(vector<vector<int> > &board){
    do{
        rotate_counterclockwise();
    }while(!ship_inbounds(board));
}

/**
 * Updates ship location on a board
 */
void place_ship(vector<vector<int> > &board){
    clear_board(board);

    for(int i = 0; i < ship_size; i++){
        board[row + (i*row_rotation)][column + (i*column_rotation)] = 1;
    }
}

private:
int row, column, row_rotation, column_rotation, ship_size;

void clear_board(vector<vector<int> > &board){
    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board.size(); j++){
            board[i][j] = 0;
        }
    }
}

};
#endif