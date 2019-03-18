client: rsp_client.cc
	g++ -o rsp_client rsp_client.cc -lboost_system -lboost_thread -lpthread
server: rsp_server.cc
	g++ -o rsp_server rsp_server.cc -lboost_system -lboost_thread -lpthread
