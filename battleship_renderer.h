//******************************************
// File: battleship_renderer.h
// Author: Jake Cirino
// Purpose: Client renderer for battleship
//******************************************
#include <ncurses.h>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

class battleship_renderer{

void draw_top_matrix(vector<vector<int> > &board,
		     int cur_row,
		     int cur_col) {

  for (int j=0;j<4;j++) {
      move(0,2*j);
      printw("+-");
    }
    move(0,2*4);
    printw("+");
  for (int i=0;i<4;i++) {
    for (int j=0;j<4;j++) {
      move(2*i+1,2*j);
      printw("|");
      move(2*i+1,2*j+1);
      if (board[i][j] == 0) {
	printw(" ");
      } else {
	printw("X");
      }
    }
    move(2*i+1,2*4);
    printw("|");
    for (int j=0;j<4;j++) {
      move(2*i+2,2*j);
      printw("+-");
    }
    move(2*i+2,2*4);
    printw("+");
  }
  move(2*cur_row+1,2*cur_col+1);
}

/**
 * Renders the screen
 */
void render_screen(vector<vector<int>> &board){
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


    draw_top_matrix(board,0,0);
}

}