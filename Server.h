/*
Daniel Reuveni
*/

#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <sys/socket.h>  // The header file socket.h includes a number of definitions of structures needed for sockets.
#include <netinet/in.h>  // The header file in.h contains constants and structures needed for internet domain addresses.
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<signal.h>
#include <sstream>
#include "CLI.h"

using namespace std;

class ClientHandler{
    public:
    virtual void handle(int socket_fd) = 0;
};

class socketIO:public DefaultIO{
	int socket_fd;
public:

	socketIO(int socket_fd):socket_fd (socket_fd) {}

	virtual string read();
	virtual void write(string text);
	virtual void write(float f);
	virtual void read(float* f);

};

class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int socket_fd){
    	socketIO sio(socket_fd);
    	CLI cli(&sio);
    	cli.start();
    }
};


class Server {
    thread* t;
    bool connection;
	int file_descriptor;
	sockaddr_in server, client;
	
public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
