//******************************************
// File: battleship_server.cc
// Author: Jake Cirino
// Purpose: Server for battleship game
//******************************************
#include <iostream>
#include <string>
#include <sstream>
#include <boost/asio.hpp>
#include "ship.h"

using namespace std;
using boost::asio::ip::tcp;

boost::asio::io_service server_service;
tcp::socket *socket1, *socket2;

/**
 * Starts the server
 */
void start_server(){
    //start listening for connections
    tcp::acceptor acceptor(server_service, tcp::endpoint(tcp::v4(), 4700));

    //wait for clients to connect
    socket1 = new tcp::socket(server_service);
    acceptor.accept(*socket1);
    cout << "Client 1 connected" << endl;
    socket2 = new tcp::socket(server_service);
    //acceptor.accept(*socket2);
    cout << "Client 2 connected" << endl;
}

/**
 * Reads packet from a socket
 */
string read_packet(tcp::socket *socket){
    boost::asio::streambuf buf;
    boost::asio::read_until(*socket, buf, "\n");
    istream is(&buf);
    string packet;
    getline(is, packet);
    
    return packet;
}

int main(){
    start_server();

    //get ship positions
    string ship1 = read_packet(socket1);
    ship s1;
    s1.from_string(ship1);
    cout << s1.to_string() << endl;
    socket1->close();
    //string ship2 = read_packet(socket2);

    //close sockets
    socket1->close();
    socket2->close();
    return 1;
}