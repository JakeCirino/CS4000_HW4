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
#include <battleship_renderer.h>

using namespace std;
using boost::asio::ip::tcp;

//sockets
int port;
boost::asio::ip::address server_ip;

//game
vector<vector<int>> board, enemy_board;

/**
 * Connects to the server and returns the connected socket
 */
tcp::socket connect_to_server(){
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
    //get server ip and port from arguments
    server_ip = boost::asio::ip::address::from_string(argv[1]);
    port = (int)argv[2];

    //connect to server and get socket
    //tcp::socket socket = connect_to_server();

    //initialize game board
    for(int i=0; i < 4; i++){
        vector<int> tmp;
        for(int j=0; j < 4; j++){
            tmp.push_back(1);
        }
        board.push_back(tmp);
    }
}