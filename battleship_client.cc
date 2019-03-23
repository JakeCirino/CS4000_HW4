//******************************************
// File: battleship_client.cc
// Author: Jake Cirino
// Purpose: Client for battleship game
//******************************************
#include <iostream>
#include <string>
#include <vector>
#include <ncurses.h>
#include <boost/asio.hpp>
#include <utility>
#include <sstream>
#include "ship.h"

using namespace std;
using boost::asio::ip::tcp;

//sockets
int port;
boost::asio::ip::address server_ip;
tcp::socket *socket1;

//game
vector<vector<int> > board, enemy_board;
ship shp;
int game_state; //state 0 = placing ship, 1 rotating ship, 2 = taking turn, 3 = paused, 4 = game over

/**
 * Connects to the server and returns the connected socket
 */
void connect_to_server(){
    //create client service
    boost::asio::io_service client_service;

    //resolve and connect to server
    tcp::resolver resolver(client_service);
    socket1 = new tcp::socket(client_service);
    socket1->connect(tcp::endpoint(server_ip, port));
}

/**
 * Formats and sends a packet to the server
 */
void send_packet(string msg){
    msg += "\n";
    boost::asio::write(*socket1, boost::asio::buffer(msg));
}

/**
 * Reads the next packet from the server
 */
string read_packet(){
    boost::asio::streambuf buf;
    boost::asio::read_until(*socket1, buf, "\n");
    istream is(&buf);
    string packet;
    getline(is, packet);
    
    return packet;
}

pair<int, int> read_coordinate(string &str){
    int vals[2];
    string tmp;
    istringstream stream(str);
    for(int i = 0; i < 2; i++){
        getline(stream, tmp, ',');
        vals[i] = atoi(tmp.c_str());
    }
    return pair<int, int>(vals[0], vals[1]);
}

void draw_matrix(vector<vector<int> > &board,
                 int cur_row,
                 int cur_col,
                 int row_offset,
                 int col_offset)
{
    for (int j = 0; j < 4; j++)
    {
        move(row_offset, 2 * j + col_offset);
        printw("+-");
    }
    move(row_offset, 2 * 4 + col_offset);
    printw("+");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            move(2 * i + 1 + row_offset, 2 * j + col_offset);
            printw("|");
            move(2 * i + 1 + row_offset, 2 * j + 1 + col_offset);
            switch(board[i][j]){
                case 0:
                    printw(" ");
                    break;
                case 1:
                    printw("X");
                    break;
                case 2:
                    printw("M");
                    break;
                case 3:
                    printw("H");
                    break;
            }
        }
        move(2 * i + 1 + row_offset, 2 * 4 + col_offset);
        printw("|");
        for (int j = 0; j < 4; j++)
        {
            move(2 * i + 2 + row_offset, 2 * j + col_offset);
            printw("+-");
        }
        move(2 * i + 2 + row_offset, 2 * 4 + col_offset);
        printw("+");
    }
    move(2 * cur_row + 1 + row_offset, 2 * cur_col + 1 + col_offset);
}

