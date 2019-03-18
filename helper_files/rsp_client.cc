//****************************************************
// File:rsp_client.cc
// Author: David W. Juedes
// Purpose: play rock-scissors-paper over the network.
// Connects to rsp server.
// rsp server acts as referee.
// returns whether you won-lost or tied.
// Client write "rock/scissors/paper"
// Server return "win/lose/tie"
//
//**************************************************************************
#include <iostream>
#include <string>
#include <boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;

int main(int argc, char *argv[]) {

  cout << "X args" << argc << endl;
  cout << "Rsp choice = " << argv[1] << endl;
  cout << "server" << argv[2] << endl;
  cout << "port" << argv[3] << endl;

  int portno = atoi(argv[3]);
  // Standard boost code to connect to a server.
  // Comes from the boost tutorial
  boost::asio::io_service my_service;

  tcp::resolver resolver(my_service);
  // Find the server/port number.
  //  tcp::resolver::results_type endpoints = resolver.resolve(argv[2], argv[3]);

  tcp::socket socket(my_service);
  
  socket.connect(tcp::endpoint(boost::asio::ip::address::from_string(argv[2]),portno));
  
  string msg;
  msg = argv[1];
  msg+= "\n";
  boost::asio::write( socket, boost::asio::buffer(msg) );

  // Get the response from the server!
  boost::asio::streambuf buf2;
  boost::asio::read_until( socket, buf2, "\n" );
  string answer = boost::asio::buffer_cast<const char*>(buf2.data());
  cout << "Result = " << answer << endl;
}
  