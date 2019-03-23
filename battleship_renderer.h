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

using namespace std;

class battleship_renderer{

private: 

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

/**
 * Renders the screen
 */
void render(vector<vector<int> > &board, int game_state){
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
        case ' ':  board[cur_row][cur_col]=0;
          draw_matrix(board,cur_row,cur_col,1,0);
          // Redraw the screen.
          refresh();

          break;
        case KEY_RIGHT:
          cur_col++;
          cur_col%=4;
          draw_matrix(board,cur_row,cur_col,1,0);
          // Redraw the screen.
          refresh();
          break;
        case KEY_LEFT:
          cur_col--;
          cur_col = (4+cur_col)%4;
          draw_matrix(board,cur_row,cur_col,1,0);
          // Redraw the screen.
          refresh();
          break;
        case KEY_UP:
          cur_row--;
          cur_row=(4+cur_row) % 4;
          draw_matrix(board,cur_row,cur_col,1,0);
          
          //      paint_markers(rows,cols,10,cur_row,cur_col);
          // Redraw the screen.
          refresh();
          break;
        case KEY_DOWN:
          cur_row++;
          cur_row%=4;
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