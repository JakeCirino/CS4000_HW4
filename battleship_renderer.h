//******************************************
// File: battleship_renderer.h
// Author: Jake Cirino
// Purpose: Client renderer for battleship
//******************************************
#ifndef RENDERER_H
#define RENDERER_H

#include <ncurses.h>
#include <vector>
#include <iostream>
#include <string>
#include "ship.h"

using namespace std;

class battleship_renderer{

private: 

//state 0 = placing ship, 1 rotating ship, 2 = taking turn
int game_state;
ship shp;

void draw_matrix(vector<vector<int> > &board,
		     int cur_row,
		     int cur_col,
             int row_offset,
             int col_offset) {
  for (int j=0;j<4;j++) {
      move(row_offset,2*j+col_offset);
      printw("+-");
    }
    move(row_offset,2*4+col_offset);
    printw("+");
  for (int i=0;i<4;i++) {
    for (int j=0;j<4;j++) {
      move(2*i+1+row_offset,2*j+col_offset);
      printw("|");
      move(2*i+1+row_offset,2*j+1+col_offset);
      if (board[i][j] == 0) {
	printw(" ");
      } else {
	printw("X");
      }
    }
    move(2*i+1+row_offset,2*4+col_offset);
    printw("|");
    for (int j=0;j<4;j++) {
      move(2*i+2+row_offset,2*j+col_offset);
      printw("+-");
    }
    move(2*i+2+row_offset,2*4+col_offset);
    printw("+");
  }
  move(2*cur_row+1+row_offset,2*cur_col+1+col_offset);
}

public:

battleship_renderer(int game_state = 0){
  this->game_state = 0;
}

int get_game_state(){
  return game_state;
}

void set_game_state(int gs){
  game_state = gs;
}

/**
 * Renders the screen
 */
void render(vector<vector<int> > &board){
    int rows;
    int cols;
    int cur_row=0;
    int cur_col=0;
    int ch;

    //init screen
    initscr();

    //clear screen
    clear();

    // Get the size of the window!
    getmaxyx(stdscr,rows,cols);

    
    cbreak();  // Pass all characters to this program!

    keypad(stdscr, TRUE); // Grab the special keys, arrow keys, etc.

    // Paint the row and column markers.
    //paint_markers(rows,cols,10,0,0);
    // Redraw the screen.
    refresh();

    printw("Your board:");
    draw_matrix(board, 0, 0, 1, 0);

    while ((ch = getch())!='q') {
      switch (ch) {
        case ' ':  
          //check game state
          if(game_state == 0){
            //place ship
            shp.set_position(cur_row, cur_col);
            shp.fit_ship_clockwise(board);
            shp.place_ship(board);
            game_state = 1;
          }
          draw_matrix(board,cur_row,cur_col,1,0);
          // Redraw the screen.
          refresh();

          break;
        case KEY_RIGHT:
          switch(game_state){
            case 0:
              cur_col++;
              cur_col%=4;
              break;
            case 1:
              shp.fit_ship_clockwise(board);
              shp.place_ship(board);
              break;
          }
          draw_matrix(board,cur_row,cur_col,1,0);
          // Redraw the screen.
          refresh();
          break;
        case KEY_LEFT:
          switch(game_state){
            case 0:
                cur_col--;
                cur_col = (4+cur_col)%4;
              break;
            case 1:
              shp.fit_ship_counterclockwise(board);
              shp.place_ship(board);
              break;
          }
          draw_matrix(board,cur_row,cur_col,1,0);
          // Redraw the screen.
          refresh();
          break;
        case KEY_UP:
          switch(game_state){
            case 0:
              cur_row--;
              cur_row=(4+cur_row) % 4;
              break;
          }
          draw_matrix(board,cur_row,cur_col,1,0);
          
          //      paint_markers(rows,cols,10,cur_row,cur_col);
          // Redraw the screen.
          refresh();
          break;
        case KEY_DOWN:
          switch(game_state){
            case 0:
              cur_row++;
              cur_row%=4;
              break;
          }
          draw_matrix(board,cur_row,cur_col,1,0);
                //paint_markers(rows,cols,10,cur_row,cur_col);
          // Redraw the screen.
          refresh();
          break;
      }
    }
  endwin();
}

};
#endif