void render(){
    int rows;
    int cols;
    int cur_row = 0;
    int cur_col = 0;
    int ch;

    //init screen
    initscr();

    //clear screen
    clear();

    // Get the size of the window!
    getmaxyx(stdscr, rows, cols);

    cbreak(); // Pass all characters to this program!

    keypad(stdscr, TRUE); // Grab the special keys, arrow keys, etc.

    // Paint the row and column markers.
    //paint_markers(rows,cols,10,0,0);
    // Redraw the screen.
    refresh();

    printw("Your Board:");
    move(10, 0);
    printw("Enemy Board:");
    draw_matrix(enemy_board, 0, 0, 11, 0);
    draw_matrix(board, 0, 0, 1, 0);

    while ((ch = getch()) != 'q')
    {
        switch (ch)
        {
        case ' ':
            switch (game_state){
                case 0:
                    //place ship
                    shp.set_position(cur_row, cur_col);
                    shp.fit_ship_clockwise(board);
                    shp.place_ship(board);
                    game_state = 1;
                    draw_matrix(enemy_board, 0, 0, 11, 0);
                    draw_matrix(board, cur_row, cur_col, 1, 0);
                    break;
                case 1:
                    //ship placed, send coordinates to server
                    send_packet(shp.to_string());
                    game_state = 2;
                    draw_matrix(board, cur_row, cur_col, 1, 0);
                    draw_matrix(enemy_board, cur_row, cur_col, 11, 0);
                    break;
                case 2:
                    //send shot coordinates
                    //pause game state
                    game_state = 3;
                    stringstream ss;
                    ss << cur_row << "," << cur_col;
                    send_packet(ss.str());

                    //read round result
                    string s = read_packet();
                    string vals[3];
                    string tmp;
                    istringstream stream(s);
                    for(int i = 0; i < 3; i++){
                        getline(stream, tmp, ',');
                        vals[i] = tmp;
                    }

                    //read shot result
                    string sr = vals[2];
                    if(sr == "M"){
                        enemy_board[cur_row][cur_col] = 2;
                    }else{
                        enemy_board[cur_row][cur_col] = 3;
                    }

                    //process result from other players shot
                    string osr = vals[0] + "," + vals[1];
                    pair<int, int> other_coords = read_coordinate(osr);
                    if(board[other_coords.first][other_coords.second] == 0){
                        board[other_coords.first][other_coords.second] = 2;
                    }else{
                        board[other_coords.first][other_coords.second] = 3;
                    }                    
                    game_state = 2;
                    draw_matrix(board, 0, 0, 1, 0);
                    draw_matrix(enemy_board, cur_row, cur_col, 11, 0);
                    refresh();
                    break;
            }
            // Redraw the screen.
            refresh();

            break;
        case KEY_RIGHT:
            switch (game_state)
            {
            case 0:
                cur_col++;
                cur_col %= 4;
                draw_matrix(enemy_board, 0, 0, 11, 0);
                draw_matrix(board, cur_row, cur_col, 1, 0);
                break;
            case 1:
                shp.fit_ship_clockwise(board);
                shp.place_ship(board);
                draw_matrix(enemy_board, 0, 0, 11, 0);
                draw_matrix(board, cur_row, cur_col, 1, 0);
                break;
            case 2:
                cur_col++;
                cur_col %= 4;
                draw_matrix(board, cur_row, cur_col, 1, 0);
                draw_matrix(enemy_board, cur_row, cur_col, 11, 0);
            }
            // Redraw the screen.
            refresh();
            break;
        case KEY_LEFT:
            switch (game_state)
            {
            case 0:
                cur_col--;
                cur_col = (4 + cur_col) % 4;
                draw_matrix(enemy_board, 0, 0, 11, 0);
                draw_matrix(board, cur_row, cur_col, 1, 0);
                break;
            case 1:
                shp.fit_ship_counterclockwise(board);
                shp.place_ship(board);
                draw_matrix(enemy_board, 0, 0, 11, 0);
                draw_matrix(board, cur_row, cur_col, 1, 0);
                break;
            case 2:
                cur_col--;
                cur_col = (4 + cur_col) % 4;
                draw_matrix(board, cur_row, cur_col, 1, 0);
                draw_matrix(enemy_board, cur_row, cur_col, 11, 0);
                break;
            }
            // Redraw the screen.
            refresh();
            break;
        case KEY_UP:
            switch (game_state)
            {
            case 0:
                cur_row--;
                cur_row = (4 + cur_row) % 4;
                draw_matrix(enemy_board, 0, 0, 11, 0);
                draw_matrix(board, cur_row, cur_col, 1, 0);
                break;
            case 2:
                cur_row--;
                cur_row = (4 + cur_row) % 4;
                draw_matrix(board, cur_row, cur_col, 1, 0);
                draw_matrix(enemy_board, cur_row, cur_col, 11, 0);
                break;
            }

            //      paint_markers(rows,cols,10,cur_row,cur_col);
            // Redraw the screen.
            refresh();
            break;
        case KEY_DOWN:
            switch (game_state)
            {
            case 0:
                cur_row++;
                cur_row %= 4;
                draw_matrix(enemy_board, 0, 0, 11, 0);
                draw_matrix(board, cur_row, cur_col, 1, 0);
                break;
            case 2:
                cur_row++;
                cur_row %= 4;
                draw_matrix(board, cur_row, cur_col, 1, 0);
                draw_matrix(enemy_board, cur_row, cur_col, 11, 0);
                break;
            }
            //paint_markers(rows,cols,10,cur_row,cur_col);
            // Redraw the screen.
            refresh();
            break;
        }
    }
    endwin();
}

int main(int argc, char *argv[]){
    //get server ip and port from arguments if available
    if(argc == 3){
        //assign entered IP/port
        server_ip = boost::asio::ip::address::from_string(argv[1]);
        port = atoi(argv[2]);
    }else{
        //assign default IP/port
        server_ip = boost::asio::ip::address::from_string("127.0.0.1");
        port = 4700;
    }


    //connect to server
    connect_to_server();

    //initialize game boards
    for(int i=0; i < 4; i++){
        vector<int> tmp;
        for(int j=0; j < 4; j++){
            tmp.push_back(0);
        }
        board.push_back(tmp);
        enemy_board.push_back(tmp);
    }

    //create renderer and begin rendering screen/gameplay
    render();
    socket1->close();
    return 1;
}