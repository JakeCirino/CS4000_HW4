//******************************************
// File: battleship_server.cc
// Author: Jake Cirino
// Purpose: Server for battleship game
//******************************************
#include <iostream>
#include <string>
#include <sstream>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;

boost::asio::io_service server_service;

/**
 * Starts the server
 */
void start_server(){
    //start listening for connections
    tcp::acceptor acceptor(server_service, tcp::endpoint(tcp::v4(), 4700));

    //wait for clients to connect
    tcp::socket socket1(server_service);
    acceptor.accept(socket1);
    tcp::socket socket2(server_service);
    acceptor.accept(socket2);
}

int main(){

    return 1;
}