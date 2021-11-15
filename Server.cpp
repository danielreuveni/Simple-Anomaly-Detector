
#include "Server.h"


Server::Server(int port)throw (const char*) {
      file_descriptor = socket(AF_INET,SOCK_STREAM,0);
	  connection = false;
	if (file_descriptor < 0) {
		throw "socket error";
    }

	  server.sin_family = AF_INET;
	  server.sin_addr.s_addr = INADDR_ANY;
	  server.sin_port = htons(port);

    if (listen(file_descriptor, 3) < 0) {
		throw "listen error";
    }

	if (bind(file_descriptor,(struct sockaddr* )& server, sizeof(server)) < 0) {
		throw "bind error";
    }
}

void socketIO::write(string text) {
	const char* text2 = text.c_str();
	send(socket_fd, text2 ,strlen(text2), 0);		
}

void socketIO::write(float f){
	char text[sizeof(f) + 1];
	sprinf("text", "%f", f)
	string s(ss.str());		
	send(socket_fd, text ,strlen(text), 0);		

}

string socketIO::read() {
	size_t i = 0;
    char c = 0;
	string input = "";
	while(c != '\n'){
		input += c;
        recv(socket_fd, &c, sizeof(char) , 0);
	}
	return input;
}



void socketIO::read(float* f) {
	recv(socket_fd, f, sizeof(*f), 0);
}



void Server::start(ClientHandler& ch) throw (const char*) {	
    this->connection = true;
	t = new thread ([&ch, this]() {
		while (connection){
			socklen_t clientSize = sizeof(client);
			int cliend_fd = accept(file_descriptor ,(struct sockaddr*) &client , &clientSize);
            if (cliend_fd < 0){
                throw "connection error"
            }
			if (cliend_fd > 0){
				ch.handle(cliend_fd);
				close(cliend_fd);
			}
		    }
	  });
}

void Server::stop(){
	this->connection = false;
	t->join(); 
}

Server::~Server() {
	// TODO Auto-generated destructor stub
}

