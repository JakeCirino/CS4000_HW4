//*****************************************************************
// File: rsp_server.cc
// AUthor: David W. Juedes
// Purpose: Rock-Scissors-Paper Server
// Accepts exactly two connections from two rsp clients.
// gets two strings from two different clients.  
//  ---
// returns the clients whether they won or lost.
//
// Acknowledgement:
// Based on the approach found here:
// https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T
//***************************************************************************
#include <iostream>
#include <string>
#include <sstream>
#include <boost/asio.hpp>

//using namespace std;

using namespace std;
using boost::asio::ip::tcp;

int rsp_int(string &x) {
  if (x=="rock\n") return 0;
  if (x=="scissors\n") return 1;
  if (x=="paper\n") return 2;
  return 3;
}

// i and j can be 0, 1, or 2.
//
int win_lose_tie(int i, int j) {
  if (i==j) return 0; // Tie
  switch (i) {
  case 0:
    switch (j) {
    case 1: return 1; // Win
    case 2: return 2; // Lose
    }
  case 1:
    switch (j) {
    case 0: return 2; // Lose
    case 2: return 1; // Win
    }
  case 2:
    switch (j) {
    case 0: return 1; // Win
    case 1: return 2; // Lose
    }
  }
}


void RSP() {
    boost::asio::io_service my_service;

    // Accept connections on
    // Advertise a service on port 4700
    tcp::acceptor acceptor(my_service, tcp::endpoint(tcp::v4(), 4700));

    tcp::socket socket1(my_service);
    // Wait for a connection from 
    acceptor.accept(socket1);
    // Connection Established!!!

    // Read from Socket 1 until newline
    boost::asio::streambuf buf;
    boost::asio::read_until( socket1, buf, "\n" );
    string data = boost::asio::buffer_cast<const char*>(buf.data());
    cout << data << endl;

    //Wait for a connection on Socket #2 
    tcp::socket socket2(my_service);
    acceptor.accept(socket2);
    // Connection Established!!!!
    
    // Read from Socket #2 until newline
    boost::asio::streambuf buf2;
    boost::asio::read_until( socket2, buf2, "\n" );
    string data2 = boost::asio::buffer_cast<const char*>(buf2.data());
    cout << data2 << endl;

    // Figure out who won!
    int p1,p2;
    p1 = rsp_int(data);
    p2 = rsp_int(data2);
    if ((p1==3) || (p2 == 3)) {
      // Write error
      string msg;
      msg = "error\n";
      boost::asio::write( socket1, boost::asio::buffer(msg) );
      boost::asio::write( socket2, boost::asio::buffer(msg) );
    } else {
      // Send results to the players 1 and 2.
      istringstream in1(data2);
      string second;
      in1 >> second;
      
      string mesg1;
      mesg1 = "Player 2: " + second + " you "; 
      switch (win_lose_tie(p1,p2)) {
      case 0: mesg1 += "Tie\n";
	break;
      case 1: mesg1 += "Win\n";
	break;
      case 2: mesg1 += "Lose\n";
	break;
      }
      boost::asio::write( socket1, boost::asio::buffer(mesg1) );
      string mesg2;
      istringstream in(data);
      string first;
      in >> first;
      mesg2 = "Player 1: " + first + " you ";
      
      
      switch (win_lose_tie(p2,p1)) {
      case 0: mesg2 += "Tie\n";
	break;
      case 1: mesg2 += "Win\n";
	break;
      case 2: mesg2 += "Lose\n";
	break;
      }
      boost::asio::write( socket2, boost::asio::buffer(mesg2) );
      
    
    }
}


int main()
{
  RSP();
  
  return 0;
}
