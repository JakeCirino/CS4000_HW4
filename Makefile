client: battleship_client.cc ship.h
	g++ -o battleship_client battleship_client.cc -lncurses -lboost_system -lboost_thread -lpthread
server: battleship_server.cc ship.h
	g++ -o battleship_server battleship_server.cc -lboost_system -lboost_thread -lpthread
