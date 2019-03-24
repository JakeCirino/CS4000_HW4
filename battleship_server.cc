//******************************************
// File: battleship_server.cc
// Author: Jake Cirino
// Purpose: Server for battleship game
//******************************************
#include <iostream>
#include <string>
#include <sstream>
#include <boost/asio.hpp>
#include <vector>
#include <utility>
#include "ship.h"

using namespace std;
using boost::asio::ip::tcp;

boost::asio::io_service server_service;
tcp::socket *socket1, *socket2;
vector<vector<int> > board1, board2;
int p1_hits = 0, p2_hits = 0;

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
    acceptor.accept(*socket2);
    cout << "Client 2 connected" << endl;
}

/**
 * Reads packet from a socket
 */
string read_packet(tcp::socket *&sck){
    boost::asio::streambuf buf;
    boost::asio::read_until(*sck, buf, "\n");
    istream is(&buf);
    string packet;
    getline(is, packet);
    
    return packet;
}

void send_packet(tcp::socket *&sck, string msg){
    msg += "\n";
    boost::asio::write(*sck, boost::asio::buffer(msg));
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

int main(){
    start_server();

    //initialize game boards
    for(int i=0; i < 4; i++){
        vector<int> tmp;
        for(int j=0; j < 4; j++){
            tmp.push_back(0);
        }
        board1.push_back(tmp);
        board2.push_back(tmp);
    }

    //get ship1 position
    string ship1 = read_packet(socket1);
    cout << ship1 << endl;
    ship s1;
    s1.from_string(ship1);
    s1.place_ship(board1);
    cout << "client1 ready" << endl;
    
    //get ship2 position
    string ship2 = read_packet(socket2);
    cout << ship2 << endl;
    ship s2;
    s2.from_string(ship2);
    s2.place_ship(board2);
    cout << "client2 ready" << endl;

    //start taking turns
    while(p1_hits != 3 && p2_hits != 3){
        //read coordinate from player1
        string packet1 = read_packet(socket1);
        pair<int,int> c1 = read_coordinate(packet1);
        cout << c1.first << ", " << c1.second << endl;
        string hit1;
        if(board2[c1.first][c1.second] == 0){
            board2[c1.first][c1.second] = 2;
            hit1 = "M";
        }else{
            board2[c1.first][c1.second] = 3;
            p1_hits++;
            hit1 = "H";
        }
        cout << "client1 shot complete" << endl;

        //read coordinate from player2
        string packet2 = read_packet(socket2);
        pair<int,int> c2 = read_coordinate(packet2);
        cout << c2.first << ", " << c2.second << endl;
        string hit2;
        if(board1[c2.first][c2.second] == 0){
            board1[c2.first][c2.second] = 2;
            hit2 = "M";
        }else{
            board1[c2.first][c2.second] = 3;
            p2_hits++;
            hit2 = "H";
        }
        cout << "client2 shot complete" << endl;

        //send packet updates to both players about the round
        string p1_status, p2_status; //0 = another round, 1 = win, 2 = loss, 3 = tie
        if(p1_hits == 3 && p2_hits == 3){
            p1_status = "3";
            p2_status = "3";
        }else if(p1_hits == 3 && p2_hits < 3){
            p1_status = "1";
            p2_status = "2";
        }else if(p1_hits < 3 && p2_hits == 3){
            p1_status = "2";
            p2_status = "1";
        }else{
            p1_status = "0";
            p2_status = "0";
        }
        string status1 = packet2 + "," + hit1 + "," + p1_status;
        string status2 = packet1 + "," + hit2 + "," + p2_status;
        send_packet(socket1, status1);
        send_packet(socket2, status2);
        cout << "round complete" << endl;
    }

    //close sockets
    socket1->close();
    socket2->close();
    return 1;
}