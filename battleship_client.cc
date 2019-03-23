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
#include "battleship_renderer.h"
#include "ship.h"

using namespace std;
using boost::asio::ip::tcp;

//sockets
int port;
boost::asio::ip::address server_ip;

//game
vector<vector<int> > board, enemy_board;

//state 0 = placing ship, 1 rotating ship, 2 = taking turn
int game_state=0;

/**
 * Connects to the server and returns the connected socket
 */
tcp::socket& connect_to_server(){
    //create client service
    boost::asio::io_service client_service;

    //resolve and connect to server
    tcp::resolver resolver(client_service);
    tcp::socket socket(client_service);
    socket.connect(tcp::endpoint(server_ip, port));

    return socket;
}

/**
 * Formats and sends a packet to the server
 */
void send_packet(tcp::socket &socket, string msg){
    msg += "\n";
    boost::asio::write(socket, boost::asio::buffer(msg));
}

/**
 * Reads the next packet from the server
 */
string read_packet(tcp::socket &socket){
    boost::asio::streambuf buf;
    boost::asio::read_until(socket, buf, "\n");
    return boost::asio::buffer_cast<const char*>(buf.data());
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


    //connect to server and get socket
    //tcp::socket socket = connect_to_server();

    //initialize game board
    for(int i=0; i < 4; i++){
        vector<int> tmp;
        for(int j=0; j < 4; j++){
            tmp.push_back(0);
        }
        board.push_back(tmp);
    }

    //create renderer and begin rendering screen
    battleship_renderer renderer;
    renderer.render(board);
}